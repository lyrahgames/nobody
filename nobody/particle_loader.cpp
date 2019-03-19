#include <fstream>
#include <iostream>
#include <nobody/particle_loader.hpp>
#include <stdexcept>

namespace nobody {

std::vector<particle> particle_system(const std::string& file_path) {
  std::fstream file(file_path);

  if (!file.is_open()) {
    throw std::runtime_error("file could not be opened!");
  }

  std::vector<particle> particle_data;

  // std::fstream out("converted.txt", std::ios::out | std::ios::trunc);

  while (!file.eof()) {
    std::string name;
    file >> name;

    particle p;
    for (int i = 0; i < 3; ++i) file >> p.position(i);
    for (int i = 0; i < 3; ++i) file >> p.velocity(i);
    // p.velocity *= 365.0f;
    file >> p.mass;

    std::cout << "particle loaded:\n"
              << name << "\n"
              << "position = (" << p.position.transpose() << ")\n"
              << "velocity = (" << p.velocity.transpose() << ")\n"
              << "mass = " << p.mass << std::endl
              << std::endl;

    if (file.fail()) {
      throw std::runtime_error("file could not be read");
    }

    // out << name << "\n"
    //     << p.position.transpose() << "\n"
    //     << p.velocity.transpose() << "\n"
    //     << p.mass << "\n";

    particle_data.push_back(p);
  }

  return particle_data;
}

}  // namespace nobody