// rk4_integrator.cc
#include "particle_system_aos.h"

void rk4_integrator(Particle_system* particles, float dt) {
  std::vector<particle>& system = *particles;
  std::vector<particle> temporary_particles = *particles;
  std::vector<Eigen::Vector3f> position_coefficients_1(system.size());
  std::vector<Eigen::Vector3f> position_coefficients_2(system.size());
  std::vector<Eigen::Vector3f> position_coefficients_3(system.size());
  std::vector<Eigen::Vector3f> position_coefficients_4(system.size());
  std::vector<Eigen::Vector3f> velocity_coefficients_1(system.size());
  std::vector<Eigen::Vector3f> velocity_coefficients_2(system.size());
  std::vector<Eigen::Vector3f> velocity_coefficients_3(system.size());
  std::vector<Eigen::Vector3f> velocity_coefficients_4(system.size());

  for (int i = 0; i < system.size(); ++i) {
    position_coefficients_1[i] = system[i].velocity;
    velocity_coefficients_1[i] = particle_acceleration(system, i);
    temporary_particles[i].position =
        system[i].position + 0.5f * dt * position_coefficients_1[i];
  }

  for (int i = 0; i < system.size(); ++i) {
    position_coefficients_2[i] =
        system[i].velocity + 0.5f * dt * velocity_coefficients_1[i];
    velocity_coefficients_2[i] = particle_acceleration(temporary_particles, i);
    temporary_particles[i].position =
        system[i].position + 0.5f * dt * position_coefficients_2[i];
  }

  for (int i = 0; i < system.size(); ++i) {
    position_coefficients_3[i] =
        system[i].velocity + 0.5f * dt * velocity_coefficients_2[i];
    velocity_coefficients_3[i] = particle_acceleration(temporary_particles, i);
    temporary_particles[i].position =
        system[i].position + dt * position_coefficients_3[i];
  }

  for (int i = 0; i < system.size(); ++i) {
    position_coefficients_4[i] =
        system[i].velocity + dt * velocity_coefficients_3[i];
    velocity_coefficients_4[i] = particle_acceleration(temporary_particles, i);
  }

  for (int i = 0; i < system.size(); ++i) {
    system[i].position +=
        dt / 6.0f *
        (position_coefficients_1[i] + 2.0f * position_coefficients_2[i] +
         2.0f * position_coefficients_3[i] + position_coefficients_4[i]);
    system[i].velocity +=
        dt / 6.0f *
        (velocity_coefficients_1[i] + 2.0f * velocity_coefficients_2[i] +
         2.0f * velocity_coefficients_3[i] + velocity_coefficients_4[i]);
  }
}