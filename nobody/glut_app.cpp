#include <nobody/glut_app.hpp>

#include <GL/glut.h>
#include <Eigen/Dense>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <vector>

#include <nobody/camera.hpp>
#include <nobody/energy.hpp>
#include <nobody/euler_integrator.hpp>
#include <nobody/particle_loader.hpp>
#include <nobody/runge_kutta_integrator.hpp>

namespace {

// const data
constexpr float rad_to_degree_scale = 180.0f / M_PI;
constexpr unsigned char glut_key_esc = 27;
constexpr unsigned char glut_key_space = 32;
constexpr unsigned char glut_key_p = 112;
constexpr unsigned char glut_key_l = 108;
constexpr unsigned char glut_key_a = 97;
constexpr unsigned char glut_key_e = 101;
constexpr unsigned char glut_key_s = 115;
constexpr unsigned char glut_key_r = 114;

// static data
std::vector<nobody::particle> particle_vector;
std::vector<nobody::particle> particle_start_data;
float time_step = 0.0005f;
float camera_altitude = 0.0f;
float camera_azimuth = 0.0f;
float camera_distance = 10.0f;
float world_cube_size = 1.0f;
int key_modifiers;
int fps_frame_count = 0;
float fps_time_bound = 1.0f;
std::chrono::time_point<std::chrono::system_clock> fps_current_time =
    std::chrono::system_clock::now();
std::chrono::time_point<std::chrono::system_clock> fps_last_time =
    std::chrono::system_clock::now();
int old_mouse_x{};
int old_mouse_y{};
bool mouse_left_pressed = false;
bool mouse_right_pressed = false;
bool paused = true;
bool render_particles = true;
int render_particle_paths = 1;
nobody::Camera camera(400, 400, M_PI_2);
nobody::Orthonormal_frame world =
    nobody::blender_orthonormal_frame(Eigen::Vector3f(0, 0, 0));
float time = 0.0f;
float time_tmp = 0.0f;
std::vector<std::list<Eigen::Vector3f>> particle_path_data;

nobody::integrator_type integrator = nobody::leapfrog_integrator;
bool use_adaptive_integrator = false;
int selection = 0;
std::fstream file;
int iteration = 0;
bool track_results = false;

// helper-function declarations and definitions
void initialize(int argc, char** argv);
void execute();
void free();
void render();
void resize(int width, int height);
void idle();
void process_normal_keys(unsigned char key, int x, int y);
void process_special_keys(int key, int x, int y);
void process_mouse_buttons(int button, int button_state, int x, int y);
void process_mouse_move(int x, int y);
void process_passive_mouse_move(int x, int y);
void process_mouse_wheel(int wheel, int direction, int x, int y);
void compute_camera_frame();

void initialize(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
  // glutInitWindowPosition(100, 100);
  glutInitWindowSize(400, 400);
  glutCreateWindow("nobody: n-body simulator");

  // glutCloseFunc(free);
  std::atexit(free);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  glutKeyboardFunc(process_normal_keys);
  glutSpecialFunc(process_special_keys);
  glutMouseFunc(process_mouse_buttons);
  glutMotionFunc(process_mouse_move);
  glutPassiveMotionFunc(process_passive_mouse_move);
  // glutMouseWheelFunc(process_mouse_wheel);

  // glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
  // GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_POINT_SMOOTH);
  // glEnable(GL_DEPTH_TEST);
  glPointSize(8.0f);
  // glEnable(GL_DEPTH_TEST);

  if (2 != argc) {
    // std::cout << "GEBE EINEN Dateipfad AN DU ARSCHLOCH!" << std::endl;
    // exit(-1);
    // initialize random particle data
    particle_vector.resize(2000);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    for (int i = 0; i < static_cast<int>(particle_vector.size()); ++i) {
      particle_vector[i].position =
          1.5f * Eigen::Vector3f(dist(rng), dist(rng), dist(rng));
      particle_vector[i].velocity = Eigen::Vector3f(0, -5, 0);
      // particle_vector[i].velocity = Eigen::Vector3f(0, 0, 0);
      // particle_vector[i].velocity =
      //     particle_vector[i].position.cross(Eigen::Vector3f(0, 0, 1));
      // particle_vector[i].velocity.normalize();
      // particle_vector[i].velocity *= 0.5f;
      // 0.1f * Eigen::Vector3f(dist(rng), dist(rng), dist(rng));
      particle_vector[i].mass = 100.0f * (0.5 * dist(rng) + 1.1f);
    }

    particle_vector[0].mass = 1e6f;
    particle_vector[0].position = Eigen::Vector3f(-4, 0, 0);
    particle_vector[0].velocity = Eigen::Vector3f(0, 1.1, 0);

    // particle_vector[1].mass = 1e6f;
    // particle_vector[1].position = Eigen::Vector3f(10, 0, 0);
    // particle_vector[1].velocity = Eigen::Vector3f(0, -1, 0);
  } else {
    particle_vector = nobody::particle_system(std::string(argv[1]));
  }

  particle_start_data = particle_vector;

  particle_path_data.resize(particle_vector.size());
  for (std::size_t i = 0; i < particle_path_data.size(); ++i) {
    particle_path_data[i].clear();
    particle_path_data[i].push_back(particle_vector[i].position);
  }

  file.open("results.txt", std::ios::out | std::ios::trunc);
}

void execute() { glutMainLoop(); }

void free() { std::cout << "Glut_app cleaned up." << std::endl; }

void resize(int width, int height) {
  camera.set_screen_resolution(width, height);
  compute_camera_frame();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, camera.screen_width(), camera.screen_height());
  gluPerspective(camera.opengl_field_of_view(), camera.aspect_ratio(), 1e-5f,
                 1e5f);
  glMatrixMode(GL_MODELVIEW);
}

void render() {
  // compute camera position
  // camera_direction = Eigen::Vector3f(
  //     cosf(camera_azimuth) * cosf(camera_altitude), sinf(camera_altitude),
  //     sinf(camera_azimuth) * cosf(camera_altitude));
  // camera_position = world_center + camera_distance * camera_direction;

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // glLoadIdentity();
  // gluLookAt(camera_position(0), camera_position(1), camera_position(2),
  //           world_center(0), world_center(1), world_center(2), world_up(0),
  //           world_up(1), world_up(2));

  glLineWidth(1.0f);
  glColor3f(0, 0, 0);
  glutWireCube(world_cube_size);

  // we have to order the points

  if (1 == render_particle_paths) {
    glLineWidth(1.5f);
    for (std::size_t i = 0; i < particle_path_data.size(); ++i) {
      const auto& particle_path = particle_path_data[i];
      glBegin(GL_LINE_STRIP);
      for (const auto& point : particle_path) glVertex3fv(point.data());
      glVertex3fv(particle_vector[i].position.data());
      glEnd();
    }
  } else if (2 == render_particle_paths) {
    glLineWidth(1.5f);
    const auto& particle_path = particle_path_data[selection];
    glBegin(GL_LINE_STRIP);
    for (const auto& point : particle_path) glVertex3fv(point.data());
    glVertex3fv(particle_vector[selection].position.data());
    glEnd();
  }

  if (render_particles) {
    for (int i = 0; i < static_cast<int>(particle_vector.size()); ++i) {
      const float distance =
          (particle_vector[i].position - camera.position()).norm();

      const float brightness = std::min(0.7f, 1.0f / (distance * distance));

      glPointSize(std::max(0.007f / camera.pixel_size() *
                               (cbrtf(particle_vector[i].mass) / (distance)),
                           5.0f));

      glBegin(GL_POINTS);
      glColor3f(brightness, brightness, brightness);
      glVertex3f(particle_vector[i].position(0), particle_vector[i].position(1),
                 particle_vector[i].position(2));
      glEnd();
    }
  }

  glutSwapBuffers();
}

void idle() {
  if (paused) return;

  // compute fps
  fps_current_time = std::chrono::system_clock::now();
  fps_frame_count++;
  const float delta_time =
      std::chrono::duration<float>(fps_current_time - fps_last_time).count();

  const float kinetic = nobody::kinetic_energy(particle_vector);
  const float potential = nobody::potential_energy(particle_vector);
  const Eigen::Vector3f angular_momentum =
      nobody::angular_momentum(particle_vector);

  if (delta_time >= 1.0f) {
    std::cout << "frame time: "
              << delta_time / static_cast<float>(fps_frame_count)
              << "s\tfps: " << static_cast<float>(fps_frame_count) / delta_time
              << std::endl;
    std::cout << "simulation time = " << time << " years" << std::endl;
    std::cout << "time step = " << time_step << " years" << std::endl;
    std::cout << "kinetic energy = " << kinetic << std::endl
              << "potential energy = " << potential << std::endl
              << "total energy = " << kinetic + potential << std::endl;
    std::cout << "angular momentum = (" << angular_momentum.transpose() << ")"
              << std::endl
              << "angular momentum norm = " << angular_momentum.norm()
              << std::endl;
    std::cout << std::endl;
    fps_frame_count = 0;
    fps_last_time = fps_current_time;
  }

  if (use_adaptive_integrator)
    adaptive_integrator(&particle_vector, time_step, integrator);
  else {
    integrator(&particle_vector, time_step);

    // time_tmp += time_step;
    // if (time_tmp >= 5.0f) {
    //   time += time_tmp;
    //   time_tmp = 0.0f;
    //   // paused = true;
    //   // std::cout << "time = " << time << " years" << std::endl;
    // }
    iteration++;
    time += time_step;

    if (track_results) {
      file << iteration << "\t" << time << "\t" << kinetic << "\t" << potential
           << "\t" << kinetic + potential << "\t"
           << angular_momentum.transpose() << "\t" << angular_momentum.norm()
           << "\n";
    }
  }

  // euler_integrator(particle_vector.data(),
  //                  static_cast<int>(particle_vector.size()), time_step);
  // rk4_integrator(particle_vector.data(),
  // static_cast<int>(particle_vector.size()), time_step);
  // rk4_integrator(&particle_vector, time_step);
  // leapfrog_integrator(&particle_vector, time_step);
  // leapfrog_adaptive_integrator(&particle_vector, time_step);
  // euler_integrator(&particle_vector, time_step);

  // world.set_origin(particle_vector[0].position);

  for (std::size_t i = 0; i < particle_path_data.size(); ++i) {
    auto& particle_path = particle_path_data[i];

    if (particle_path.back() != particle_vector[i].position) {
      if (particle_path.size() < 2) {
        particle_path.push_back(particle_vector[i].position);
      } else {
        auto p = particle_path.end();
        --p;
        --p;
        const Eigen::Vector3f last = particle_path.back() - *p;
        const Eigen::Vector3f next =
            particle_vector[i].position - particle_path.back();
        const float distance = next.norm();
        const float cos_angle = next.dot(last) / (distance * last.norm());
        if (cos_angle < 0.9999f) {
          particle_path.push_back(particle_vector[i].position);
          // std::cout << "added position to path: " << particle_path.size()
          // << std::endl;
        }

        if (particle_path.size() > 512) particle_path.pop_front();
      }
    }
  }

  glutPostRedisplay();
}

void process_normal_keys(unsigned char key, int x, int y) {
  key_modifiers = glutGetModifiers();

  switch (key) {
    case '0':
      selection = 0;
      break;
    case '1':
      selection = 1;
      break;
    case '2':
      selection = 2;
      break;
    case '3':
      selection = 3;
      break;
    case '4':
      selection = 4;
      break;
    case '5':
      selection = 5;
      break;
    case '6':
      selection = 6;
      break;
    case '7':
      selection = 7;
      break;
    case '8':
      selection = 8;
      break;
    case '9':
      selection = 9;
      break;

    case 'x' - 0x60:
      particle_vector[selection].mass = 0.0f;
      std::cout << "Mass of particle " << selection << " set to 0 EM."
                << std::endl;
      break;

    case 'm' - 0x60:
      particle_vector[selection].mass *= 1.5f;
      std::cout << "Mass of particle " << selection << " set to "
                << particle_vector[selection].mass << " EM." << std::endl;
      break;

    case 'L':
      std::cout << "Integrator set to leapfrog_integrator." << std::endl;
      integrator = nobody::leapfrog_integrator;
      break;

    case 'A':
      use_adaptive_integrator = !use_adaptive_integrator;
      if (use_adaptive_integrator)
        std::cout << "Integration is set to adaptive time step mode."
                  << std::endl;
      else
        std::cout << "Integrator is set to non-adaptive time step mode."
                  << std::endl;
      break;

    case 'R':
      std::cout << "Integrator set to rk4_integrator." << std::endl;
      integrator = nobody::rk4_integrator;
      break;

    case 'E':
      integrator = nobody::euler_integrator;
      std::cout << "Integrator set to euler_integrator." << std::endl;
      break;

    case 'S':
      integrator = nobody::symplectic_euler_integrator;
      std::cout << "Integrator set to symplectic_euler_integrator."
                << std::endl;
      break;

    case glut_key_esc:
      exit(0);
      break;

    case glut_key_space:
      paused = !paused;
      break;

    case 'p':
      render_particles = !render_particles;
      break;

    case 'l':
      render_particle_paths = (render_particle_paths + 1) % 3;
      break;

    case 'r':
      particle_vector = particle_start_data;
      for (std::size_t i = 0; i < particle_path_data.size(); ++i) {
        particle_path_data[i].clear();
        particle_path_data[i].push_back(particle_vector[i].position);
      }
      time = 0.0f;
      file.close();
      file.open("results.txt", std::ios::out | std::ios::trunc);
      break;

    case 't':
      track_results = !track_results;
      std::cout << "results tracking set to: " << std::boolalpha
                << track_results << std::endl;
      break;

    case 'v':
      world = nobody::opengl_orthonormal_frame(Eigen::Vector3f::Zero());
      compute_camera_frame();
      break;

    case 'w':
      world = nobody::blender_orthonormal_frame(Eigen::Vector3f::Zero());
      compute_camera_frame();
      break;

    case 'e':
      const float kinetic = nobody::kinetic_energy(particle_vector);
      const float potential = nobody::potential_energy(particle_vector);
      std::cout << "kinetic energy = " << kinetic << std::endl
                << "potential energy = " << potential << std::endl
                << "total energy = " << kinetic + potential << std::endl
                << "time = " << time << std::endl;

      const Eigen::Vector3f relative_position =
          particle_vector[selection].position - particle_vector[0].position;
      const float distance = relative_position.norm();
      const float potential_selection =
          -nobody::gravitation_const * particle_vector[0].mass *
          particle_vector[selection].mass / distance;
      const float kinetic_selection =
          0.5f * particle_vector[selection].mass *
          particle_vector[selection].velocity.squaredNorm();
      const float energy_selection = kinetic_selection + potential_selection;
      const Eigen::Vector3f angular_momentum_selection =
          particle_vector[selection].mass *
          relative_position.cross(particle_vector[selection].velocity);
      const float angular_momentum_norm_selection =
          angular_momentum_selection.norm();
      const float eccentricity_selection = std::sqrt(
          1 + (2.0 * energy_selection * angular_momentum_norm_selection *
               angular_momentum_norm_selection) /
                  (nobody::gravitation_const * nobody::gravitation_const *
                   particle_vector[0].mass * particle_vector[0].mass *
                   particle_vector[selection].mass *
                   particle_vector[selection].mass *
                   particle_vector[selection].mass));
      const float semi_major_axis_selection =
          -0.5f * nobody::gravitation_const * particle_vector[0].mass *
          particle_vector[selection].mass / energy_selection;

      std::cout << "selection data: " << std::endl
                << "kinetic energy = " << kinetic_selection << std::endl
                << "potential energy = " << potential_selection << std::endl
                << "total energy = " << energy_selection << std::endl
                << "angular momentum = "
                << angular_momentum_selection.transpose() << std::endl
                << "angular_momentum_norm_selection = "
                << angular_momentum_norm_selection << std::endl
                << "semi-major axis = " << semi_major_axis_selection << " AE"
                << std::endl
                << "eccentricity = " << eccentricity_selection << std::endl
                << std::endl;

      break;
  }

  selection = std::min(static_cast<int>(particle_vector.size()) - 1, selection);
  std::cout << "selection = " << selection << std::endl;
  compute_camera_frame();
}

void process_special_keys(int key, int x, int y) {
  key_modifiers = glutGetModifiers();

  switch (key_modifiers) {
    case GLUT_ACTIVE_CTRL:
      switch (key) {
        case GLUT_KEY_UP:
          camera_distance -= 0.5f;
          break;
        case GLUT_KEY_DOWN:
          camera_distance += 0.5f;
          break;
      }
      break;

    default:
      switch (key) {
        case GLUT_KEY_UP:
          camera_altitude += 0.1f;
          break;
        case GLUT_KEY_DOWN:
          camera_altitude -= 0.1f;
          break;
        case GLUT_KEY_LEFT:
          camera_azimuth += 0.1f;
          break;
        case GLUT_KEY_RIGHT:
          camera_azimuth -= 0.1f;
      }
      break;
  }

  compute_camera_frame();
}

void process_mouse_buttons(int button, int button_state, int x, int y) {
  key_modifiers = glutGetModifiers();

  if ((button == GLUT_LEFT_BUTTON) && (button_state == GLUT_DOWN)) {
    mouse_left_pressed = true;
  }
  if ((button == GLUT_LEFT_BUTTON) && (button_state == GLUT_UP)) {
    mouse_left_pressed = false;
  }
  if ((button == GLUT_RIGHT_BUTTON) && (button_state == GLUT_DOWN)) {
    mouse_right_pressed = true;
  }
  if ((button == GLUT_RIGHT_BUTTON) && (button_state == GLUT_UP)) {
    mouse_right_pressed = false;
  }
}

void process_mouse_move(int x, int y) {
  const float x_difference = static_cast<float>(x - old_mouse_x);
  const float y_difference = static_cast<float>(y - old_mouse_y);

  switch (key_modifiers) {
    case GLUT_ACTIVE_CTRL:
      if (mouse_left_pressed) {
        camera_distance += 0.02f * y_difference * camera_distance;
        if (camera_distance < 1e-5f) camera_distance = 1e-5f;
      } else if (mouse_right_pressed) {
        time_step += 0.02f * y_difference * time_step;
        if (time_step < 1e-6f) time_step = 1e-6f;
        std::cout << "time step = " << time_step << " years" << std::endl;
      }
      break;

    // moving
    case GLUT_ACTIVE_SHIFT:
      world.set_origin(world.origin() -
                       camera.frame().right() * x_difference *
                           camera.pixel_size() * camera_distance +
                       camera.frame().up() * y_difference *
                           camera.pixel_size() * camera_distance);
      break;

    default:
      camera_azimuth -= static_cast<float>(x - old_mouse_x) * 0.01f;
      camera_altitude += static_cast<float>(y - old_mouse_y) * 0.01f;
      if (camera_altitude > M_PI_2 - 0.0001f)
        camera_altitude = M_PI_2 - 0.0001f;
      if (camera_altitude < -M_PI_2 + 0.0001f)
        camera_altitude = -M_PI_2 + 0.0001f;
  }

  compute_camera_frame();

  old_mouse_x = x;
  old_mouse_y = y;
}

void process_passive_mouse_move(int x, int y) {
  old_mouse_x = x;
  old_mouse_y = y;
}

void process_mouse_wheel(int wheel, int direction, int x, int y) {}

void compute_camera_frame() {
  camera.look_at(horizontal_coordinates(world, camera_distance, camera_altitude,
                                        camera_azimuth),
                 world.origin(), world.up());
  camera.set_horizontal_field_of_view(M_PI_2);

  glLoadIdentity();
  gluLookAt(camera.position()(0), camera.position()(1), camera.position()(2),
            world.origin()(0), world.origin()(1), world.origin()(2),
            world.up()(0), world.up()(1), world.up()(2));

  glutPostRedisplay();
}

}  // namespace

Glut_app::Glut_app(int argc, char** argv) {
  static bool glut_initialized = false;
  if (glut_initialized) return;
  initialize(argc, argv);
  glut_initialized = true;
}

Glut_app::Glut_app() : Glut_app(0, nullptr) {}

void Glut_app::execute() { ::execute(); }