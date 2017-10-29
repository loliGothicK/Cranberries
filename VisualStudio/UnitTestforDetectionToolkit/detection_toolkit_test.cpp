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

      static_assert(
        is_satisfied_v<random_accsess_iter_require, typename std::vector<int>::iterator>,
        "failure"
        );
      static_assert(
        is_satisfied_v<random_accsess_iter_require, int*>,
        "failure"
        );
      static_assert(
        is_satisfied_v<arithmetic4_require, int>,
        "failure"
        );
      static_assert(
        is_satisfied_v<equality_require, int>,
        "failure"
        );
      static_assert(
        is_satisfied_v<equivalence_require, int>,
        "failure"
        );


    }

  };
}