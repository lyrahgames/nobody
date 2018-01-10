#include "particle_loader.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace nobody {

std::vector<particle> particle_system(const std::string& file_path) {
  std::fstream file(file_path);

  if (!file.is_open()) {
    throw std::runtime_error("file could not be opened!");
  }

  std::vector<particle> particle_data;

  while (!file.eof()) {
    std::string name;
    file >> name;

    particle p;
    for (int i = 0; i < 3; ++i) file >> p.position(i);
    for (int i = 0; i < 3; ++i) file >> p.velocity(i);
    file >> p.mass;

    if (file.fail()) {
      throw std::runtime_error("file could not be read");
    }

    std::cout << "particle loaded:\n"
              << name << "\n"
              << "position = (" << p.position.transpose() << ")\n"
              << "velocity = (" << p.velocity.transpose() << ")\n"
              << "mass = " << p.mass << std::endl
              << std::endl;

    particle_data.push_back(p);
  }

  return particle_data;
}

}  // namespace nobody