// leapfrog_integrator.cc
#include "particle_system_aos.h"

void leapfrog_integrator(Particle_system* particles, float dt) {
  std::vector<particle>& system = *particles;
  std::vector<Eigen::Vector3f> half_step_velocities(system.size());

  for (int i = 0; i < system.size(); ++i) {
    half_step_velocities[i] =
        system[i].velocity + 0.5f * dt * particle_acceleration(system, i);
  }

  for (int i = 0; i < system.size(); ++i) {
    system[i].position += dt * half_step_velocities[i];
  }

  for (int i = 0; i < system.size(); ++i) {
    system[i].velocity =
        half_step_velocities[i] + 0.5f * dt * particle_acceleration(system, i);
  }
}