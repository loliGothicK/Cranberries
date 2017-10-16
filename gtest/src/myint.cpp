#include "myint.hpp"

MyInt::MyInt(int num)
:num_(num)
{}

bool MyInt::isOdd()
{
  // 奇数かどうかを判定する
  return (num_ % 2) != 0 ? true : false;
}

bool MyInt::isEven()
{
  return isOdd();  // 例として、ここにバグがある
}