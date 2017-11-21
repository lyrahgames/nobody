#include <GL/glut.h>
#include <Eigen/Core>
#include <iostream>
#include <random>
#include <vector>

#include "euler_integrator.h"

nobody::Particle* particle;
int particle_count;
float time_step;

void init() {
  using nobody::Particle;

  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

  particle_count = 1000;

  particle = new Particle[particle_count];

  for (int i = 0; i < particle_count; i++) {
    particle[i].position = 2.0f * nobody::VectorF3(dist(rng), dist(rng), 1.0f);
    particle[i].velocity = 0.0f * nobody::VectorF3(dist(rng), dist(rng), 0.0f);
    particle[i].mass = 1.0f / particle_count;
  }

  // particle[0].mass = 100.0f;
  // particle[0].position = nobody::VectorF3(0, 0, 0);
  // particle[0].velocity = nobody::VectorF3(0, 0, 0);

  time_step = 0.5f;
}

void opengl_init() { glPointSize(5.0f); }

void render_scene() {
  static float angle = 0.0f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(angle, 0.0f, 0.1f, 0.0f);

  glBegin(GL_POINTS);
  for (int i = 0; i < particle_count; i++) {
    glVertex3f(particle[i].position(0), particle[i].position(1),
               particle[i].position(2));
  }
  glEnd();

  // angle += 0.005f;
  nobody::euler_integrator(particle, particle_count, time_step);

  glutSwapBuffers();
}

void change_size(int w, int h) {
  if (h == 0) h = 1;

  const float ratio = 1.0f * w / h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
  std::cout << "Eigen3 test run:" << std::endl;

  Eigen::Matrix3d m = Eigen::Matrix3d::Random();
  Eigen::Vector3d v(1, 2, 3);
  std::cout << m << std::endl
            << "*" << std::endl
            << v << std::endl
            << "=" << std::endl
            << m * v << std::endl
            << std::endl;

  std::cout << "OpenGL and GLUT test run..." << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(320, 320);
  glutCreateWindow("OpenGL and GLUT test run");

  glutDisplayFunc(render_scene);
  glutReshapeFunc(change_size);
  glutIdleFunc(render_scene);

  opengl_init();
  init();

  glutMainLoop();
}
