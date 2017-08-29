#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../cranberries/common/macros.hpp"
#include "../../cranberries/type_traits.hpp"
#include "../../cranberries/pack_operations.hpp"
#include "../../cranberries/meta_bind.hpp"
#include "../../cranberries/utility.hpp"
#include <vector>
#include <list>
#include <array>
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct F {
  void operator()( int, int ) { }
};

struct G {
  void operator()( int, int ) noexcept { }
};


namespace CommonLibTest
{		
	TEST_CLASS(CommonLibTest)
	{
	public:
		
		TEST_METHOD(CommonLibTestMethod)
		{
      using namespace cranberries;
      static_assert(
        std::is_same<cranberries::generate_tuple_t<int&, 3>, std::tuple<int&, int&, int&>>::value,
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
        is_callable<F( int, int ), void>::value,
        "fail"
        );
      static_assert(
        is_callable<F( int, int ), return_any>::value,
        "fail"
        );
      static_assert(
        is_nothrow_callable<G( int, int ), void>::value,
        "fail"
        );
      static_assert(
        is_nothrow_callable<G( int, int ), return_any>::value,
        "fail"
        );
      static_assert(
        !is_nothrow_callable<F( int, int ), return_any>::value,
        "fail"
        );
      static_assert(
        is_callable<F( int, double ), return_any>::value,
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
        is_range_v<std::array<int, 5>>,
        "fail"
        );
      static_assert(
        is_range_v<int[5]>,
        "fail"
        );
      static_assert(
        std::is_same<
          make_reversed_index_sequence<5>,
          std::integer_sequence<size_t,4,3,2,1,0>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_reverse_t<std::tuple<int,long>>,
        std::tuple<long,int>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_replace_all_t<std::tuple<int,int>,long,long>,
        std::tuple<long, long>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_replace_t<std::tuple<int, int>, int, long>,
        std::tuple<long, long>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_replace_if_t<std::tuple<int, int, std::string>, std::is_integral, double>,
        std::tuple<double, double,std::string>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        repack_t<std::vector<int>, std::list>,
        std::list<int>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
          pack_element_t<2,type_pack<int,int,long>>,
          long
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        arg_element_t<2,struct hoge(int,int,double,int)>,
        double
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_sliced_l<2, type_pack<int, int, double, int>>,
        type_pack<int,int>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_sliced_r<2, type_pack<int, int, double, int>>,
        type_pack<double,int>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        value_pack<int,0,1>,
        pack_sliced_l<2, value_pack<int, 0,1,2,3,4,5>>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_sliced_r<2, value_pack<int, 0,1,2,3,4,5>>,
        value_pack<int, 2,3,4,5>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_remove_t<int,std::tuple<int,long,int,double>>,
        std::tuple<long, double>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_remove_if_t<std::is_integral, std::tuple<int, long, int, double>>,
        std::tuple<double>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_insert_t<2,int,std::tuple<int, long, int, double>>,
        std::tuple<int, long, int, int, double>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_pop_back_t<std::tuple<int, long, int, double>>,
        std::tuple<int, long, int>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_erase_t<3,std::tuple<int, long, int, double>>,
        std::tuple<int, long, int>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_range_erase_t<1,2, std::tuple<int, long, int, double>>,
        std::tuple<int, double>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_range_erase_t<0, 2, std::tuple<int, long, int, double>>,
        std::tuple<int,double>
        >::value,
        "fail"
        );
      static_assert(
        std::is_same<
        pack_half_sliced_l<std::tuple<int, long, int, double>>,
        std::tuple<int,long>
        >::value,
        "fail"
        );
      static_assert(
        max_sizeof<int, long, int, double>::value == 8,
        "fail"
        );
      static_assert(
        std::is_same<
          element_type_of_t<int*>,
          int
        >::value,
        "fail"
        );

    }

	};
}