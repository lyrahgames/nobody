cmake_minimum_required(VERSION 3.0.0)

project(nobody VERSION 0.1)

add_executable(nobody src/main.cc src/euler_integrator.cc src/glut_app.cc)

set_property(TARGET nobody PROPERTY CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(nobody ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(nobody Eigen3::Eigen)