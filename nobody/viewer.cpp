#include <nobody/viewer.hpp>

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

namespace nobody {

viewer::viewer(QWidget* parent) : QOpenGLWidget(parent) {
  setMouseTracking(true);
  show();
}

viewer::~viewer() = default;

void viewer::initializeGL() { glClearColor(0, 0, 1, 1); }

void viewer::resizeGL(int width, int height) {}

void viewer::paintGL() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void viewer::mousePressEvent(QMouseEvent* event) {}

void viewer::mouseReleaseEvent(QMouseEvent* event) {}

void viewer::mouseMoveEvent(QMouseEvent* event) {}

void viewer::wheelEvent(QWheelEvent* event) {}

void viewer::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) QApplication::quit();
}

void viewer::keyReleaseEvent(QKeyEvent* event) {}

}  // namespace nobody