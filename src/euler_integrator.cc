#include "euler_integrator.h"

namespace nobody {

void euler_integrator(particle *particles, int particle_count,
                      float time_step) {
  for (int i = 0; i < particle_count; i++) {
    VectorF3 acceleration(0, 0, 0);

    for (int j = 0; j < particle_count; j++) {
      const VectorF3 direction = particles[j].position - particles[i].position;
      const float squared_distance = direction.squaredNorm();
      const float distance = sqrtf(squared_distance);
      const float scale =
          particles[j].mass / (distance * squared_distance + softening_param);
      acceleration += scale * direction;
    }

    acceleration *= gravitation_const;

    particles[i].velocity += time_step * acceleration;
  }

  for (int i = 0; i < particle_count; i++) {
    particles[i].position += time_step * particles[i].velocity;
  }
}

}  // namespace nobody