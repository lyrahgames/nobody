// symplectic_euler_integrator.cc
#include "particle_system_aos.h"

void symplectic_euler_integrator(Particle_system* particles, float dt) {
  std::vector<particle>& system = *particles;

  for (int i = 0; i < system.size(); ++i) {
    system[i].velocity += dt * particle_acceleration(system, i);
  }

  for (int i = 0; i < system.size(); ++i) {
    system[i].position += dt * system[i].velocity;
  }
}
