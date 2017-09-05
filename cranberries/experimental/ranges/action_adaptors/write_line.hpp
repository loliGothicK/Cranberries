#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Ostream = std::ostream > 
class WriteLine
  : private tag::adaptor_tag
  , private tag::action_tag
{
  Ostream& os;
public:
  WriteLine() : os(std::cout) {}
  WriteLine(Ostream& os) : os(os) {}

  template < class SentinelRange >
  decltype(auto)
    apply(SentinelRange&& range) {
    auto iter = std::begin(range);
    std::cout << *iter; ++iter;
    for (; iter != std::end(range); ++iter)
      std::cout << " " << *iter;
    std::cout << std::endl;
    return std::forward<SentinelRange>(range);
  }
};

namespace action {
  template < class Ostream = std::ostream >
  WriteLine<std::decay_t<Ostream>>
  write_line(Ostream&& os) { return {std::forward<Ostream>(os)}; }

  WriteLine<> write_line() { return {}; }

}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP
