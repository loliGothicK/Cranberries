#ifndef CRANBERRIES_RANGES_OPERATIONS_FOR_EACH_HPP
#define CRANBERRIES_RANGES_OPERATIONS_FOR_EACH_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class F >
class WriteLine
	: private tag::adaptor_tag
	, private tag::action_tag
{
	F f;
public:
	template < class _F >
	WriteLine(_F&& _f) noexcept : f{ std::forward<_F>(_f) } {}

	template < class Traversable >
	using requires
		= typename concepts::iterable::template requires<Traversable>;


	template < class Traversable,
		std::enable_if_t<!is_tuple_v<typename remove_cvr_t<Traversable>::value_type>, std::nullptr_t> = nullptr >
	decltype(auto)
		operator()(Traversable&& traversal) {
		using std::begin;
		using std::end;

		auto iter = begin(traversal);
		os << *iter; ++iter;
		for (; iter != end(traversal); ++iter)
			os << " " << *iter;
		os << std::endl;
		return std::forward<Traversable>(traversal);
	}

	template < class Traversable,
		std::enable_if_t<is_tuple_v<typename remove_cvr_t<Traversable>::value_type>, std::nullptr_t> = nullptr >
	decltype(auto)
		operator()(Traversable&& traversal) {

		auto printf_ = [](auto&& head, auto&&... tail) -> void {
			std::cout << "[" << head;
			(void)std::initializer_list<int>{
				(void(std::cout << "," << tail), 0)...
			};
			std::cout << "]";
		};

		auto iter = std::begin(traversal);
		cranberries::apply(printf_, *iter); ++iter;
		for (; iter != std::end(traversal); ++iter) {
			os << " ";
			cranberries::apply(printf_, *iter);
		}
		os << std::endl;
		return std::forward<Traversable>(traversal);
	}

};

namespace operation {
	inline WriteLine write_line(std::ostream& os = std::cout) noexcept
	{ return WriteLine{os}; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP
