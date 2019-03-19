#ifndef RUNGE_KUTTA_INTEGRATOR_H_
#define RUNGE_KUTTA_INTEGRATOR_H_

#include <omp.h>
#include <vector>

#include <nobody/euler_integrator.hpp>
#include <nobody/particle.hpp>

namespace nobody {

// constexpr float gravitation_const = 0.0001f;
// constexpr float softening_param = 1e-7f

using integrator_type = void (*)(std::vector<particle> *, float);

VectorF3 acceleration_at_position(const particle *particles, int particle_count,
                                  const VectorF3 &position);
VectorF3 particle_acceleration(const particle *particles, int particle_count,
                               int index);
void rk4_integrator(particle *particles, int particle_count, float dt);

Eigen::Vector3f particle_acceleration(const std::vector<particle> &particles,
                                      int index);
void rk4_integrator(std::vector<particle> *particles, float dt);

void leapfrog_integrator(std::vector<particle> *particles, float dt);

void euler_integrator(std::vector<particle> *particles, float dt);

void symplectic_euler_integrator(std::vector<particle> *particles, float dt);

void leapfrog_adaptive_integrator(std::vector<particle> *particles,
                                  float &time);

void adaptive_integrator(std::vector<particle> *particles, float &time,
                         integrator_type integrator = leapfrog_integrator);

}  // namespace nobody

#endif  // RUNGE_KUTTA_INTEGRATOR_H_