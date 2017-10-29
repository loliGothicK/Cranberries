#ifndef CRANBERRIES_RANDOMIZE_HPP
#define CRANBERRIES_RANDOMIZE_HPP
#include <random>
#include <cstdint>
#include <array>
#include <utility>
#include <algorithm>

namespace cranberries {

  template < class Generator >
  auto gen_mt(Generator&& generator) {
    std::array<std::uint32_t, 10> seeds{{}};
    std::generate(std::begin(seeds), std::end(seeds), std::ref(generator));
    std::seed_seq seq(std::begin(seeds), std::end(seeds));
    return std::mt19937(seq);
  }

  template < class Generator >
  auto gen_mt_64(Generator&& generator) {
    std::array<std::uint32_t, 10> seeds{{}};
    std::generate(std::begin(seeds), std::end(seeds), std::ref(generator));
    std::seed_seq seq(std::begin(seeds), std::end(seeds));
    return std::mt19937_64(seq);
  }


  template < class Distribution >
  class randomize {
    Distribution dist;
    std::mt19937_64 mt;
  public:
    randomize() = default;
    template < class Generator, class... Args >
    randomize(Generator&& generator, Args&&... args)
      : dist(std::forward<Args>(args)...)
      , mt{gen_mt_64(std::forward<Generator>(generator))} {}

    template < class... Args >
    randomize& init_dist(Args&&... args) {
      dist = Distribution{ std::forward<Args>(args)... };
    }

    template < class Generator >
    randomize& init_seeds_from(Generator&& generator) {
      mt = gen_mt_64(std::forward<Generator>(generator));
    }

    template < class Generator >
    randomize& init_seeds_default() {
      mt = gen_mt_64(std::random_device{});
    }

    auto operator()() {
      return dist(mt);
    }

  };
}

#endif