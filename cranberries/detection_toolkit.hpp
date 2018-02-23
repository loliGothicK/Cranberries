#ifndef CRANBERRIES_DETECTION_TOOLKIT_HPP
#define CRANBERRIES_DETECTION_TOOLKIT_HPP
#include <type_traits>
#include <utility>
#include "type_traits.hpp"

namespace cranberries {
namespace cranberries_magic {


	template < class Default
					 , class
					 , template < class... > class Require
					 , class... Args >
	struct detector {
		using type = Default;
		static constexpr bool value = false;
	};

	template < class Default
					 , template < class... > class Require
					 , class... Args >
	struct detector<Default, void_t<Require<Args...>>, Require, Args...> {
		using type = Require<Args...>;
		static constexpr bool value = true;
	};

}

	template < template < class... > class Require, class... Args >
	using is_satisfied = cranberries_magic::detector<void, void, Require, Args...>;

	template < template < class... > class Require, class... Args >
	using is_satisfied_t = typename is_satisfied<Require, Args...>::type;

	template < template < class... > class Require, class... Args >
	constexpr bool is_satisfied_v = is_satisfied<Require, Args...>::value;

}

#endif
