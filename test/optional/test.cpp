#include <iostream>
#include "../../cranberries/unit_test/unit_test.hpp"
#include "../../cranberries/optional.hpp"
namespace unit = cranberries::unit_test_framework;
int main()
try{
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
  % unit::assertion::is_true(some(1).has_value())
    .labeled("has_value( some )")
  % unit::assertion::is_false(nil<int>.has_value())
    .labeled("has_value( nil )")
  | unit::collect;

unit::make_unit_test_container(unit::default_logger, "optional operator== tests")
  % unit::assertion::is_true(some(1) == 1)
    .labeled("opt(has value) == val")
  % unit::assertion::is_false(nil<int> == 1)
    .labeled("opt(null) == val")
  % unit::assertion::is_true(nil<int> == nil<int>)
    .labeled("opt(null) == opt(null)")
  % unit::assertion::is_false(nil<int> == some(1))
    .labeled("opt(null) == opt(has value)")
  % unit::assertion::is_false(some(1) == nil<int>)
    .labeled("opt(has value) == opt(null)")
  % unit::assertion::is_true(some(1) == some(1))
    .labeled("opt(has value) == opt(has value)")
  | unit::collect;

unit::make_unit_test_container(unit::default_logger, "optional operator!= tests")
  % unit::assertion::is_false(some(1) != 1)
    .labeled("opt(has value) != val")
  % unit::assertion::is_true(nil<int> != 1)
    .labeled("opt(null) != val")
  % unit::assertion::is_false(nil<int> != nil<int>)
    .labeled("opt(null) != opt(null)")
  % unit::assertion::is_true(nil<int> != some(1))
    .labeled("opt(null) != opt(has value)")
  % unit::assertion::is_true(some(1) != nil<int>)
    .labeled("opt(has value) != opt(null)")
  % unit::assertion::is_false(some(1) != some(1))
    .labeled("opt(has value) != opt(has value)")
  | unit::collect;

unit::make_unit_test_container(unit::default_logger, "optional operator< tests")
  % unit::assertion::less(some(1), 2)
    .labeled("opt(has value) < val")
  % unit::assertion::less(nil<int>, 1)
    .labeled("opt(null) < val")
  % unit::assertion::is_false(nil<int> < nil<int>)
    .labeled("opt(null) < opt(null)")
  % unit::assertion::less(nil<int>, some(1))
    .labeled("opt(null) < opt(has value)")
  % unit::assertion::is_false(some(1) < nil<int>)
    .labeled("opt(has value) < opt(null)")
  % unit::assertion::less(some(1), some(2))
    .labeled("opt(has value) < opt(has value)")
  | unit::collect;

unit::make_unit_test_container(unit::default_logger, "optional operator<= tests")
  % unit::assertion::less_equal(some(1), 2)
    .labeled("opt(has value) <= val")
  % unit::assertion::less_equal(nil<int>, 1)
    .labeled("opt(null) <= val")
  % unit::assertion::less_equal(nil<int>, nil<int>)
    .labeled("opt(null) <= opt(null)")
  % unit::assertion::less_equal(nil<int>, some(1))
    .labeled("opt(null) <= opt(has value)")
  % unit::assertion::is_false(some(1) <= nil<int>)
    .labeled("opt(has value) <= opt(null)")
  % unit::assertion::less_equal(some(1) <= some(1))
    .labeled("opt(has value) <= opt(has value)")
  | unit::collect;

unit::make_unit_test_container(unit::default_logger, "optional operator> tests")
  % unit::assertion::is_false(some(1) > 2)
    .labeled("opt(has value) > val")
  % unit::assertion::is_false(nil<int> > 1)
    .labeled("opt(null) > val")
  % unit::assertion::is_false(nil<int> > nil<int>)
    .labeled("opt(null) > opt(null)")
  % unit::assertion::is_false(nil<int> > some(1))
    .labeled("opt(null) > opt(has value)")
  % unit::assertion::greater(some(1), nil<int>)
    .labeled("opt(has value) > opt(null)")
  % unit::assertion::is_false(some(1) > some(2))
    .labeled("opt(has value) > opt(has value)")
  | unit::collect;

unit::make_unit_test_container(unit::default_logger, "optional operator>= tests")
  % unit::assertion::greater_equal(some(3), 2)
    .labeled("opt(has value) >= val")
  % unit::assertion::greater_equal(nil<int>, 1)
    .labeled("opt(null) >= val")
  % unit::assertion::greater_equal(nil<int>, nil<int>)
    .labeled("opt(null) >= opt(null)")
  % unit::assertion::is_false(nil<int> >= some(1))
    .labeled("opt(null) >= opt(has value)")
  % unit::assertion::greater_equal(some(1), nil<int>)
    .labeled("opt(has value) >= opt(null)")
  % unit::assertion::greater_equal(some(1), some(1))
    .labeled("opt(has value) >= opt(has value)")
  | unit::collect;

unit::make_unit_test_container(unit::default_logger, "optional extention tests")
  % unit::assertion::are_equal(optional<int>{}.value_or(1), 1)
    .labeled("value_or:nullopt")
  % unit::assertion::are_equal(optional<int>{2}.value_or(1), 2)
    .labeled("value_or:valid")
  % unit::assertion::are_equal(optional<int>{}.value_or_else([] { return 1; }), 1)
    .labeled( "value_or_else:nullopt" )
  % unit::assertion::are_equal(optional<int>{2}.value_or_else([] { return 1; }), 2)
    .labeled("value_or_else:valid")
  % unit::assertion::are_equal(optional<int>{2}.map([](auto a) { return a * 2; }), 4)
    .labeled("map:valid")
  % unit::assertion::are_equal(optional<int>{}.map([](auto a) { return a * 2.; }), nullopt)
    .labeled("map:nullopt")
  % unit::assertion::are_equal(optional<int>{2}.map_or([](auto a) { return a * 2; }, 3), 4)
    .labeled("map_or:valid")
  % unit::assertion::are_equal(optional<int>{}.map_or([](auto a) { return a * 2; }, 3), 3)
    .labeled("map_or:nullopt")
  % unit::assertion::are_equal(optional<int>{2}.map_or_else([](auto a) { return a * 2; }, [] { return 3; }), 4)
    .labeled("map_or_else:valid")
  % unit::assertion::are_equal(optional<int>{}.map_or_else([](auto a) { return a * 2; }, [] { return 3; }), 3)
    .labeled("map_or_else:nullopt")
  % unit::assertion::are_equal(optional<int>{}.and_(optional<int>{2}), nullopt)
    .labeled("and_:nullopt & valid")
  % unit::assertion::are_equal(optional<int>{2}.and_(optional<int>{}), nullopt)
    .labeled("and_:valid & nullopt")
  % unit::assertion::are_equal(optional<int>{2}.and_(optional<int>{2}), 2)
    .labeled("and_:valid & valid")
  % unit::assertion::are_equal(optional<int>{}.and_then([](auto a) { return a * 2; }), nullopt)
    .labeled("and_then:nullopt")
  % unit::assertion::are_equal(optional<int>{2}.and_then([](auto a) { return a * 2; }), 4)
    .labeled("and_then:valid")
  % unit::assertion::are_equal(optional<int>{}.or_(optional<int>{2}), 2)
    .labeled("or_:nullopt & valid")
  % unit::assertion::are_equal(optional<int>{2}.or_(optional<int>{}), 2)
    .labeled("or_:valid & nullopt")
  % unit::assertion::are_equal(optional<int>{2}.or_(optional<int>{2}), 2)
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
