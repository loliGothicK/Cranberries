#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../randomize.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {


template < class Generator >
class Generate
	: private tag::sentinel_traversal_tag
{
	class forward_sentinel {
		Generator generator_;
	public:
		using value_type = decltype(std::declval<Generator>()());
		forward_sentinel(Generator gen) noexcept : generator_{ std::forward<Generator>(gen) } {}
		forward_sentinel(const forward_sentinel&) noexcept {}
		auto get() { return generator_(); }
		void next() noexcept {}
		std::false_type is_end() noexcept { return {}; }
	};
public:
	using sentinel = forward_sentinel;
	using iterator = sentinel_iterator<sentinel>;
	using value_type = decltype(std::declval<Generator>()());

	Generate(Generator gen) noexcept : generator{ std::forward<Generator>(gen) } {}

	iterator begin() noexcept { return { std::make_unique<sentinel>(generator) }; }
	iterator end() noexcept { return {}; }
private:
	Generator generator;
};

namespace create {
	template < class Generator >
	Generate<Generator>
		generate(Generator&& generator) noexcept { return {std::forward<Generator>(generator)}; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP
