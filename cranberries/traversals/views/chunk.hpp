#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_CHUNK_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_CHUNK_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <array>
#include "../../common/concepts.hpp"
#include "../../span.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < size_t, class >
class Chunk;


namespace _detail {
	template < class, class = void >
	struct size_impl_injection_for_chunk {};

	template < class Trav, size_t N >
	struct size_impl_injection_for_chunk<Chunk<N, Trav>,
		::cranberries::void_t<decltype(std::declval<const Trav&>().size())>
	> {
		size_t size() const { return (static_cast<const Chunk<N, Trav>*>(this)->trav).size() / N; }
	};

	template < class Trav >
	struct size_impl_injection_for_chunk<Chunk<static_cast<size_t>(-1), Trav>,
		::cranberries::void_t<decltype(std::declval<const Trav&>().size())>
	> {
		size_t size() const {
			return (static_cast<int>((static_cast<const Chunk<static_cast<size_t>(-1), Trav>*>(this)->trav).size())-1)
				/ (static_cast<const Chunk<static_cast<size_t>(-1), Trav>*>(this)->n)
				+ 1; 
		}
	};

}

template < size_t N, class Traversal >
class Chunk
	: private tag::sentinel_traversal_tag
	, public reversable_traversal
	, public _detail::conjunction_bound<Traversal>
	, public _detail::size_impl_injection_for_chunk<Chunk<N, Traversal>>
{
	friend struct view_get;
	template <class, class> friend struct _detail::size_impl_injection_for_chunk;
public:
	using value_type = std::array<typename ::cranberries::remove_cvr_t<Traversal>::value_type, N>;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;
	using immutable = std::nullptr_t;

	Chunk(Traversal trav): trav{ std::forward<Traversal>(trav) } {}
	Chunk(Chunk const&) = delete;
	Chunk& operator=(Chunk const&) = delete;
	Chunk(Chunk&&) = default;
	Chunk& operator=(Chunk&&) = default;
private:
	class fwd_view final : public polymorphic_view<value_type>
	{
		std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> view;
		value_type chunk;
		mutable value_type next_;
		bool eov{ false };
		bool eov_next{ false };
	public:
		fwd_view(std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> p)
			: view{ std::move(p) }
			, chunk{ {} }
			, eov{ view->is_end() }
		{
			for (auto& e: chunk) {
				if (view->is_end()) { eov = true; break; }
				else { e = view->get(); }
				view->next();
			}
			for (auto& e : next_) {
				if (view->is_end()) { eov_next = true; break; }
				else { e = view->get(); }
				view->next();
			}
		}
		fwd_view(const fwd_view&) = default;
	protected:
		value_type get() const override final { return chunk; }
		void next() override final {
			eov = eov_next;
			chunk = std::move(next_);
			for (auto& e : next_) {
				if (view->is_end()) { eov_next = true; break; }
				else { e = view->get(); }
				view->next();
			}
		}
		bool is_end() const override final { return eov; }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<fwd_view>(view_get::fwd(trav));
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<default_rev_view<value_type>>(this->fwd());
	}
public:
	iterator begin() const { return { this->fwd() }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { this->rev() }; }
	reverse_iterator rend() const { return {}; }
private:
	Traversal trav;
};

template < class Traversal >
class Chunk<static_cast<size_t>(-1), Traversal>
	: private tag::sentinel_traversal_tag
	, public reversable_traversal
	, public _detail::conjunction_bound<Traversal>
	, public _detail::size_impl_injection_for_chunk<Chunk<static_cast<size_t>(-1), Traversal>>
{
	friend struct view_get;
	template < class, class > friend struct _detail::size_impl_injection_for_chunk;
public:
	using value_type = ::cranberries::readonly_span_t<const std::vector<typename remove_cvr_t<Traversal>::value_type>>;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;
	using immutable_ = std::nullptr_t;

	Chunk(Traversal trav, size_t n): trav{ std::forward<Traversal>(trav) }, n{ n } {}
	Chunk(Chunk const&) = delete;
	Chunk& operator=(Chunk const&) = delete;
	Chunk(Chunk&&) = default;
	Chunk& operator=(Chunk&&) = default;
private:
	class fwd_view final : public polymorphic_view <value_type>
	{
		std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> view;
		size_t n;
		std::vector<typename remove_cvr_t<Traversal>::value_type> chunk{};
		bool eov{ false };
		bool eov_next{ false };
	public:
		fwd_view(std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> p, size_t n)
			: view{ std::move(p) }
			, n{n}
			, chunk(n)
			, eov{ view->is_end() }
		{
			for (size_t i{}; i < n; ++i, view->next()) {
				if (view->is_end()) { eov_next = true; break; }
				else { chunk[n] = view->get(); }
			}
		}
		fwd_view(const fwd_view&) = default;
	protected:
		value_type get() const override final { return ::cranberries::readonly_span(chunk, 0, n); }
		void next() override final {
			eov = eov_next;
			for (size_t i{}; i < n; ++i, view->next()) {
				if (view->is_end()) { eov_next = true; n = i; break; }
				else { chunk[i] = view->get(); }
			}
		}
		bool is_end() const override final { return eov; }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<fwd_view>(view_get::fwd(trav), n);
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<default_rev_view<value_type>>(this->fwd());
	}
public:
	iterator begin() const { return { this->fwd() }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { this->rev() }; }
	reverse_iterator rend() const { return {}; }
private:
	Traversal trav;
	size_t n;
};

namespace _proxy {
	template < size_t N >
	class ChunkX
		: private tag::adaptor_tag
	{
	public:
		ChunkX() = default;

		template < class Traversable >
		using requires
			= typename concepts::iterable::template requires<Traversable>;

		template < class Traversal >
		auto operator()(Traversal&& traversal) {
			return Chunk<N, Traversal>{ std::forward<Traversal>(traversal) };
		}
	};

	template < >
	class ChunkX<static_cast<size_t>(-1)>
		: private tag::adaptor_tag
	{
		size_t n;
	public:
		ChunkX(size_t n) : n{ n } {}

		template < class Traversable >
		using requires
			= typename concepts::iterable::template requires<Traversable>;

		template < class Traversal >
		auto operator()(Traversal&& traversal) {
			return Chunk<static_cast<size_t>(-1), Traversal>{ std::forward<Traversal>(traversal), n };
		}
	};
}
namespace view {
	template < size_t N, enabler_t<bool(N)> = nullptr >
	_proxy::ChunkX<N> chunk() { return {}; }

	inline _proxy::ChunkX<static_cast<size_t>(-1)> chunk(size_t n) { return {n}; }
}

}}}
#endif