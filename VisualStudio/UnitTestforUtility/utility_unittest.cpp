#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../cranberries/utility.hpp"
#include <vector>
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestforUtility
{		
  TEST_CLASS(UnitTestforUtility)
  {
  public:
    
    TEST_METHOD(UtilityTestMethod)
    {
      using namespace cranberries;
      index_for_each(
        construct_from_tuple<std::vector<int>>(std::make_tuple(1, 2, 3, 4)),
        [](int i, auto e) noexcept {
        assert(i == e);
      });
      auto overload = make_overload(
        [](int a) { return "int"; },
        [](double a) { return "double"; }
      );
      assert(overload(1) == "double");
      assert(overload(1.0) == "double");
    }

  };
}