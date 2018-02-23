#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_MULTI_DIMENSIONAL_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_MULTI_DIMENSIONAL_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>

namespace cranberries {
inline namespace experimental {
namespace traversals {

	template < class Traversable1, class Traversable2 >
	class MultiDemensional : private tag::sentinel_traversal_tag
	{
		class forward_sentinel {
			Traversable1 traversal_1;
			Traversable2 traversal_2;
			typename std::decay_t<Traversable1>::iterator iter_1;
			typename std::decay_t<Traversable1>::iterator last_1;
			typename std::decay_t<Traversable2>::iterator iter_2;
			typename std::decay_t<Traversable2>::iterator last_2;
		public:
			using value_type = decltype(::cranberries::back_magic::make_tuple_cat(std::declval<typename std::decay_t<Traversable1>::value_type>(), std::declval<typename std::decay_t<Traversable2>::value_type>()));
			forward_sentinel(Traversable1 traversal1, Traversable2 traversal2)
				: traversal_1{ traversal1 }
				, traversal_2{ traversal2 }
				, iter_1{ cranberries::back_magic::begin(traversal_1) }
				, last_1{ cranberries::back_magic::end(traversal_1) }
				, iter_2{ cranberries::back_magic::begin(traversal_2) }
				, last_2{ cranberries::back_magic::end(traversal_2) }
			{}

			auto get() { return ::cranberries::back_magic::make_tuple_cat(*iter_1, *iter_2); }
			void next() {
				if (++iter_2 == last_2)
					{ iter_2 = cranberries::back_magic::begin(traversal_2); ++iter_1; }
			}
			bool is_end() { return iter_1 == last_1; }
		};

	public:
		using value_type = decltype(::cranberries::back_magic::make_tuple_cat(std::declval<typename std::decay_t<Traversable1>::value_type>(), std::declval<typename std::decay_t<Traversable2>::value_type>()));
		using forward_sentinel = forward_sentinel;
		using iterator = sentinel_iterator<forward_sentinel>;

		MultiDemensional(Traversable1 traversal1, Traversable2 traversal2)
			: traversal1{ traversal1 }
			, traversal2{ traversal2 }
		{}

		iterator begin() const { return { std::make_unique<forward_sentinel>(traversal1, traversal2) }; }
		iterator end() const { return {}; }

	private:
		Traversable1 traversal1;
		Traversable2 traversal2;
	};


namespace view {
	template < class Traversable >
	auto multi_demensional(Traversable&& trv) {
		return make_proxy<MultiDemensional>::embedded(std::forward<Traversable>(trv));
	}
}

}}}
#endif