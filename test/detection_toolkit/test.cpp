#include "../../cranberries/common/concepts.hpp"
#include <vector>


template < class T >
using has_value = decltype(T::value);

using namespace cranberries;

static_assert(
  is_satisfied_v<has_value, std::true_type>,
  "failure"
  );
static_assert(
  concepts::required_v<std::vector<int>, concepts::iterable>,
  "failure"
  );

int main(){}