

macro(set_lib_edv_properties target_name config_name bin_root bin_name lib_root lib_name)
set_property(TARGET ${target_name} APPEND PROPERTY IMPORTED_CONFIGURATIONS ${config_name})

# 指定动态库: 动态库文件
set(bin_file ${bin_root}/${config_name}/${bin_name})
set_target_properties(${target_name}
PROPERTIES
"IMPORTED_LOCATION_${config_name}" ${bin_file}
)

# 指定lib库文件
set(lib_file ${lib_root}/${config_name}/${lib_name})
set_target_properties(${target_name}
PROPERTIES
"IMPORTED_IMPLIB_${config_name}" ${lib_file}
)


# message("bin=${bin_file}")
# message("lib=${lib_file}")
endmacro()


# 指定根目录
set(lib_edv_root 
${CMAKE_CURRENT_LIST_DIR}/..
)


if (NOT TARGET oct::edv)
# message("1111")
# 建库
add_library(oct::edv SHARED IMPORTED)
endif()

if (TARGET oct::edv)
# message("1111")
# 指定头文件属性
set_property(TARGET oct::edv PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${lib_edv_root}/include)

set_lib_edv_properties( oct::edv DEBUG   ${lib_edv_root}/bin  "OctExeDllVersionD.dll" 	${lib_edv_root}/lib "OctExeDllVersionD.lib" )
set_lib_edv_properties( oct::edv RELEASE ${lib_edv_root}/bin  "OctExeDllVersion.dll" 	${lib_edv_root}/lib "OctExeDllVersion.lib"   )

# set_property(TARGET oct::edv APPEND PROPERTY IMPORTED_CONFIGURATIONS Debug)
# set_property(TARGET oct::edv APPEND PROPERTY IMPORTED_CONFIGURATIONS Release)

# set_target_properties(oct::edv 
# PROPERTIES 
# "IMPORTED_LOCATION_DEBUG" ${lib_edv_root_dir}/bin/Debug/OctExeDllVersionD.dll
# "IMPORTED_IMPLIB_DEBUG" ${lib_edv_root_dir}/lib/Debug/OctExeDllVersionD.lib
# )

# set_target_properties(oct::edv 
# PROPERTIES 
# "IMPORTED_LOCATION_RELEASE" ${lib_edv_root_dir}/bin/Release/OctExeDllVersion.dll
# "IMPORTED_IMPLIB_RELEASE" ${lib_edv_root_dir}/lib/Release/OctExeDllVersion.lib
# )

endif()















