cmake_minimum_required(VERSION 3.15)
project(computer-graphics)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

add_executable(lab1 1/main.cpp)
target_link_libraries(lab1 ${GLFW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

add_executable(lab2 2/main.cpp)
target_link_libraries(lab2 ${GLFW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

add_executable(lab3 3/main.cpp)
target_link_libraries(lab3 ${GLFW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

add_executable(lab4 4/main.cpp)
target_link_libraries(lab4 ${GLFW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

