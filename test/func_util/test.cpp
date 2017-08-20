#include <iostream>
#include "../../cranberries/func_util.hpp"
#include "../../cranberries/utility.hpp"
struct Twice {
  template < class T >
  constexpr auto operator()(T a_) const noexcept { return a_ * 2; }
};
struct Add {
  template < class T >
  constexpr auto operator()(T a_, T b_) const noexcept { return a_ + b_; }
};


int main() {
  auto println = [](auto&& head, auto&&... tail) {
    std::cout << head;
    (void)cranberries::Swallows{ (void(std::cout << " " << tail),0)... };
    std::cout << std::endl;
  };

  using namespace cranberries::func_util;

  std::cout << "all apply" << "\n";
  println <<= std::forward_as_tuple(1, 2, 3, 4);


  std::cout << "each apply" << "\n";
  println |= std::forward_as_tuple(1, 2, 3, 4);

  std::cout << "chunk apply" << "\n";
  println |= chunk<2>::apply(1, 2, 3, 4, 5, 6);

  constexpr Twice f{};
  constexpr Add g{};


  std::cout << "conbinator & chunk apply" << "\n";
  constexpr auto var1 = f*g |= chunk<2>::apply(1, 2, 3, 4, 5, 6);
  static_assert(
    std::get<0>(var1) == 6 &&
    std::get<1>(var1) == 14 &&
    std::get<2>(var1) == 22,
    ""
    );

  println <<= f*g |= chunk<2>::apply(1, 2, 3, 4, 5, 6);



  std::cout << "conbinator & adjacent apply" << "\n";
  constexpr auto var2 = f*g |= adjacent<2>::apply(1, 2, 3, 4, 5, 6);
  static_assert(
    std::get<0>(var2) == 6 &&
    std::get<1>(var2) == 10 &&
    std::get<2>(var2) == 14,
    ""
    );
  println <<= f*g |= adjacent<2>::apply(1, 2, 3, 4);

  std::cout << "conbinator & chunk apply(random sampling)" << "\n";
  println <<= f*g |= chunk<2, random_sampling<10_times>>::apply(1, 2, 3, 4, 5, 6);

  std::cout << "permutations" << "\n";
  println |= permutation<2>::apply(1, 2, 3);

  std::cout << "combinations" << "\n";
  println |= combination<2>::apply(1, 2, 3);
}