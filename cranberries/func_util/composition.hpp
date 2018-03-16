/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_FUNC_UTIL_COMPOSITION_HPP
#define CRANBERRIES_FUNC_UTIL_COMPOSITION_HPP

#include <type_traits>
#include <utility>

namespace cranberries {

template <class F, class G>
class composited {
  F f;
  G g;

 public:
  constexpr composited(const F &f, const G &g) noexcept : f(f), g(g) {}
  constexpr composited(F &&f, const G &g) noexcept : f{std::move(f)}, g(g) {}
  constexpr composited(const F &f, G &&g) noexcept : f(f), g{std::move(g)} {}
  constexpr composited(F &&f, G &&g) noexcept
      : f{std::move(f)}, g{std::move(g)} {}
  constexpr composited() noexcept : f{}, g{} {}

  template <class... Args>
      constexpr decltype(auto) operator()(Args &&... args) &
      noexcept(noexcept(f(g(std::forward<Args>(args)...)))) {
    return f(g(std::forward<Args>(args)...));
  }
  template <class... Args>
  constexpr decltype(auto) operator()(Args &&... args) const &noexcept(
      noexcept(f(g(std::forward<Args>(args)...)))) {
    return f(g(std::forward<Args>(args)...));
  }
  template <class... Args>
      constexpr decltype(auto) operator()(Args &&... args) &&
      noexcept(noexcept(f(g(std::forward<Args>(args)...)))) {
    return std::move(f)(std::move(g)(std::forward<Args>(args)...));
  }
  template <class... Args>
  constexpr decltype(auto) operator()(Args &&... args) const &&noexcept(
      noexcept(f(g(std::forward<Args>(args)...)))) {
    return std::move(f)(std::move(g)(std::forward<Args>(args)...));
  }
};

template <class F, class G>
constexpr composited<std::decay_t<F>, std::decay_t<G>> composition(
    F &&f, G &&g) noexcept {
  return {std::forward<F>(f), std::forward<G>(g)};
}

template <class F, class G, class... Rest>
constexpr auto composition(F &&f, G &&g, Rest &&... rest) noexcept {
  using compsite_t = composited<std::decay_t<F>, std::decay_t<G>>;
  return composition(compsite_t{std::forward<F>(f), std::forward<G>(g)},
                     std::forward<Rest>(rest)...);
}

}  // namespace cranberries

#endif