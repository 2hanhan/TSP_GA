cmake_minimum_required(VERSION 3.10)
project(GA)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

#包含路径：<头文件include>
include_directories(
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/include
)

#将cpp文件生成lib，不需要包含<头文件include>
add_library(lib
src/GA.cpp)

#生成可执行文件 并且链接lib
add_executable(main main.cpp)
target_link_libraries(main lib)
