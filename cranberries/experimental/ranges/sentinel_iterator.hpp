#ifndef CRANBERRIES_RANGES_SENTINEL_ITERATOR_HPP
#define CRANBERRIES_RANGES_SENTINEL_ITERATOR_HPP
#include "ranges_tag.hpp"
#include <memory>
#include <utility>
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../../type_traits.hpp"

namespace cranberries {
namespace experimental {
namespace ranges {

	template < template <class...> class SeltinelRange, class... Args >
	class AdaptorProxy : private tag::adaptor_proxy_tag
	{
		std::tuple<Args...> bind;

		template < class Range, size_t... I >
		SeltinelRange<Range, Args...> make_sentinel_range(Range&& range, std::index_sequence<I...>) const {
			return { std::forward<Range>(range), std::get<I>(bind)... };
		}
	public:
		template < class... Args_ >
		AdaptorProxy(Args_&&... args) : bind{ std::forward<Args_...>(args)... } {}

		template < class Range >
		decltype(auto) operator()(Range&& range) const {
			return make_sentinel_range(std::forward<Range>(range), std::index_sequence_for<Args...>{});
		}
	};


enum class sentinel_flag {
	on, off
};

template < class Sentinel >
class sentinel_iterator
	: private tag::sentinel_iterator_tag
{
public:
	// For std::iterator_traits<sentinel_iterator>
	using value_type = typename Sentinel::value_type;
	using difference_type = int;
	using pointer = std::nullptr_t; // disable to pointer
	using reference = value_type&;
	using iterator_category = std::output_iterator_tag;

	// Constructor for Sentinel Range
	sentinel_iterator(std::unique_ptr<Sentinel>&& ptr)
		: sentinel{ std::move(ptr) }, is_sentinel{ sentinel_flag::off } {}


	// Disable to default construct
	sentinel_iterator()
		: sentinel{ nullptr }, is_sentinel{ sentinel_flag::on } {};

	// Copy constructor/Copy assignment operator
	/*[Note: Copy constructor makes deep-copied sentinel_iterator.
		It meens copying current state of sentinel.
		- end note ]*/
	sentinel_iterator(const sentinel_iterator& iter)
		: sentinel{ std::unique_ptr<Sentinel>(new Sentinel(*(iter.sentinel))) } {}
	
	sentinel_iterator& operator=(const sentinel_iterator& rhs) { return deep_copy(rhs); }


	// Move constructor/Move assignment operator
	sentinel_iterator& operator=(sentinel_iterator&&) = default;
	sentinel_iterator(sentinel_iterator&&) = default;
	
	// Destructor
	~sentinel_iterator() = default;

	// For iterator requirements
	void swap(sentinel_iterator& iter) & { std::swap(sentinel, iter.sentinel); }
	void swap(sentinel_iterator&& iter) & { std::swap(sentinel, iter.sentinel); }

	// copy construct/copy asignment implementation
	sentinel_iterator deep_copy(const sentinel_iterator& iter) & {
		this->swap(sentinel_iterator{ iter });
		return *this;
	}

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