#ifndef CRANBERRIES_STREAMS_OPERATORS_MAP_HPP
#define CRANBERRIES_STREAMS_OPERATORS_MAP_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {


	template < typename F >
	struct MapProxy {
		F f;
	};

	template <
		typename UnaryFunc
	>
	struct Transform
	{
		using tree_tag = detail::not_tree;

    template < typename F, typename T >
    void apply( F&& f, T&& a ) {
      a =  f(std::forward<T>(a));
    }

		template <
			typename STREAM,
      std::enable_if_t<detail::is_range_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream_
		) {
      auto&& v = stream_.get();
      size_t i{};
      for(;i<v.size() % 8;++i)
          apply(f,v[i]);
      for(; i<v.size();i+=8){
          apply(f,v[i]);
          apply(f,v[i+1]);
          apply(f,v[i+2]);
          apply(f,v[i+3]);
          apply(f,v[i+4]);
          apply(f,v[i+5]);
          apply(f,v[i+6]);
          apply(f,v[i+7]);
      }
      return std::forward<STREAM>(stream_);
		}

    template <
      typename T,
      std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
    >
		auto operator()(T&& a) const noexcept {
			return f(a);
		}



		// mapper

		UnaryFunc f;
	};

	template <
		typename OldStream,
		typename UnaryFunc
	>
	struct Map
	{
		using tree_tag = detail::not_tree;

    template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream_
		) {
			stream_.reserve(old.size());
      auto&& v = old.get();
      size_t i{};
      for(;i<v.size() % 8;++i)
          stream_.emplace_back(v[i]);
      for(; i<v.size();i+=8){
          stream_.emplace_back(v[i]);
          stream_.emplace_back(v[i+1]);
          stream_.emplace_back(v[i+2]);
          stream_.emplace_back(v[i+3]);
          stream_.emplace_back(v[i+4]);
          stream_.emplace_back(v[i+5]);
          stream_.emplace_back(v[i+6]);
          stream_.emplace_back(v[i+7]);
      }
			return std::forward<STREAM>(stream_);
		}

		// member
		OldStream old;
		UnaryFunc f;
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif