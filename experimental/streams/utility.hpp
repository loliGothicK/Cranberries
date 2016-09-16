#ifndef CRANBERRIES_STREAMS_OPERATORS_UTILITY_HPP
#define CRANBERRIES_STREAMS_OPERATORS_UTILITY_HPP
#include <utility>
#include "detail/tag.hpp"

namespace cranberries {

  template < typename F >
  class Finally
  {
     F f_;
  public:
    Finally( F f ) noexcept : f_( f ) {}
    Finally() = delete;
    Finally( const Finally& ) = delete;
    Finally( Finally&& ) = default;
    Finally& operator=( const Finally& ) = delete;
    Finally& operator=( Finally&& ) = default;
    ~Finally() noexcept { f_(); }
  };

  template < typename F >
  Finally<F> make_finally(F&& f){
    return Finally<F>{std::forward<F>(f)};
  }



namespace detail {

  template <class T>
  struct is_reference_wrapper : std::false_type {};
  template <class U>
  struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
  template <class T>
  constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;
 
  template <class Base, class T, class Derived, class... Args>
  auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
      noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
   -> std::enable_if_t<std::is_function<T>::value &&
                       std::is_base_of<Base, std::decay_t<Derived>>::value,
      decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>
  {
        return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
  }
 
  template <class Base, class T, class RefWrap, class... Args>
  auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
      noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
   -> std::enable_if_t<std::is_function<T>::value &&
                       is_reference_wrapper_v<std::decay_t<RefWrap>>,
      decltype((ref.get().*pmf)(std::forward<Args>(args)...))>
 
  {
        return (ref.get().*pmf)(std::forward<Args>(args)...);
  }
 
  template <class Base, class T, class Pointer, class... Args>
  auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
      noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
   -> std::enable_if_t<std::is_function<T>::value &&
                       !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                       !std::is_base_of<Base, std::decay_t<Pointer>>::value,
      decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))>
  {
        return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
  }
 
  template <class Base, class T, class Derived>
  auto INVOKE(T Base::*pmd, Derived&& ref)
      noexcept(noexcept(std::forward<Derived>(ref).*pmd))
   -> std::enable_if_t<!std::is_function<T>::value &&
                       std::is_base_of<Base, std::decay_t<Derived>>::value,
      decltype(std::forward<Derived>(ref).*pmd)>
  {
        return std::forward<Derived>(ref).*pmd;
  }
 
  template <class Base, class T, class RefWrap>
  auto INVOKE(T Base::*pmd, RefWrap&& ref)
      noexcept(noexcept(ref.get().*pmd))
   -> std::enable_if_t<!std::is_function<T>::value &&
                       is_reference_wrapper_v<std::decay_t<RefWrap>>,
      decltype(ref.get().*pmd)>
  {
        return ref.get().*pmd;
  }
 
  template <class Base, class T, class Pointer>
  auto INVOKE(T Base::*pmd, Pointer&& ptr)
      noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
   -> std::enable_if_t<!std::is_function<T>::value &&
                       !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                       !std::is_base_of<Base, std::decay_t<Pointer>>::value,
      decltype((*std::forward<Pointer>(ptr)).*pmd)>
  {
        return (*std::forward<Pointer>(ptr)).*pmd;
  }
 
  template <class F, class... Args>
  auto INVOKE(F&& f, Args&&... args)
      noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
   -> std::enable_if_t<!std::is_member_pointer<std::decay_t<F>>::value,
      decltype(std::forward<F>(f)(std::forward<Args>(args)...))>
  {
        return std::forward<F>(f)(std::forward<Args>(args)...);
  }

} // ! namespace detail
 
  template< class F, class... ArgTypes >
  auto invoke(F&& f, ArgTypes&&... args)
      // exception specification for QoI
      noexcept(noexcept(detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...)))
   -> decltype(detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...))
  {
      return detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...);
  }

namespace detail {

  template <class F, class Tuple, std::size_t... I>
  constexpr decltype(auto) apply_impl( F&& f, Tuple&& t, std::index_sequence<I...> )
  {
    return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
    // Note: std::invoke is a C++17 feature
  }

} // ! namespace detail
 
  template <class F, class Tuple>
  constexpr decltype(auto) apply(F&& f, Tuple&& t)
  {
      return detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
          std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
  }

namespace detail{

  struct is_callable_impl
  {
    template < typename F, typename ...Args >
    static auto check( F&& f, std::tuple<Args...>&& tup )->decltype( apply( f, tup ), std::true_type{} );
  
    template < typename F, typename ...Args >
    static std::false_type check(...);
  };

} // ! namespace detail

  template < typename F, typename ...Args >
  class is_callable : public decltype( detail::is_callable_impl::check<F, Args...>( std::declval<F>(), std::declval<std::tuple<Args...>>() ) ) {};

  template < typename F, typename ...Args >
  constexpr bool is_callable_v = is_callable<F, Args...>::value;

namespace detail{

  struct is_range_impl1
  {
    template < typename T >
    static auto check(T&& v)->decltype(begin(v), end(v), std::true_type{});
  
    template < typename T >
    static std::false_type check(...);
  };

  struct is_range_impl2
  {
    template < typename T >
    static auto check(T&& v)->decltype(v.begin(), v.end(), std::true_type{});
  
    template < typename T >
    static std::false_type check(...);
  };

  template < typename T >
  class is_range1 : public decltype( is_range_impl1::check<T>( std::declval<T>() ) ) {};

  template < typename T >
  class is_range2 : public decltype( is_range_impl2::check<T>( std::declval<T>() ) ) {};

} // ! namespace detail

  template < typename T >
  class is_range : public std::conditional_t<detail::is_range1<T>::value || detail::is_range2<T>::value,std::true_type,std::false_type> {};

  template < typename T >
  constexpr bool is_range_v = is_range<std::decay_t<T>>::value;

  template< typename T,typename U, typename = void >
  struct is_equality_comparable : std::false_type
  {};

  template< typename T, typename U >
  struct is_equality_comparable<T,U,
    std::enable_if_t<
        true, 
        decltype(std::declval<T&>() == std::declval<U&>(), (void)0)
        >
    > : std::true_type
  {};

  template< typename T >
  struct is_equality_comparable<T,T,
    std::enable_if_t<
        true, 
        decltype(std::declval<T&>() == std::declval<T&>(), (void)0)
        >
    > : std::true_type
  {};


  template < typename T, typename U >
  constexpr bool is_equality_comparable_to_v = is_equality_comparable<T,U>::value;

  template < typename T >
  constexpr bool is_equality_comparable_v = is_equality_comparable<T,T>::value;

namespace streams {
namespace detail {

  class InfiniteStreamBase {};

  template < typename T >
  constexpr bool is_infinite_stream_v = std::is_base_of<InfiniteStreamBase, std::decay_t<T>>::value;

  class FiniteStreamBase {};

  template < typename T >
  constexpr bool is_finite_stream_v = std::is_base_of<FiniteStreamBase, std::decay_t<T>>::value;

  template < typename T >
  constexpr bool is_stream_v = is_finite_stream_v<T> || is_infinite_stream_v<T>;

  class IntermidiateStreamOperatorBase{};

  class TerminateStreamOperatorBase{};

  class StreamOperatorBase{};

  class StreamFilterBase{};

  template < typename T >
  constexpr bool is_intermidiate_v = std::is_base_of<IntermidiateStreamOperatorBase, T>::value;

  template < typename T >
  constexpr bool is_terminate_v = std::is_base_of<TerminateStreamOperatorBase, T>::value;

  template < typename T >
  constexpr bool is_stream_operator_v = std::is_base_of<StreamOperatorBase, T>::value;

  template < typename T >
  constexpr bool is_stream_filter_v = std::is_base_of<StreamFilterBase, T>::value;

  template <
    typename Stream,
    std::enable_if_t<
      is_finite_stream_v<std::decay_t<Stream>>,
      std::nullptr_t
    > = nullptr
  >
  inline
  decltype( auto ) begin( Stream&& a ) {
    return a.begin();
  }

  template <
    typename Stream,
    std::enable_if_t<
      is_finite_stream_v<std::decay_t<Stream>>,
      std::nullptr_t
    > = nullptr
  >
  inline
  decltype( auto ) end( Stream&& a ) {
    return a.end();
  }


  template < typename F, typename Range >
  void
  expanded_loop
  (
    F&& f,
    Range&& v
  ) {
    size_t i{};
    for ( ; i < v.size() % 8; ++i )
        apply( f, v[i] );
    for ( ; i < v.size(); i += 8 )
    {
        apply( f, v[i] );
        apply( f, v[i + 1] );
        apply( f, v[i + 2] );
        apply( f, v[i + 3] );
        apply( f, v[i + 4] );
        apply( f, v[i + 5] );
        apply( f, v[i + 6] );
        apply( f, v[i + 7] );
    }
  }

 }


} // ! namespace stream

  template <
    typename Range,
    typename BinaryOp,
    typename T = typename std::decay_t<Range>::value_type,
    std::enable_if_t<
      is_range_v<Range>
      && is_callable_v<BinaryOp,T,T>,
    std::nullptr_t
    > = nullptr
  >
  void adjacent_for_each
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

} // ! namespace cranberries

#endif