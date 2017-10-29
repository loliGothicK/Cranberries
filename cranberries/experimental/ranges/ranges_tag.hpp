#ifndef CRANBERRIES_RANGES_TAG_HPP
#define CRANBERRIES_RANGES_TAG_HPP
#include "../../type_traits.hpp"

namespace cranberries {
namespace experimental {
namespace ranges {

namespace tag {
  struct adaptor_tag {}; // adaptor marking
  struct action_tag {}; // action adaptor marking
  struct view_tag {}; // view adaptor marking
  struct adaptor_proxy_tag {}; // adaptor proxy marking
  struct sentinel_range_tag {}; // sentinel range marking
  struct sentinel_iterator_tag {}; // sentinel iterator marking
}

template < class Iterator >
constexpr bool is_sentinel_iterator_v = std::is_base_of<tag::sentinel_iterator_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_sentinel_range_v = std::is_base_of<tag::sentinel_range_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_view_adaptor_v = std::is_base_of<tag::view_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_action_adaptor_v = std::is_base_of<tag::action_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_adaptor_v = std::is_base_of<tag::adaptor_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_adaptor_proxy_v = std::is_base_of<tag::adaptor_proxy_tag, Iterator>::value;


template < class T >
struct is_sentinel_iterator : bool_constant<is_sentinel_iterator_v<T>> {};
template < class T >
struct is_sentinel_range : bool_constant<is_sentinel_range_v<T>> {};
template < class T >
struct is_view_adaptor : bool_constant<is_view_adaptor_v<T>> {};
template < class T >
struct is_action_adaptor : bool_constant<is_action_adaptor_v<T>> {};
template < class T >
struct is_adaptor : bool_constant<is_adaptor_v<T>> {};
template < class T >
struct is_adaptor_proxy : bool_constant<is_adaptor_proxy_v<T>> {};




}}}
#endif