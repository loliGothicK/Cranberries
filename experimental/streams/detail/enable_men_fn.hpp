#ifndef CRANBERRIES_STREAMS_DETAIL_ENABLE_MEN_FN
#define CRANBERRIES_STREAMS_DETAIL_ENABLE_MEN_FN
#include <type_traits>
#include <iostream>
#include <string>
#include <utility>

#include "..\forward.hpp"
#include "..\utility.hpp"
#include "..\operators.hpp"
#include "..\operatoins.hpp"
#include "..\InfiniteStream.hpp"

namespace cranberries{
namespace streams{
namespace detail{

	template <
		typename Derived
	>
	struct enable_men_fn
	{
		template < typename F >
		decltype(auto)
		adjacent_for_each(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::adjacent_for_each(std::forward<F>(f));
		}

		template < typename F >
		decltype(auto)
		all_of(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::all_of(std::forward<F>(f));
		}

		template < typename F >
		decltype(auto)
		any_of(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::any_of(std::forward<F>(f));
		}

		template < typename Stream >
		decltype(auto)
		concat(Stream&& stream_) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::concat(std::forward<Stream>(stream_));
		}

		decltype(auto)
		distinct() noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::distinctly();
		}

		template < typename F >
		decltype(auto)
		filter(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::filtered(std::forward<F>(f));
		}

		template < typename F >
		decltype(auto)
		for_each(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::for_each(std::forward<F>(f));
		}

		template < typename F >
		decltype(auto)
		invoke(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::invoked(std::forward<F>(f));
		}

		decltype(auto)
		take(size_t lim)noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::taken(lim);
		}

		template < typename F >
		decltype(auto)
		map(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::mapped(std::forward<F>(f));
		}

		template < typename To >
		decltype(auto)
		map_to() noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::map_to<To>();
		}

		decltype(auto)
		reverse() noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::reversed();
		}

		decltype(auto)
		drop(size_t lim) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::dropped(lim);
		}

		decltype(auto)
		sort() noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::sorted();
		}

		template < typename Pred >
		decltype(auto)
		sort(Pred&& pred) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::sorted(std::forward<Pred>(Pred));
		}

		template < typename Pred >
		decltype(auto)
		drop_while(Pred&& pred) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::dropped_while(std::forward<Pred>(Pred));
		}

		template < typename Pred >
		decltype(auto)
		take_while(Pred&& pred) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::taken_while(std::forward<Pred>(Pred));
		}

		decltype(auto)
		unique() noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::uniquely();
		}

		template < typename Stream >
		decltype(auto)
		merge(Stream&& stream_) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::merged(std::forward<Stream>(stream_));
		}

		template < typename F >
		decltype(auto)
		peek(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::peeked(std::forward<F>(f));
		}

		
		template < typename F >
		decltype( auto ) transform( F&& f ) {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::transformed( std::forward<F>(f) );
		}

		decltype(auto) print_to(std::ostream& os = std::cout, std::string delim = ", "){
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::print_to( os, delim );
		}



	};

	template <
		typename Derived
	>
	struct enable_men_fn_inf
	{


		template < typename Stream >
		decltype(auto)
		concat(Stream&& stream_) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::concat(std::forward<Stream>(stream_));
		}

		decltype(auto)
		distinct() noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::distinctly();
		}

		template < typename F >
		decltype(auto)
		filter(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::filtered(std::forward<F>(f));
		}


		template < typename F >
		decltype(auto)
		invoke(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::invoked(std::forward<F>(f));
		}

		decltype(auto)
		take(size_t lim) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::taken(lim);
		}

		template < typename F >
		decltype(auto)
		map(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::mapped(std::forward<F>(f));
		}

		template < typename To >
		decltype(auto)
		map_to() noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::map_to<To>();
		}


		decltype(auto)
		drop(size_t lim) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::dropped(lim);
		}


		template < typename Pred >
		decltype(auto)
		drop_while(Pred&& pred) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::dropped_while(std::forward<Pred>(Pred));
		}

		template < typename Pred >
		decltype(auto)
		take_while(Pred&& pred) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::taken_while(std::forward<Pred>(Pred));
		}

		decltype(auto)
		unique() noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::uniquely();
		}

		template < typename Stream >
		decltype(auto)
		merge(Stream&& stream_) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::merged(std::forward<Stream>(stream_));
		}

		template < typename F >
		decltype(auto)
		peek(F&& f) noexcept {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::peeked(std::forward<F>(f));
		}

		
		template < typename F >
		decltype( auto ) transform( F&& f ) {
			return std::move(*static_cast<Derived*>(this))
				>> cranberries::streams::transformed( std::forward<F>(f) );
		}



	};


}
}
}

#endif