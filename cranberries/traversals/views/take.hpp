#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_TAKE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_TAKE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Traversal >
class Take
	: private tag::sentinel_traversal_tag
	, public reversable_traversal
{
	friend struct view_get;
public:
	using value_type = typename remove_cvr_t<Traversal>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;
	using finite_ = std::nullptr_t;
	using immutable_ = std::nullptr_t;

	Take(Traversal traversal, long long n)
		: traversal{ std::forward<Traversal>(traversal) }
		, n(n) {}
	Take(Take const&) = delete;
	Take& operator=(Take const&) = delete;
	Take(Take&&) = default;
	Take& operator=(Take&&) = default;
private:
	class fwd_view final : public polymorphic_view<value_type>
	{
		std::unique_ptr<polymorphic_view<value_type>> view;
		long long n;
	public:
		fwd_view(std::unique_ptr<polymorphic_view<value_type>> p, long long n)
			: view{ std::move(p) }
			, n{n}
		{}
	protected:
		value_type get() const override final { return view->get(); }
		void next() override final { --n, view->next(); }
		bool is_end() const override final { return n <= 0 || view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<fwd_view>(view_get::fwd(traversal), n);
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<default_rev_view<value_type>>(this->fwd());
	}
public:
	auto begin() const { return iterator{ this->fwd() }; }
	auto end() const { return iterator{}; }
	auto rbegin() const { return reverse_iterator{ this->rev() }; }
	auto rend() const { return reverse_iterator{}; }
	size_t size() const { return n; }
private:
	Traversal traversal;
	long long n;
};

namespace _proxy {
	class TakeX: tag::adaptor_tag {
		long long n;
	public:
		explicit TakeX(size_t n) noexcept: n{static_cast<long long>(n)} {}

		template < class Traversal >
		auto operator()(Traversal&& traversal) {
			return Take<Traversal>{ std::forward<Traversal>(traversal), n };
		}
	};
}

namespace view {
	inline auto take(size_t n) { return _proxy::TakeX{ n }; }
}

}}}
#endif