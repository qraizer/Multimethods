#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_03;

using MM::TList;
using MM::NullType;

/* ���� �������� ������ */
class B1;
class D11;
class D12;
class D13;
class D14;

/* ������ ����� ��������. ���� ����� ���� ��� ������ ����������. */
typedef TList<D13, TList<D12, TList<D11, TList<B1, TList<D14, NullType> > > > > Param1List;

/* ��������� ������������: ������������ � ������� �����������; ���������� void */
typedef MM::LinkDynamic<Param1List, const int&> DynaParam1Type;
/* "��������" ������������ - ���� �������� */
typedef TList<DynaParam1Type, NullType> Dyna1Simple;

/* ��������� �������������: � ������ �����������, ���������� void */
typedef MM::LinkStatic<      std::string,  const int&> StatParam1Type;        // �� ��������
typedef MM::LinkStatic<const std::string&, const int&> StatParam2Type;        // �� ����������� ������
typedef MM::LinkStatic<      std::string&, const int&> StatParam3Type;        // �� ������������� ������
/* "���������" ������������� - �� ������ ��������� */
typedef TList<StatParam1Type, NullType> Stat1Simple;
typedef TList<StatParam2Type, NullType> Stat2Simple;
typedef TList<StatParam3Type, NullType> Stat3Simple;

/* ��������� ��������� � �������������� � �� ������������ */
struct TestDispatch1;

struct TestDispatch11;
struct TestDispatch12;
struct TestDispatch13;

/* ������ ���������� */
MM::Dispatcher<TestDispatch1,  Dyna1Simple, const int&> disp1;

MM::Dispatcher<TestDispatch11, Stat1Simple, const int&> disp11;
MM::Dispatcher<TestDispatch12, Stat2Simple, const int&> disp12;
MM::Dispatcher<TestDispatch13, Stat3Simple, const int&> disp13;

/*******************************************************\
**  ������ ��� �����������                             **
\*******************************************************/

/* ��� ����� �������� ������ */
int rets[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

/* ���������� ������ */
class B1
{
public:
  MAKE_ACCEPTABLE(const int&, DynaParam1Type, B1);
  virtual ~B1(){}
};

class D11 : public B1
{
public:
  MAKE_ACCEPTABLE(const int&, DynaParam1Type, D11);
};

class D12 : public B1
{
public:
  MAKE_ACCEPTABLE(const int&, DynaParam1Type, D12);
};

class D13 : public D11
{
public:
  MAKE_ACCEPTABLE(const int&, DynaParam1Type, D13);
};

class D14 : public D12
{
public:
  MAKE_ACCEPTABLE(const int&, DynaParam1Type, D14);
};

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
