/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_FUNC_UTIL_CURRY_HPP
#define CRANBERRIES_FUNC_UTIL_CURRY_HPP

#include "../utility/utility.hpp"
#include "../detection_toolkit.hpp"

namespace cranberries {
    
  template< class Fn, typename... Bind>
  class bind_expr {
    Fn fn;
    std::tuple<Bind...> bind_args;
      
    template < size_t... I, class F, class Tuple, typename... Args >
    static constexpr decltype(auto)
    invoke( std::index_sequence<I...>,
            F&& f,
            Tuple&& t,
            Args&&... args )
      noexcept(noexcept(f(std::get<I>(std::declval<Tuple>())..., std::declval<Args>()...)))
    {
      return std::forward<F>(f)(std::get<I>(t)..., std::forward<Args>(args)...);
    }
  public:
    template < typename... A >
    constexpr bind_expr(const Fn& fn, A&&... a) noexcept : fn(fn), bind_args{std::forward<A>(a)...} {}
    template < typename... A >
    constexpr bind_expr(Fn&& fn, A&&... a) noexcept : fn{fn}, bind_args{std::forward<A>(a)...} {}

    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) & noexcept {
        return invoke(std::index_sequence_for<Bind...>{}, fn, bind_args, std::forward<Args>(args)...);
    }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) && noexcept {
        return invoke(std::index_sequence_for<Bind...>{}, std::move(fn), std::move(bind_args), std::forward<Args>(args)...);
    }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) const& noexcept {
        return invoke(std::index_sequence_for<Bind...>{}, fn, bind_args, std::forward<Args>(args)...);
    }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) const&& noexcept {
        return invoke(std::index_sequence_for<Bind...>{}, std::move(fn), std::move(bind_args), std::forward<Args>(args)...);
    }
  };
  
  template < class F, typename... Args >
  inline constexpr
  bind_expr<std::decay_t<F>, Args...>
  make_bind_expr(F&& f, Args&&... args){
    return { std::forward<F>(f), std::forward<Args>(args)... };
  }
      
  template<typename Fn>
  class curried {
    Fn fn;

    template<typename F, typename... Args>
    static constexpr
    curried<bind_expr<Fn, std::decay_t<Args>...>>
    apply(F&& f, Args&&... args) noexcept {        
        return {{std::forward<F>(f), std::forward<Args>(args)...}};
    }

    template<typename F>
    static constexpr decltype(auto) apply(F&& f)
      noexcept(noexcept(std::forward<F>(f)()))
    {
        return std::forward<F>(f)();
    }
  public:
    constexpr curried(const Fn& fn) noexcept : fn{fn} {}
    constexpr curried(Fn&& fn) noexcept : fn{std::move(fn)} {}
    constexpr curried() noexcept : fn{} {}
    
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) & noexcept { return apply(fn, std::forward<Args>(args)...); }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) && noexcept { return apply(std::move(fn), std::forward<Args>(args)...); }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) const& noexcept { return apply(fn, std::forward<Args>(args)...); }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) const&& noexcept { return apply(std::move(fn), std::forward<Args>(args)...); }
  };
      
  template<size_t N, typename Fn>
  class curried_ {
    Fn fn;

    template<typename F, typename... Args>
    static constexpr
    std::enable_if_t<(N-sizeof...(Args))!=0,
        curried_<N-sizeof...(Args), bind_expr<Fn, std::decay_t<Args>...>>>
    apply(F&& f, Args&&... args) noexcept
    {        
        return {{std::forward<F>(f), std::forward<Args>(args)...}};
    }

    template<typename F, typename... Args, std::enable_if_t<(N-sizeof...(Args))==0,std::nullptr_t> = nullptr>
    static constexpr decltype(auto) apply(F&& f, Args&&... args)
      noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
    {
        return std::forward<F>(f)(std::forward<Args>(args)...);
    }
  public:
    constexpr curried_(const Fn& fn) noexcept : fn{fn} {}
    constexpr curried_(Fn&& fn) noexcept : fn{std::move(fn)} {}
    constexpr curried_() noexcept : fn{} {}
    
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) & noexcept { return apply(fn, std::forward<Args>(args)...); }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) && noexcept { return apply(std::move(fn), std::forward<Args>(args)...); }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) const& noexcept { return apply(fn, std::forward<Args>(args)...); }
    template<typename ...Args>
    constexpr decltype(auto) operator()(Args&& ...args) const&& noexcept { return apply(std::move(fn), std::forward<Args>(args)...); }
  };
  
  
  template < class F >
  inline constexpr
  curried<std::decay_t<F>>
  curry(F&& f) noexcept
  { return { std::forward<F>(f) }; }
      
  template < size_t N, class F >
  inline constexpr
  curried_<N, std::decay_t<F>>
  curry_(F&& f) noexcept
  { return { std::forward<F>(f) }; }

  template < class R, class... Args >
  inline constexpr
  auto
  curry_(R(*f)(Args...)) noexcept
  { 
    return curry_<sizeof...(Args)>( make_bind_expr(f) );
  }
  
} // ! end namespace cranberries

#endif