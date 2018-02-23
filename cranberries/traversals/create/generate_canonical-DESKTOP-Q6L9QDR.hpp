#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../randomize.hpp"
#include <random>
#include <limits>

namespace cranberries {
inline namespace experimental {
namespace traversals {


template < class RealType >
class GenerateCanonical
	: private tag::sentinel_traversal_tag
{
	class forward_sentinel {
		std::mt19937_64 engine;
	public:
		using value_type = RealType;
		forward_sentinel() : engine(gen_mt_64(std::random_device{})) {}
		forward_sentinel(const forward_sentinel&) = default;
		auto get() { return std::generate_canonical<RealType, std::numeric_limits<RealType>::digits>(engine); }
		std::false_type next() { return {}; }
		std::false_type is_end() { return {}; }
	};
public:
	using sentinel = forward_sentinel;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_iterator = sentinel_iterator<sentinel>;
	using value_type = RealType;

	GenerateCanonical() {}

	iterator begin() { return { std::make_unique<sentinel>() }; }
	iterator end() { return {}; }
	reverse_iterator rbegin() { return { std::make_unique<sentinel>() }; }
	reverse_iterator rend() { return {}; }
};

template < class Distribution, class Engine = std::mt19937_64 >
class GenerateDistribution
	: private tag::sentinel_traversal_tag
{
	class forward_sentinel {
		Engine engine;
		Distribution dist;
	public:
		using value_type = decltype(std::declval<Distribution>()( std::declval<Engine>() ));
		template < class Init, class... Args >
		forward_sentinel(Init&& init, Args&&... args)
			: engine{ std::forward<Init>(init)() }
			, dist{ std::forward<Args>(args)... }
		{}
		forward_sentinel(const forward_sentinel&) = default;
		auto get() { return dist(engine); }
		void next() { return {}; }
		std::false_type is_end() { return {}; }
	};
public:
	using sentinel = forward_sentinel;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_iterator = sentinel_iterator<sentinel>;
	using value_type = decltype(std::declval<Distribution>()(std::declval<Engine>()));

	GenerateDistribution() {}

	iterator begin() { return { std::make_unique<sentinel>() }; }
	iterator end() { return {}; }
	reverse_iterator rbegin() { return { std::make_unique<sentinel>() }; }
	reverse_iterator rend() { return {}; }
};


namespace create {
	template < typename RealType = double,
		std::enable_if_t<std::is_floating_point<RealType>::value, std::nullptr_t> = nullptr >
	GenerateCanonical<RealType> generate_canonical() { return {}; }

	template < class Dist, class Engine, class... Args >
	GenerateDistribution<Dist, Engine> generate_distribution(Args&&... args)
		{ return { std::forward<Args>(args)... }; }

}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP
