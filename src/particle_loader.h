#ifndef NOBODY_PARTICLE_LOADER_H_
#define NOBODY_PARTICLE_LOADER_H_

#include <string>
#include <vector>
#include "particle.h"

namespace nobody {

std::vector<particle> particle_system(const std::string& file_path);

}  // namespace nobody

#endif  // NOBODY_PARTICLE_LOADER_H_