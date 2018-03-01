#ifndef NOBODY_ENERGY_H_
#define NOBODY_ENERGY_H_

#include <vector>
#include "particle.h"

namespace nobody {

float kinetic_energy(const std::vector<particle>& particles);
float potential_energy(const std::vector<particle>& particles);

Eigen::Vector3f angular_momentum(const std::vector<particle>& particles);

}  // namespace nobody

#endif  // NOBODY_ENERGY_H_