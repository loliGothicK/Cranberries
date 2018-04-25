#include <iostream>
#include "../../cranberries/unit_test/unit_test.hpp"
#include "../../cranberries/optional.hpp"
int main()
try{
    namespace unit = cranberries::unit_test_framework;
    using cranberries::optional;
    using cranberries::nullopt;
    using cranberries::in_place;
    using cranberries::some;
    using cranberries::nil;

				unit::make_unit_test_container(unit::default_logger, "optional basic tests")
					% unit::assertion::excact_throw<cranberries::bad_optional_access>([] { optional<int>{}.value(); })
					.labeled("bad access")
					% unit::assertion::are_equal(*optional<int>{1}, 1)
					.labeled("force access")
					% unit::assertion::are_equal(optional<std::pair<int, int>>{in_place, 1, 1}->first, 1)
					.labeled("member to pointer")
					% unit::assertion::are_equal(nil<std::tuple<int, int, int>>.emplace(1, 2, 3), std::make_tuple(1, 2, 3))
					.labeled("emplace")
					% unit::assertion::are_equal(cranberries::make_optional(1), 1)
					.labeled("make_optional")
					% unit::assertion::are_equal(some(1), 1)
					.labeled("some extention")
					% unit::assertion::are_equal([]{ auto opt = some(1); opt.reset(); return opt; }(), nullopt)
						.labeled("reset")
					% unit::assertion::are_equal(some(1).has_value(), true)
						.labeled("has_value( some )")
					% unit::assertion::are_equal(nil<int>.has_value(), false)
						.labeled("has_value( nil )")
					| unit::collect;

				unit::make_unit_test_container(unit::default_logger, "optional operator== tests")
					% unit::assertion::are_equal(some(1) == 1, true)
						.labeled("opt(has value) == val")
					% unit::assertion::are_equal(nil<int> == 1, false)
						.labeled("opt(null) == val")
					% unit::assertion::are_equal(nil<int> == nil<int>, true)
						.labeled("opt(null) == opt(null)")
					% unit::assertion::are_equal(nil<int> == some(1), false)
						.labeled("opt(null) == opt(has value)")
					% unit::assertion::are_equal(some(1) == nil<int>, false)
						.labeled("opt(has value) == opt(null)")
					% unit::assertion::are_equal(some(1) == some(1), true)
						.labeled("opt(has value) == opt(has value)")
					| unit::collect;

				unit::make_unit_test_container(unit::default_logger, "optional operator!= tests")
					% unit::assertion::are_equal(some(1) != 1, false)
						.labeled("opt(has value) != val")
					% unit::assertion::are_equal(nil<int> != 1, true)
						.labeled("opt(null) != val")
					% unit::assertion::are_equal(nil<int> != nil<int>, false)
						.labeled("opt(null) != opt(null)")
					% unit::assertion::are_equal(nil<int> != some(1), true)
						.labeled("opt(null) != opt(has value)")
					% unit::assertion::are_equal(some(1) != nil<int>, true)
						.labeled("opt(has value) != opt(null)")
					% unit::assertion::are_equal(some(1) != some(1), false)
						.labeled("opt(has value) != opt(has value)")
					| unit::collect;

				unit::make_unit_test_container(unit::default_logger, "optional operator< tests")
					% unit::assertion::are_equal(some(1) < 2, true)
						.labeled("opt(has value) < val")
					% unit::assertion::are_equal(nil<int> < 1, true)
						.labeled("opt(null) < val")
					% unit::assertion::are_equal(nil<int> < nil<int>, false)
						.labeled("opt(null) < opt(null)")
					% unit::assertion::are_equal(nil<int> < some(1), true)
						.labeled("opt(null) < opt(has value)")
					% unit::assertion::are_equal(some(1) < nil<int>, false)
						.labeled("opt(has value) < opt(null)")
					% unit::assertion::are_equal(some(1) < some(2), true)
						.labeled("opt(has value) < opt(has value)")
					| unit::collect;

				unit::make_unit_test_container(unit::default_logger, "optional operator<= tests")
					% unit::assertion::are_equal(some(1) <= 2, true)
						.labeled("opt(has value) <= val")
					% unit::assertion::are_equal(nil<int> <= 1, true)
						.labeled("opt(null) <= val")
					% unit::assertion::are_equal(nil<int> <= nil<int>, true)
						.labeled("opt(null) <= opt(null)")
					% unit::assertion::are_equal(nil<int> <= some(1), true)
						.labeled("opt(null) <= opt(has value)")
					% unit::assertion::are_equal(some(1) <= nil<int>, false)
						.labeled("opt(has value) <= opt(null)")
					% unit::assertion::are_equal(some(1) <= some(1), true)
						.labeled("opt(has value) <= opt(has value)")
					| unit::collect;

				unit::make_unit_test_container(unit::default_logger, "optional operator> tests")
					% unit::assertion::are_equal(some(1) > 2, false)
						.labeled("opt(has value) > val")
					% unit::assertion::are_equal(nil<int> > 1, false)
						.labeled("opt(null) > val")
					% unit::assertion::are_equal(nil<int> > nil<int>, false)
						.labeled("opt(null) > opt(null)")
					% unit::assertion::are_equal(nil<int> > some(1), false)
						.labeled("opt(null) > opt(has value)")
					% unit::assertion::are_equal(some(1) > nil<int>, true)
						.labeled("opt(has value) > opt(null)")
					% unit::assertion::are_equal(some(1) > some(2), false)
						.labeled("opt(has value) > opt(has value)")
					| unit::collect;

				unit::make_unit_test_container(unit::default_logger, "optional operator>= tests")
					% unit::assertion::are_equal(some(3) >= 2, true)
						.labeled("opt(has value) >= val")
					% unit::assertion::are_equal(nil<int> >= 1, false)
						.labeled("opt(null) >= val")
					% unit::assertion::are_equal(nil<int> >= nil<int>, true)
						.labeled("opt(null) >= opt(null)")
					% unit::assertion::are_equal(nil<int> >= some(1), false)
						.labeled("opt(null) >= opt(has value)")
					% unit::assertion::are_equal(some(1) >= nil<int>, true)
						.labeled("opt(has value) >= opt(null)")
					% unit::assertion::are_equal(some(1) >= some(1), true)
						.labeled("opt(has value) >= opt(has value)")
					| unit::collect;

				unit::make_unit_test_container(unit::default_logger, "optional extention tests")
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

}
catch (unit::detail_::assert_except const&) {
  std::cout << "Test Failed\n";
  return EXIT_FAILURE;
}
catch (...) {
  std::cout << "Unhandled exception";
  return EXIT_FAILURE;
}
