#include <iostream>
#include "../cranberries/type_traits.hpp"
#include "../cranberries/utility.hpp"
#include <vector>
#include <array>

using std::cout;
using std::endl;


int main()
try{
  using cranberries;
      static_assert(
        std::is_same<cranberries::generate_tuple_t<int, 3>, std::tuple<int, int, int>>::value,
        "fail"
        );
      static_assert(
        conjunction_v< std::true_type, std::is_pod<int> >,
        "fail"
        );
      static_assert(
        disjunction_v< std::true_type, std::false_type >,
        "fail"
        );
      static_assert(
        negation_v< std::false_type >,
        "fail"
        );
      static_assert(
        apply_result_v< std::is_same, int, int >,
        "fail"
        );
      static_assert(
        apply_result_v< std::is_same, int, int >,
        "fail"
        );
      static_assert(
        apply_decay_result_v< std::is_same, int&, int&& >,
        "fail"
        );
      static_assert(
        apply_remove_cvr_result_v< std::is_same, int const&, int >,
        "fail"
        );
      static_assert(
        apply_result_v< bind_<std::is_same, int, x_>::expr, int >,
        "fail"
        );
      static_assert(
        apply_result_v< bind_1st<is_equality_comparable>::expr, int >,
        "fail"
        );
      static_assert(
        apply_result_v< bind_1st<std::is_same, int>::expr, int >,
        "fail"
        );
      static_assert(
        apply_result_v< bind_2nd<std::is_same, int>::expr, int >,
        "fail"
        );
      static_assert(
        all_match_v< int, int, int, int >,
        "fail"
        );
      static_assert(
        any_match_v< int, int, double, float >,
        "fail"
        );
      static_assert(
        none_match_v< int, float, unsigned, double >,
        "fail"
        );
      static_assert(
        all_match_if_v< std::is_signed, float, int, double >,
        "fail"
        );
      static_assert(
        any_match_if_v< std::is_signed, float, int, unsigned >,
        "fail"
        );
      static_assert(
        none_match_if_v< std::is_unsigned, float, int, double >,
        "fail"
        );
      static_assert(
        is_callable<F(int,int),void>::value,
        "fail"
        );
      static_assert(
        is_callable<F(int, int), return_any>::value,
        "fail"
        );
      static_assert(
        is_nothrow_callable<G(int, int), void>::value,
        "fail"
        );
      static_assert(
        is_nothrow_callable<G(int, int), return_any>::value,
        "fail"
        );
      static_assert(
        !is_nothrow_callable<F(int, int), return_any>::value,
        "fail"
        );
      static_assert(
        is_callable<F(int, double), return_any>::value,
        "fail"
        );
      static_assert(
        is_iterator_v<typename std::vector<int>::iterator>,
        "fail"
        );
      static_assert(
        is_range_v<std::vector<int>>,
        "fail"
        );
      static_assert(
        is_range_v<std::array<int,5>>,
        "fail"
        );
      static_assert(
        is_range_v<int[5]>,
        "fail"
        );
      static_assert(
        apply_decay_result_v<bind_1st<is_equality_comparable_to,int>::expr,double&>,
        "fail"
        );

}
catch (cranberries::runtime_error const& e) {
  cout << e.what() << endl;
  return 0;
}