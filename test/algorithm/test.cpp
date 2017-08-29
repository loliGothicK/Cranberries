#include "../../cranberries/algorithm.hpp"
#include <vector>
#include <array>
#include <cassert>

int main(){

{
  std::array<int, 6> v{ { 4,5,1,2,3,6 } };

  cranberries::descending_radix_sort(begin(v), end(v));
  assert((v == std::array<int, 6>{ { 6, 5, 4, 3, 2, 1 }}));

  cranberries::ascending_radix_sort(begin(v), end(v));
  assert((v == std::array<int, 6>{ { 1, 2, 3, 4, 5, 6 }}));
}
{
  std::array<int, 3> v{ { 1,2,3 } };
  size_t i{};
  std::array<std::array<int,3>, 6> expected{{
    {{ 1, 2, 3 }},
    {{ 1, 3, 2 }},
    {{ 2, 1, 3 }},
    {{ 2, 3 ,1 }},
    {{ 3, 1, 2 }},
    {{ 3, 2, 1 }}
  }};
  do
  {
    assert( (v == expected[i++]) );
  } while (cranberries::next_partial_permutation(begin(v), begin(v) + 2, end(v)));
}
{
  std::array<int, 3> v{ { 1,2,3 } };
  size_t i{};
  std::array<std::array<int,3>, 3> expected{{
    {{ 1, 2, 3 }},
    {{ 1, 3, 2 }},
    {{ 2, 3 ,1 }}
  }};
  do
  {
    assert( (v == expected[i++]) );
  } while (cranberries::next_combination(begin(v), begin(v) + 2, end(v)));
}

}