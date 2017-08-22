/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_FUNC_UTIL_COMPOSITION_HPP
#define CRANBERRIES_FUNC_UTIL_COMPOSITION_HPP

#include <utility>
#include <type_traits>

namespace cranberries {
namespace cranberries_magic {

  template < class F, class G >
  struct composition_t {
    F f;
    G g;
    template < class... Args >
    constexpr decltype(auto) operator()(Args&&... args) const
      noexcept(noexcept(f(g(std::forward<Args>(args)...))))
    {
      return f(g(std::forward<Args>(args)...));
    }
  };

} // ! namespace cranberries_magic

namespace func_util {
template < class F, class G >
constexpr
cranberries_magic::composition_t<std::decay_t<F>, std::decay_t<G>>
operator * (F&& f, G&& g) noexcept {
  return { std::forward<F>(f), std::forward<G>(g) };
}
} // ! namespace func_util

template < class F, class G >
constexpr
cranberries_magic::composition_t<std::decay_t<F>, std::decay_t<G>>
composition(F&& f, G&& g) noexcept {
  return { std::forward<F>(f), std::forward<G>(g) };
}

template < class F, class G, class... Rest >
constexpr
auto
composition(F&& f, G&& g, Rest&&... rest) noexcept {
  using compsite_t = cranberries_magic::composition_t<std::decay_t<F>, std::decay_t<G>>;
  return composition(compsite_t{ std::forward<F>(f), std::forward<G>(g) }, std::forward<Rest>(rest)... );
}


} // ! namespace cranberries

#endif