/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_UTILITY_HPP
#define CRANBERRIES_STREAMS_OPERATORS_UTILITY_HPP
#include <utility>
#include <type_traits>
#include "./integers.hpp"
#include "./OperationTree.hpp"

namespace cranberries {
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
    std::enable_if_t<
      cranberries_magic::is_finite_stream_v<Stream>, std::nullptr_t
    > = nullptr
  >
  inline
  decltype(auto) begin(Stream&& a) {
    return a.begin();
  }

  template <
    typename Stream,
    std::enable_if_t<
      cranberries_magic::is_finite_stream_v<Stream>, std::nullptr_t
    > = nullptr
  >
  inline
  decltype(auto) end(Stream&& a) {
    return a.end();
  }
} // ! namespace cranberries

#endif