// adaptive_integrator.cc
#include <algortihm>
#include "particle_system_aos.h"

using integrator_type = void (*)(Particle_system *, float);

void adaptive_integrator(Particle_system *particles, float &dt,
                         integrator_type integrator) {
  std::vector<particle> &system = *particles;
  constexpr float tolerance = 1e-5f;

  // integrate with dt
  std::vector<particle> system1 = system;
  integrator(&system1, dt);

  // integrato with 0.5*dt
  std::vector<particle> system2 = system;
  integrator(&system2, 0.5 * dt);
  integrator(&system2, 0.5 * dt);

  // compute residuum
  float residuum = 0.0f;
  for (int i = 0; i < system.size(); ++i) {
    residuum =
        std::max(residuum, (system1[i].position - system2[i].position).norm());
  }

  // decide if you want to accept the result
  if (residuum < tolerance) {
    system = system2;
    for (int i = 0; i < system.size(); ++i) {
      system[i].position += system2[i].position - system1[i].position;
      system[i].velocity += system2[i].velocity - system1[i].velocity;
    }
  }

  // adjust time step
  dt = 0.9f * dt * std::min(2.0f, std::max(0.3f, tolerance / residuum));
}