#ifndef CRANBERRIES_DETECTION_TOOLKIT_HPP
#define CRANBERRIES_DETECTION_TOOLKIT_HPP
#include <type_traits>
#include <utility>
#include "type_traits.hpp"
#include "utility/utility.hpp"

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
		static constexpr bool value = false;
	};

}

	template < template < class... > class Require, class... Args >
	using is_satisfied = cranberries_magic::detector<void, void, Require, Args...>;

	template < template < class... > class Require, class... Args >
	using is_satisfied_t = typename is_satisfied<Require, Args...>::type;

	template < template < class... > class Require, class... Args >
	constexpr bool is_satisfied_v = is_satisfied<Require, Args...>::value;

	template < class Default, template < class... > class Requirements, class... Ts >
	struct concept_require
		: nested_type_class<std::enable_if_t<is_satisfied_v<Requirements, Ts...>,Default>> {};

	template < class Default, template < class... > class Req, class... Ts >
	using satisfied_if_t = typename concept_require<Default, Req, Ts...>::type;

	template < class Iter, class IfType = std::nullptr_t >
	using iter_require
		= std::enable_if_t<conjunction_v<std::is_same<Iter, decltype(std::declval<Iter&>()++)>,
																		 std::is_same<Iter&,decltype(++std::declval<Iter&>())>,
																		 std::is_copy_constructible<Iter>,
																		 std::is_copy_assignable<Iter>,
																		 std::is_destructible<Iter>,
																		 is_dereferencable<Iter>,
																		 is_swappable<Iter>,
																		 true_t<typename std::iterator_traits<Iter>::difference_type>,
																		 true_t<typename std::iterator_traits<Iter>::value_type>,
																		 true_t<typename std::iterator_traits<Iter>::pointer>,
																		 true_t<typename std::iterator_traits<Iter>::reference>,
																		 true_t<typename std::iterator_traits<Iter>::iterator_category>>,
																	 IfType >;

	template < class Iter, class IfType = std::nullptr_t >
	using input_iter_require
		= std::enable_if_t<conjunction_v<
				iter_require<Iter,std::true_type>,
				std::is_constructible<bool, decltype(std::declval<const Iter&>() == std::declval<const Iter&>())>,
				std::is_constructible<bool, decltype(std::declval<const Iter&>() != std::declval<const Iter&>())>>,
			IfType >;


	template < class Iter, class IfType = std::nullptr_t >
	using output_iter_require
		= std::enable_if_t<conjunction_v<
				iter_require<Iter,std::true_type>,
				std::is_assignable<decltype(*std::declval<Iter>()), decltype(*std::declval<Iter>())> >,
			IfType >;

	template < class Iter, class IfType = std::nullptr_t >
	using forward_iter_require
		= std::enable_if_t<conjunction_v<
				input_iter_require<Iter, std::true_type>,
				output_iter_require<Iter, std::true_type>,
				std::is_default_constructible<Iter>>,
			IfType >;

	template < class Iter, class IfType = std::nullptr_t >
	using bidirectional_iter_require
		= std::enable_if_t<conjunction_v<
				std::is_same<Iter, decltype(std::declval<Iter&>()--)>,
				std::is_same<Iter&, decltype(--std::declval<Iter&>())>,
				forward_iter_require<Iter, std::true_type>>,
			IfType >;

	template < class Iter, class IfType = std::nullptr_t >
	using random_accsess_iter_require
		= std::enable_if_t<conjunction_v<
				std::is_same<Iter, decltype(std::declval<const Iter&>()+2)>,
				std::is_same<Iter, decltype(std::declval<const Iter&>()-2)>,
				std::is_same<Iter&, decltype(std::declval<Iter&>()+=2)>,
				std::is_same<Iter&, decltype(std::declval<Iter&>()-=2)>,
				true_t<decltype(bool(std::declval<const Iter&>()<std::declval<const Iter&>()))>,
				true_t<decltype(bool(std::declval<const Iter&>()<=std::declval<const Iter&>()))>,
				true_t<decltype(bool(std::declval<const Iter&>()>std::declval<const Iter&>()))>,
				true_t<decltype(bool(std::declval<const Iter&>()>=std::declval<const Iter&>()))>,
				bidirectional_iter_require<Iter, std::true_type>>,
			IfType >;

	template < class T, class IfType = std::nullptr_t >
	using arithmetic4_require
		= std::enable_if_t<conjunction_v<
				std::is_convertible<T, std::decay_t<decltype(std::declval<const T&>() + std::declval<const T&>())>>,
				std::is_convertible<T, std::decay_t<decltype(std::declval<const T&>() - std::declval<const T&>())>>,
				std::is_convertible<T, std::decay_t<decltype(std::declval<const T&>() * std::declval<const T&>())>>,
				std::is_convertible<T, std::decay_t<decltype(std::declval<const T&>() / std::declval<const T&>())>>>,
			IfType >;

	// 振る舞いについては定義を信頼する
	template < class T, class IfType = std::nullptr_t >
	using equality_require
		= std::enable_if_t<conjunction_v<
				true_t<decltype(bool(std::declval<const T&>() == std::declval<const T&>()))>,
				true_t<decltype(bool(std::declval<const T&>() != std::declval<const T&>()))>>,
			IfType >;

	// 振る舞いについては定義を信頼する
	template < class T, class IfType = std::nullptr_t >
	using equivalence_require
		= std::enable_if_t<conjunction_v<
				equality_require<T,std::true_type>,
				std::is_constructible<bool, decltype(std::declval<const T&>() < std::declval<const T&>())>,
				std::is_constructible<bool, decltype(std::declval<const T&>() <= std::declval<const T&>())>,
				std::is_constructible<bool, decltype(std::declval<const T&>() > std::declval<const T&>())>,
				std::is_constructible<bool, decltype(std::declval<const T&>() >= std::declval<const T&>())>>,
			IfType >;

	template < class Range, class IfType = std::nullptr_t >
	using range_require
		= std::enable_if_t<conjunction_v<
				std::is_same<decltype(std::begin(std::declval<Range&>())), decltype(std::end(std::declval<Range&>()))>,
				//std::is_same<decltype(std::begin(std::declval<const Range&>())), decltype(std::end(std::declval<const Range&>()))>,
				std::is_convertible<decltype(*(++std::begin(std::declval<Range&>()))), typename std::decay_t<Range>::value_type>>,
			IfType>;

	template < class Range, class IfType = std::nullptr_t >
	using reversable_range_require
		= std::enable_if_t<conjunction_v<
			std::is_same<decltype(std::rbegin(std::declval<Range&>())), decltype(std::rend(std::declval<Range&>()))>,
			//std::is_same<decltype(std::begin(std::declval<const Range&>())), decltype(std::end(std::declval<const Range&>()))>,
			std::is_convertible<decltype(*(++std::rbegin(std::declval<Range&>()))), typename std::decay_t<Range>::value_type>>,
		IfType>;

}

#endif
