#include "camera.h"

namespace nobody {

float Camera::vertical_field_of_view() const {
  // return 2.0 * atanf(tanf(field_of_view_ * 0.5f) / aspect_ratio_);
  return field_of_view_;
}

float Camera::horizontal_field_of_view() const {
  return 2.0 * std::atan(std::tan(field_of_view_ * 0.5f) * aspect_ratio_);
}

float Camera::opengl_field_of_view() const {
  return field_of_view_ * 180 / M_PI;
}

void Camera::look_at(const Eigen::Vector3f& eye, const Eigen::Vector3f& center,
                     const Eigen::Vector3f& up) {
  frame_ = Orthonormal_frame(eye, eye - center, up);
}

void Camera::set_screen_resolution(int width, int height) {
  screen_height_ = height;
  screen_width_ = width;
  compute_pixel_size();
  compute_aspect_ratio();
}

void Camera::set_field_of_view(float fov) {
  if (fov <= 0.0f || fov >= static_cast<float>(M_PI)) return;
  field_of_view_ = fov;
  compute_pixel_size();
}

void Camera::set_horizontal_field_of_view(float fov) {
  if (fov <= 0.0f || fov >= static_cast<float>(M_PI)) return;
  const float vfov = 2.0 * std::atan(std::tan(fov * 0.5f) / aspect_ratio_);
  set_field_of_view(vfov);
}

void Camera::compute_pixel_size() {
  // pixel_size_ =
  //     2.0f * tanf(field_of_view_ * 0.5f) / static_cast<float>(screen_width_);
  pixel_size_ = 2.0f * std::tan(field_of_view_ * 0.5f) /
                static_cast<float>(screen_height_);
}

void Camera::compute_aspect_ratio() {
  aspect_ratio_ =
      static_cast<float>(screen_width_) / static_cast<float>(screen_height_);
}

}  // namespace nobody