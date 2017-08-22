/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

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

#if !defined(CRANBERRIES_ASSERT)
#define CRANBERRIES_ASSERT(expr) static_assert((expr), #expr)
#endif

#define CRANBERRIES_CONCEPT_ASSERT(expr) static_assert((expr), "Concepts not satisfied : "#expr)

/*------------------------------------------------------------------------------------------------*/

#include "../cranberries/common/exception.hpp"
#include "./interval/cranberries_magic/detail.hpp"
#include "./interval/rounding_control.hpp"
#include "./interval/math_impl.hpp"
#include "./interval/interval.hpp"
#include "./interval/compare.hpp"
#include "./interval/relational.hpp"
#include "./interval/four_arithmetic.hpp"
#include "./interval/algorithm.hpp"
#include "./interval/arithmetic.hpp"
#include "./interval/arithmetic2.hpp"
#include "./interval/utility.hpp"


#include "./interval/io.hpp"
#include "./interval/version.hpp"

#undef CRANBERRIES_MAKE_INTERVAL
#undef UPWARD_PLICY
#undef DOWNWARD_PLICY
#endif