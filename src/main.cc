#include <GL/glut.h>
#include <iostream>

void render_scene() {
  static float angle = 0.0f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(angle, 0.0f, 0.1f, 0.0f);

  glBegin(GL_TRIANGLES);
  glVertex3f(-2.0f, -2.0f, 0.0f);
  glVertex3f(2.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 2.0f, 0.0f);
  glEnd();

  angle += 0.001f;

  glutSwapBuffers();
}

void change_size(int w, int h) {
  if (h == 0) h = 1;

  const float ratio = 1.0f * w / h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
  std::cout << "Test run for OpenGL and GLUT." << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(320, 320);
  glutCreateWindow("OpenGL and GLUT testrun");

  glutDisplayFunc(render_scene);
  glutReshapeFunc(change_size);
  glutIdleFunc(render_scene);

  glutMainLoop();
}
