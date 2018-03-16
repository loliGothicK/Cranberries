/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_INTERVAL_LIB_COMPARE_ODER_SYMBOL_HPP
#define CRANBERRIES_INTERVAL_LIB_COMPARE_ODER_SYMBOL_HPP

namespace cranberries {
namespace compare {

//---------------------//
/*   Ordering Symbol   */
//---------------------//

enum class OrderPolocy { Weak, Partial, Interval, Set };

/*  3 Values of Ordering  */
enum class partial_ordering { less, unordered, greater };
enum class weak_ordering { less, equivalent, greater };
enum class total_ordering { less, equal, greater };
enum class interval_ordering { less, unordered, greater };

}  // namespace compare
}  // namespace cranberries

#endif