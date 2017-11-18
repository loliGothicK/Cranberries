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
      index_for_each(0,
        construct_from_tuple<std::vector<int>>(std::make_tuple(0, 1, 2, 3)),
        [](size_t i, size_t e) {
        if (i != e) throw "fail: line 21";
      });
      auto overload = make_overload(
        [](int a) { return "int"; },
        [](double a) { return "double"; }
      );
      if (overload(1) != "int") throw "fail: line 27";
      if (overload(1.0) != "double") throw "fail: line 28";
    }

  };
}