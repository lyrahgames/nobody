// euler_integrator.cc
#include "particle_system_aos.h"

void euler_integrator(Particle_system* particles, float dt) {
  std::vector<particle>& system = *particles;
  std::vector<Eigen::Vector3f> new_velocities(system.size());

  for (int i = 0; i < system.size(); ++i) {
    new_velocities[i] =
        system[i].velocity + dt * particle_acceleration(system, i);
  }

  for (int i = 0; i < system.size(); ++i) {
    system[i].position += dt * system[i].velocity;
    system[i].velocity = new_velocities[i];
  }
}