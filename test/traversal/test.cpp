#include <iostream>
#include "../../cranberries/traversals/all.hpp"
#include "../../cranberries/functional/lambda.hpp"
#include "../../cranberries/traversals/comprehension/list_comprehension.hpp"

int main()
try{
		namespace create = cranberries::experimental::traversals::create;
		namespace view = cranberries::experimental::traversals::view;
		namespace operation = cranberries::experimental::traversals::operation;
		namespace collect = cranberries::experimental::traversals::collect;

		using cranberries::placeholders::_;
		using cranberries::literals::placeholder_literals::operator"" _;


		create::irange(1, 5)
			<= operation::write_line();

		create::semi_irange(1, 5)
			<= operation::write_line();

		create::generate([] { return 5; })
			<= view::take(5)
			<= operation::write_line();

		create::iterate( 1, [](auto i) { return i*5; } )
			<= view::take(5)
			<= operation::write_line();

		create::generate_canonical()
			<= view::take(5)
			<= operation::write_line();

		create::repeat(1)
			<= view::take(5)
			<= operation::write_line();

		create::random()
			<= view::take(5)
			<= operation::write_line();

		create::uniform_distribution(1, 5)
			<= view::take(5)
			<= operation::write_line();

		create::uniform_distribution(0.0, 1.0)
			<= view::take(5)
			<= operation::write_line();

    create::irange(1, 3)
      <= view::concat(create::irange(1, 3))
      <= operation::write_line();

    create::repeat(1)
      <= view::take(5)
      <= operation::write_line();

		create::irange(1, 3)
			<= view::cyclic()
			<= view::take(8)
			<= operation::write_line();


			create::irange(1, 4)
				<= view::map(_ * 2)
				<= operation::write_line();


		create::irange(1,5)
			<= view::index_map(1_*2_)
			<= operation::write_line();


		create::irange(1, 5)
			<= view::rev
			<= operation::write_line();


		create::irange(1, 4)
			<= view::zip_with(create::irange(1, 3))
			<= operation::write_line();


		create::irange(1, 10)
			<= view::take_while(_ < 5)
			<= operation::write_line();


		create::irange(1, 9)
			<= view::drop(5)
			<= operation::write_line();


		create::irange(1,10)
			<= view::drop_while(_ < 5)
			<= operation::write_line();


		create::irange(1,10)
			<= view::filter(_%2==0)
			<= operation::write_line();


		create::irange(1, 10)
			<= view::replace(10, 0)
			<= operation::write_line();

		create::irange(1, 10)
			<= view::replace_if(_%2==0, 0)
			<= operation::write_line();


		create::irange(1,5)
			<= view::concat(create::irange(3, 8))
			<= view::distinct
			<= operation::write_line();

		create::uniform_distribution(1, 9)
			<= view::take(10)
			<= view::sort()
			<= operation::write_line();

		create::irange(1, 9)
			<= view::shuffle
			<= operation::write_line();

		create::irange(1, 5)
			<= view::indexed()
			<= operation::write_line();

}
catch (cranberries::runtime_error const& e) {
  cout << e.what() << endl;
  return 0;
}