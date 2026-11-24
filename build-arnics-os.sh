#!/usr/bin/env bash
# ==============================================================================
# 脚本说明：
# 该脚本用于配置并构建基于 Linux 平台的 C/C++ 工程，支持常规编译与交叉编译。
# 
# 可用参数：
#   -t, --type <Debug|Release|...> : 指定 CMake 构建类型（默认: Debug）。
#   -b, --build-dir <dir>          : 指定自定义的构建输出目录（默认: out/<构建类型>，如 out/Debug）。
#   -c, --clean                    : 编译前先清理（删除）构建目录。
#   -x, --cross <prefix>           : 启用交叉编译并指定工具链前缀，例如: arm-linux-gnueabihf-
#   -h, --help                     : 打印此帮助信息并退出。
# 
# 示例用法：
#   常规编译: ./build-arnics-os.sh -t Release
#   清理并编译: ./build-arnics-os.sh -c
#   交叉编译: ./build-arnics-os.sh -x arm-linux-gnueabihf-
# ==============================================================================

# 启用严格模式：
# -e: 任何命令执行失败（返回值非0）即退出脚本
# -u: 使用未定义的变量时报错并退出
# -o pipefail: 管道命令中任何一个子命令失败，整个管道的返回值即为失败
set -euo pipefail

# 获取脚本所在的绝对路径作为工程根目录
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# 初始化默认的构建参数
BUILD_TYPE="${BUILD_TYPE:-Debug}" # 默认构建类型为 Debug
CLEAN=0                           # 默认不清理构建目录
BUILD_DIR=""                      # 构建目录默认留空，后续动态生成
CROSS_COMPILE=""                  # 交叉编译工具链前缀，如 arm-linux-gnueabihf-

# 解析命令行参数
while [[ $# -gt 0 ]]; do
  case "$1" in
    -t|--type)
      # 指定构建类型，如 Debug 或 Release
      BUILD_TYPE="${2:-}"
      shift 2 # 消耗两个参数（键和值）
      ;;
    -b|--build-dir)
      # 指定自定义构建目录
      BUILD_DIR="${2:-}"
      shift 2
      ;;
    -c|--clean)
      # 标记为需要清理历史构建
      CLEAN=1
      shift # 消耗一个参数
      ;;
    -x|--cross)
      # 指定交叉编译工具链前缀
      CROSS_COMPILE="${2:-}"
      shift 2
      ;;
    -h|--help)
      # 打印帮助信息
      echo "Usage: $0 [-t Debug|Release] [-b <build_dir>] [-c] [-x <cross_prefix>]"
      exit 0
      ;;
    *)
      # 处理未知参数
      echo "Unknown argument: $1" >&2
      echo "Usage: $0 [-t Debug|Release] [-b <build_dir>] [-c] [-x <cross_prefix>]" >&2
      exit 2
      ;;
  esac
done

# 校验构建类型是否合法，CMake 默认支持这四种标准构建类型
if [[ "${BUILD_TYPE}" != "Debug" && "${BUILD_TYPE}" != "Release" && "${BUILD_TYPE}" != "RelWithDebInfo" && "${BUILD_TYPE}" != "MinSizeRel" ]]; then
  echo "Invalid build type: ${BUILD_TYPE}" >&2
  exit 2
fi

# 如果未指定构建目录，则根据当前构建类型自动生成一个默认的目录路径
if [[ -z "${BUILD_DIR}" ]]; then
  # 路径格式示例： out/Debug、out/Release（可执行文件即 out/Debug/arnics-os）
  # 交叉编译时追加工具链前缀，避免与主机构建的产物混在一起，例如 out/Debug-arm-linux-gnueabihf
  _BUILD_SUFFIX=""
  if [[ -n "${CROSS_COMPILE}" ]]; then
    _BUILD_SUFFIX="-${CROSS_COMPILE%-}"
  fi
  BUILD_DIR="${ROOT_DIR}/out/${BUILD_TYPE}${_BUILD_SUFFIX}"
fi

# 检查环境中是否安装了 cmake 工具
if ! command -v cmake >/dev/null 2>&1; then
  echo "cmake not found in PATH" >&2
  exit 127
fi

# 检查平台宏定义，确保 Linux 平台已启用
# 说明：projDefine.h 里的平台宏现在按编译器内建宏自动识别，Linux 分支中
# 仍然保留 "#define PLATFORM_LINUX 1" 一行，因此这里按文本匹配依然有效。
PROJ_DEFINE_H="${ROOT_DIR}/arnics-os/Inc/projDefine.h"
if [[ -f "${PROJ_DEFINE_H}" ]]; then
  # 使用正则匹配是否定义了 PLATFORM_LINUX 为 1
  if ! grep -Eq '^[[:space:]]*#define[[:space:]]+PLATFORM_LINUX[[:space:]]+1' "${PROJ_DEFINE_H}"; then
    echo "ProjDefine.h 未启用 PLATFORM_LINUX=1,请先切换平台宏后再编译:" >&2
    echo "  ${PROJ_DEFINE_H}" >&2
    exit 3
  fi
fi

# 收集 CMake 生成器参数
GEN_ARGS=()
# 优先检查并使用 Ninja 构建系统以提高编译速度
if command -v ninja >/dev/null 2>&1; then
  GEN_ARGS+=(-G Ninja)
fi

# 如果指定了交叉编译前缀，则配置对应的编译器及系统名
if [[ -n "${CROSS_COMPILE}" ]]; then
  GEN_ARGS+=(-DCMAKE_SYSTEM_NAME=Linux)
  GEN_ARGS+=(-DCMAKE_C_COMPILER="${CROSS_COMPILE}gcc")
  GEN_ARGS+=(-DCMAKE_CXX_COMPILER="${CROSS_COMPILE}g++")
fi

# 如果指定了 --clean 参数，则删除之前的构建目录
if [[ "${CLEAN}" -eq 1 ]]; then
  rm -rf "${BUILD_DIR}"
fi

# 运行 CMake 配置项目
# -S: 指定源码目录
# -B: 指定构建目录
# -D...: 强制加上 -pthread 以全局支持 POSIX 线程
cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" \
  "${GEN_ARGS[@]}" \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DCMAKE_C_FLAGS="-pthread" \
  -DCMAKE_CXX_FLAGS="-pthread" \
  -DCMAKE_EXE_LINKER_FLAGS="-pthread"

# 开始编译
# --build: 执行编译过程
# -j: 开启多线程编译，默认使用当前系统的 CPU 核心数，获取不到则默认使用 4 线程
cmake --build "${BUILD_DIR}" -- -j"$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)"

# 提示可执行文件的最终路径（构建目录即产物目录，方便直接运行）
BIN_PATH="${BUILD_DIR}/arnics-os"
if [[ -f "${BIN_PATH}" ]]; then
  echo ""
  echo "构建完成，可执行文件： ${BIN_PATH}"
  echo "  相对路径： ${BIN_PATH#"${ROOT_DIR}/"}"
fi
