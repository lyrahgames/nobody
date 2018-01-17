#ifndef RUNGE_KUTTA_INTEGRATOR_H_
#define RUNGE_KUTTA_INTEGRATOR_H_

#include <omp.h>
#include <vector>
#include "euler_integrator.h"
#include "particle.h"

namespace nobody {

// constexpr float gravitation_const = 0.0001f;
// constexpr float softening_param = 1e-7f

VectorF3 acceleration_at_position(const particle *particles, int particle_count,
                                  const VectorF3 &position);
VectorF3 particle_acceleration(const particle *particles, int particle_count,
                               int index);
void rk4_integrator(particle *particles, int particle_count, float dt);

Eigen::Vector3f particle_acceleration(const std::vector<particle> &particles,
                                      int index);
void rk4_integrator(std::vector<particle> *particles, float dt);

}  // namespace nobody

#endif  // RUNGE_KUTTA_INTEGRATOR_H_