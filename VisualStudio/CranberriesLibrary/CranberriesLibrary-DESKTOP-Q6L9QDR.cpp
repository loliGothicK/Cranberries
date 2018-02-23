#include <iostream>
#include <cranberries/unit_test/unit_test.hpp>
#include <cranberries/traversals/all.hpp>
#include <cranberries/func_util.hpp>
#include <initializer_list>
#include <chrono>
#include <algorithm>
#include <cranberries/span.hpp>


#include <cranberries/traversals/create/comprehension/list_comprehension.hpp>

int main() {

	{
		using namespace cranberries::experimental::traversals;
		using namespace cranberries::literals::placeholder_literals;

		auto even = [](auto i) { return i % 2 == 0; };

		( 1_ + 2_ | create::range(1, 3) & create::range(1, 4) & create::range(1, 5) )
			<= view::filter(even)
			<= operation::write_line();

		auto f = [](auto a, auto b) { return a + b; };
		auto g = [](auto a, auto b) { return a * b; };

		using cranberries::placeholders::_;	
		auto bind_expr = cranberries::bind(g, cranberries::bind(f, 1_, 2_), 2_);
		std::cout << bind_expr(11, 2) << std::endl; // 26

	}












	getchar();
}











//int main() {
//	using namespace cranberries::experimental::traversals;
//	using clock = std::chrono::high_resolution_clock;
//	using cranberries::make_finally;
//	using std::this_thread::sleep_for;
//	std::cout << std::boolalpha;
//
//	auto println = [](auto&& head, auto&&... tail) -> void {
//		std::cout << head;
//		(void)std::initializer_list<int>{
//			(void(std::cout << " " << tail), 0)...
//		};
//		std::cout << std::endl;
//	};
//	using namespace cranberries::func_util;
//
//	for (auto&& [a,b] : create::of({ 1,2,3,1,2,3,1,2,3 })
//										<= view::replace(1, 0)
//										<= view::zip_with(create::of({ 0,2,3,0,2,3,0,2,3 })))
//	{
//		std::cout << (a==b) << " ";
//	}
//
//	auto arr = std::array<int, 7>{{1, 2, 3, 4, 5, 6, 7}};
//
//	for (auto&& elem : cranberries::experimental::readonly_span(arr, 0, 2)) {
//		std::cout << elem << " ";
//	}
//	std::cout << std::endl;
//
//	{
//		int *pointer = arr.data();
//		for (auto&& elem : cranberries::experimental::readonly_span(pointer, 0, 2)) {
//			std::cout << elem << " ";
//		}
//		std::cout << std::endl;
//	}
//
//
//	for (auto&& chunk : arr <= view::chunk(2)) {
//		for (auto&& elem : chunk) std::cout << elem << " ";
//		std::cout << std::endl;
//	}
//
//	std::cout << std::boolalpha << bool{} << std::endl;
//
//	create::range(1, 32)
//		<= view::span(1, 3)
//		<= operation::write_line();
//
//	create::range(1, 10)
//		<= view::filter([](int i) { return i % 2 == 0; })
//		<= view::shuffle()
//		<= operation::deconstruct<4>()
//		| println;
//
//
//	auto p = [](std::string s) {std::cout << "\n" << s << "\n"; };
//
//	// Fibonacci numbers
//	p("[ iterate -> take ] // Fibonacci numbers");
//	create::iterate(1, [prev = 0](int v)mutable{
//		return make_finally([&] {prev = v; }), prev + v;
//	})
//		<= view::take(7)                 // [1,1,2,3,5,8,13]
//		<= operation::write_line();
//
//	p("[ generate -> transform -> take ] // tick");
//	create::generate([lap = clock::now()]()mutable->std::chrono::duration<double, std::milli>{
//		auto tmp = clock::now();
//		return make_finally([&] {lap = tmp; }), tmp - lap;
//	})
//		<= view::transform([](auto duration) { return std::to_string(duration.count()) + "[ms]"; })
//		<= view::take(7)
//		<= operation::regex_match(R"(\d+[ms])")
//		| println;
//
//	p("[ traversal ]");
//	create::range(1, 5)               // [1,5) = [1,2,3,4]
//		<= operation::write_line();
//
//	p("[ cyclic -> take ]");
//	view::cyclic(create::range(1, 5)) // [1,2,3,4,1,2,4,1,...]
//		<= view::take(5)                 // [1,2,3,4,1]
//		<= operation::write_line();
//
//	p("[ cyclic(traversal) -> reverse -> take -> reverse -> replace ]");
//	view::cyclic(create::range(1, 5)) // [1,2,3,4,1,2,4,1,...]
//		<= view::reverse()             // [4,3,2,1,4,3,2,1,...]
//		<= view::take(5)                 // [4,3,2,1,4]
//		<= view::reverse()             // [4,1,2,3,4]
//		<= view::replace(4, 0)
//		<= operation::write_line();
//
//	p("[ cyclic(traversal) -> take -> concat(traversal) -> distinct -> reverse ]");
//	view::cyclic(create::range(1, 5))    // [1,2,3,4,1,2,4,1,...]
//		<= view::take(7)                    // [1,2,3,4,1,2,3]
//		<= view::concat(create::range(1, 6)) // [1,2,3,4,1,2,3,1,2,3,4,5]
//		<= view::distinct()               // [1,2,3,4,5]
//		<= view::reverse()                // [5,4,3,2,1]
//		<= operation::write_line();
//
//	p("[ traversal -> zip_with ]");
//	create::range(1, 4)
//		<= view::zip_with(std::vector<std::string>{"one", "two", "three"})
//		<= operation::write_line();
//
//
//	p("[ traversal -> take_while -> drop ]");
//	create::range(1, 20)                            // [1,2,3,...,19]
//		<= view::take_while([](auto i) {return i < 8; }) // [1,2,3,4,5,6,7]
//		<= view::drop(4)                               // [5,6,7]
//		<= operation::write_line();
//
//	p("[ traversal -> take_while -> drop_while ]");
//	create::range(1, 20)                            // [1,2,3,...,19]
//		<= view::take_while([](auto i) {return i < 8; }) // [1,2,3,4,5,6,7]
//		<= view::drop_while([](auto i) {return i < 5; }) // [5,6,7]
//		<= operation::write_line();
//
//	p("[ traversal -> transform_with_index ]");
//	create::range(1, 6)
//		<= view::transform_with_index([](size_t i, auto e) { return e*i; })
//		<= operation::write_line();
//
//}