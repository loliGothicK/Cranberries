#ifndef CRANBERRIES_STREAMS_OPERATORS_SPLITTER_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SPLITTER_HPP
#include <string>
#include <regex>
#include <utility>
#include "../detail/tag.hpp"
#include "..\StreamConverter.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  struct Splitter
  {
    Splitter(std::string s) : r_{ s } {}
    
    Splitter& operator+= ( std::string const& str ) & {
      target_ = str;
      return *this;
    }

    Splitter&& operator+= ( std::string const& str ) && {
      target_ = str;
      return std::move(*this);
    }

    template < class Cont >
    operator Cont() {
      return Cont{
        std::sregex_token_iterator{ target_.begin(), target_.end(), r_, -1 },
        std::sregex_token_iterator{}
      };
    }

    template < template < class T, class Allocator = std::allocator<T> > class Cont >
    Cont<std::string> convert_to() {
      return Cont<std::string>{
        std::sregex_token_iterator{ target_.begin(), target_.end(), r_, -1 },
        std::sregex_token_iterator{}
      };
    }


    std::string target_;
    std::regex r_{};
  };

  Splitter& operator |
  (
    std::string const& str,
    Splitter& sp
  ) {
    return sp += str;
  }

  Splitter&& operator |
  (
    std::string const& str,
    Splitter&& sp
  ) {
    return std::move(sp += str);
  }

  template < template < class T, class Allocator = std::allocator<T> > class Cont >
  Cont<std::string> operator |
  (
    Splitter&& sp,
    detail::ConvertTo<Cont>
  ) {
    return sp.convert_to<Cont>();
  }

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif