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
    transform(F&& f) noexcept {
      return std::move(*static_cast<Derived*>(this))
        >> cranberries::streams::transformed(std::forward<F>(f));
    }

    template < typename To >
    decltype(auto)
    transform_to() noexcept {
      return std::move(*static_cast<Derived*>(this))
        >> cranberries::streams::transformed_to<To>();
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
        >> cranberries::streams::sorted(std::forward<Pred>(pred));
    }

    decltype(auto)
    stable_sort() noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::stable_sorted();
    }

    template < typename Pred >
    decltype(auto)
    stable_sort( Pred&& pred ) noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::stable_sorted( std::forward<Pred>( pred ) );
    }

    decltype(auto)
    partial_sort(size_t n) noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::partial_sorted(n);
    }

    template < typename Pred >
    decltype(auto)
    partial_sort( size_t n, Pred&& pred ) noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::partial_sorted( n, std::forward<Pred>( pred ) );
    }

    decltype(auto)
    nth_element(size_t n) noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::nth_element(n);
    }

    template < typename Pred >
    decltype(auto)
    nth_element( size_t n, Pred&& pred ) noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::nth_element( n, std::forward<Pred>( pred ) );
    }

    template < bool B = true >
    decltype(auto)
    radix_sort() noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::radix_sorted<B>();
    }


    template < typename Pred >
    decltype(auto)
    drop_while(Pred&& pred) noexcept {
      return std::move(*static_cast<Derived*>(this))
        >> cranberries::streams::dropped_while(std::forward<Pred>(pred));
    }

    template < typename Pred >
    decltype(auto)
    take_while(Pred&& pred) noexcept {
      return std::move(*static_cast<Derived*>(this))
        >> cranberries::streams::taken_while(std::forward<Pred>(pred));
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

    
    decltype(auto)
    print_to(std::ostream& os = std::cout, std::string delim = ", "){
      return std::move(*static_cast<Derived*>(this))
        >> cranberries::streams::print_to( os, delim );
    }

    decltype(auto)
    println_to( std::ostream& os = std::cout, std::string delim = ", " ) {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::println_to( os, delim );
    }


    decltype(auto)
    shuffle() noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::shuffled();
    }

    template < typename T >
    decltype(auto)
    replace(T&& v1, T&& v2) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::replaced( std::forward<T>( v1 ), std::forward<T>( v2 ) );
    }

    template < typename T1, typename T2>
    decltype(auto)
    replace_if(T1&& v1, T2&& v2) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::replaced_if( std::forward<T1>( v1 ), std::forward<T2>( v2 ) );
    }

    template < typename T >
    decltype(auto)
    count(T&& v1) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::count( std::forward<T>( v1 ) );
    }

    template < typename T >
    decltype(auto)
    count_if(T&& v1) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::count_if( std::forward<T>( v1 ) );
    }

    decltype(auto)
    accumulate() noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::accumulate();
    }

    template < typename T >
    decltype(auto)
    accumulate(T&& v1) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::accumulate( std::forward<T>( v1 ) );
    }
 
    template < typename T1, typename T2>
    decltype(auto)
    accumulate(T1&& v1, T2&& v2) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::accumulate( std::forward<T1>( v1 ), std::forward<T2>( v2 ) );
    }

    decltype(auto)
    product() noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::product();
    }

    decltype(auto)
    average() noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::average();
    }

    decltype(auto)
    sum() noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::sum();
    }

    template < typename T >
    decltype(auto)
    sum_if(T&& v) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::sum_if(std::forward<T>(v));
    }

    decltype(auto)
    median() noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::median();
    }

    decltype(auto)
    mode() noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::mode();
    }

    decltype(auto)
    stride(size_t n) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::strided(n);
    }

    decltype(auto)
    slice(size_t l, size_t u) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::sliced(l,u);
    }

    template < typename F >
    decltype(auto)
    flat_map(F&& f) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::flat_transformed( std::forward<F>( f ) );
    }
    
    decltype(auto)
    flat() noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::flatten();
    }

    decltype(auto)
    all_flat() noexcept {
      return std::move( *static_cast<Derived*>(this) )
        >> cranberries::streams::all_flatten();
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

    template < typename To >
    decltype(auto)
    transform_to() noexcept {
      return std::move(*static_cast<Derived*>(this))
        >> cranberries::streams::transformed_to<To>();
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
        >> cranberries::streams::dropped_while(std::forward<Pred>(pred));
    }

    template < typename Pred >
    decltype(auto)
    take_while(Pred&& pred) noexcept {
      return std::move(*static_cast<Derived*>(this))
        >> cranberries::streams::taken_while(std::forward<Pred>(pred));
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
    decltype( auto )
    transform( F&& f ) {
      return std::move(*static_cast<Derived*>(this))
        >> cranberries::streams::transformed( std::forward<F>(f) );
    }

    template < typename T >
    decltype(auto)
    replace(T&& v1, T&& v2) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::replaced( std::forward<T>( v1 ), std::forward<T>( v2 ) );
    }

    template < typename T1, typename T2>
    decltype(auto)
    replace_if(T1&& v1, T2&& v2) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::replaced_if( std::forward<T1>( v1 ), std::forward<T2>( v2 ) );
    }

    decltype(auto)
    stride(size_t n) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::strided(n);
    }

    decltype(auto)
    slice(size_t l, size_t u) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::sliced(l,u);
    }

    template < typename F >
    decltype(auto)
    flat_map(F&& f) noexcept {
      return std::move( *static_cast<Derived*>( this ) )
        >> cranberries::streams::flat_transformed( std::forward<F>( f ) );
    }
};


} // ! namespace detail
} // ! namespace streams
} // ! namespace cranberries

#endif