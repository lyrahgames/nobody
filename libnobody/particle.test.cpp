#include <doctest/doctest.h>

#include <Eigen/Dense>
#include <libnobody/particle.hpp>
#include <type_traits>

TEST_CASE("The particle") {
  using vector_type = Eigen::Vector3f;
  using particle = nobody::particle<vector_type>;

  static_assert(std::is_same_v<particle::real_type, float>);
  static_assert(std::is_same_v<particle::real_type, vector_type::value_type>);

  SUBCASE("Setters and getters") {
    particle p{};
    CHECK(p.position() == particle::vector_type{0, 0, 0});
    CHECK(p.velocity() == particle::vector_type{0, 0, 0});
    CHECK(p.mass() == 1);

    p.position() = {1, 2, 3};
    CHECK(p.position() == particle::vector_type{1, 2, 3});
    CHECK(p.velocity() == particle::vector_type{0, 0, 0});
    CHECK(p.mass() == 1);

    p.velocity() = {4, 5, 6};
    CHECK(p.position() == particle::vector_type{1, 2, 3});
    CHECK(p.velocity() == particle::vector_type{4, 5, 6});
    CHECK(p.mass() == 1);

    CHECK_THROWS_AS(p.mass(0), std::invalid_argument);
    CHECK_THROWS_AS(p.mass(-1), std::invalid_argument);
    p.mass(2);
    CHECK(p.position() == particle::vector_type{1, 2, 3});
    CHECK(p.velocity() == particle::vector_type{4, 5, 6});
    CHECK(p.mass() == 2);
  }

  SUBCASE("lvalue setters") {
    particle p{};
    p.position({1, 1, 1}).velocity({2, 2, 2}).mass(3);
    CHECK(p.position() == particle::vector_type{1, 1, 1});
    CHECK(p.velocity() == particle::vector_type{2, 2, 2});
    CHECK(p.mass() == 3);
  }

  SUBCASE("rvalue setters") {
    const auto p = particle{}.position({5, 5, 5}).velocity({6, 6, 6}).mass(2);
    CHECK(p.position() == particle::vector_type{5, 5, 5});
    CHECK(p.velocity() == particle::vector_type{6, 6, 6});
    CHECK(p.mass() == 2);
  }

  SUBCASE("constructors") {
    CHECK_THROWS_AS(particle({1, 9, 2}, {8, 3, 7}, 0), std::invalid_argument);
    CHECK_THROWS_AS(particle({1, 9, 2}, {8, 3, 7}, -1), std::invalid_argument);
    particle p{{1, 9, 2}, {8, 3, 7}, 4};
    CHECK(p.position() == particle::vector_type{1, 9, 2});
    CHECK(p.velocity() == particle::vector_type{8, 3, 7});
    CHECK(p.mass() == 4);
  }

  SUBCASE("constructors") {
    particle p{{1, 9, 2}};
    CHECK(p.position() == particle::vector_type{1, 9, 2});
    CHECK(p.velocity() == particle::vector_type{0, 0, 0});
    CHECK(p.mass() == 1);
  }

  SUBCASE("constructors") {
    particle p{{1, 9, 2}, {1, 2, 3}};
    CHECK(p.position() == particle::vector_type{1, 9, 2});
    CHECK(p.velocity() == particle::vector_type{1, 2, 3});
    CHECK(p.mass() == 1);
  }

  SUBCASE("constructors") {
    CHECK_THROWS_AS(particle{0}, std::invalid_argument);
    CHECK_THROWS_AS(particle{-1}, std::invalid_argument);
    particle p{2};
    CHECK(p.position() == particle::vector_type{0, 0, 0});
    CHECK(p.velocity() == particle::vector_type{0, 0, 0});
    CHECK(p.mass() == 2);
  }
}