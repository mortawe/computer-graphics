cmake_minimum_required(VERSION 3.15)
project(computer-graphics)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(/usr/local/include/yaml-cpp)

add_executable(lab1 1/main.cpp)
target_link_libraries(lab1 ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(lab2 2/main.cpp)
target_link_libraries(lab2 ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(lab3 3/main.cpp)
target_link_libraries(lab3 ${GLFW_LIBRARIES}  ${OPENGL_LIBRARIES})

add_executable(lab4 4/main.cpp)
target_link_libraries(lab4 ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(lab5 5/main.cpp)
target_link_libraries(lab5 ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(lab6 6/main.cpp)
target_link_libraries(lab6 -lyaml-cpp ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

add_executable(lab7 7/main.cpp)
target_link_libraries(lab7 -lyaml-cpp ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

add_executable(lab8 8/main.cpp)
target_link_libraries(lab8 -lyaml-cpp -lGLEW  ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})
