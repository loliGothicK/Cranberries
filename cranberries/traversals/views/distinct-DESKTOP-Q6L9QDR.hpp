#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_DISTINCT_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_DISTINCT_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include <type_traits>
#include <memory>
#include <unordered_map>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range >
class Distinct
	: private tag::sentinel_traversal_tag
{
	class forward_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		forward_sentinel(Range traversal)
			: traversal_{ traversal }
			, iter{ cranberries::back_magic::begin(traversal_) }
			, last{ cranberries::back_magic::end(traversal_) }
			, hash{}
		{
			hash[*iter] = true;
		}

		auto get() { return *iter; }
		void next() {
			if (iter == last)
				return;

			while (++iter != last) {
				if (!hash[*iter]) {
					hash[*iter] = true;
					return;
				}
			}
		}
		bool is_end() { return iter == last; }
	private:
		Range traversal_;
		typename std::decay_t<Range>::iterator iter;
		typename std::decay_t<Range>::iterator last;
		std::unordered_map<value_type, bool> hash;
	};
	class reverse_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		reverse_sentinel(Range traversal)
			: traversal_{ traversal }
			, iter{ cranberries::back_magic::rbegin(traversal_) }
			, last{ cranberries::back_magic::rend(traversal_) }
			, hash{}
		{
			hash[*iter] = true;
		}

		auto get() { return *iter; }
		void next() {
			if (iter == last)
				return;

			while(++iter != last) {
				if (!hash[*iter]) {
					hash[*iter] = true;
					return;
				}
			}
		}
		bool is_end() { return iter == last; }
	private:
		Range traversal_;
		typename std::decay_t<Range>::reverse_iterator iter;
		typename std::decay_t<Range>::reverse_iterator last;
		std::unordered_map<value_type, bool> hash;
	};

public:
	using forward_sentinel = forward_sentinel;
	using reverse_sentinel = reverse_sentinel;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;
	using value_type = typename std::decay_t<Range>::value_type;

	Distinct(Range traversal)
		: traversal{ traversal } {}

	iterator begin() const { return { std::make_unique<forward_sentinel>(traversal) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(traversal) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
};


namespace view {
	inline auto distinct() noexcept { return make_proxy<Distinct>::forward(); }
}

}}}
#endif