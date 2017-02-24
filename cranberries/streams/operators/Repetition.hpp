#ifndef CRANBERRIES_STREAMS_OPERATORS_REPETITION_HPP
#define CRANBERRIES_STREAMS_OPERATORS_REPETITION_HPP
#include <utility>
#include "../cranberries_magic/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  //------------------------------//
  // Just Time Evaluate Operation //
  //------------------------------//

  // Just Time Evaluate Operation
  struct Repetition { size_t times; };

  // Just Time Evaluate Operation
  struct Run {};



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif