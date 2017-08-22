/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATION_TREE_HPP
#define CRANBERRIES_STREAMS_OPERATION_TREE_HPP
#include "./utility.hpp"

namespace cranberries {
namespace streams {


  template <
    typename Op1, // evaluate first
    typename Op2  // evaluate second
  >
  struct OperationTree
  {
    using first_t = Op1;
    using second_t = Op2;

    OperationTree() = default;
    OperationTree(OperationTree const&) = default;
    OperationTree(OperationTree&&) = default;
    OperationTree(Op1 op1, Op2 op2) : op1{ std::forward<Op1>(op1) }, op2{std::forward<Op2>(op2)} {}

    auto first() { return op1; }
    auto second() { return op2; }

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream
    )
      noexcept(false)
    {
      return op2(op1(std::forward<Stream>(stream)));
    }

    // members
    Op1 op1;
    Op2 op2;
  };

}
}

#endif