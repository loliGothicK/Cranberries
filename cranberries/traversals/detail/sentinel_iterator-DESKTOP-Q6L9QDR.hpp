#ifndef CRANBERRIES_RANGES_SENTINEL_ITERATOR_HPP
#define CRANBERRIES_RANGES_SENTINEL_ITERATOR_HPP
#include "tags.hpp"
#include <memory>
#include <utility>
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../../common/concepts.hpp"
#include "../../meta_bind.hpp"


namespace cranberries {
inline namespace experimental {
namespace traversals {


	template < template < class... > class, class, class, class >
	class adaptor_proxy; // forward decralation

	// Proxy class
	template < template < class... > class SentinelRange, class... Embedded, class... Forward >
	class adaptor_proxy<SentinelRange, void, std::tuple<Embedded...>, std::tuple<Forward...>>
		: private tag::adaptor_tag
	{
		std::tuple<Embedded..., Forward...> bind;

	public:
		// constraints
		template < class Range >
		using requires
			= typename constraints_detection::has_requires_or<
				SentinelRange<Range, Embedded...>, // If has requires use its definition
				concepts::iterable								 // Else default definition is iterable
			>::template requires<Range>;

		adaptor_proxy(std::tuple<Embedded..., Forward...> t)
			: bind(t) {}

		// view call invoker
		template < class Range, size_t... I >
		SentinelRange<Range, Embedded...> make_sentinel_traversal(Range&& traversal, std::index_sequence<I...>)
		{
			return { std::forward<Range>(traversal), std::get<I>(bind)... };
		}

		// view call
		template < class Range >
		decltype(auto) operator()(Range&& traversal) {
			return make_sentinel_traversal(std::forward<Range>(traversal), std::index_sequence_for<Embedded..., Forward...>{});
		}
	};

	template < template < class... > class SentinelRange, class pPack, class... Embedded, class... Forward >
	class adaptor_proxy<SentinelRange, pPack, std::tuple<Embedded...>, std::tuple<Forward...>>
		: private tag::adaptor_tag
	{
		std::tuple<Embedded..., Forward...> bind;

	public:
		// constraints
		template < class Range >
		using requires
			= typename constraints_detection::has_requires_or<
			SentinelRange<Range, typename pPack::template result_pack<Range>, Embedded...>, // If has requires use its definition
			concepts::iterable								 // Else default definition is iterable
			>::template requires<Range>;

		adaptor_proxy(std::tuple<Embedded..., Forward...> t)
			: bind(t) {}

		// view call invoker
		template < class Range, size_t... I >
		SentinelRange<Range, typename pPack::template result_pack<Range>, Embedded...>
			make_sentinel_traversal(Range&& traversal, std::index_sequence<I...>)
		{
			return { std::forward<Range>(traversal), std::get<I>(bind)... };
		}

		// view call
		template < class Range >
		decltype(auto) operator()(Range&& traversal) {
			return make_sentinel_traversal(std::forward<Range>(traversal), std::index_sequence_for<Embedded..., Forward...>{});
		}
	};


	template < template < class... > class, class = void >
	struct make_proxy;

	// make proxy helper class
	template < template < class... > class SentinelRange >
	struct make_proxy<SentinelRange, void> {

		// invoker for piecewise
		template < class... Args >
		struct make_proxy_invoker {
			std::tuple<Args...> t;

			template < class... Args_, size_t... I >
			auto invoke(std::index_sequence<I...>, Args_&&... args) {
				return adaptor_proxy<SentinelRange, void, std::tuple<Args...>, std::tuple<Args_...>>{
					{ std::get<I>(t)..., std::forward<Args_>(args)... }
				};
			}
			
			template < class... Args_ >
			auto operator()(Args_&&... args) {
				return invoke(std::index_sequence_for<Args...>{}, std::forward<Args_>(args)...);
			}
		};

		// piecewise(...embedded)(...forward)
		template < class... Args >
		static auto piecewise(Args&&... args) {
			return make_proxy_invoker<Args...>{ std::forward<Args>(args)... };
		}
		// for args to embedded 
		template < class... Args >
		static auto embedded(Args&&... args) {
			return adaptor_proxy<SentinelRange, void, std::tuple<Args...>, std::tuple<>>{ {std::forward<Args>(args)...} };
		}
		// for args to simple forward
		template < class... Args >
		static auto forward(Args&&... args) {
			return adaptor_proxy<SentinelRange, void, std::tuple<>, std::tuple<Args...>>{ {std::forward<Args>(args)...}};
		}
	};


	// make proxy helper class
	template < template < class... > class SentinelRange, template < class > class... Preds >
	struct make_proxy<SentinelRange, pred_sequence<Preds...>> {

		// invoker for piecewise
		template < class... Args >
		struct make_proxy_invoker {
			std::tuple<Args...> t;

			template < class... Args_, size_t... I >
			auto invoke(std::index_sequence<I...>, Args_&&... args) {
				return adaptor_proxy<SentinelRange, pred_sequence<Preds...>, std::tuple<Args...>, std::tuple<Args_...>>{
					{ std::get<I>(t)..., std::forward<Args_>(args)... }
				};
			}

			template < class... Args_ >
			auto operator()(Args_&&... args) {
				return invoke(std::index_sequence_for<Args...>{}, std::forward<Args_>(args)...);
			}
		};

		// piecewise(...embedded)(...forward)
		template < class... Args >
		static auto piecewise(Args&&... args) {
			return make_proxy_invoker<Args...>{ std::forward<Args>(args)... };
		}
		// for args to embedded 
		template < class... Args >
		static auto embedded(Args&&... args) {
			return adaptor_proxy<SentinelRange, pred_sequence<Preds...>, std::tuple<Args...>, std::tuple<>>{ {std::forward<Args>(args)...} };
		}
		// for args to simple forward
		template < class... Args >
		static auto forward(Args&&... args) {
			return adaptor_proxy<SentinelRange, pred_sequence<Preds...>, std::tuple<>, std::tuple<Args...>>{ {std::forward<Args>(args)...}};
		}
	};

enum class sentinel_flag {
	on, off
};


template < class Sentinel >
class sentinel_iterator
	: private tag::sentinel_iterator_tag
{
protected:
	// copy construct/copy asignment implementation
	sentinel_iterator deep_copy(const sentinel_iterator& iter) & {
		this->swap(sentinel_iterator{ iter });
		return *this;
	}
public:
	// For std::iterator_traits<sentinel_iterator>
	using value_type = typename Sentinel::value_type;
	using difference_type = int;
	using pointer = std::nullptr_t; // disable
	using reference = value_type&;
	using iterator_category = std::output_iterator_tag;

	// Constructor for Sentinel Range
	sentinel_iterator(std::unique_ptr<Sentinel>&& ptr)
		: sentinel{ std::move(ptr) }, is_sentinel{ sentinel_flag::off } {}


	// Default Constructor for End Sentinel
	sentinel_iterator()
		: sentinel{ nullptr }, is_sentinel{ sentinel_flag::on } {};

	// Copy constructor/Copy assignment operator
	/*[Note: Copy constructor makes deep-copied sentinel_iterator.
		It meens copying current state of sentinel.
		- end note ]*/
	sentinel_iterator(const sentinel_iterator& iter)
		: sentinel{ std::make_unique<Sentinel>(iter.sentinel) } {}
	
	sentinel_iterator& operator=(const sentinel_iterator& rhs) { return deep_copy(rhs); }


	// Move constructor/Move assignment operator
	sentinel_iterator& operator=(sentinel_iterator&&) = default;
	sentinel_iterator(sentinel_iterator&&) = default;
	
	// Destructor
	~sentinel_iterator() = default;

	// For iterator requirements
	void swap(sentinel_iterator& iter) & { std::swap(sentinel, iter.sentinel); }
	void swap(sentinel_iterator&& iter) & { std::swap(sentinel, iter.sentinel); }

	// Dereferene 
	decltype(auto) get() const { return sentinel->get(); }
	decltype(auto) operator*() const { return sentinel->get(); }

	// Increment
	bool next() { return sentinel->next(); }
	decltype(auto) operator++() { return sentinel->next(), *this; }
	auto operator++(int) { return make_finally([&]() { sentinel->next(); }), deep_copy(*this); }

	// Sentinel invoke
	bool is_end() const { return sentinel->is_end(); }

	bool operator==(const sentinel_iterator& iter) const {
		return is_sentinel == sentinel_flag::off ?
			iter.is_sentinel == sentinel_flag::off ? false : this->is_end()
			: iter.is_end();
	}
	bool operator!=(const sentinel_iterator& iter) const {
		return is_sentinel == sentinel_flag::off ?
			iter.is_sentinel == sentinel_flag::off ? true : !this->is_end()
			: !iter.is_end();
	}

private:
	// Iteration state
	/*[Note: This is implementation of Sentinel Iterator.
		It manages iteration, access and end-point checking.
	-end note]*/
	std::unique_ptr<Sentinel> sentinel;

	sentinel_flag is_sentinel;
};


}}}
#endif