#include <iostream>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_Visitor;

/* Определяем тривиальный мультиметод. Пусть хоть возвращает что-то нетривиальное. */
struct TestDispatch0
{
  static std::string apply()
  {
    return "Trivial";
  }
};

MM::Dispatcher<MM::Prototype<TestDispatch0, std::string>> disp0;     // Диспетчер

int main()
{
  std::cout << disp0() << std::endl;
}
