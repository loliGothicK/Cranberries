#include "../../cranberries/detection_toolkit.hpp"
#include <vector>


template < class T >
using has_value = decltype(T::value);

using namespace cranberries;

static_assert(
  is_satisfied_v<has_value, std::true_type>,
  "failure"
  );

static_assert(
  CRANBERRIES_HAS_TYPE(std::true_type, value_type),
  "failure"
  );
static_assert(
  CRANBERRIES_HAS_FIELD(std::true_type, value),
  "failure"
  );
static_assert(
  CRANBERRIES_HAS_NONTYPE(std::true_type, operator()),
  "failure"
  );
static_assert(
  is_satisfied_v<random_accsess_iter_require, typename std::vector<int>::iterator>,
  "failure"
  );
static_assert(
  is_satisfied_v<random_accsess_iter_require, int*>,
  "failure"
  );
static_assert(
  is_satisfied_v<arithmetic4_require, int>,
  "failure"
  );
static_assert(
  is_satisfied_v<equality_require, int>,
  "failure"
  );
static_assert(
  is_satisfied_v<equivalence_require, int>,
  "failure"
  );

int main(){}