#include "glut_app.h"

namespace nobody {
namespace glut_app {

std::vector<particle> particle_vector;
float time_step = 0.1f;
Eigen::Vector3f camera_position = Eigen::Vector3f(0.0f, 0.0f, 10.0f);
Eigen::Vector3f camera_direction;
float camera_altitude = 0.0f;
float camera_azimuth = 0.0f;
float camera_field_of_view = 45.0f;
float camera_distance = 10.0f;
Eigen::Vector3f world_center = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
Eigen::Vector3f world_up = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
float world_cube_size = 1.0f;
Eigen::Vector2i old_mouse(0, 0);
int key_modifiers;

void init(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(400, 400);
  glutCreateWindow("nobody: n-body simulator");

  glutCloseFunc(close);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  glutKeyboardFunc(process_normal_keys);
  glutSpecialFunc(process_special_keys);
  glutMouseFunc(process_mouse_buttons);
  glutMotionFunc(process_mouse_move);
  glutPassiveMotionFunc(process_passive_mouse_move);
  glutMouseWheelFunc(process_mouse_wheel);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_POINT_SMOOTH);
  glPointSize(8.0f);

  // init random particle data
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

  particle_vector[0].mass = 10.0f;
  particle_vector[0].position = Eigen::Vector3f(-1, 0, 0);
  particle_vector[0].velocity = Eigen::Vector3f(0, 0.01, 0);

  particle_vector[1].mass = 10.0f;
  particle_vector[1].position = Eigen::Vector3f(1, 0, 0);
  particle_vector[1].velocity = Eigen::Vector3f(0, -0.01, 0);
}

void exec() { glutMainLoop(); }

void close() {}

void resize(int width, int height) {
  if (height == 0) height = 1;

  const float aspect_ratio =
      static_cast<float>(width) / static_cast<float>(height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  gluPerspective(camera_field_of_view, aspect_ratio, 0.1f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
}

void render() {
  // compute camera position
  camera_direction = Eigen::Vector3f(
      cosf(camera_azimuth) * cosf(camera_altitude), sinf(camera_altitude),
      sinf(camera_azimuth) * cosf(camera_altitude));
  camera_position = world_center + camera_distance * camera_direction;

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(camera_position(0), camera_position(1), camera_position(2),
            world_center(0), world_center(1), world_center(2), world_up(0),
            world_up(1), world_up(2));

  glColor3f(0, 0, 0);
  glutWireCube(world_cube_size);

  for (int i = 0; i < static_cast<int>(particle_vector.size()); ++i) {
    const float distance =
        (particle_vector[i].position - camera_position).norm();

    glPointSize(100.0f * cbrtf(particle_vector[i].mass) / distance);

    glBegin(GL_POINTS);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex3f(particle_vector[i].position(0), particle_vector[i].position(1),
               particle_vector[i].position(2));
    glEnd();
  }

  glutSwapBuffers();
}

void idle() {
  euler_integrator(particle_vector.data(),
                   static_cast<int>(particle_vector.size()), time_step);
  glutPostRedisplay();
}

void process_normal_keys(unsigned char key, int x, int y) {
  switch (key) {
    case glut_key_esc:
      glutLeaveMainLoop();
      break;
  }
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
          camera_azimuth -= 0.1f;
          break;
        case GLUT_KEY_RIGHT:
          camera_azimuth += 0.1f;
      }
      break;
  }
}

void process_mouse_buttons(int button, int button_state, int x, int y) {
  key_modifiers = glutGetModifiers();

  // switch (modifier) {
  //   case GLUT_ACTIVE_CTRL:
  //     switch (button) {
  //       case GLUT_LEFT_BUTTON:
  //         camera_distance += static_cast<float>(y - old_mouse(1)) * 0.5f;
  //         break;
  //     }
  //     break;

  //   default:
  //     switch (button) {
  //       case GLUT_LEFT_BUTTON:
  //         camera_azimuth += static_cast<float>(x - old_mouse(0)) * 0.01f;
  //         camera_altitude += static_cast<float>(y - old_mouse(1)) * 0.01f;

  //         break;
  //     }
  // }

  // old_mouse = Eigen::Vector2i(x, y);
}

void process_mouse_move(int x, int y) {
  // int modifier = glutGetModifiers();

  switch (key_modifiers) {
    case GLUT_ACTIVE_CTRL:
      camera_distance += static_cast<float>(y - old_mouse(1)) * 0.1f;
      break;

    default:
      camera_azimuth += static_cast<float>(x - old_mouse(0)) * 0.01f;
      camera_altitude += static_cast<float>(y - old_mouse(1)) * 0.01f;
  }

  old_mouse = Eigen::Vector2i(x, y);
}

void process_passive_mouse_move(int x, int y) {
  old_mouse = Eigen::Vector2i(x, y);
}

void process_mouse_wheel(int wheel, int direction, int x, int y) {
  camera_distance += 0.1f;
}

}  // namespace glut_app
}  // nobody