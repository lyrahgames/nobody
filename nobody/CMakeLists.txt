find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(nobody)
target_sources(nobody
  PRIVATE
    main.cpp
    euler_integrator.cpp
    glut_app.cpp
    runge_kutta_integrator.cpp
    particle_loader.cpp
    orthonormal_frame.cpp
    camera.cpp
    energy.cpp
)
target_compile_features(nobody
  PRIVATE
    cxx_std_17
)
target_include_directories(nobody
  PRIVATE
    ..
)
target_link_libraries(nobody
  PRIVATE
    Eigen3::Eigen
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    OpenMP::OpenMP_CXX
)