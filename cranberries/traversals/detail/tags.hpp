#ifndef CRANBERRIES_RANGES_TAG_HPP
#define CRANBERRIES_RANGES_TAG_HPP
#include "../../type_traits.hpp"


namespace cranberries {
inline namespace experimental {
namespace traversals {

namespace tag {
	struct adaptor_tag {}; // adaptor marking
	struct action_tag {}; // action adaptor marking
	struct view_tag {}; // view adaptor marking
	struct sentinel_traversal_tag {}; // sentinel traversal marking
	struct sentinel_iterator_tag {}; // sentinel iterator marking

	// for dispatch
	struct imutable_ {};
	struct mutable_ {};
	struct sized_ {};
	struct finite_ {};
	struct infinite_ {};
}



template < class Iterator >
constexpr bool is_sentinel_iterator_v = std::is_base_of<tag::sentinel_iterator_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_sentinel_traversal_v = std::is_base_of<tag::sentinel_traversal_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_view_adaptor_v = std::is_base_of<tag::view_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_action_adaptor_v = std::is_base_of<tag::action_tag, Iterator>::value;
template < class Iterator >
constexpr bool is_adaptor_v = std::is_base_of<tag::adaptor_tag, Iterator>::value;


template < class T >
struct is_sentinel_iterator : bool_constant<is_sentinel_iterator_v<T>> {};
template < class T >
struct is_sentinel_traversal : bool_constant<is_sentinel_traversal_v<T>> {};
template < class T >
struct is_view_adaptor : bool_constant<is_view_adaptor_v<T>> {};
template < class T >
struct is_action_adaptor : bool_constant<is_action_adaptor_v<T>> {};
template < class T >
struct is_adaptor : bool_constant<is_adaptor_v<T>> {};


}}}
#endif