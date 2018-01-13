#include <iostream>
#include "glut_app.h"

int main(int argc, char** argv) {
  std::cout << "nobody: n-body simulator:" << std::endl << std::endl;

  Glut_app glut_app(argc, argv);
  glut_app.execute();
}
