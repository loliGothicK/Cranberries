#ifndef CRANBERRIES_EXPECTED_HPP
#define CRANBERRIES_EXPECTED_HPP
#include <string>

namespace cranberries {
  template < class T, class E = std::string >
  class expected {
    T value_;
    E error_;
  public:

  };
}

#endif
