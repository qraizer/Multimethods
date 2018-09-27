#include <iostream>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_Visitor;

/* ���������� ����������� �����������. ����� ���� ���������� ���-�� �������������. */
struct TestDispatch0
{
  static std::string apply()
  {
    return "Trivial";
  }
};

MM::Dispatcher<MM::Prototype<TestDispatch0, std::string>> disp0;     // ���������

int main()
{
  std::cout << disp0() << std::endl;
}
