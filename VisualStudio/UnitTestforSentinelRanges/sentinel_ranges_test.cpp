#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../cranberries/experimental/ranges.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestforSentinelRanges
{		
  TEST_CLASS(UnitTestforSentinelRanges)
  {
  public:
    
    TEST_METHOD(SentinelRangesTestMethod)
    {
      using namespace cranberries::experimental::ranges;

      for (auto const& e
        : create::generate_canonical() | lazy::take(4))
      {
        std::cout << e << " ";
      }

      std::cout << std::endl;

      std::vector<std::string> v{ "vector", "adapting", "test", "dummy" };

      v | lazy::take(3)
        | action::write_line();

    }

  };
}