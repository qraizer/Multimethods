#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_RTTI;

/* ���� �������� ������ */
class B1;
class D11;
class D12;
class D13;
class D14;

/* ������ ����� ��������. ���� ����� ���� ��� ������ ����������. */
typedef MM::MakeTList<D13, D12, D11, B1, D14> Param1List;

/*******************************************************\
**  ������ ��� �����������                             **
\*******************************************************/

/* ��� ����� �������� ������ */
int rets[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

/* ���������� ������ */
class B1
{
public:
  virtual ~B1(){}
};

class D11 : public B1 {};
class D12 : public B1 {};
class D13 : public D11{};
class D14 : public D12{};

/* ���������� ���� ������������ � �� ���������� */
/* ������������ ���������� */
struct TestDispatch1
{
  static const int& apply(B1*  o)                     // �������
  {
    std::cout << "Single B1"  << '\t';
    return rets[0];
  }
  static const int& apply(D12* o)                     // �������������
  {
    std::cout << "Single D12" << '\t';
    return rets[1];
  }
  static const int& apply(D13* o)                     // ��� ���� �������������
  {
    std::cout << "Single D13" << '\t';
    return rets[2];
  }
};

/* ����������� ����������: ���������� ������ - ��� �������� */
struct TestDispatch11
{
  static const int& apply(std::string str)
  {
    std::cout << "Single - " << str << &str << '\t';
    str = "changing inside the multimethod";
    return rets[3];
  }
};
struct TestDispatch12
{
  static const int& apply(const std::string& str)
  {
    std::cout << "Single - " << str << &str << '\t';
    return rets[4];
  }
};
struct TestDispatch13
{
  static const int& apply(std::string& str)
  {
    std::cout << "Single - " << str << &str << '\t';
    str = "changing inside a multimethod";
    return rets[5];
  }
};

/* ������ ���������� */
MM::Dispatcher<TestDispatch1,  const int&, Param1List*> disp1;

MM::Dispatcher<TestDispatch11, const int&,       std::string>  disp11;
MM::Dispatcher<TestDispatch12, const int&, const std::string&> disp12;
MM::Dispatcher<TestDispatch13, const int&,       std::string&> disp13;

int main()
{
  B1  o;
  D11 o1;
  D12 o2;
  D13 o3;
  D14 o4;

  /* ���� �������� ���������� */
  std::cout << &disp1(&o ) - rets << '\n';
  std::cout << &disp1(&o1) - rets << '\n';
  std::cout << &disp1(&o2) - rets << '\n';
  std::cout << &disp1(&o3) - rets << '\n';
  std::cout << &disp1(&o4) - rets << '\n';

  std::cout << std::endl;

  /* ���� ������� ���������� */
  std::string s = "pointer to string object is ";

  std::cout << "Before the passing on value: " << s << static_cast<void*>(&s) << std::endl;
  std::cout << &disp11(s) - rets << '\n';
  std::cout << "Outside a multimethod:       " << s << static_cast<void*>(&s) << std::endl << std::endl ;

  std::cout << "Before the passing on const ref: " << s << static_cast<void*>(&s) << std::endl;
  std::cout << &disp12(s) - rets << '\n';
  std::cout << std::endl;

  std::cout << "Before the passing on non const ref: " << s << static_cast<void*>(&s) << std::endl;
  std::cout << &disp13(s) - rets << '\n';
  std::cout << "Outside a multimethod:               " << s << std::endl;
}
