#ifndef CRANBERRIES_STREAMS_DETAIL_TAG
#define CRANBERRIES_STREAMS_DETAIL_TAG

namespace cranberries {
namespace streams {

  enum class opt {
    ordered,
    unordered,
    left,
    right,
    ascending,
    descending,
  };

namespace cranberries_magic {

  // Defaluted Tag
  struct defaulted_t {};

} // ! namespace cranberries_magic
} // ! namespace stream
} // ! namespace cranberries

#endif