#include <iostream>
#include "../cranberries/type_traits.hpp"
#include "../cranberries/utility.hpp"

using std::cout;
using std::endl;


int main()
try{
  static_assert(
    std::is_same<cranberries::generate_tuple_t<int,3>,std::tuple<int,int,int>>::value,
    "fail"
  );
}
catch (cranberries::runtime_error const& e) {
  cout << e.what() << endl;
  return 0;
}