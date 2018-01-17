#include "energy.h"
#include "euler_integrator.h"

namespace nobody {

float kinetic_energy(const std::vector<particle>& particles) {
  float energy = 0.0f;
  for (const auto& p : particles) energy += p.mass * p.velocity.squaredNorm();
  return 0.5f * energy;
}

float potential_energy(const std::vector<particle>& particles) {
  float energy = 0.0f;
  for (std::size_t i = 0; i < particles.size(); ++i) {
    for (std::size_t j = 0; j < i; ++j) {
      energy += particles[i].mass * particles[j].mass /
                (particles[i].position - particles[j].position).norm();
    }
  }
  return -gravitation_const * energy;
}

}  // namespace nobody