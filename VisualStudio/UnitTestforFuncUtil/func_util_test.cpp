#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../cranberries/func_util.hpp"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestforFuncUtil
{
  struct Twice {
    template < class T >
    constexpr auto operator()(T a_) const noexcept { return a_ * 2; }
  };
  struct Add {
    template < class T >
    constexpr auto operator()(T a_, T b_) const noexcept { return a_ + b_; }
  };

  TEST_CLASS(UnitTestforFuncUtil)
  {
  public:
    TEST_METHOD(FuncUtilTestMethod)
    {
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
      println |= chunk<2>::bind(1, 2, 3, 4, 5, 6);

      constexpr Twice f{};
      constexpr Add g{};
      constexpr auto h = cranberries::composition(f, g);


      std::cout << "conbinator & chunk apply" << "\n";
      constexpr auto var1 = h |= chunk<2>::bind(1, 2, 3, 4, 5, 6);
      static_assert(
        std::get<0>(var1) == 6 &&
        std::get<1>(var1) == 14 &&
        std::get<2>(var1) == 22,
        ""
        );

      println <<= h |= chunk<2>::bind(1, 2, 3, 4, 5, 6);

      std::cout << "conbinator & adjacent apply" << "\n";
      constexpr auto var2 = h |= adjacent<2>::bind(1, 2, 3, 4, 5, 6);
      static_assert(
        std::get<0>(var2) == 6 &&
        std::get<1>(var2) == 10 &&
        std::get<2>(var2) == 14,
        ""
        );
      println <<= h |= adjacent<2>::bind(1, 2, 3, 4);

      std::cout << "conbinator & chunk apply(random sampling)" << "\n";
      println <<= h |= chunk<2, randomized<5>>::bind(1, 2, 3, 4, 5, 6);

      std::cout << "permutations" << "\n";
      println |= permutation<2>::bind(1, 2, 3);

      std::cout << "combinations" << "\n";
      println |= combination<2>::bind(1, 2, 3);

      std::cout << "permutations" << "\n";
      println |= permutation<>::bind(1, 2, 3);

      std::cout << "combinations" << "\n";
      println |= combination<>::bind(1, 2, 3);
    }

  };
}