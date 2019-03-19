#ifndef NOBODY_PARTICLE_LOADER_H_
#define NOBODY_PARTICLE_LOADER_H_

#include <nobody/particle.hpp>
#include <string>
#include <vector>

namespace nobody {

std::vector<particle> particle_system(const std::string& file_path);

}  // namespace nobody

#endif  // NOBODY_PARTICLE_LOADER_H_