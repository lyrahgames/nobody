#include "euler_integrator.h"

namespace nobody {

void euler_integrator(Particle *particle, int particle_count, float time_step) {
  for (int i = 0; i < particle_count; i++) {
    VectorF3 acceleration(0, 0, 0);

    for (int j = 0; j < particle_count; j++) {
      const VectorF3 direction = particle[j].position - particle[i].position;
      const float squared_distance = direction.squaredNorm();
      const float distance = sqrtf(squared_distance);
      const float scale =
          particle[j].mass / (distance * squared_distance + softening_param);
      acceleration += scale * direction;
    }

    acceleration *= gravitation_const;

    particle[i].velocity += time_step * acceleration;
  }

  for (int i = 0; i < particle_count; i++) {
    particle[i].position += time_step * particle[i].velocity;
  }
}

}  // namespace nobody