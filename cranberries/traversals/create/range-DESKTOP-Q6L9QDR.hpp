#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_RANGE_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_RANGE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/default_sentinel.hpp"
#include "../../common/concepts.hpp"
#include "../../functional.hpp"


namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class ValueType, bool = concepts::required_v<ValueType, concepts::decrementable> >
class Ranged;

template < class ValueType >
class Ranged<ValueType, true>
	: private tag::sentinel_traversal_tag
{
	class forward_sentinel {
		ValueType value;
		ValueType upper_bound;
		equal_to<> eq{};
	public:
		using value_type = ValueType;
		forward_sentinel(ValueType _1, ValueType _2) : value{ _1 }, upper_bound{ _2 } {}
		forward_sentinel(const forward_sentinel&) = default;
		auto get() { return value; }
		void next() { ++value; }
		bool is_end() { return eq(upper_bound, value); }
	};
	class reverse_sentinel {
		ValueType value;
		ValueType lower_bound;
		equal_to<> eq{};
	public:
		using value_type = ValueType;
		reverse_sentinel(ValueType init, ValueType upper) : value{ --upper }, lower_bound{ --init } {}
		reverse_sentinel(const reverse_sentinel&) noexcept : value{}, lower_bound{} {}
		auto get() { return value; }
		void next() { --value; }
		bool is_end() { return eq(value, lower_bound); }
	};
public:
	using forward_sentinel = forward_sentinel;
	using reverse_sentinel = reverse_sentinel;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;
	using value_type = ValueType;

	Ranged(value_type _1, value_type _2) : init{ _1 }, bound{ _2 } {}

	iterator begin() const { return { std::make_unique<forward_sentinel>(init, bound) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(init, bound) }; }
	reverse_iterator rend() const { return {}; }
	size_t size() const noexcept { return bound - init; }
private:
	value_type init;
	value_type bound;
};

template < class ValueType >
class Ranged<ValueType, false>
	: private tag::sentinel_traversal_tag
{
	class forward_sentinel {
		ValueType value;
		ValueType upper_bound;
		equal_to<> eq{};
	public:
		using value_type = ValueType;
		forward_sentinel(ValueType _1, ValueType _2) noexcept : value{ _1 }, upper_bound{ _2 } {}
		forward_sentinel(const forward_sentinel&) noexcept : value{}, upper_bound{} {}
		auto get() const noexcept { return value; }
		void next() noexcept { ++value; }
		bool is_end() const noexcept { return eq(upper_bound, value); }
	};
public:
	using value_type = ValueType;
	using forward_sentinel = forward_sentinel;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	Ranged(value_type _1, value_type _2) : init{ _1 }, bound{ _2 } {}

	iterator begin() const { return { std::make_unique<forward_sentinel>(init, bound) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const noexcept { return magic_arts::make_default_reverse_sentinel(*this); }
	reverse_iterator rend() const noexcept { return {}; }
	size_t size() const noexcept { return bound - init; }
private:
	value_type init;
	value_type bound;
};


namespace create {

	template < typename ValueType,
		concepts::concept_required<ValueType,
			concepts::incrementable,
			concepts::regular_type
		> = nullptr
	>
	Ranged<ValueType>
	range(ValueType init, ValueType upper_bound)
		{ return {init, upper_bound}; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP
