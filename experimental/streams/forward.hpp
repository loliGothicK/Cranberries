#ifndef CRANBERRIES_STREAMS_FORWARD_HPP
#define CRANBERRIES_STREAMS_FORWARD_HPP

#include "detail\tag.hpp"
#include "operators\Identity.hpp"

namespace cranberries {
namespace streams {

  template < typename T, typename Operation = operators::Identity >
  class stream;

namespace operators {


  template < typename BinaryFunc >
  class AdjacentForEach;

  template < typename Pred >
  class AllOf;

  template < typename Pred >
  class AnyOf;

  template < typename Branch >
  class Concatenate;

  struct DistinctProxy;

  template < typename T >
  class Distinct;

  template < typename Pred >
  class Filter;

  template < typename F >
  class Tinvoker;

  template < typename F >
  class Iinvoker;

  class Take;

  class Taker;

  template < typename OldStream, typename UnaryFunc  >
  class Map;

  template < typename FromStream,  typename To  >
  class MapTo;

  template < typename Pred >
  class NoneOf;

  template < typename F >
  struct MapProxy;

  template < typename UnaryFunc  >
  class Transform;

  struct Repetition;

  class Reverse;

  struct Identity;

  class Drop;

  template < typename Pred = detail::defaulted >
  class Sort;

  struct Splitter;

  template < typename To >
  struct MapToProxy;

  struct Run;

  class Printer;

  template < typename Pred >
  class TakeWhile;

  template < typename UnaryOp >
  class ForEach;

  template < typename UnaryOp >
  class Peek;

  template < typename Pred >
  class DropWhile;

  template < typename Pred >
  class DropperWhile;

  template < typename Stream >
  class MergeProxy;

  template < typename Rnage,bool,bool >
  class Merge;

  struct UniqueProxy;

  template < typename T >
  class Unique;
  
  template <
    typename InitialType = detail::defaulted,
    typename BinaryOp = detail::defaulted
  >
  class Accumulate;

  class Product;

  template < typename Pred >
  class SumIf;

  template < typename Target >
  class Count;

  template < typename Pred >
  class CountIf;

  class Average;

  class Median;

  class Mode;

  class Stride;

  class Slice;

  template < typename Engine = std::mt19937 >
  class Shuffle;

  template < typename ElemType >
  class Replace;

  template < typename UnaryOp, typename New >
  class ReplaceIf;

  template < typename Range >
  class SetUnion;

  template < typename Range >
  class SetIntersection;

  template < typename Range >
  class SetDiff;

  template < typename Range >
  class SetSymmetricDiff;

} // ! namesapce operators
} // ! namespace stream
} // ! namespace cranberries

#endif