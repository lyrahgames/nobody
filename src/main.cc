#include <iostream>
#include "glut_app.h"

int main(int argc, char** argv) {
  std::cout << "nobody: n-body simulator:" << std::endl << std::endl;

  nobody::glut_app::init(argc, argv);
  nobody::glut_app::exec();

  std::cout << "quit" << std::endl;
}
