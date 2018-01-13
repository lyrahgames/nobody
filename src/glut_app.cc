#include "glut_app.h"
#include <GL/glut.h>
#include <Eigen/Dense>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <vector>
#include "camera.h"
#include "euler_integrator.h"
#include "particle_loader.h"
#include "runge_kutta_integrator.h"

namespace {

// const data
constexpr float rad_to_degree_scale = 180.0f / M_PI;
constexpr unsigned char glut_key_esc = 27;
constexpr unsigned char glut_key_space = 32;
constexpr unsigned char glut_key_p = 112;
constexpr unsigned char glut_key_l = 108;

// static data
std::vector<nobody::particle> particle_vector;
float time_step = 0.005f;
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
bool paused = false;
bool render_particles = true;
bool render_particle_paths = true;
nobody::Camera camera(400, 400, M_PI_2);
nobody::Orthonormal_frame world =
    nobody::blender_orthonormal_frame(Eigen::Vector3f(0, 0, 0));
float time = 0.0f;
float time_tmp = 0.0f;
std::vector<std::list<Eigen::Vector3f>> particle_path_data;

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
  glEnable(GL_DEPTH_TEST);
  glPointSize(8.0f);

  if (2 != argc) {
    std::cout << "GEBE EINEN Dateipfad AN DU ARSCHLOCH!" << std::endl;
    // exit(-1);
    // initialize random particle data
    particle_vector.resize(100);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    for (int i = 0; i < static_cast<int>(particle_vector.size()); ++i) {
      particle_vector[i].position =
          Eigen::Vector3f(dist(rng), dist(rng), dist(rng));
      particle_vector[i].velocity =
          0.01f * Eigen::Vector3f(dist(rng), dist(rng), dist(rng));
      particle_vector[i].mass = 0.1f / particle_vector.size();
    }

    particle_vector[0].mass = 100.0f;
    particle_vector[0].position = Eigen::Vector3f(-1, 0, 0);
    particle_vector[0].velocity = Eigen::Vector3f(0, 0.02, 0);

    particle_vector[1].mass = 100.0f;
    particle_vector[1].position = Eigen::Vector3f(1, 0, 0);
    particle_vector[1].velocity = Eigen::Vector3f(0, -0.02, 0);
  } else {
    particle_vector = nobody::particle_system(std::string(argv[1]));
  }

  particle_path_data.resize(particle_vector.size());
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
  // compute fps
  fps_current_time = std::chrono::system_clock::now();
  fps_frame_count++;
  const float delta_time =
      std::chrono::duration<float>(fps_current_time - fps_last_time).count();
  if (delta_time >= 1.0f) {
    std::cout << "frame time: "
              << delta_time / static_cast<float>(fps_frame_count)
              << "s\tfps: " << static_cast<float>(fps_frame_count) / delta_time
              << std::endl;
    std::cout << "simulation time = " << time << " years" << std::endl;
    std::cout << "time step = " << time_step << " years" << std::endl;
    std::cout << std::endl;
    fps_frame_count = 0;
    fps_last_time = fps_current_time;
  }

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

  if (render_particle_paths) {
    glLineWidth(1.5f);
    for (std::size_t i = 0; i < particle_path_data.size(); ++i) {
      const auto& particle_path = particle_path_data[i];
      glBegin(GL_LINE_STRIP);
      for (const auto& point : particle_path) glVertex3fv(point.data());
      glVertex3fv(particle_vector[i].position.data());
      glEnd();
    }
  }

  if (render_particles) {
    for (int i = 0; i < static_cast<int>(particle_vector.size()); ++i) {
      const float distance =
          (particle_vector[i].position - camera.position()).norm();

      const float brightness = std::min(0.7f, 1.0f / (distance * distance));

      glPointSize(std::max(0.005f / camera.pixel_size() *
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
  // euler_integrator(particle_vector.data(),
  //                  static_cast<int>(particle_vector.size()), time_step);
  rk4_integrator(particle_vector.data(),
                 static_cast<int>(particle_vector.size()), time_step);

  // world.set_origin(particle_vector[0].position);

  time_tmp += time_step;
  if (time_tmp >= 5.0f) {
    time += time_tmp;
    time_tmp = 0.0f;
    // paused = true;
    // std::cout << "time = " << time << " years" << std::endl;
  }

  for (std::size_t i = 0; i < particle_path_data.size(); ++i) {
    auto& particle_path = particle_path_data[i];

    if (particle_path.size() < 2)
      particle_path.push_back(particle_vector[i].position);
    else {
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

  glutPostRedisplay();
}

void process_normal_keys(unsigned char key, int x, int y) {
  switch (key) {
    case glut_key_esc:
      // glutLeaveMainLoop();
      exit(0);
      break;

    case glut_key_space:
      paused = !paused;
      break;

    case glut_key_p:
      render_particles = !render_particles;
      break;

    case glut_key_l:
      render_particle_paths = !render_particle_paths;
      break;
  }

  glutPostRedisplay();
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

  glutPostRedisplay();
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