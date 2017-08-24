/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_UTILITY_UTILITY_HPP
#define CRANBERRIES_UTILITY_UTILITY_HPP

#include <utility>
#include <type_traits>
#include "../type_traits.hpp"


namespace cranberries
{
  template <class C>
  inline constexpr auto size( const C& c ) -> decltype(c.size())
  {
    return c.size();
  }

  template <class T, std::size_t N>
  inline constexpr std::size_t size( const T(&)[N] ) noexcept
  {
    return N;
  }

  template < class , class = void >
  struct enable_get : std::false_type{};
  
  template < class T >
  struct enable_get<T,
    cranberries::void_t<decltype(std::get<0>(std::declval<T>()))>
  > : std::true_type {};
  
  template < class T >
  constexpr bool enable_get_v = enable_get<T>::value;  
  
  struct Swallows {
    template < class ... Dummy >
    constexpr Swallows(Dummy&&...) noexcept {}
  };

namespace cranberries_magic
{
  template<class T>
  using not_ref_wrapper = negation<is_reference_wrapper<std::decay_t<T>>>;

  template <class D, class...> struct return_type_helper { using type = D; };
  template <class... Types>
  struct return_type_helper<void, Types...> : std::common_type<Types...> {
    static_assert(conjunction_v<not_ref_wrapper<Types>...>,
      "Types cannot contain reference_wrappers when D is void");
  };

  template <class D, class... Types>
  using return_type = std::array<typename return_type_helper<D, Types...>::type,
    sizeof...(Types)>;

} // ! namespace cranberries_magic


  template < class D = void, class... Types>
  inline constexpr cranberries_magic::return_type<D, Types...> make_array( Types&&... t ) {
    return{{ std::forward<Types>( t )... }};
  }

  template < typename F >
  class Finally
  {
     F f_;
  public:
    constexpr Finally( F f ) noexcept : f_( f ) {}
    Finally() = delete;
    Finally( const Finally& ) = delete;
    Finally( Finally&& ) = delete;
    Finally& operator=( const Finally& ) = delete;
    Finally& operator=( Finally&& ) = delete;
    ~Finally() noexcept { f_(); }
  };

  template < typename F >
  inline constexpr Finally<std::decay_t<F>> make_finally(F&& f){
    return {std::forward<F>(f)};
  }

  template <class F>
  struct fix_result
  {
    F f_;

    template <class ...Args>
    constexpr auto operator()(Args&& ...args) const
      noexcept(noexcept(f_(*std::declval<fix_result const *>(), std::declval<Args>()...)))
      -> decltype(f_(*std::declval<fix_result const *>(), std::declval<Args>()...))
    {
      return f_(std::move(*this), std::forward<Args>(args)...);
    }
  };

  template <class F>
  inline constexpr fix_result<std::decay_t<F>> make_fix(F &&f) noexcept
  {
    return { std::forward<F>(f) };
  }
  
namespace cranberries_magic {
 
  template <class Base, class T, class Derived, class... Args>
  inline auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
      noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
   -> std::enable_if_t<std::is_function<T>::value &&
                       std::is_base_of<Base, std::decay_t<Derived>>::value,
      decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>
  {
        return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
  }
 
  template <class Base, class T, class RefWrap, class... Args>
  inline auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
      noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
   -> std::enable_if_t<std::is_function<T>::value &&
                       is_reference_wrapper_v<std::decay_t<RefWrap>>,
      decltype((ref.get().*pmf)(std::forward<Args>(args)...))>
 
  {
        return (ref.get().*pmf)(std::forward<Args>(args)...);
  }
 
  template <class Base, class T, class Pointer, class... Args>
  inline auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
      noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
   -> std::enable_if_t<std::is_function<T>::value &&
                       !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                       !std::is_base_of<Base, std::decay_t<Pointer>>::value,
      decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))>
  {
        return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
  }
 
  template <class Base, class T, class Derived>
  inline auto INVOKE(T Base::*pmd, Derived&& ref)
      noexcept(noexcept(std::forward<Derived>(ref).*pmd))
   -> std::enable_if_t<!std::is_function<T>::value &&
                       std::is_base_of<Base, std::decay_t<Derived>>::value,
      decltype(std::forward<Derived>(ref).*pmd)>
  {
        return std::forward<Derived>(ref).*pmd;
  }
 
  template <class Base, class T, class RefWrap>
  inline auto INVOKE(T Base::*pmd, RefWrap&& ref)
      noexcept(noexcept(ref.get().*pmd))
   -> std::enable_if_t<!std::is_function<T>::value &&
                       is_reference_wrapper_v<std::decay_t<RefWrap>>,
      decltype(ref.get().*pmd)>
  {
        return ref.get().*pmd;
  }
 
  template <class Base, class T, class Pointer>
  inline auto INVOKE(T Base::*pmd, Pointer&& ptr)
      noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
   -> std::enable_if_t<!std::is_function<T>::value &&
                       !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                       !std::is_base_of<Base, std::decay_t<Pointer>>::value,
      decltype((*std::forward<Pointer>(ptr)).*pmd)>
  {
        return (*std::forward<Pointer>(ptr)).*pmd;
  }
 
  template <class F, class... Args>
  inline auto INVOKE(F&& f, Args&&... args)
      noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
   -> std::enable_if_t<!std::is_member_pointer<std::decay_t<F>>::value,
      decltype(std::forward<F>(f)(std::forward<Args>(args)...))>
  {
        return std::forward<F>(f)(std::forward<Args>(args)...);
  }

} // ! namespace cranberries_magic
 
  template< class F, class... ArgTypes >
  inline auto invoke(F&& f, ArgTypes&&... args)
      // exception specification for QoI
      noexcept(noexcept(cranberries_magic::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...)))
   -> decltype(cranberries_magic::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...))
  {
      return cranberries_magic::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...);
  }

namespace cranberries_magic {

  template <class F, class Tuple, std::size_t... I>
  inline constexpr decltype(auto) apply_impl( F&& f, Tuple&& t, std::index_sequence<I...> )
  {
    return cranberries::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
    // Note: std::invoke is a C++17 feature
  }

} // ! namespace cranberries_magic
 
  template <class F, class Tuple>
  inline constexpr decltype(auto) apply(F&& f, Tuple&& t)
  {
      return cranberries_magic::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
          std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
  }



  template <
    typename Range,
    typename BinaryOp,
    typename T = element_type_of_t<Range>,
    enabler_t<conjunction_v<
          is_range<Range>,
          is_callable<BinaryOp,T,T>
    >> = nullptr
  >
  inline void adjacent_for_each
  (
    Range&& range,
    BinaryOp&& f
  ) {
    // for ADL
    using std::begin;
    using std::end;

    auto&& first = begin(range);
    auto&& last = end(range);

    while (std::next(first) != last)
    {
      auto&& a = *first;
      ++first;
      auto&& b = *first;
      f(a, b);
    }
  }

  template < typename F >
  inline auto make_delegate(F&& f) noexcept {
    return[f = std::move(f)](auto&&... args){
      return f(std::forward<decltype(args)>(args)...);
    };
  }

  template < size_t B, size_t E >
  struct Exp_ : size_constant<B*Exp_<B,E-1>::value> {};
  
  template < size_t B >
  struct Exp_<B,1> : size_constant<B> {};
  
  
  template < size_t Head, size_t... Digits >
  struct to_decimal
    : size_constant< (Head-size_t('0'))*Exp_<10, sizeof...(Digits)>::value + to_decimal<Digits...>::value > {};
  
  template < size_t Head >
  struct to_decimal<Head>
    : size_constant< Head-size_t('0') > {};

  
} // ! - end namespace cranberries
#endif