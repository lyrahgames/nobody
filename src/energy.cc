#include "energy.h"

#include <Eigen/Dense>
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

Eigen::Vector3f angular_momentum(const std::vector<particle>& particles) {
  Eigen::Vector3f result(0, 0, 0);
  for (const auto& particle : particles) {
    result += particle.mass * particle.position.cross(particle.velocity);
  }
  return result;
}

}  // namespace nobody