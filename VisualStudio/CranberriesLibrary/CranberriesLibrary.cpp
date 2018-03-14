#include <iostream>
#include <cranberries/func_util.hpp>
#include <cranberries/span.hpp>
#include <list>
#include <cranberries/common/macros.hpp>
// cranberries::traversal
#include <cranberries/traversals/all.hpp>
#include <cranberries/functional/lambda.hpp>
#include <cranberries/traversals/comprehension/list_comprehension.hpp>
#include <cranberries/stopwatch.hpp>

int main() {
	{
		using std::literals::chrono_literals::operator""ms;
		cranberries::stopwatch<> timer{ cranberries::timer::config::auto_ };

		std::this_thread::sleep_for(1ms);
		timer.lap();
		std::this_thread::sleep_for(2ms);
		timer.lap();
		std::this_thread::sleep_for(3ms);
		timer.lap();

		timer
			.elapsed([]{ 
				std::this_thread::sleep_for(1ms);
			})
			.elapsed([]{
				std::this_thread::sleep_for(2ms);
			})
			.elapsed([]{
				std::this_thread::sleep_for(3ms);
			})
			.stop();
	}











//	{
//		namespace create = cranberries::experimental::traversals::create;
//		namespace view = cranberries::experimental::traversals::view;
//		namespace operation = cranberries::experimental::traversals::operation;
//		namespace collect = cranberries::experimental::traversals::collect;
//
//		using cranberries::placeholders::_;
//		using cranberries::literals::placeholder_literals::operator"" _;
//
//		std::cout << "lambda tests\n";
//
//		(1_ + 2_ | create::irange(1, 5) <= view::filter(_%2==0) & create::irange(1, 5))
//			<= operation::write_line();
//
//		getchar();
//		return 0;
//
//#pragma region CREATE_MODULE
//		create::irange(1, 5)
//			<= operation::write_line();
//
//		create::semi_irange(1, 5)
//			<= operation::write_line();
//
//		create::generate([] { return 5; })
//			<= view::take(5)
//			<= operation::write_line();
//
//		create::iterate( 1, [](auto i) { return i*5; } )
//			<= view::take(5)
//			<= operation::write_line();
//
//		create::generate_canonical()
//			<= view::take(5)
//			<= operation::write_line();
//
//		create::repeat(1)
//			<= view::take(5)
//			<= operation::write_line();
//
//		create::random()
//			<= view::take(5)
//			<= operation::write_line();
//
//		create::uniform_distribution(1, 5)
//			<= view::take(5)
//			<= operation::write_line();
//
//		create::uniform_distribution(0.0, 1.0)
//			<= view::take(5)
//			<= operation::write_line();
//
//#pragma endregion
//
//#pragma region VIEW_MODULE
//#pragma region view::chunk
//
//		//{
//		//	auto chunked
//		//		= create::irange(1, 5)
//		//		<= view::chunk(2);
//
//		//	for (auto&& span : chunked) {
//		//		for (auto&& elem : span)	std::cout << elem << " ";
//		//		std::cout << "\n";
//		//	}
//		//}
//#pragma endregion
//#pragma region view::concat
//		{
//			create::irange(1, 3)
//				<= view::concat(create::irange(1, 3))
//				<= operation::write_line();
//		}
//#pragma endregion
//#pragma region view::take
//		{
//			create::repeat(1)
//				<= view::take(5)
//				<= operation::write_line();
//		}
//#pragma endregion
//#pragma region view::cyclic
//		create::irange(1, 3)
//			<= view::cyclic()
//			<= view::take(8)
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::map
//			create::irange(1, 4)
//				<= view::map(_ * 2)
//				<= operation::write_line();
//#pragma endregion
//#pragma region view::index_map
//		create::irange(1,5)
//			<= view::index_map(1_*2_)
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::rev
//		create::irange(1, 5)
//			<= view::rev
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::zip_with
//		create::irange(1, 4)
//			<= view::zip_with(create::irange(1, 3))
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::take_while
//		create::irange(1, 10)
//			<= view::take_while(_ < 5)
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::drop
//		create::irange(1, 9)
//			<= view::drop(5)
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::drop_while
//		create::irange(1,10)
//			<= view::drop_while(_ < 5)
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::filter
//		create::irange(1,10)
//			<= view::filter(_%2==0)
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::replace
//		create::irange(1, 10)
//			<= view::replace(10, 0)
//			<= operation::write_line();
//
//		create::irange(1, 10)
//			<= view::replace_if(_%2==0, 0)
//			<= operation::write_line();
//#pragma endregion
//#pragma region view::distinct
//		create::irange(1,5)
//			<= view::concat(create::irange(3, 8))
//			<= view::distinct
//			<= operation::write_line();
//#pragma endregion
//		create::uniform_distribution(1, 9)
//			<= view::take(10)
//			<= view::sort()
//			<= operation::write_line();
//
//		create::irange(1, 9)
//			<= view::shuffle
//			<= operation::write_line();
//
//		create::irange(1, 5)
//			<= view::indexed()
//			<= operation::write_line();
//
//#pragma endregion
//
//
//
//	}
//



	getchar();
}
