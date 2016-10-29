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

#define men_fn_def(men_fn, adaptor_fn)\
template < typename ...Args >\
decltype(auto) men_fn(Args&& ...args)\
noexcept( noexcept( adaptor_fn(std::forward<Args>(args)...) ) )\
{\
  return std::move(*static_cast<Derived*>(this))\
    >> cranberries::streams::adaptor_fn( std::forward<Args>(args)... );\
}

namespace cranberries{
namespace streams{
namespace detail{

  template <
    typename Derived
  >
  struct enable_men_fn
  {
    men_fn_def( adjacent_for_each, adjacent_for_each );

    men_fn_def( all_of, all_of );

    men_fn_def( any_of, any_of );

    men_fn_def( none_of, none_of );

    men_fn_def( join, joined );

    men_fn_def( distinct, distinctly );

    men_fn_def( filter, filtered );

    men_fn_def( for_each, for_each );

    men_fn_def( invoke, invoked );

    men_fn_def( take, taken );

    men_fn_def( transform, transformed );

    men_fn_def( transform_to, transformed_to );

    men_fn_def( reverse, reversed );

    men_fn_def( drop, dropped );

    men_fn_def( sort, sorted );

    men_fn_def( stable_sort, stable_sorted );

    men_fn_def( partial_sort, partial_sorted );

    men_fn_def( nth_element, nth_elemented );
    
    men_fn_def( ascending_radix_sort, ascending_radix_sorted );

    men_fn_def( descending_radix_sort, descending_radix_sorted );

    men_fn_def( drop_while, dropped_while );

    men_fn_def( take_while, taken_while );

    men_fn_def( unique, uniquely );

    men_fn_def( merge, merged );

    men_fn_def( peek, peeked );
    
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

    men_fn_def( shuffle, shuffled );

    men_fn_def( replace, replaced );

    men_fn_def( replace_if, replaced_if );

    men_fn_def( count, count );

    men_fn_def( count_if, count_if );

    men_fn_def( accumulate, accumulate );

    men_fn_def( product, product );

    men_fn_def( average, average );

    men_fn_def( sum, sum );

    men_fn_def( sum_if, sum_if );

    men_fn_def( median, median );

    men_fn_def( mode, mode );

    men_fn_def( stride, strided );

    men_fn_def( slice, sliced );
    
    men_fn_def( flat_transform, flat_transformed );

    men_fn_def( flat, flatten );

    men_fn_def( all_flat, all_flatten );

    men_fn_def( summary_stat, summary_stat );
  };

  template <
    typename Derived
  >
  struct enable_men_fn_inf
  {
    men_fn_def( join, joined );

    men_fn_def( distinct, distinctly );

    men_fn_def( filter, filtered );

    men_fn_def( invoke, invoked );

    men_fn_def( take, taken );

    men_fn_def( transform_to, transformed_to );

    men_fn_def( drop, dropped );

    men_fn_def( drop_while, dropped_while );

    men_fn_def( take_while, taken_while );

    men_fn_def( unique, uniquely );

    men_fn_def( merge, merged );

    men_fn_def( peek, peeked );
    
    men_fn_def( transform, transformed );

    men_fn_def( replace, replaced );

    men_fn_def( replace_if, replaced_if );

    men_fn_def( stride, strided );

    men_fn_def( slice, sliced );

    men_fn_def( flat_transform, flat_transformed );

    men_fn_def( flat, flatten );

    men_fn_def( all_flat, all_flatten );

};


} // ! namespace detail
} // ! namespace streams
} // ! namespace cranberries
#undef men_fn_def
#endif