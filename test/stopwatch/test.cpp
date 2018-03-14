#include <iostream>
#include "../../cranberries/stopwatch.hpp"
#include <thread>
int main(){
  using namespace std::chrono_literals;
  // auto start
  auto timer = cranberries::stopwatch<>{ cranberries::timer::config::auto_ };

  // lap
  std::this_thread::sleep_for(1ms);
  timer.lap();
  std::this_thread::sleep_for(2ms);
  timer.lap();
  std::this_thread::sleep_for(3ms);
  timer.lap();

  // time elapsed
  timer
    .elapsed([]{
      std::this_thread::sleep_for(1ms); 
    })
    .elapsed([]{
      std::this_thread::sleep_for(2ms);
    })
    .elapsed([]{
      std::this_thread::sleep_for(3ms);
    })
    .stop();
}