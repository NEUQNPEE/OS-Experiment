cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(HelloWorld LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "e:/QT/6.2.4/mingw_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
aux_source_directory(./src srcs)

# 收集UI文件夹
aux_source_directory(./src/ui/filewindow_txt srcs)
aux_source_directory(./src/ui/mycomputer srcs)
aux_source_directory(./src/ui/task_manager srcs)
aux_source_directory(./src/ui/win srcs)

# 收集application文件夹
aux_source_directory(./src/application srcs)

# 收集ui_Interaction文件夹
aux_source_directory(./src/ui_Interaction srcs)

# 收集disk文件夹
aux_source_directory(./src/disk srcs)




# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement 
    ${srcs} 
) 
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)