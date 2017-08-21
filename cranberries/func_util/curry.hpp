#ifndef CRANBERRIES_FUNC_UTIL_CURRY_HPP
#define CRANBERRIES_FUNC_UTIL_CURRY_HPP

#include "../utility/utility.hpp"

namespace cranberries {
namespace cranberries_magic {
    template < size_t N, class Fn, class... Remainder >
    struct curried_{
    Fn f;
    std::tuple<Remainder...> remaind;
        
    template < class... Args > 
    std::enable_if_t<(sizeof...(Args) != N),
    curried_<N - sizeof...(Args), std::decay_t<Fn>, Remainder..., std::decay_t<Args>...>>
    operator()(Args&&... args) const noexcept
    {
        return { std::move(f), std::tuple_cat(remaind, std::forward_as_tuple(std::forward<Args>(args)...)) };
    }
        
    template < class... Args, std::enable_if_t<(sizeof...(Args) == N), std::nullptr_t> = nullptr > 
    decltype(auto)
    operator()(Args&&... args) const
        noexcept(noexcept(f(std::declval<Remainder>()..., std::declval<Args>()...)))
    {
        return cranberries::apply(std::move(f), std::tuple_cat(remaind, std::forward_as_tuple(std::forward<Args>(args)...)));
    }
    };
} // ! end namespace cranberries_magic
    template < size_t N, class F >
    cranberries_magic::curried_<N,F> curry(F&& f){
        return { std::forward<F>(f), {} };
    }
    template < class R, class... Args, bool IsNoExcept >
    auto curry(R(*f)(Args...)noexcept(IsNoExcept)){
        return curry<sizeof...(Args)>([f=f](auto&&...args)noexcept(IsNoExcept){ return f(std::forward<decltype(args)>(args)...); });
    }
} // ! end namespace cranberries

#endif