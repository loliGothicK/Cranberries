#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "../../cranberries/detection_toolkit.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestforDetectionToolkit
{		
  template < class T >
  using has_value = decltype(T::value);
  using namespace cranberries;

  TEST_CLASS(UnitTestforDetectionToolkit)
  {
  public:
    TEST_METHOD(DetectionToolkitTestMethod)
    {
      static_assert(
        is_satisfied_v<has_value, std::true_type>,
        "failure"
        );

    }

  };
}