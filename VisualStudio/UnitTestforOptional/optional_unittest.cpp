#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../cranberries/optional.hpp"
#include "../../cranberries/unit_test/unit_test.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestforOptional
{		
	TEST_CLASS(OptionalUnitTest)
	{
	public:
		
		TEST_METHOD(OptionalTestMethod)
		{
			namespace unit = cranberries::unit_test_framework;
			using cranberries::optional;
			using cranberries::nullopt;

			auto fuck_ms = [](auto&& val) {
				std::ostringstream ss;
				ss << val;
				Logger::WriteMessage(ss.str().c_str());
			};

			try {
#pragma region opt_extention_tests
				unit::make_unit_test_container(unit::make_logger(fuck_ms), "optional extention tests")
/* 01 */	% unit::assertion::are_equal(optional<int>{}.value_or(1), 1)
						.labeled("value_or:nullopt")
/* 02 */	% unit::assertion::are_equal(optional<int>{2}.value_or(1), 2)
						.labeled("value_or:valid")
/* 03 */	% unit::assertion::are_equal(optional<int>{}.value_or_else([] { return 1; }), 1)
						.labeled( "value_or_else:nullopt" )
/* 04 */	% unit::assertion::are_equal(optional<int>{2}.value_or_else([] { return 1; }), 2)
						.labeled("value_or_else:valid")
/* 05 */	% unit::assertion::are_equal(optional<int>{2}.map([](auto a) { return a * 2; }), 4)
						.labeled("map:valid")
/* 06 */	% unit::assertion::are_equal(optional<int>{}.map([](auto a) { return a * 2.; }), nullopt)
						.labeled("map:nullopt")
/* 07 */	% unit::assertion::are_equal(optional<int>{2}.map_or([](auto a) { return a * 2; }, 3), 4)
						.labeled("map_or:valid")
/* 08 */	% unit::assertion::are_equal(optional<int>{}.map_or([](auto a) { return a * 2; }, 3), 3)
						.labeled("map_or:nullopt")
/* 09 */	% unit::assertion::are_equal(optional<int>{2}.map_or_else([](auto a) { return a * 2; }, [] { return 3; }), 4)
						.labeled("map_or_else:valid")
/* 10 */	% unit::assertion::are_equal(optional<int>{}.map_or_else([](auto a) { return a * 2; }, [] { return 3; }), 3)
						.labeled("map_or_else:nullopt")
/* 11 */	% unit::assertion::are_equal(optional<int>{}.and_(optional<int>{2}), nullopt)
						.labeled("and_:nullopt & valid")
/* 12 */	% unit::assertion::are_equal(optional<int>{2}.and_(optional<int>{}), nullopt)
						.labeled("and_:valid & nullopt")
/* 13 */	% unit::assertion::are_equal(optional<int>{2}.and_(optional<int>{2}), 2)
						.labeled("and_:valid & valid")
/* 14 */	% unit::assertion::are_equal(optional<int>{}.and_then([](auto a) { return a * 2; }), nullopt)
						.labeled("and_then:nullopt")
/* 15 */	% unit::assertion::are_equal(optional<int>{2}.and_then([](auto a) { return a * 2; }), 4)
						.labeled("and_then:valid")
/* 16 */ % unit::assertion::are_equal(optional<int>{}.or_(optional<int>{2}), 2)
						.labeled("or_:nullopt & valid")
/* 17 */ % unit::assertion::are_equal(optional<int>{2}.or_(optional<int>{}), 2)
						.labeled("or_:valid & nullopt")
/* 18 */ % unit::assertion::are_equal(optional<int>{2}.or_(optional<int>{2}), 2)
						.labeled("or_:valid & valid")
					| unit::collect;
#pragma endregion

			}
			catch (unit::detail_::assert_except const&) {
				Assert::Fail(L"Test Failed");
			}
			catch (...) {
				Assert::Fail(L"Unhandled exception throw");
			}


		}

	};
}