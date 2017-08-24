#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../cranberries/algorithm.hpp"
#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestforAlgorithm
{		
	TEST_CLASS(UnitTestforAlgorithm)
	{
	public:
		
		TEST_METHOD(AlgorithmTestMethod)
		{
      std::array<int, 6> v{ { 4,5,1,2,3,6 } };

      cranberries::ascending_radix_sort(begin(v), end(v));
      v == std::array<int, 6>{ { 1, 2, 3, 4, 5, 6 }};

      cranberries::descending_radix_sort(begin(v), end(v));
      v == std::array<int, 6>{{ 6, 5, 4, 3, 2, 1 }};
    }

	};
}