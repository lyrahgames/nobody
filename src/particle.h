#ifndef NOBODY_PARTICLE_H_
#define NOBODY_PARTICLE_H_

#include "util.h"

namespace nobody {

struct Particle {
  VectorF3 position;
  VectorF3 velocity;
  float mass;
};

}  // namespace nobody

#endif  // NOBODY_PARTICLE_H_