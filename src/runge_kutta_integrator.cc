#include "runge_kutta_integrator.h"

namespace nobody {

VectorF3 acceleration_at_position(const particle *particles, int particle_count,
                                  const VectorF3 &position) {
  VectorF3 acceleration(0, 0, 0);
  for (int j = 0; j < particle_count; j++) {
    const VectorF3 direction = particles[j].position - position;
    const float squared_distance = direction.squaredNorm();
    const float distance = sqrtf(squared_distance);
    const float scale =
        particles[j].mass / (distance * squared_distance + softening_param);
    acceleration += scale * direction;
  }
  return (acceleration *= gravitation_const);
}

VectorF3 particle_acceleration(const particle *particles, int particle_count,
                               int index) {
  VectorF3 acceleration(0, 0, 0);
  for (int j = 0; j < particle_count; j++) {
    const VectorF3 direction =
        particles[j].position - particles[index].position;
    const float squared_distance = direction.squaredNorm();
    const float distance = sqrtf(squared_distance);
    const float scale =
        particles[j].mass / (distance * squared_distance + softening_param);
    acceleration += scale * direction;
  }
  return (acceleration *= gravitation_const);
}

void rk4_integrator(particle *particles, int particle_count, float dt) {
  std::vector<VectorF3> step(particle_count);

  // #pragma omp parallel for schedule(guided)
  for (int i = 0; i < particle_count; i++) {
    const VectorF3 current_position = particles[i].position;

    const VectorF3 velocity_coeff_0 =
        particle_acceleration(particles, particle_count, i);
    const VectorF3 position_coeff_0 = particles[i].velocity;

    particles[i].position = current_position + 0.5f * dt * position_coeff_0;
    const VectorF3 velocity_coeff_1 =
        particle_acceleration(particles, particle_count, i);
    const VectorF3 position_coeff_1 = static_cast<Eigen::Vector3f>(
        static_cast<Eigen::Array3f>(particles[i].velocity) +
        0.5f * dt * static_cast<Eigen::Array3f>(velocity_coeff_0));

    particles[i].position = current_position + 0.5f * dt * position_coeff_1;
    const VectorF3 velocity_coeff_2 =
        particle_acceleration(particles, particle_count, i);
    const VectorF3 position_coeff_2 = static_cast<Eigen::Vector3f>(
        static_cast<Eigen::Array3f>(particles[i].velocity) +
        0.5f * dt * static_cast<Eigen::Array3f>(velocity_coeff_1));

    particles[i].position = current_position + dt * position_coeff_2;
    const VectorF3 velocity_coeff_3 =
        particle_acceleration(particles, particle_count, i);
    const VectorF3 position_coeff_3 = static_cast<Eigen::Vector3f>(
        static_cast<Eigen::Array3f>(particles[i].velocity) +
        1.0f * dt * static_cast<Eigen::Array3f>(velocity_coeff_2));

    particles[i].velocity +=
        (dt / 6.0f) * (velocity_coeff_0 + 2.0f * velocity_coeff_1 +
                       2.0f * velocity_coeff_2 + velocity_coeff_3);

    particles[i].position = current_position;
    step[i] = (dt / 6.0f) * (position_coeff_0 + 2.0f * position_coeff_1 +
                             2.0f * position_coeff_2 + position_coeff_3);
  }

  for (int i = 0; i < particle_count; i++) {
    particles[i].position += step[i];
  }
}

}  // namespace nobody