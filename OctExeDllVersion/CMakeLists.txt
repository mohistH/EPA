cmake_minimum_required(VERSION 3.18)
project(OctExeDllVersion)

# 指定源码目录
set(oct_edv_root
	${CMAKE_CURRENT_SOURCE_DIR}/src
)






# message(${common_cmake_root})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_DEBUG_POSTFIX D) 

	# 有这些文件需要纳入编译: 不推荐这样用， 应该一个一个文件的手动加入到这里
	file(GLOB_RECURSE 
	edv_files 
	${oct_edv_root}/*.h
	${oct_edv_root}/*.cpp
	)


	# 建库
	add_library(${PROJECT_NAME} 
	SHARED
	${edv_files}
	)

	SET(CMAKE_MFC_FLAG 2)
	# add_definitions(-DUNICODE -D_UNICODE)

	# 指定头文件搜索路径
	target_include_directories(${PROJECT_NAME}
	PRIVATE ${oct_edv_root}
	)

	# 增加预处理器定义#_CRT_SECURE_NO_WARNINGS
	target_compile_definitions(${PROJECT_NAME}
		PRIVATE _CRT_SECURE_NO_WARNINGS
		PRIVATE UNICODE _UNICODE
	)

	# 需要链接windows库
	target_link_libraries(${PROJECT_NAME}
	PRIVATE version
	PRIVATE shlwapi
	)

	# 将输出头文件拷贝到include目录
	copy_file_macro(${oct_edv_root}/IExeVersion.h ${pir})

	# 拷贝到输出目录
	copy_folder_macro(${CMAKE_CURRENT_SOURCE_DIR}/cmake ${common_cmake_root})



	# 如果是windows，设置管理员权限。注意使用if需要配合endif使用
	if (CMAKE_SYSTEM_NAME MATCHES "Windows")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/level='requireAdministrator' /uiAccess='true'" )
	endif()

	# 关闭控制台
	if (CMAKE_SYSTEM_NAME MATCHES "Windows")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()
endif()






























