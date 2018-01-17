#ifndef NOBODY_EULER_INTEGRATOR_H_
#define NOBODY_EULER_INTEGRATOR_H_

#include <omp.h>
#include "particle.h"
#include "util.h"

namespace nobody {

constexpr float gravitation_const = 0.000118406f;
constexpr float softening_param = 1e-7f;  // 1e-7f;

void euler_integrator(particle *particles, int particle_count, float dt);

}  // namespace nobody

#endif  // NOBODY_EULER_INTEGRATOR_H_