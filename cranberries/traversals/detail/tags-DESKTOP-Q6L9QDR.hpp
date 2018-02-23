#ifndef CRANBERRIES_RANGES_TAG_HPP
#define CRANBERRIES_RANGES_TAG_HPP
#include "../../common/concepts.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

	namespace constraints_detection {
		template < template < class ... > class... >
		using template_void_t = void;

		template <
			class Default
			, class
			, class >
		struct constraints_detector
		{
			template < class T >
			using requires = typename Default::template requires<T>;
			static constexpr bool value = false;
		};

		template <
			class Default
			, class Adpt >
		struct constraints_detector<
			Default
			, template_void_t<std::decay_t<Adpt>::template requires>
			, Adpt >
		{
			template < class T >
			using requires = typename std::decay_t<Adpt>::template requires<T>;
			static constexpr bool value = true;
		};

		template < class Adpt, class Default >
		using has_requires_or = constraints_detector<Default, void, Adpt>;
	}



namespace tag {
	struct adaptor_tag {}; // adaptor marking
	struct action_tag {}; // action adaptor marking
	struct view_tag {}; // view adaptor marking
	struct adaptor_proxy_tag {}; // adaptor proxy marking
	struct sentinel_traversal_tag {}; // sentinel traversal marking
	struct sentinel_iterator_tag {}; // sentinel iterator marking
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
template < class Iterator >
constexpr bool is_adaptor_proxy_v = std::is_base_of<tag::adaptor_proxy_tag, Iterator>::value;


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
template < class T >
struct is_adaptor_proxy : bool_constant<is_adaptor_proxy_v<T>> {};




}}}
#endif