#include "../../cranberries/experimental/ranges.hpp"
#include <iostream>

int main()
{
  using namespace cranberries::experimental::ranges;
  
  create::generate_canonical()
    | lazy::take(4)
    | action::write_line();

  create::range(1, 10)
    | action::write_line();

  create::iterate(1, [](auto v) { return v * 2; })
    | lazy::take(5)
    | action::write_line();

  for (auto const& e : create::range(1, 10) )
    std::cout << e << " ";

  std::cout << std::endl;

  std::vector<std::string> v{ "a", "b", "c", "dummy" };

  v | lazy::take(3)
    | action::write_line();

  return exit_status1 && exit_status2 ? EXIT_SUCCESS : EXIT_FAILURE;
}
