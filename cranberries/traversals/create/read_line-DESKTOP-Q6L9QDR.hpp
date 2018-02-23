#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_FOPEN_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_FOPEN_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/default_sentinel.hpp"
#include <fstream>
#include <iostream>

namespace cranberries {
inline namespace experimental {
namespace traversals {

namespace detail_ { enum class delim { defaulted, specified }; }

template < detail_::delim >
class ReadLine;

template < >
class ReadLine<detail_::delim::specified>
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
		std::istream& ifs;
		std::string line;
		char delim;
	public:
		using value_type = std::string;
		sentinel_impl(std::istream& ifs, char delim) : ifs(ifs), delim{delim} {}
		sentinel_impl(const sentinel_impl&) = default;
		auto get() { return line; }
		void next() { std::getline(ifs, line, delim); }
		bool is_end() { return ifs.eof(); }
	};
public:
	using value_type = std::string;
	using sentinel = sentinel_impl;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;


	ReadLine(std::istream& ifs, char delim) : ifs{ ifs }, delim{ delim } {}

	iterator begin() { return { std::make_unique<sentinel>(ifs, delim) }; }
	iterator end() { return {}; }
	reverse_iterator rbegin() { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() { return {}; }

private:
	std::istream& ifs;
	char delim;
};

template < >
class ReadLine<detail_::delim::defaulted>
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
		std::istream& ifs;
		std::string line;
	public:
		using value_type = std::string;
		sentinel_impl(std::istream& ifs) : ifs(ifs) {}
		sentinel_impl(const sentinel_impl&) = default;
		auto get() { return line; }
		void next() { std::getline(ifs, line); }
		bool is_end() { return ifs.eof(); }
	};
public:
	using value_type = std::string;
	using sentinel = sentinel_impl;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;


	ReadLine(std::istream& ifs) : ifs{ ifs }  {}

	iterator begin() { return { std::make_unique<sentinel>(ifs) }; }
	iterator end() { return {}; }
	reverse_iterator rbegin() { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() { return {}; }

private:
	std::istream& ifs;
};


namespace create {
	ReadLine<detail_::delim::specified> read_line(std::istream& ifs, char delim) { return {ifs, delim}; }
	ReadLine<detail_::delim::defaulted> read_line(std::istream& ifs) { return { ifs }; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP
