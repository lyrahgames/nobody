#pragma once
#ifndef NOBODY_NOBODY_VIEWER_HPP_
#define NOBODY_NOBODY_VIEWER_HPP_

#include <QOpenGLWidget>

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
  int old_mouse_x_{};
  int old_mouse_y_{};

  float eye_distance_{1};
  float eye_azimuth_{};
  float eye_altitude_{};
};

}  // namespace nobody

#endif  // NOBODY_NOBODY_VIEWER_HPP_