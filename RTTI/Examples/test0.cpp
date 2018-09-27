#include <iostream>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_RTTI;

/* ���������� ����������� �����������. ����� ���� ���������� ���-�� �������������. */
struct TestDispatch0
{
  static std::string apply()
  {
    return "Trivial";
  }
};

MM::Dispatcher<TestDispatch0, std::string> disp0;     // ���������

int main()
{
  std::cout << disp0() << std::endl;
}
