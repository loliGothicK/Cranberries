#include <iostream>
#include "../../cranberries/dual_number.hpp"
#include "../../cranberries/interval.hpp"

int main()
try
{
  using cranberries::dual_number;
  using cranberries::make_dual;
  using cranberries::interval;
  using namespace cranberries::literals;
  using std::cout;
  using std::endl;

  auto x = make_dual(1.0, 1.0);
  auto y = make_dual(0.5,1.0);
  auto z = make_dual(-1.0,1.0);

  cout << dual_number<>{} << endl;
  cout << make_dual(1.0) << endl;
  cout << make_dual(2.0, 1.0) << endl;
  cout << 2.0_dual << endl;
  
  auto a = dual_number<interval<>>{ interval<>{1,3}, interval<>{1} };
  cout << sin(a) << endl;

  auto v1 = x;
  cout << (v1 += 1) << endl;
  cout << (v1 += v1) << endl;
  cout << x + x << endl;
  cout << x + 1 << endl;
  cout << 1 + x << endl;

  auto v2 = x;
  cout << (v2 -= 1) << endl;
  cout << (v2 -= v2) << endl;
  cout << x - x << endl;
  cout << x - 1 << endl;
  cout << 1 - x << endl;

  auto v3 = x;
  cout << (v3 *= 1) << endl;
  cout << (v3 *= v3) << endl;
  cout << x * x << endl;
  cout << 1 * x << endl;
  cout << x * 1 << endl;

  auto v4 = x;
  cout << (v4 /= 1) << endl;
  cout << (v4 /= v4) << endl;
  cout << x / x << endl;
  cout << 1 / x << endl;
  cout << x / 1 << endl;

  cout << sin(x) << endl;
  cout << cos(x) << endl;
  cout << tan(x) << endl;
  cout << sec(x) << endl;
  cout << csc(x) << endl;
  cout << cot(x) << endl;
  cout << asin(y) << endl;
  cout << acos(y) << endl;
  cout << atan(y) << endl;
  cout << sinh(x) << endl;
  cout << cosh(x) << endl;
  cout << tanh(x) << endl;
  cout << sech(x) << endl;
  cout << csch(x) << endl;
  cout << coth(x) << endl;
  cout << asinh(x) << endl;
  cout << acosh(make_dual(1.2,1.0)) << endl;
  cout << atanh(y) << endl;
  cout << asech(y) << endl;
  cout << acsch(make_dual(-2.0,1.0)) << endl;
  cout << acoth(make_dual(-2.0, 1.0)) << endl;
  cout << abs(z) << endl;
  cout << log(x) << endl;
  cout << exp(x) << endl;
  cout << pow(x,2) << endl;
  cout << sqrt(y) << endl;
  cout << cbrt(y) << endl;
}
catch (const std::exception& e)
{
  std::cout << e.what() << std::endl;
  return 1;
}