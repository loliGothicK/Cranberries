#ifndef CRANBERRIES_STREAMS_OPERATIONS_HPP
#define CRANBERRIES_STREAMS_OPERATIONS_HPP
#include <utility>
#include <string>
#include <type_traits>
#include "forward.hpp"
#include "detail/tag.hpp"


namespace cranberries {
namespace streams {

  //----------------------//
  // Intermediate process //
  //----------------------//


  inline operators::Splitter split(std::string s) noexcept { return{ s }; }

  template <
    typename Pred
  >
  inline
  operators::Filter<Pred>
  filtered
  (
    Pred&& pred
  )
    noexcept
  {
    return{ std::forward<Pred>(pred) };
  }

  inline operators::Sort<> sorted() noexcept { return{}; }

  template <
    typename Pred
  >
  inline
  operators::Sort<Pred>
  sorted
  (
    Pred&& pred
  )
    noexcept
  {
    return{ std::forward<Pred>(pred) };
  }

  inline operators::DistinctProxy distinctly() noexcept { return{}; }

  inline operators::Drop dropped(std::size_t n)  noexcept { return{ n }; }

  inline operators::Take taken(std::size_t n) noexcept { return{ n }; }

  template < typename To >
  inline operators::MapToProxy<To> map_to() noexcept { return{}; }

  inline operators::MapToProxy<std::string> to_string() noexcept { return{}; }

  inline operators::Repetition repeat(size_t times = 1) noexcept { return{ times }; }

  inline operators::Run run() noexcept { return{}; }

  template <
    typename BinaryFunc
  >
  inline
  operators::AdjacentForEach<BinaryFunc>
  adjacent_for_each
  (
    BinaryFunc&& f
  )
    noexcept
  {
    return{ std::forward<BinaryFunc>(f) };
  }

  template <
    typename UnaryFunc
  >
  inline
  operators::ForEach<UnaryFunc>
  for_each
  (
    UnaryFunc&& func
  )
    noexcept
  {
    return{ std::forward<UnaryFunc>(func) };
  }

  template <
    typename UnaryFunc
  >
  inline
  operators::Peek<UnaryFunc>
  peeked
  (
    UnaryFunc&& func
  )
    noexcept
  {
    return{ std::forward<UnaryFunc>(func) };
  }


  template <
    typename UnaryFunc
  >
  inline
  operators::MapProxy<UnaryFunc>
  mapped
  (
    UnaryFunc&& func
  )
    noexcept
  {
    return{ std::forward<UnaryFunc>(func) };
  }

  template <
    typename UnaryFunc
  >
  inline
  operators::Transform<UnaryFunc>
  transformed
  (
    UnaryFunc&& func
  )
    noexcept
  {
    return{ std::forward<UnaryFunc>(func) };
  }

  inline operators::UniqueProxy uniquely() noexcept { return{}; }
  
  inline operators::Reverse reversed() noexcept { return{}; }

  template < typename Engine = std::mt19937 >
  inline operators::Shuffle<Engine&&> shuffled( Engine&& engine_ = std::mt19937{} ) { return{ std::forward<Engine>( engine_ ) }; }

  template < typename ElemType >
  inline operators::Replace<ElemType&&> replaced( ElemType&& old_value, ElemType&& new_value ) { return{ std::forward<ElemType>( old_value ), std::forward<ElemType>( new_value ) }; }

  template < typename UnaryOp, typename New >
  inline operators::ReplaceIf<UnaryOp&&, New&&> replaced_if( UnaryOp&& op, New&& new_value ) { return{ std::forward<UnaryOp>( op ), std::forward<New>( new_value ) }; }

  template < typename Range >
  inline operators::SetUnion<Range&&> set_union( Range&& range ) { return{ std::forward<Range>( range ) }; }

  template < typename Range >
  inline operators::SetIntersection<Range&&> set_intersection( Range&& range ) { return{ std::forward<Range>( range ) }; }

  template < typename Range >
  inline operators::SetDiff<Range&&> set_difference( Range&& range ) { return{ std::forward<Range>( range ) }; }

  template < typename Range >
  inline operators::SetSymmetricDiff<Range&&> set_symmetric_difference( Range&& range ) { return{ std::forward<Range>( range ) }; }

  template <
    typename Stream,
    std::enable_if_t<
      detail::is_infinite_stream_v<std::decay_t<Stream>>
      || detail::is_finite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  operators::Merge<Stream>
  merged
  (
    Stream&& stream_
  )
    noexcept
  {
    return{ std::forward<Stream>(stream_) };
  }

  template <
    typename Stream,
    std::enable_if_t<detail::is_stream_v<std::decay_t<Stream>>, std::nullptr_t> = nullptr
  >
  inline
  operators::Concatenate<Stream&&>
  concat(
    Stream&& stream_
  )
    noexcept
  {
    return{ std::forward<Stream>(stream_) };
  }


  inline
  operators::Printer
  print_to
  (
    std::ostream& os = std::cout, // default output std::cout
    std::string delim = ", " // default delimiter ", "
  ) {
    return{ os, delim };
  }

  template <
    typename Pred
  >
  inline
  operators::AllOf<Pred>
  all_of
  (
    Pred&& pred
  )
    noexcept
  {
    return{ std::forward<Pred>(pred) };
  }

  template <
    typename Pred
  >
  inline
  operators::AnyOf<Pred>
  any_of
  (
    Pred&& pred
  )
    noexcept
  {
    return{ std::forward<Pred>(pred) };
  }

  template <
    typename Pred
  >
  inline
  operators::NoneOf<Pred>
  none_of
  (
    Pred&& pred
  )
    noexcept
  {
    return{ std::forward<Pred>(pred) };
  }

  template <
    typename InitialType,
    typename BinaryOp
  >
  inline
  operators::Accumulate<
    InitialType,
    BinaryOp
  > accumulate
  (
    InitialType init,
    BinaryOp&& op
  )
    noexcept
  {
    return{ init, std::forward<BinaryOp>(op) };
  }

  template <
    typename InitialType
  >
  inline
  operators::Accumulate<
    InitialType,
    detail::defaulted
  > accumulate
  (
    InitialType init
  )
    noexcept
  {
    return{init};
  }

  template < typename Target >
  inline operators::Count<Target> count( Target v ) noexcept { return{ v }; }

  template < typename Pred >
  inline operators::CountIf<Pred> count_if( Pred&& pred_ ) noexcept { return{ pred_ }; }

  inline operators::Accumulate<detail::defaulted,detail::defaulted> accumulate() noexcept { return{}; }

  inline operators::Accumulate<detail::defaulted,detail::defaulted> sum() noexcept { return{}; }

  template < typename Pred >
  inline operators::SumIf<Pred> sum_if( Pred&& pred_ ) noexcept { return{ Pred_ }; }

  inline operators::Product prod() noexcept { return{}; }

  inline operators::Average average() noexcept { return{}; }

  inline operators::Median median() noexcept { return{}; }

  inline operators::Mode mode() noexcept { return{}; }

  inline operators::Stride strided( size_t step ) noexcept { return{ step }; }

  inline operators::Slice sliced( size_t low, size_t up ) noexcept { return{ low,up }; }

  template <
    typename Pred
  >
  inline
  operators::TakeWhile<Pred>
  taken_while
  (
    Pred&& pred
  )
    noexcept
  {
    return{ std::forward<Pred>(pred) };
  }

  template <
    typename Pred
  >
  inline
  operators::DropWhile<Pred>
  dropped_while
  (
    Pred&& pred
  )
    noexcept
  {
    return{ std::forward<Pred>(pred) };
  }

  //----------------------//
  // Operation Assistants //
  //----------------------//

  template <
    typename Func
  >
  inline
  decltype(auto)
  invoked(
    Func&& f
  )
    noexcept
  {
    return operators::Iinvoker<Func>{ std::forward<Func>(f) };
  }

  template <
    typename Func
  >
  inline
  decltype(auto)
  invoker(
    Func&& f
  )
    noexcept
  {
    return operators::Tinvoker<Func>{ std::forward<Func>(f) };
  }




} // ! namespace stream
} // ! namespace cranberries

#endif