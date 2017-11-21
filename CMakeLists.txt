cmake_minimum_required(VERSION 3.0.0)

project(nobody)

add_executable(nobody src/main.cc)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(nobody ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})