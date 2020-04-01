cmake_minimum_required(VERSION 3.15)
project(computer-graphics)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

pkg_search_module(GLU REQUIRED glu)

include_directories(${GLFW_INCLUDE_DIRS})

add_executable(lab1 1/main.cpp)
target_link_libraries(lab1 ${GLFW_LIBRARIES})
target_link_libraries(lab1 ${GLFW_STATIC_LIBRARIES})

add_executable(lab2 2/main.cpp)
target_link_libraries(lab2 ${GLFW_LIBRARIES})
target_link_libraries(lab2 ${GLFW_STATIC_LIBRARIES})

add_executable(lab3 3/main.cpp)
target_link_libraries(lab3 ${GLFW_LIBRARIES})
target_link_libraries(lab3 ${GLFW_STATIC_LIBRARIES})


