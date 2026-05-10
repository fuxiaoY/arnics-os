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
# 1) 目标(可执行文件/库)
# =========================
#
# 这里创建可执行文件目标 arnics-os。
# 说明：
# - add_executable() 只负责“创建目标”，不建议把源文件直接塞进去
#
add_executable(arnics-os)

#
# =========================
# 2) 头文件搜索路径(include)
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
# 3) 源文件列表(arnics-os以外的文件，显式列出)
# =========================
#
# 显式列出 .c/.cpp 文件
#
set(ARNICS_OS_SOURCES
  "${PROJECT_SOURCE_DIR}/main.c"
)

# 设置排除路径(用于源文件搜集过滤)
set(ARNICS_OS_EXCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/arnics-os/document"
  "${PROJECT_SOURCE_DIR}/arnics-os/thirdParty/packages"
# 以下为支持平台，现已不需要手动排除
#  "${PROJECT_SOURCE_DIR}/arnics-os/rtosInterface/freertos"
#  "${PROJECT_SOURCE_DIR}/arnics-os/rtosInterface/linux"
#  "${PROJECT_SOURCE_DIR}/arnics-os/rtosInterface/win"
)


#
# =========================
# 4) 自动搜集源文件
# =========================
#
# 功能：新增 .c 都手动加到 ARNICS_OS_GLOB_SOURCES
# 注意：它依赖 CMake 重新配置来识别新增文件；
#
# CONFIGURE_DEPENDS 可让多数场景自动触发重配:
#- 让 CMake 把这次 glob 到的“文件列表”当成 配置阶段的依赖 。
#- 这样当你 新增/删除 匹配的源文件(比如新加一个 .c )，下次构建时构建系统会触发 自动重新运行 CMake 配置(re-configure) ，从而更新源文件列表。

file(GLOB_RECURSE ARNICS_OS_GLOB_SOURCES
  CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/arnics-os/**/*.c"
  "${PROJECT_SOURCE_DIR}/arnics-os/**/*.cpp"
)
# =========================
# 5) 从递归搜集到的源文件列表中，排除 ARNICS_OS_EXCLUDE_DIRS 指定目录下的文件
# =========================
# 1) 统一为 CMake 风格路径(/)
# 2) 转义路径中的正则特殊字符，避免正则匹配误伤
# 3) 用 list(FILTER ... EXCLUDE REGEX ...) 过滤掉该目录及其子目录下的文件
foreach(exclude_dir IN LISTS ARNICS_OS_EXCLUDE_DIRS)
  file(TO_CMAKE_PATH "${exclude_dir}" exclude_dir_cmake)
  string(REGEX REPLACE "([][+.*^$(){}|\\\\])" "\\\\\\1" exclude_dir_escaped "${exclude_dir_cmake}")
  list(FILTER ARNICS_OS_GLOB_SOURCES EXCLUDE REGEX "^${exclude_dir_escaped}/")
endforeach()

# 功能：把“手动指定的源文件 + 过滤后的 glob 源文件”合并后挂到目标 arnics-os 上
list(APPEND ARNICS_OS_SOURCES ${ARNICS_OS_GLOB_SOURCES})
target_sources(arnics-os PRIVATE ${ARNICS_OS_SOURCES})
#
# =========================
# 5) 编译标准(按需改)
# =========================
#
# 当前项目主要是 C(main.c 等)，先设一个通用的 C 标准。
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
# 6) 编码/字符集(解决中文注释/字符串在 MSVC 下的编码问题)
# =========================
#
# 你看到的“该文件包含不能在当前代码页(936)中表示的字符”通常和字符集有关。
# 对 MSVC 来说，/utf-8 可以让编译器按 UTF-8 读取源码(含中文注释/字符串)。
#
if (MSVC)
  target_compile_options(arnics-os PRIVATE /utf-8)
endif()

if (UNIX)
  target_link_libraries(arnics-os PRIVATE m)
endif()


