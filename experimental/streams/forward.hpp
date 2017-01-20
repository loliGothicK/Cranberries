#ifndef CRANBERRIES_STREAMS_FORWARD_HPP
#define CRANBERRIES_STREAMS_FORWARD_HPP

#include <random>
#include "cranberries_magic/tag.hpp"
#include "operators/Identity.hpp"

namespace cranberries {
namespace streams {

  template < typename T, typename Operation = operators::Identity >
  class stream;

  template < typename T, typename Operation = operators::Identity >
  class file_stream;

namespace operators {


  template < typename BinaryFunc >
  class AdjacentForEach;

  template < typename Pred >
  class AllOf;

  template < typename Pred >
  class AnyOf;

  template < typename Branch, bool B >
  class Join;

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
  class Transform;

  template < typename FromStream,  typename To  >
  class MapTo;

  template < typename Pred >
  class NoneOf;

  template < typename F >
  class TransformProxy;

  template < typename UnaryFunc  >
  class Endomorphism;

  struct Repetition;

  class Reverse;

  struct Identity;

  class Drop;

  template < typename Pred = cranberries_magic::defaulted_t >
  class Sort;

  struct Splitter;

  template < typename To >
  struct MapToProxy;

  template < typename UnaryOp >
  class FlatMap;

  struct Run;

  template < bool B >
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
    typename InitialType = cranberries_magic::defaulted_t,
    typename BinaryOp = cranberries_magic::defaulted_t
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

  template <
    opt Option,
    template <class, class> class Seeq,
    typename Keygen
  >
  class GroupingBy;

  template <
    opt Option,
    template <class, class> class Seeq,
    typename Keygen
  >
  class PartitioningBy;

  template < typename Stream >
  class Flat;

  class FlatProxy;

  template < typename Stream >
  class FlatAll;

  class FlatAllProxy;

  template < typename Pred = cranberries_magic::defaulted_t >
  class StableSort;


  template < typename Pred = cranberries_magic::defaulted_t >
  class PartialSort;

  template < typename Pred = cranberries_magic::defaulted_t >
  class NthElement;

  template < typename G = cranberries_magic::defaulted_t, opt option = opt::ascending >
  class RadixSort;

} // ! namesapce operators
} // ! namespace stream
} // ! namespace cranberries

#endif