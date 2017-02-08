#ifndef CRANBERRIES_STREAMS_DETAIL_CONNECTOR_HPP
#define CRANBERRIES_STREAMS_DETAIL_CONNECTOR_HPP
#include <utility>
#include "../forward.hpp"
#include "../stream.hpp"
#include "../operators/Identity.hpp"
#include "../utility.hpp"
#include "../OperationTree.hpp"

namespace cranberries {
namespace streams {

  template <
    typename OP1, // evaluation first
    typename OP2  // evaluation second
  >
  class Connect 
    : private cranberries_magic::SequencialOperatorBase
  {
  public:
    using first_t = OP1;
    using second_t = OP2;

    Connect(OP1&& op1, OP2&& op2) : op1_{ op1 }, op2_{ op2 } {}

    template < typename Stream >
    decltype(auto)
    operator()(Stream&& stream_) {
      for (auto&& e : stream_) e = op2_[op1_[e]];
      return std::forward<Stream>(stream_);
    }

    template < typename T >
    auto&
    operator[](T&& a) {
      return op2_[op1_[a]];
    }
  private:
    OP1 op1_;
    OP2 op2_;
  };

  template <
    typename OP2
  >
  constexpr
  auto
  make_op_tree(operators::Identity&&, OP2&& op2){
    return std::move(op2);
  }

  template <
    typename OP1,
    typename OP2,
    enabler_t<conjunction_v<
      negation<cranberries_magic::is_operation_tree<OP1>>,
      cranberries_magic::is_sequencial_operator<OP1>,
      cranberries_magic::is_sequencial_operator<OP2>
    >> = nullptr
  >
  constexpr
  Connect<OP1,OP2>
  make_op_tree(OP1&& op1, OP2&& op2) {
    return { std::move(op1), std::move(op2) };
  }

  template <
    typename OP1,
    typename OP2,
    enabler_t<conjunction_v<
      negation<cranberries_magic::is_operation_tree<OP1>>,
      disjunction<
        negation<cranberries_magic::is_sequencial_operator<OP1>>,
        negation<cranberries_magic::is_sequencial_operator<OP2>>>
    >> = nullptr
  >
  constexpr
  OperationTree<OP1, OP2>
  make_op_tree(OP1&& op1, OP2&& op2) {
    return { std::move(op1), std::move(op2) };
  }

  template <
	  typename OP1, typename OP2,
	  enabler_t<conjunction_v<
      cranberries_magic::is_operation_tree<OP1>,
      cranberries_magic::is_sequencial_operator<typename OP1::seoncd_t>
    >> = nullptr
  >
  constexpr
  OperationTree<typename OP1::first_t, Connect<typename OP1::second_t, OP2>>
  make_op_tree(OP1&& op1, OP2&& op2) {
    return { op1.first(), { op1.second(), std::move(op2) } };
  }

  template <
    typename OP1, typename OP2,
    enabler_t<conjunction_v<
      cranberries_magic::is_operation_tree<OP1>,
      negation<cranberries_magic::is_sequencial_operator<typename OP1::seoncd_t>>
    >> = nullptr
  >
  OperationTree<OP1,OP2>
  make_op_tree(OP1&& op1, OP2&& op2) {
    return { std::move(op1), std::move(op2) };
  }

} // ! namespace stream
} // ! namespace cranberries
#endif