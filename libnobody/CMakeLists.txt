add_library(libnobody INTERFACE)
target_sources(libnobody
  INTERFACE
    particle.hpp
)
target_include_directories(libnobody
  INTERFACE
    ..
)
target_compile_features(libnobody
  INTERFACE
    cxx_std_17
)
add_library(nobody::libnobody ALIAS libnobody)


find_package(doctest REQUIRED)
find_package(Eigen3 REQUIRED)
add_executable(libnobody-internal-tests)
target_sources(libnobody-internal-tests
  PRIVATE
    main.test.cpp
    particle.test.cpp
)
target_compile_features(libnobody-internal-tests
  PRIVATE
    cxx_std_17
)
target_link_libraries(libnobody-internal-tests
  PRIVATE
    doctest::doctest
    Eigen3::Eigen
    nobody::libnobody
)
add_test(libnobody-internal-tests libnobody-internal-tests)