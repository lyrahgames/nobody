#ifndef NOBODY_EULER_INTEGRATOR_H_
#define NOBODY_EULER_INTEGRATOR_H_

#include "particle.h"
#include "util.h"

namespace nobody {

constexpr float gravitation_const = 0.0001f;
constexpr float softening_param = 1e-7f;

void euler_integrator(Particle *particle, int particle_count, float dt);

}  // namespace nobody

#endif  // NOBODY_EULER_INTEGRATOR_H_