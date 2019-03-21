#include <QApplication>
#include <iostream>
#include <nobody/viewer.hpp>

using namespace std;

int main(int argc, char** argv) {
  // if (2 != argc) {
  //   cout << "usage:\n" << argv[0] << " <data file>\n";
  //   return -1;
  // }

  QApplication application(argc, argv);
  nobody::viewer viewer{};
  return application.exec();
}