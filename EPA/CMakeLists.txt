
cmake_minimum_required(VERSION 3.23)

project(EPA)

set(CMAKE_PREFIX_PATH 
${CMAKE_PREFIX_PATH}
"C:/major/development/tools/qt/5.14/install/5.14.2/msvc2015_64"
${common_cmake_root}
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${linguist_cmake})

# SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

set(QT_VERSION 5)
# set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Core Gui Widgets Sql)

# CMAKE包查找
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS_QUALIFIED} REQUIRED)
find_package(oct_edv REQUIRED)

# 指定源码目录
set(proj_root
	${CMAKE_CURRENT_SOURCE_DIR}/src
)


# 有这些文件需要纳入编译: 不推荐这样用， 应该一个一个文件的手动加入到这里
file(GLOB_RECURSE 
epa_files 
${proj_root}/*.h
${proj_root}/*.cpp
${proj_root}/*.inl
)

# 设置build输出目录 PROJECT_BINARY_DIR
set(project_bin_root
	${CMAKE_CURRENT_SOURCE_DIR}/resource
)

# 目标目录下的qrc
set(project_qrc
${project_bin_root}/epa.qrc
)



# 拷贝资源到输出目录

# 创建可执行程序项目
add_executable(${PROJECT_NAME} 
${epa_files}
${project_qrc}
)

# 翻译文件
set(proj_ts_file 
	${CMAKE_CURRENT_SOURCE_DIR}/lang/zh_CN.ts
)

# 添加翻译
# 参数1 项目名称， 参数2：输出目录下的qrc, 参数3： ts文件， 参数4：， 参数5：要翻译哪些文件夹
create_qm_file(${PROJECT_NAME} ${project_qrc} ${proj_ts_file} ${project_bin_root}/lang ${proj_root})



# 指定头文件搜索路径
target_include_directories(${PROJECT_NAME}
PRIVATE ${proj_root}
)

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
	PRIVATE Qt${QT_VERSION}::Sql Qt${QT_VERSION}::Widgets Qt${QT_VERSION}::Core Qt${QT_VERSION}::Gui
)

# 需要链接 自己 专门修改exe和dll的动态库
target_link_libraries(${PROJECT_NAME}
	PUBLIC oct::edv
	)


# 拷贝配置文件到输出路径
copy_folder_macro(${CMAKE_CURRENT_SOURCE_DIR}/fr ${pbd})
copy_folder_macro(${CMAKE_CURRENT_SOURCE_DIR}/fr ${pbr})
# query
copy_folder_macro(${CMAKE_CURRENT_SOURCE_DIR}/vswhere ${pbd})
copy_folder_macro(${CMAKE_CURRENT_SOURCE_DIR}/vswhere ${pbr})



# 如果是windows，设置管理员权限。注意使用if需要配合endif使用
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/level='requireAdministrator'" )
endif()

# 关闭控制台
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()














