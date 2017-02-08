#ifndef CRANBERRIES_STREAMS_OPERATORS_UTILITY_HPP
#define CRANBERRIES_STREAMS_OPERATORS_UTILITY_HPP
#include <utility>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <bitset>
#include "./integers.hpp"
#include "./OperationTree.hpp"

namespace cranberries {

  template < bool Pred, typename IfType = std::nullptr_t >
  using enabler_t = std::enable_if_t<Pred, IfType>;

  template < typename T, typename U, typename IfType = std::nullptr_t >
  using is_same_if_t = std::enable_if_t<std::is_same<std::decay_t<T>,std::decay_t<U>>::value,IfType>;

  template < typename T >
  struct remove_cvr {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
  };

  template < typename T >
  using remove_cvr_t = typename remove_cvr<T>::type;

  template < typename ...Dummy >
  using void_t = void;

  template <class C>
  constexpr auto size( const C& c ) -> decltype(c.size())
  {
    return c.size();
  }

  template <class T, std::size_t N>
  constexpr std::size_t size( const T( &array )[N] ) noexcept
  {
    return N;
  }

  template < typename Head, typename ...Tail >
  struct conj_impl : bool_constant<Head::value && conj_impl<Tail...>::value> {};

  template < typename B >
  struct conj_impl<B> : bool_constant<B::value> {};

  template < typename ...B >
  struct conjunction : conj_impl<B...> {};

  template < typename Head, typename ...Tail >
  struct disj_impl : bool_constant<Head::value || conj_impl<Tail...>::value> {};

  template < typename B >
  struct disj_impl<B> : bool_constant<B::value> {};

  template < typename ...B >
  struct disjunction : disj_impl<B...> {};

  template < typename B >
  struct negation : bool_constant<!B::value> {};


  template < typename ...B >
  constexpr bool conjunction_v = conjunction<B...>::value;

  template < typename ...B >
  constexpr bool disjunction_v = disjunction<B...>::value;

  template < typename B >
  constexpr bool negation_v = negation<B>::value;


namespace cranberries_magic
{
    template < class T >
    struct is_tuple_impl : std::false_type
    {};

    template < class...Args >
    struct is_tuple_impl<std::tuple<Args...>> : std::true_type
    {};

} // ! namespace cranberries_magic

  template < typename T >
  struct is_tuple : cranberries_magic::is_tuple_impl<T>
  {};

  template < typename T >
  constexpr bool is_tuple_v = is_tuple<T>::value;


namespace cranberries_magic {
  template < class T >
  struct is_bitset_impl : std::false_type {};

  template < size_t N >
  struct is_bitset_impl<std::bitset<N>> : std::true_type {};

}

  template < typename T >
  struct is_bitset : cranberries_magic::is_bitset_impl<T> {};

  template < typename T >
  constexpr bool is_bitset_v = is_bitset<T>::value;

namespace cranberries_magic {

    template < class, class = void >
    struct enable_std_begin_end : std::false_type
    {};

    template < class T >
    struct enable_std_begin_end<T,
      void_t<decltype(std::begin( std::declval<T&>() ), std::end( std::declval<T&>() ))>
    > : std::true_type
    {};

    template < class, class = void >
    struct enable_adl_begin_end : std::false_type
    {};

    template < class T >
    struct enable_adl_begin_end<T,
      void_t<decltype(begin( std::declval<T&>() ), end( std::declval<T&>() ))>
    > : std::true_type
    {};

} // ! namespace cranberries_magic

  template < typename T >
  struct is_range
    : bool_constant<
      disjunction_v<cranberries_magic::enable_std_begin_end<T>, cranberries_magic::enable_adl_begin_end<T>>
    >
  {};

  template < typename T >
  constexpr bool is_range_v = is_range<T>::value;

  template< class, class = void >
  struct is_equality_comparable : std::false_type
  {};

  template< class T >
  struct is_equality_comparable<T,
    void_t<decltype(std::declval<T&>() == std::declval<T&>())>
  > : std::true_type
  {};

  template< class, class, class = void >
  struct is_equality_comparable_to : std::false_type
  {};


  template< typename T, typename U >
  struct is_equality_comparable_to<T, U,
    void_t<decltype(std::declval<T&>() == std::declval<U&>())>
  > : std::true_type
  {};


  template < typename T, typename U >
  constexpr bool is_equality_comparable_to_v = is_equality_comparable_to<T, U>::value;

  template < typename T >
  constexpr bool is_equality_comparable_v = is_equality_comparable<T>::value;


  template < class, class = void >
  struct has_value_type : std::false_type
  {};
  
  template < class T >
  struct has_value_type<T,
    void_t<decltype(std::declval<typename std::decay_t<T>::value_type>())>
  > : std::true_type
  {};

  template < typename T >
  constexpr bool has_value_type_v = has_value_type<T>::value;

  template < class, class = void >
  struct has_value_field : std::false_type
  {};

  template < class T >
  struct has_value_field<T,
    void_t<decltype(std::declval<std::decay_t<T>::value>())>
  > : std::true_type
  {};

  template < typename T >
  constexpr bool has_value_field_v = has_value_field<T>::value;

  template < typename T, template<class> class Head, template<class> class ...Tail >
  struct conjunctional_impl : bool_constant< Head<T>::value && conjunctional_impl<T, Tail...>::value > {};

  template < typename T, template<class> class Pred >
  struct conjunctional_impl<T,Pred> : bool_constant< Pred<T>::value > {};

  template <
    template<class>class ...Preds
  >
  struct conjunctional
  {
    template < typename T >
    using type = conjunctional_impl<T, Preds...>;
  };

  template < typename T, template<class> class Head, template<class> class ...Tail >
  struct disjunctional_impl : bool_constant< Head<T>::value && conjunctional_impl<T, Tail...>::value > {};

  template < typename T, template<class> class Pred >
  struct disjunctional_impl<T, Pred> : bool_constant< Pred<T>::value > {};

  template < template<class>class ...Preds >
  struct disjunctional
  {
    template < typename T >
    using type = conjunctional_impl<T, Preds...>;

  };

  template < typename T, template<class> class Pred >
  struct negational_impl : bool_constant<!Pred<T>::value> {};

  template < template<class> class Pred >
  struct negational
  {
    template < typename T >
    using type = negational_impl<T, Pred>;
  };

  template < typename T, typename Tuple >
  struct tuple_all_match;

  template < typename T >
  struct tuple_all_match<T, std::tuple<>> : std::false_type {};

  template < typename T, typename U, typename ...Rest >
  struct tuple_all_match<T, std::tuple<U, Rest...>> : std::false_type {};

  template < typename T, typename ...Rest >
  struct tuple_all_match<T, std::tuple<T, Rest...>> : tuple_all_match<T, std::tuple<Rest...>> {};

  template < typename T >
  struct tuple_all_match<T, std::tuple<T>> : std::true_type {};

  template < typename T, typename Tuple >
  struct tuple_any_match;

  template < typename T >
  struct tuple_any_match<T, std::tuple<>> : std::false_type {};

  template < typename T, typename U, typename ...Rest >
  struct tuple_any_match<T, std::tuple<U, Rest...>> : tuple_any_match<T, std::tuple<Rest...>> {};

  template < typename T, typename ...Rest >
  struct tuple_any_match<T, std::tuple<T, Rest...>> : std::true_type {};

  template < typename T, typename Tuple >
  struct tuple_none_match;

  template < typename T >
  struct tuple_none_match<T, std::tuple<>> : std::false_type {};

  template < typename T, typename U, typename ...Rest >
  struct tuple_none_match<T, std::tuple<U, Rest...>> : tuple_none_match<T, std::tuple<Rest...>> {};

  template < typename T, typename ...Rest >
  struct tuple_none_match < T, std::tuple<T, Rest...>> : std::false_type {};

  template < typename T, typename U >
  struct tuple_none_match<T, std::tuple<U>> : std::true_type {};

  template < template <class> class Pred, typename Tuple >
  struct tuple_all_match_if;

  template < template <class> class Pred >
  struct tuple_all_match_if<Pred, std::tuple<>> : std::false_type {};

  template < template <class> class Pred, typename T, typename ...Rest >
  struct tuple_all_match_if<Pred, std::tuple<T, Rest...>> : std::conditional_t<Pred<T>::value, tuple_all_match_if<Pred,std::tuple<Rest...>>,std::false_type> {};

  template < template <class> class Pred, typename T >
  struct tuple_all_match_if<Pred, std::tuple<T>> : std::conditional_t<Pred<T>::value, std::true_type, std::false_type> {};

  template < template <class> class Pred, typename Tuple >
  struct tuple_any_match_if;

  template < template <class> class Pred >
  struct tuple_any_match_if<Pred, std::tuple<>> : std::false_type {};

  template < template <class> class Pred, typename T, typename ...Rest >
  struct tuple_any_match_if<Pred, std::tuple<T, Rest...>> : std::conditional_t<Pred<T>::value, std::true_type, tuple_any_match_if<Pred, std::tuple<Rest...>>> {};

  template < template <class> class Pred, typename T >
  struct tuple_any_match_if<Pred, std::tuple<T>> : std::conditional_t<Pred<T>::value, std::true_type, std::false_type> {};

  template < template <class> class Pred, typename Tuple >
  struct tuple_none_match_if;

  template < template <class> class Pred >
  struct tuple_none_match_if<Pred, std::tuple<>> : std::false_type {};

  template < template <class> class Pred, typename T, typename ...Rest >
  struct tuple_none_match_if<Pred, std::tuple<T, Rest...>> : std::conditional_t<!Pred<T>::value, tuple_all_match_if<Pred, std::tuple<Rest...>>, std::false_type> {};

  template < template <class> class Pred, typename T >
  struct tuple_none_match_if<Pred, std::tuple<T>> : std::conditional_t<!Pred<T>::value, std::true_type, std::false_type> {};


  template < typename T, class Tuple >
  constexpr bool tuple_all_match_v = tuple_all_match<T, Tuple>::value;

  template < typename T, class Tuple >
  constexpr bool tuple_any_match_v = tuple_any_match<T, Tuple>::value;
  
  template < typename T, class Tuple >
  constexpr bool tuple_none_match_v = tuple_none_match<T, Tuple>::value;

  template < template<class> class Pred, class Tuple >
  constexpr bool tuple_all_match_if_v = tuple_all_match_if<Pred, Tuple>::value;

  template < template<class> class Pred, class Tuple >
  constexpr bool tuple_any_match_if_v = tuple_any_match_if<Pred, Tuple>::value;

  template < template<class> class Pred, class Tuple >
  constexpr bool tuple_none_match_if_v = tuple_none_match_if<Pred, Tuple>::value;

  template < typename T, typename ...Args >
  struct all_match : tuple_all_match<T, std::tuple<Args...>> {};

  template < typename T, typename ...Args >
  struct any_match : tuple_any_match<T, std::tuple<Args...>> {};

  template < typename T, typename ...Args >
  struct none_match : tuple_none_match<T, std::tuple<Args...>> {};

  template < template<class> class Pred, typename ...Args >
  struct all_match_if : tuple_all_match_if<Pred, std::tuple<Args...>> {};

  template < template<class> class Pred, typename ...Args >
  struct any_match_if : tuple_any_match_if<Pred, std::tuple<Args...>> {};

  template < template<class> class Pred, typename ...Args >
  struct none_match_if : tuple_none_match_if<Pred, std::tuple<Args...>> {};


  template < typename T, typename ...Args >
  constexpr bool all_match_v = all_match<T, Args...>::value;

  template < typename T, typename ...Args >
  constexpr bool any_match_v = any_match<T, Args...>::value;

  template < typename T, typename ...Args >
  constexpr bool none_match_v = none_match<T, Args...>::value;

  template < template<class> class Pred, typename ...Args >
  constexpr bool all_match_if_v = all_match_if<Pred, Args...>::value;

  template < template<class> class Pred, typename ...Args >
  constexpr bool any_match_if_v = any_match_if<Pred, Args...>::value;

  template < template<class> class Pred, typename ...Args >
  constexpr bool none_match_if_v = none_match_if<Pred, Args...>::value;
  
namespace cranberries_magic
{
    template<class> struct is_ref_wrapper : std::false_type {};
    template<class T> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

    template<class T>
    using not_ref_wrapper = negation<is_ref_wrapper<std::decay_t<T>>>;

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
  constexpr cranberries_magic::return_type<D, Types...> make_array( Types&&... t ) {
    return{ std::forward<Types>( t )... };
  }

  template <
    typename T,
    bool A = std::is_array<remove_cvr_t<T>>::value,
    bool B = is_tuple_v<remove_cvr_t<T>>
  >
  struct element_type_of
  {
    using type = typename remove_cvr_t<T>::value_type;
  };

  template <
    typename T
  >
  struct element_type_of<T, true, false>
  {
    using type = std::remove_extent_t<remove_cvr_t<T>>;
  };

  template <
    typename T
  >
  struct element_type_of<T, false, true>
  {
    static_assert(
      tuple_all_match<
        std::tuple_element_t<0, remove_cvr_t<T>>, remove_cvr_t<T>
      >::value,
      "tuple"
    );
    using type = typename std::tuple_element_t<0, remove_cvr_t<T>>;
  };


  template < typename T >
  using element_type_of_t = typename element_type_of<T>::type;

  template <
    typename T,
    bool B = is_range_v<T>
  >
  struct root_element_type_of {
    using type = typename root_element_type_of<element_type_of_t<T>>::type;
  };

  template < typename T >
  struct root_element_type_of<T, false> {
    using type = T;
  };

  template < typename T >
  using root_element_type_of_t = typename root_element_type_of<T>::type;

  template < class, class = std::nullptr_t >
  struct is_iterator : std::false_type {};

  template < typename T >
  struct is_iterator<T,
    enabler_t<disjunction_v<
      std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>,
      std::is_base_of<std::output_iterator_tag, typename std::iterator_traits<T>::iterator_category>
    >>
  > : std::true_type
  {};

  template < typename T >
  constexpr bool is_iterator_v = is_iterator<T>::value;

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
    return {std::forward<F>(f)};
  }



namespace cranberries_magic {

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

} // ! namespace cranberries_magic
 
  template< class F, class... ArgTypes >
  auto invoke(F&& f, ArgTypes&&... args)
      // exception specification for QoI
      noexcept(noexcept(cranberries_magic::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...)))
   -> decltype(cranberries_magic::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...))
  {
      return cranberries_magic::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...);
  }

namespace cranberries_magic {

  template <class F, class Tuple, std::size_t... I>
  constexpr decltype(auto) apply_impl( F&& f, Tuple&& t, std::index_sequence<I...> )
  {
    return cranberries::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
    // Note: std::invoke is a C++17 feature
  }

} // ! namespace cranberries_magic
 
  template <class F, class Tuple>
  constexpr decltype(auto) apply(F&& f, Tuple&& t)
  {
      return cranberries_magic::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
          std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
  }

namespace cranberries_magic{

  struct is_callable_impl
  {
    template < typename F, typename ...Args >
    static auto check( F&& f, std::tuple<Args...>&& tup )->decltype( apply( f, tup ), std::true_type{} );
  
    template < typename F, typename ...Args >
    static std::false_type check(...);
  };

} // ! namespace cranberries_magic

  template < typename F, typename ...Args >
  class is_callable : public decltype( cranberries_magic::is_callable_impl::check<F, Args...>( std::declval<F>(), std::declval<std::tuple<Args...>>() ) ) {};

  template < typename F, typename ...Args >
  constexpr bool is_callable_v = is_callable<F, Args...>::value;




namespace cranberries_magic {

  class InfiniteStreamBase {};

  template < typename T >
  constexpr bool is_infinite_stream_v = std::is_base_of<InfiniteStreamBase, std::decay_t<T>>::value;

  template < typename T >
  struct is_infinite_stream {
    static constexpr bool value = is_infinite_stream_v<T>;
  };

  class FiniteStreamBase {};

  template < typename T >
  constexpr bool is_finite_stream_v = std::is_base_of<FiniteStreamBase, std::decay_t<T>>::value;

  template < typename T >
  struct is_finite_stream {
    static constexpr bool value = is_finite_stream_v<T>;
  };

  template < typename T >
  constexpr bool is_stream_v = is_finite_stream_v<T> || is_infinite_stream_v<T>;

  template < typename T >
  struct is_stream {
    static constexpr bool value = is_stream_v<T>;
  };

  class LazyOperationModuleBase{};

  class EagerOperationModuleBase{};

  class StreamOperatorBase{};

  class StreamFilterBase{};

  template < typename T >
  constexpr bool is_lazy_v = std::is_base_of<LazyOperationModuleBase, T>::value;

  template < typename T >
  struct is_lazy {
    static constexpr bool value = is_lazy_v<T>;
  };

  template < typename T >
  constexpr bool is_eager_v = std::is_base_of<EagerOperationModuleBase, T>::value;

  template < typename T >
  struct is_eager {
    static constexpr bool value = is_eager_v<T>;
  };

  template < typename T >
  constexpr bool is_stream_operator_v = std::is_base_of<StreamOperatorBase, T>::value;

  template < typename T >
  struct is_stream_operator {
    static constexpr bool value = is_stream_operator_v<T>;
  };

  template < typename T >
  constexpr bool is_stream_filter_v = std::is_base_of<StreamFilterBase, T>::value;

  template < typename T >
  struct is_stream_filter {
    static constexpr bool value = is_stream_filter_v<T>;
  };

  class SequencialOperatorBase{};

  class RangeOperatorBase{};

  class FilteringOperatorBase{};

  template < typename T >
  constexpr bool is_sequencial_operator_v = std::is_base_of<SequencialOperatorBase, std::decay_t<T>>::value;

  template < typename T >
  struct is_sequencial_operator {
    static constexpr bool value = is_sequencial_operator_v<T>;
  };

  template < typename T >
  constexpr bool is_range_operator_v = std::is_base_of<RangeOperatorBase, std::decay_t<T>>::value;

  template < typename T >
  struct is_range_operator {
    static constexpr bool value = is_range_operator_v<T>;
  };

  template < typename T >
  constexpr bool is_filtering_operator_v = std::is_base_of<FilteringOperatorBase, std::decay_t<T>>::value;

  template < typename T >
  struct is_filtering_operator {
    static constexpr bool value = is_filtering_operator_v<T>;
  };

  template < class T >
  struct is_operation_tree_impl : std::false_type
  {};

  template < class O, class P >
  struct is_operation_tree_impl<streams::OperationTree<O,P>> : std::true_type
  {};

  template <
    typename T
  >
  struct is_operation_tree : cranberries_magic::is_operation_tree_impl<std::decay_t<T>> {};

  template < typename T >
  constexpr bool is_operation_tree_v = is_operation_tree<T>::value;

} // ! namespace cranberries_magic

  template <
    typename Stream,
    enabler_t<
    cranberries_magic::is_finite_stream_v<Stream>
    > = nullptr
  >
  inline
  decltype(auto) begin(Stream&& a) {
    return a.begin();
  }

  template <
    typename Stream,
    enabler_t<
    cranberries_magic::is_finite_stream_v<Stream>
    > = nullptr
  >
  inline
  decltype(auto) end(Stream&& a) {
    return a.end();
  }


  template <
    typename Range,
    typename BinaryOp,
    typename T = element_type_of_t<Range>,
    enabler_t<
      conjunction_v<
        is_range<Range>, is_callable<BinaryOp,T,T>
    >> = nullptr
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