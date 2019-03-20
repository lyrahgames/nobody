cmake_minimum_required(VERSION 3.13)
project(nobody VERSION 0.1 LANGUAGES CXX)
enable_testing()

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS
      "Setting build type to '${default_build_type}' as none was specified."
  )
  set(
    CMAKE_BUILD_TYPE "${default_build_type}" CACHE
    STRING "Choose the type of build." FORCE
  )
else()
  message(
    STATUS
      "Setting build type to '${CMAKE_BUILD_TYPE}'."
  )
endif()

add_subdirectory(nobody)
add_subdirectory(tests)