#ifndef CRANBERRIES_RANGES_DETAIL_DEFAULT_SENTINEL_HPP
#define CRANBERRIES_RANGES_DETAIL_DEFAULT_SENTINEL_HPP
#include "./tags.hpp"
#include "./sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../../common/concepts.hpp"
#include <type_traits>
#include <memory>
#include <vector>

namespace cranberries {
inline namespace experimental {
namespace traversals {


namespace magic_arts {



	template < class ValueType >
	class default_forward_sentinel : public polymorphic_view<ValueType> {
	public:
		using value_type = ValueType;
		template < class Range >
		default_forward_sentinel(Range&& traversal_)
			: traversal{}
			, iter{}
			, last{}
		{
			for (const auto& e : traversal_) traversal.emplace_back(e);
			iter = traversal.begin();
			last = traversal.end();
		}
	protected:
		value_type get() const override final { return *iter; }
		void next() override final { ++iter; }
		bool is_end() override final { return iter == last; }
	private:
		std::vector<value_type> traversal;
		typename std::vector<value_type>::reverse_iterator iter;
		typename std::vector<value_type>::reverse_iterator last;
	};


	template < class ValueType >
	class default_reverse_sentinel: public polymorphic_view<ValueType> {
	public:
		using value_type = ValueType;
		template < class Range >
		default_reverse_sentinel(Range&& traversal_)
			: traversal{}
			, iter{}
			, last{}
		{
			for (const auto& e : traversal_) traversal.emplace_back(e);
			iter = traversal.rbegin();
			last = traversal.rend();
		}
	protected:
		value_type get() const override final { return *iter; }
		void next() override final { ++iter; }
		bool is_end() override final { return iter == last; }
	private:
		std::vector<value_type> traversal;
		typename std::vector<value_type>::reverse_iterator iter;
		typename std::vector<value_type>::reverse_iterator last;
	};

	template < class Range >
	decltype(auto) make_default_reverse_sentinel(Range&& traversal) {
		return std::make_unique<default_reverse_sentinel<typename std::decay_t<Range>::value_type>>( std::forward<Range>(traversal) );
	}

}}}}
#endif