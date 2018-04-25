#include <iostream>
#include <vector>
#include <locale> 
#include <list>
#include <sstream>
#include "../../cranberries/traversals/all.hpp"
#include "../../cranberries/functional/lambda.hpp"
#include "../../cranberries/traversals/comprehension/list_comprehension.hpp"
#include "../../cranberries/unit_test/unit_test.hpp"
using namespace cranberries::traversals;
namespace unit = cranberries::unit_test_framework;

int main()
try{

				unit::make_unit_test_container(unit::default_logger, "create tests")
					% unit::assertion::range_equal(
						create::generate([val = 1]()mutable->int{ return val *= 2; }) <= view::take(3)
						, { 2,4,8 })
						.labeled("create::generate test")
					% unit::assertion::range_match(
						create::generate_canonical() <= view::take(3)
						, [](auto d) { return 0.0 <= d && d < 1.0; })
						.labeled("create::generate_canonical test")
					% unit::assertion::range_equal(
						create::iterate(1, [](auto i) {return i * 2; }) <= view::take(4)
						, { 1,2,4,8 })
						.labeled("create::iterate test")	
					% unit::assertion::range_equal(
						create::of({ 1,2,4,8 })
						, { 1,2,4,8 })
						.labeled("create::of test")
					% unit::assertion::range_equal(
						create::irange(1,4)
						, { 1,2,3,4 })
						.labeled("create::range test")
					% unit::assertion::range_equal(
						create::repeat(1) <= view::take(4)
						, { 1,1,1,1 })
						.labeled("create::repeat test")
					| unit::collect;



				unit::make_unit_test_container(unit::default_logger, "views tests")
				//	% unit::assertion::range_match(
				//		create::irange(1, 10) <= view::chunk(2)
				//		, [span = cranberries::span(cranberries::make_array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10), 0, 2)]
				//		(auto&& elem) mutable
				//{ return cranberries::make_finally([&] { if (span.has_next_segment()) span.next_segment(2); }), span == elem; })
				//	.labeled("view::chunk test")
					% unit::assertion::range_equal(
						create::irange(1, 4) <= view::concat(create::irange(5, 9))
						, { 1,2,3,4,5,6,7,8,9 })
					.labeled("view::concat test")
					% unit::assertion::range_equal(
						create::irange(1, 3) <= view::cyclic() <= view::take(9)
						, { 1,2,3,1,2,3,1,2,3 })
					.labeled("view::cyclic test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,1,2,3,1,2,3 }) <= view::distinct
						, { 1,2,3 })
					.labeled("view::distinct test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,1,2,3,1,2,3 }) <= view::drop(5)
						, { 3,1,2,3 })
					.labeled("view::drop test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,4,5,6,7 }) <= view::drop_while([](auto i) {return i < 4; })
						, { 4,5,6,7 })
					.labeled("view::drop_while test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,4,5,6,7 }) <= view::filter([](auto i) { return i % 2 == 0; })
						, { 2,4,6 })
					.labeled("view::filter test")
					% unit::assertion::range_equal(
						create::irange(1, 5) <= view::indexed(1)
						, { { 1,1 }, { 2,2 },{ 3,3 },{ 4,4 },{ 5,5 } })
					.labeled("view::indexed test")
					% unit::assertion::range_equal(
						view::make_zipped(create::irange(1, 5), create::irange(1, 5))
						, { { 1,1 },{ 2,2 },{ 3,3 },{ 4,4 },{ 5,5 } })
					.labeled("view::make_zipped test")
					% unit::assertion::range_equal(
						create::repeat(std::string{ "foo" }) <= view::replace(std::string{ "foo" }, std::string{"bar"}) <= view::take(4)
						, { "bar","bar","bar","bar" })
						.labeled("view::replace test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,1,2,3,1,2,3 }) <= view::replace_if([](auto i) { return i % 2 == 0; }, 0)
						, { 1,0,3,1,0,3,1,0,3 })
						.labeled("view::replace_if test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,4,5 }) <= view::rev
						, { 5,4,3,2,1 })
						.labeled("view::reverse test")
					% unit::assertion::are_equal(
						create::of({ 1,2,3 }) <= view::shuffle <= collect::sum()
						, 6)
						.labeled("view::shuffle test")
					//% unit::assertion::range_equal(
					//	create::of({ 1,2,3,4,5 }) <= view::span(1,3)
					//	, { 2,3,4 })
					//	.labeled("view::span test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,4,5 }) <= view::take(4)
						, { 1,2,3,4 })
						.labeled("view::take test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,4,5 }) <= view::take_while([](auto i) { return i < 5; })
						, { 1,2,3,4 })
						.labeled("view::take_while test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,4,5 }) <= view::map([](auto i) { return i * 3; })
						, { 3,6,9,12,15 })
						.labeled("view::map test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,4,5 }) <= view::index_map([](auto i, auto item) { return i + item; })
						, { 1,3,5,7,9 })
						.labeled("view::index_map test")
					% unit::assertion::range_equal(
						create::of({ 1,2,3,4,5 }) <= view::zip_with(create::of({ 1,2,3,4,5 }))
						, { { 1,1 },{ 2,2 },{ 3,3 },{ 4,4 },{ 5,5 } })
						.labeled("view::zip_with test")
					| unit::collect;



					unit::make_unit_test_container(unit::default_logger, "operation tests")
						% unit::assertion::are_equal(
							create::generate([i = 0]()mutable{ return i += 2; }) <= view::take(100) <= collect::all_match([](auto i) {return i % 2 == 0; })
							, true)
							.labeled("operation::all_match test")
						% unit::assertion::are_equal(
							create::generate([i = 0]()mutable{ return i += 2; }) <= view::take(100) <= view::concat(create::of({ 1 })) <= collect::any_match([](auto i) {return i % 2 != 0; })
							, true)
							.labeled("operation::any_match test")
						% unit::assertion::range_equal(
							create::irange(1, 5) <= operation::convert<std::list>()
							, { 1,2,3,4,5 })
							.labeled("operation::convert test")
						% unit::assertion::range_equal(
							create::of({ create::irange(1, 5) <= operation::deconstruct<5>() })
							, { { 1,2,3,4,5 } })
							.labeled("operation::deconstruct test")
						% unit::assertion::are_equal(
							create::irange(1, 5) <= collect::none_match([](auto i) { return i < 6; })
							, true)
							.labeled("operation::none_match test")
						% unit::assertion::are_equal(
								[] {
									std::stringstream ss{};
									create::irange(1, 5) <= operation::peek([&](auto item) { ss << item; });
									return ss.str();
								}()
							, "12345")
							.labeled("operation::peek test")
						% unit::assertion::are_equal(
							create::of<std::string>({ "foo", "bar", "123" }) <= collect::regex_match(R"(\d+)")
							, true)
							.labeled("operation::regex_match test")
						% unit::assertion::are_equal(
							create::irange(1,100) <= collect::sum()
							, 5050)
							.labeled("operation::sum test")
						% unit::assertion::are_equal(
								[] {
									create::irange(1, 5) <= operation::write_line();
									return unit::test_status::passed;
								}()
							, unit::test_status::passed)
							.labeled("operation::write_line test")
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
