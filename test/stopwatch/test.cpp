#include <iostream>
#include "../../cranberries/stopwatch.hpp"

int main(){
  cranberries::stopwatch timer(true);
  timer.get_ticks();
  timer.is_started();
  timer.lap();
  timer.stop();
  timer.clear();
}