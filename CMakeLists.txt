cmake_minimum_required(VERSION 3.0.0)

project(nobody VERSION 0.1)

option(DEVELOPER_MODE "Use compiler flags for development and diagnostics" OFF)
if (DEVELOPER_MODE)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -pedantic -ansi")
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror-all -Wall -ansi")
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic")
  endif()
endif (DEVELOPER_MODE)

add_executable(nobody src/main.cc)

set_property(TARGET nobody PROPERTY CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(nobody ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(nobody Eigen3::Eigen)