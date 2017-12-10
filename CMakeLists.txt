cmake_minimum_required(VERSION 3.9.0)

project(nobody VERSION 0.1 LANGUAGES CXX)

add_executable(nobody src/main.cc src/euler_integrator.cc src/glut_app.cc)

set_property(TARGET nobody PROPERTY CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_link_libraries(nobody OpenGL::GL OpenGL::GLU GLUT::GLUT)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(nobody Eigen3::Eigen)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(nobody OpenMP::OpenMP_CXX)
endif()
