#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Cranberries/cranberries/stopwatch.hpp"
#include <iostream>
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestforStopwatch
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(StopwatchTestMethod)
		{
      auto timer = cranberries::stopwatch{};
      timer.start();
      timer.get_ticks();
      timer.is_started();
      timer.lap();
      timer.stop();
      timer.clear();
		}

	};
}