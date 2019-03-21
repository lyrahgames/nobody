#include <nobody/viewer.hpp>

#include <algorithm>
#include <cmath>
#include <random>

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include <glm/ext.hpp>

using namespace std;

namespace nobody {

viewer::viewer(QWidget* parent) : QOpenGLWidget(parent) {
  setMouseTracking(true);
  show();

  mt19937 rng{std::random_device{}()};
  uniform_real_distribution<float> distribution{};
  constexpr int count = 100;
  data.reserve(3 * count);
  for (int i = 0; i < 3 * count; ++i) data.push_back(distribution(rng));
}

viewer::~viewer() = default;

void viewer::initializeGL() { glClearColor(1, 1, 1, 1); }

void viewer::resizeGL(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  glLoadMatrixf(glm::value_ptr(glm::perspective(
      glm::degrees(vertical_fov_), static_cast<float>(width) / height,
      min_distance_, max_distance_)));
  glMatrixMode(GL_MODELVIEW);
  compute_look_at();
}

void viewer::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPointSize(5);
  for (int i = 0; i < data.size(); i += 3) {
    glBegin(GL_POINTS);
    glColor3f(0, 0, 0);
    glVertex3f(data[i], data[i + 1], data[i + 2]);
    glEnd();
  }
}

void viewer::mousePressEvent(QMouseEvent* event) {}

void viewer::mouseReleaseEvent(QMouseEvent* event) {}

void viewer::mouseMoveEvent(QMouseEvent* event) {
  const auto x_difference = event->x() - old_mouse_x_;
  const auto y_difference = event->y() - old_mouse_y_;

  switch (event->buttons()) {
    case Qt::LeftButton:
      rotate_on_screen(x_difference, y_difference);
      break;

    case Qt::RightButton:
      move_origin_on_screen(x_difference, y_difference);
      break;
  }

  old_mouse_x_ = event->x();
  old_mouse_y_ = event->y();
}

void viewer::wheelEvent(QWheelEvent* event) {
  constexpr float scale_factor = 0.003;
  zoom(scale_factor * event->angleDelta().y());
}

void viewer::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) QApplication::quit();
}

void viewer::keyReleaseEvent(QKeyEvent* event) {}

void viewer::compute_look_at() {
  const glm::vec3 world_up = {0, 1, 0};
  direction_ = {cos(eye_azimuth_) * cos(eye_altitude_), sin(eye_altitude_),
                sin(eye_azimuth_) * cos(eye_altitude_)};
  up_ = glm::normalize(world_up - glm::dot(direction_, world_up) * direction_);
  right_ = glm::cross(up_, direction_);
  const glm::vec3 position = origin_ + eye_distance_ * direction_;

  makeCurrent();
  glLoadIdentity();
  glLoadMatrixf(glm::value_ptr(glm::lookAt(position, origin_, world_up)));
  update();
}

float viewer::pixel_size() const noexcept {
  return 2 * tan(0.5 * vertical_fov_) / height();
}

void viewer::move_origin_on_screen(int x_difference,
                                   int y_difference) noexcept {
  origin_ += pixel_size() * eye_distance_ *
             (-static_cast<float>(x_difference) * right_ +
              static_cast<float>(y_difference) * up_);
  compute_look_at();
}

void viewer::rotate_on_screen(int x_difference, int y_difference) noexcept {
  constexpr float max_altitude = 0.5 * pi_ - 1e-5;
  constexpr float scale_factor = 0.01;

  eye_azimuth_ += x_difference * scale_factor;
  eye_altitude_ += y_difference * scale_factor;
  eye_altitude_ = clamp(eye_altitude_, -max_altitude, max_altitude);
  compute_look_at();
}

void viewer::zoom(float scale) noexcept {
  eye_distance_ *= exp(-scale);
  eye_distance_ = clamp(eye_distance_, min_distance_, max_distance_);
  compute_look_at();
}

}  // namespace nobody