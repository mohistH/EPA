cmake_minimum_required(VERSION 3.18)
project(OCT_EPA)

# 设置通用cmake
set( common_root
        ${CMAKE_CURRENT_SOURCE_DIR}/Common
)

set( common_output_root 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 加载common 中的 cmake 脚本设置
set(common_cmake         ${common_root}/common.cmake)
set(linguist_cmake      ${common_root}/linguist.cmake)
include(${common_cmake})


# option 
option(oct_epa_enabled "OCT_EPA PROJECT IS ENABLED" ON)
option(oct_exe_dll_version_enabled "OCT_EPA PROJECT IS ENABLED" ON)


# 主工程
if (oct_epa_enabled)
        add_subdirectory("EPA")
endif(oct_epa_enabled)

# 动态库项目
if (oct_exe_dll_version_enabled)
        add_subdirectory("OctExeDllVersion")
endif(oct_exe_dll_version_enabled)