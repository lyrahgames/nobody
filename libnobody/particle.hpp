#pragma once
#ifndef NOBODY_LIBNOBODY_PARTICLE_HPP_
#define NOBODY_LIBNOBODY_PARTICLE_HPP_

#include <type_traits>
#include <utility>

namespace nobody {

template <typename Vector>
class particle {
 public:
  using vector_type = Vector;
  using real_type = std::decay_t<decltype(Vector{}[0])>;

  particle() = default;
  particle(const particle&) = default;
  particle& operator=(const particle&) = default;
  particle(particle&&) = default;
  particle& operator=(particle&&) = default;
  ~particle() = default;

  explicit particle(real_type mass) : mass_{validated_mass(mass)} {}
  explicit particle(const vector_type& position) noexcept
      : position_{position} {}
  particle(const vector_type& position, const vector_type& velocity) noexcept
      : position_{position}, velocity_{velocity} {}
  particle(const vector_type& position, const vector_type& velocity,
           real_type mass)
      : position_{position}, velocity_{velocity}, mass_{validated_mass(mass)} {}

  const auto& position() const noexcept { return position_; }
  auto& position() noexcept { return position_; }
  particle& position(const vector_type& p) & noexcept {
    position_ = p;
    return *this;
  }
  particle position(const vector_type& p) && noexcept {
    position_ = p;
    return std::move(*this);
  }

  const auto& velocity() const noexcept { return velocity_; }
  auto& velocity() noexcept { return velocity_; }
  particle& velocity(const vector_type& v) & noexcept {
    velocity_ = v;
    return *this;
  }
  particle velocity(const vector_type& v) && noexcept {
    velocity_ = v;
    return std::move(*this);
  }

  auto mass() const noexcept { return mass_; }
  particle& mass(real_type m) & {
    mass_ = validated_mass(m);
    return *this;
  }
  particle mass(real_type m) && {
    mass_ = validated_mass(m);
    return std::move(*this);
  }

 private:
  static auto validated_mass(real_type mass) {
    if (mass <= 0)
      throw std::invalid_argument(
          "Mass of a particle cannot be lower or equal to zero!");
    return mass;
  }

 private:
  vector_type position_{};
  vector_type velocity_{};
  real_type mass_{1};
};

}  // namespace nobody

#endif  // NOBODY_LIBNOBODY_PARTICLE_HPP_