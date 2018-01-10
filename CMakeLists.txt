cmake_minimum_required(VERSION 3.9.0)

project(nobody VERSION 0.1 LANGUAGES CXX)

add_executable(nobody
  src/main.cc
  src/euler_integrator.cc
  src/glut_app.cc
  src/runge_kutta_integrator.cc
  src/particle_loader.cc
  src/orthonormal_frame.cc
  src/camera.cc
)

set_property(TARGET nobody PROPERTY CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenMP)

target_link_libraries(nobody OpenGL::GL OpenGL::GLU GLUT::GLUT)
target_link_libraries(nobody Eigen3::Eigen)

if(OpenMP_CXX_FOUND)
  target_link_libraries(nobody OpenMP::OpenMP_CXX)
endif()
