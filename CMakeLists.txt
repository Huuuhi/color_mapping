cmake_minimum_required(VERSION 3.12)
project(color_mapping)


# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(PRJ_SRC_LIST color_mapping.cpp glad.c)
set(CON_SRC_LIST contour.cpp glad.c)

# 添加可执行文件
add_executable(color_mapping ${PRJ_SRC_LIST})
add_executable(contour ${CON_SRC_LIST})

# 在项目文件夹中搜索头文件路径
set(GLAD_INCLUDE_DIR glad/include)
set(GLFW_INCLUDE_DIR glfw-3.3.8.bin.WIN32/include)
set(GLM_INCLUDE_DIR glm)
set(GLEW_INCLUDE_DIR glew-2.1.0/include)

# 添加头文件路径
target_include_directories(color_mapping PRIVATE ${CMAKE_SOURCE_DIR} ${GLAD_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
target_include_directories(contour PRIVATE ${CMAKE_SOURCE_DIR} ${GLAD_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})

# 在项目文件夹中搜索库文件路径
find_library(GLFW_LIB glfw3 PATHS ${CMAKE_SOURCE_DIR}/glfw-3.3.8.bin.WIN32/lib-vc2019)
find_library(GLEW_LIB glew32s PATHS ${CMAKE_SOURCE_DIR}/glew-2.1.0/lib/Release/Win32)

# 添加链接的库
target_link_libraries(color_mapping ${GLFW_LIB} ${GLEW_LIB})
target_link_libraries(contour ${GLFW_LIB} ${GLEW_LIB})

# 设置输出目录
set_target_properties(color_mapping PROPERTIES
    CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
)
set_target_properties(contour PROPERTIES
    CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
)

