# CMakeList.txt: arnics-os 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required(VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(arnics-os LANGUAGES C CXX)

#
# =========================
# 1) 目标（可执行文件/库）
# =========================
#
# 这里创建可执行文件目标 arnics-os。
# 说明：
# - add_executable() 只负责“创建目标”，不建议把源文件直接塞进去
# - 源文件使用 target_sources() 添加，后续维护更清晰（新增/删除文件就改这一块）
#
add_executable(arnics-os)

#
# =========================
# 2) 头文件搜索路径（include）
# =========================
#
# 你说“所有 include 都以 arnics-os 目录作为根目录”：
#   例如：#include "core/arnicsCore.h"
#
# 为了支持这种写法，把 <工程根>/arnics-os 作为 include 根目录即可。
# 如果你还有其它“根目录”写法，也可以继续往这里加。
#
target_include_directories(arnics-os
  PRIVATE
    "${PROJECT_SOURCE_DIR}/arnics-os"
    "${PROJECT_SOURCE_DIR}/arnics-os/Inc"
)

#
# =========================
# 3) 源文件列表（推荐：显式列出）
# =========================
#
# 推荐方式：显式列出 .c/.cpp 文件
# 好处：可控、改动可追踪、跨平台/跨生成器最稳定
#
set(ARNICS_OS_SOURCES
  "${PROJECT_SOURCE_DIR}/main.c"
)

# 设置排除路径（用于源文件搜集过滤，便于后期拓展）
set(ARNICS_OS_EXCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/arnics-os/document"
#  "${PROJECT_SOURCE_DIR}/arnics-os/rtosInterface/freertos"
#  "${PROJECT_SOURCE_DIR}/arnics-os/rtosInterface/linux"
#  "${PROJECT_SOURCE_DIR}/arnics-os/rtosInterface/win"
)

file(GLOB_RECURSE ARNICS_OS_GLOB_SOURCES
  CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/arnics-os/**/*.c"
  "${PROJECT_SOURCE_DIR}/arnics-os/**/*.cpp"
)

foreach(exclude_dir IN LISTS ARNICS_OS_EXCLUDE_DIRS)
  file(TO_CMAKE_PATH "${exclude_dir}" exclude_dir_cmake)
  string(REGEX REPLACE "([][+.*^$(){}|\\\\])" "\\\\\\1" exclude_dir_escaped "${exclude_dir_cmake}")
  list(FILTER ARNICS_OS_GLOB_SOURCES EXCLUDE REGEX "^${exclude_dir_escaped}/")
endforeach()

list(APPEND ARNICS_OS_SOURCES ${ARNICS_OS_GLOB_SOURCES})
target_sources(arnics-os PRIVATE ${ARNICS_OS_SOURCES})
#
# =========================
# 4) 可选：自动搜集源文件（更省事，但不如显式稳定）
# =========================
#
# 如果你不想每次新增 .c 都手动加到 ARNICS_OS_SOURCES，可以改用下面这种方式：
# 注意：它依赖 CMake 重新配置来识别新增文件；CONFIGURE_DEPENDS 可让多数场景自动触发重配
#
# file(GLOB_RECURSE ARNICS_OS_SOURCES
#   CONFIGURE_DEPENDS
#   "${PROJECT_SOURCE_DIR}/main.c"
#   "${PROJECT_SOURCE_DIR}/arnics-os/**/*.c"
#   "${PROJECT_SOURCE_DIR}/arnics-os/**/*.cpp"
# )
# target_sources(arnics-os PRIVATE ${ARNICS_OS_SOURCES})

#
# =========================
# 5) 编译标准（按需改）
# =========================
#
# 当前项目主要是 C（main.c 等），先设一个通用的 C 标准。
# 如果后续你加入 C++ 文件，再用 CXX_STANDARD。
#
set_property(TARGET arnics-os PROPERTY C_STANDARD 11)
set_property(TARGET arnics-os PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET arnics-os PROPERTY C_EXTENSIONS OFF)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET arnics-os PROPERTY CXX_STANDARD 20)
  set_property(TARGET arnics-os PROPERTY CXX_STANDARD_REQUIRED ON)
  set_property(TARGET arnics-os PROPERTY CXX_EXTENSIONS OFF)
endif()

#
# =========================
# 6) 编码/字符集（解决中文注释/字符串在 MSVC 下的编码问题）
# =========================
#
# 你看到的“该文件包含不能在当前代码页(936)中表示的字符”通常和字符集有关。
# 对 MSVC 来说，/utf-8 可以让编译器按 UTF-8 读取源码（含中文注释/字符串）。
#
if (MSVC)
  target_compile_options(arnics-os PRIVATE /utf-8)
endif()

if (UNIX)
  target_link_libraries(arnics-os PRIVATE m)
endif()

# TODO: 如有需要，请添加测试并安装目标。
