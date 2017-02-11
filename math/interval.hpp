#ifndef CRANBERRIES_INTERVAL_HPP
#define CRANBERRIES_INTERVAL_HPP

#include <cstdint>
#include <limits>
#include <cmath>
#include <typeinfo>
#include <type_traits>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <cfenv>

/*------------------------------------------------------------------------------------------------*/
//                    MACROs
/*------------------------------------------------------------------------------------------------*/

#define CRANBERRIES_ASSERT(expr) static_assert((expr), #expr)

#define CRANBERRIES_CONCEPT_ASSERT(expr) static_assert((expr), "Concepts not satisfied : "#expr)

/*------------------------------------------------------------------------------------------------*/

#include "./interval/detail/detail.hpp"
#include "./interval/rounding_control.hpp"
#include "./interval/exception.hpp"
#include "./interval/math_impl.hpp"
#include "./interval/interval.hpp"
#include "./interval/relational.hpp"
#include "./interval/compare.hpp"
#include "./interval/four_arithmetic_impl.hpp"
#include "./interval/four_arithmetic.hpp"
#include "./interval/algorithm.hpp"
#include "./interval/arithmetic.hpp"
#include "./interval/arithmetic2.hpp"
#include "./interval/utility.hpp"


#include "./interval/io.hpp"
#include "./interval/expression.hpp"
#include "./interval/version.hpp"


#undef ACCURACY_ASSURANCE
#undef ACCURACY_ASSURANCE_WITH_OVERFLOW_CHECK
#endif