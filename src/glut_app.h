#ifndef NOBODY_GLUT_APP_H_
#define NOBODY_GLUT_APP_H_

#include <GL/freeglut.h>
#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include "euler_integrator.h"

namespace nobody {
namespace glut_app {

constexpr float rad_to_degree_scale = 180.0f / M_PI;
constexpr unsigned char glut_key_esc = 27;

extern std::vector<particle> particle_vector;
extern float time_step;
extern Eigen::Vector3f camera_position;
extern Eigen::Vector3f camera_direction;
extern float camera_azimuth;
extern float camera_altitude;
extern float camera_field_of_view;
extern float camera_distance;
extern Eigen::Vector3f world_center;
extern Eigen::Vector3f world_up;
extern float world_cube_size;
extern Eigen::Vector2i old_mouse;
extern int key_modifiers;

void init(int argc, char** argv);
void exec();

void close();
void render();
void resize(int width, int height);
void idle();
void process_normal_keys(unsigned char key, int x, int y);
void process_special_keys(int key, int x, int y);
void process_mouse_buttons(int button, int button_state, int x, int y);
void process_mouse_move(int x, int y);
void process_passive_mouse_move(int x, int y);
void process_mouse_wheel(int wheel, int direction, int x, int y);

}  // namespace glut_app
}  // namespace nobody

#endif  // NOBODY_GLUT_APP_H_