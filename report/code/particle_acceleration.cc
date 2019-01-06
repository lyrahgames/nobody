// particle_acceleration.cc
#include "particle_system_aos.h"

// return acceleration of particle with index 'index'
// in given particle system 'particles'
Eigen::Vector3f particle_acceleration(const Particle_system& particles,
                                      int index) {
  Eigen::Vector3f acceleration(0, 0, 0);
  // magic numbers
  constexpr float gravitation_const = 0.000118406f;
  constexpr float softening_param = 1e-7f;

  for (int j = 0; j < particles.size(); j++) {
    const Eigen::Vector3f direction =
        particles[j].position - particles[index].position;
    const float squared_distance = direction.squaredNorm();
    const float distance = sqrtf(squared_distance);
    const float scale =
        particles[j].mass / (distance * squared_distance + softening_param);
    acceleration += scale * direction;
  }

  return (acceleration *= gravitation_const);
}