#include <iostream>
#include "../../cranberries/stopwatch.hpp"

int main(){
  cranberries::stopwatch timer();
  timer.start();
  timer.get_ticks();
  timer.is_started();
  timer.lap();
  timer.stop();
  timer.clear();
}