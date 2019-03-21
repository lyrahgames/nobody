#pragma once
#ifndef NOBODY_NOBODY_VIEWER_HPP_
#define NOBODY_NOBODY_VIEWER_HPP_

#include <vector>

#include <QOpenGLWidget>

#include <glm/glm.hpp>

namespace nobody {

class viewer : public QOpenGLWidget {
 public:
  viewer(QWidget* parent = nullptr);
  virtual ~viewer();

 protected:
  virtual void initializeGL() override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL() override;

  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;

  virtual void wheelEvent(QWheelEvent* event) override;

  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void keyReleaseEvent(QKeyEvent* event) override;

 private:
  void compute_look_at();

 private:
  int old_mouse_x_{};
  int old_mouse_y_{};

  static constexpr float pi_ = 3.141592654;
  float vertical_fov_{0.25 * pi_};
  float eye_distance_{10};
  float eye_azimuth_{};
  float eye_altitude_{};
  glm::vec3 origin_{};
  glm::vec3 up_{};
  glm::vec3 right_{};
  glm::vec3 direction_{};

  std::vector<float> data;
};

}  // namespace nobody

#endif  // NOBODY_NOBODY_VIEWER_HPP_