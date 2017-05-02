#include <iostream>
#include "../../cranberries/stopwatch.hpp"
using std::cout;
using std::endl;
using cranberries::make_finally;


int main()
try{
  auto timer = cranberries::stopwatch{};
  timer.start();
  timer.get_ticks();
  timer.is_started();
  timer.lap();
  timer.stop();
  timer.clear();
}
catch (cranberries::runtime_error const& e) {
  cout << e.what() << endl;
  return 0;
}