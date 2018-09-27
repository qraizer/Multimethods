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

class B2;
class D21;
class D22;
class D23;

/* ������ ����� ��������. ���� ����� ���� ��� ������ ����������. */
typedef TList<D13, TList<D12, TList<D11, TList<B1, TList<D14, NullType> > > > > Param1List;
typedef TList<B2,  TList<D21, TList<D22, TList<D23,           NullType> > > >   Param2List;

/* ��������� ������������: ��� ������ ���� � ������� �����������; ���������� int */
typedef MM::LinkDynamic<Param1List, int> Dyna1ParamType;
typedef MM::LinkDynamic<Param2List, int> Dyna2ParamType;
/* ��������� �������������: � ������ �����������, ���������� int */
typedef MM::LinkStatic<      std::string,  int> StatParam1Type; // �� ��������
typedef MM::LinkStatic<const std::string&, int> StatParam2Type; // �� ����������� ������
typedef MM::LinkStatic<      std::string&, int> StatParam3Type; // �� ������������� ������

/* "���������" ������������� */
typedef TList<Dyna2ParamType,                               /* ��� ������� ��������� ������ */
        TList<Dyna1ParamType,                               /* ���� � ������� �����         */
        TList<Dyna2ParamType, NullType> > > Dyna1Dyna2Dyna1;/* ��� � ������� �����������    */

typedef TList<StatParam1Type,                         /* �� ��������, �� ����������� */
        TList<StatParam2Type,                         /* � �� ������������� �������  */
        TList<StatParam3Type, NullType> > > Stat1Stat2Stat3;/* ��� � ������ �����������    */
typedef TList<StatParam2Type,                /* ���������� �� ������� ���������� */
        TList<StatParam3Type,       
        TList<StatParam1Type, NullType> > > Stat2Stat3Stat1;
typedef TList<StatParam3Type,                /* ��� ���� ����� */
        TList<StatParam1Type,       
        TList<StatParam2Type, NullType> > > Stat3Stat1Stat2;

typedef TList<Dyna2ParamType,                /* ��� ������� ��������� ������  */
        TList<StatParam1Type,                /* ����� � ������� ����������� � */
        TList<Dyna1ParamType, NullType> > > Dyna1Stat1Dyna2;/* ������� � ������ �� ��������  */
typedef TList<Dyna2ParamType,                /* �� �� �� ������� �� */
        TList<StatParam2Type,                /* ����������� ������  */
        TList<Dyna1ParamType, NullType> > > Dyna1Stat2Dyna2;
typedef TList<Dyna2ParamType,                /* �� �� �� � ������������� ������� */
        TList<StatParam3Type,       
        TList<Dyna1ParamType, NullType> > > Dyna1Stat3Dyna2;

typedef TList<StatParam1Type,                /* ��� ������� ��������� ������  */
        TList<Dyna2ParamType,                /* ����� � ������ ����������� �  */
        TList<StatParam2Type, NullType> > > Stat1Dyna2Stat2;/* ������� � ������� �� �������� */
typedef TList<StatParam2Type,                /* �� �� �� ������� ����� ������� */
        TList<Dyna2ParamType,                /* ����������                     */
        TList<StatParam3Type, NullType> > > Stat2Dyna2Stat3;
typedef TList<StatParam3Type,                /* ��� ���� ����� */
        TList<Dyna2ParamType,       
        TList<StatParam1Type, NullType> > > Stat3Dyna2Stat1;

/* ���������� ��������� ������������� */
struct TestDispatch1;
struct TestDispatch21;
struct TestDispatch22;
struct TestDispatch23;
struct TestDispatch31;
struct TestDispatch32;
struct TestDispatch33;
struct TestDispatch41;
struct TestDispatch42;
struct TestDispatch43;

MM::Dispatcher<TestDispatch1,  Dyna1Dyna2Dyna1, int> disp3;
MM::Dispatcher<TestDispatch21, Dyna1Stat1Dyna2, int> disp311;
MM::Dispatcher<TestDispatch22, Dyna1Stat2Dyna2, int> disp312;
MM::Dispatcher<TestDispatch23, Dyna1Stat3Dyna2, int> disp313;
MM::Dispatcher<TestDispatch31, Stat1Stat2Stat3, int> disp321;
MM::Dispatcher<TestDispatch32, Stat2Stat3Stat1, int> disp322;
MM::Dispatcher<TestDispatch33, Stat3Stat1Stat2, int> disp323;
MM::Dispatcher<TestDispatch41, Stat1Dyna2Stat2, int> disp331;
MM::Dispatcher<TestDispatch42, Stat2Dyna2Stat3, int> disp332;
MM::Dispatcher<TestDispatch43, Stat3Dyna2Stat1, int> disp333;

/*******************************************************\
**  ������ ��� �����������                             **
\*******************************************************/

/* ���������� ������ */
/* ������ �������� */
class B1
{
public:
  MAKE_ACCEPTABLE(int, Dyna1ParamType, B1);
  virtual ~B1(){}
};

class D11 : public B1
{
public:
  MAKE_ACCEPTABLE(int, Dyna1ParamType, D11);
};

class D12 : public B1
{
public:
  MAKE_ACCEPTABLE(int, Dyna1ParamType, D12);
};

class D13 : public D11
{
public:
  MAKE_ACCEPTABLE(int, Dyna1ParamType, D13);
};

class D14 : public D12
{
public:
  MAKE_ACCEPTABLE(int, Dyna1ParamType, D14);
};

/* ������ �������� */
class B2
{
public:
  MAKE_ACCEPTABLE(int, Dyna2ParamType, B2);
  virtual ~B2(){}
};

class D21 : public B2
{
public:
  MAKE_ACCEPTABLE(int, Dyna2ParamType, D21);
};

class D22 : public D21
{
public:
  MAKE_ACCEPTABLE(int, Dyna2ParamType, D22);
};

class D23 : public B2
{
public:
  MAKE_ACCEPTABLE(int, Dyna2ParamType, D23);
};


/* ���������� ���� ������������ � �� ���������� */
/* ������������ ���������� ���� ���������� */
struct TestDispatch1
{
  static int apply(B2*,  B1*,  B2*)
  {
    std::cout << "Triple B2 -B1 -B2  Dispatch: ";
    return 1;
  }
  static int apply(D21*, B1*,  B2*)
  {
    std::cout << "Triple D21-B1 -B2  Dispatch: ";
    return 2;
  }
  static int apply(D21*, D12*, D22*)
  {
    std::cout << "Triple D21-D12-D22 Dispatch: ";
    return 3;
  }
  static int apply(D22*, B1*,  D23*)
  {
    std::cout << "Triple D22-B1 -D23 Dispatch: ";
    return 4;
  }
  static int apply(D23*, B1* , D21*)
  {
    std::cout << "Triple D23-B1 -D21 Dispatch: ";
    return 5;
  }
  static int apply(D23*, D11*, D23*)
  {
    std::cout << "Triple D23-D11-D23 Dispatch: ";
    return 6;
  }
  static int apply(D23*, D13*, D22*)
  {
    std::cout << "Triple D23-D13-D22 Dispatch: ";
    return 7;
  }
  static int apply(D23*, D14*, D22*)
  {
    std::cout << "Triple D23-D14-D22 Dispatch: ";
    return 8;
  }
};
/* ����������� ���������� �������� ��������� �� �������� */
struct TestDispatch21
{
  static int apply(B2*,  std::string str,  B1*)
  {
    std::cout << "Triple B2 -value-B1  Dispatch: " << &str << " - ";
    str += '1';
    return 11;
  }
  static int apply(D21*, std::string str,  B1*)
  {
    std::cout << "Triple D21-value-B1  Dispatch: " << &str << " - ";
    str += '2';
    return 12;
  }
  static int apply(D22*, std::string str, D12*)
  {
    std::cout << "Triple D22-value-D12 Dispatch: " << &str << " - ";
    str += '3';
    return 13;
  }
  static int apply(D22*, std::string str,  B1*)
  {
    std::cout << "Triple D22-value-B1  Dispatch: " << &str << " - ";
    str += '4';
    return 14;
  }
  static int apply(D23*, std::string str,  B1*)
  {
    std::cout << "Triple D23-value-B1  Dispatch: " << &str << " - ";
    str += '5';
    return 15;
  }
  static int apply(D23*, std::string str, D11*)
  {
    std::cout << "Triple D23-value-D11 Dispatch: " << &str << " - ";
    str += '6';
    return 16;
  }
  static int apply(D23*, std::string str, D13*)
  {
    std::cout << "Triple D23-value-D13 Dispatch: " << &str << " - ";
    str += '7';
    return 17;
  }
  static int apply(D22*, std::string str, D14*)
  {
    std::cout << "Triple D22-value-D14 Dispatch: " << &str << " - ";
    str += '8';
    return 18;
  }
};
/* ����������� ���������� �������� ��������� �� ����������� ������ */
struct TestDispatch22
{
  static int apply(B2*,  const std::string& str,  B1*)
  {
    std::cout << "Triple B2 -const ref-B1  Dispatch: " << &str << " - ";
    return 21;
  }
  static int apply(D21*, const std::string& str,  B1*)
  {
    std::cout << "Triple D21-const ref-B1  Dispatch: " << &str << " - ";
    return 22;
  }
  static int apply(D22*, const std::string& str, D12*)
  {
    std::cout << "Triple D22-const ref-D12 Dispatch: " << &str << " - ";
    return 23;
  }
  static int apply(D22*, const std::string& str,  B1*)
  {
    std::cout << "Triple D22-const ref-B1  Dispatch: " << &str << " - ";
    return 24;
  }
  static int apply(D23*, const std::string& str,  B1*)
  {
    std::cout << "Triple D23-const ref-B1  Dispatch: " << &str << " - ";
    return 25;
  }
  static int apply(D23*, const std::string& str, D11*)
  {
    std::cout << "Triple D23-const ref-D11 Dispatch: " << &str << " - ";
    return 26;
  }
  static int apply(D23*, const std::string& str, D13*)
  {
    std::cout << "Triple D23-const ref-D13 Dispatch: " << &str << " - ";
    return 27;
  }
  static int apply(D22*, const std::string& str, D14*)
  {
    std::cout << "Triple D22-const ref-D14 Dispatch: " << &str << " - ";
    return 28;
  }
};
/* ����������� ���������� �������� ��������� �� �������������  ������ */
struct TestDispatch23
{
  static int apply(B2*,  std::string& str,  B1*)
  {
    std::cout << "Triple B2 -nonconst ref-B1  Dispatch: " << &str << " - ";
    str += '0';
    return 31;
  }
  static int apply(D21*, std::string& str,  B1*)
  {
    std::cout << "Triple D21-nonconst ref-B1  Dispatch: " << &str << " - ";
    str += '1';
    return 32;
  }
  static int apply(D22*, std::string& str, D12*)
  {
    std::cout << "Triple D22-nonconst ref-D12 Dispatch: " << &str << " - ";
    str += '2';
    return 33;
  }
  static int apply(D22*, std::string& str,  B1*)
  {
    std::cout << "Triple D22-nonconst ref-B1  Dispatch: " << &str << " - ";
    str += '3';
    return 34;
  }
  static int apply(D23*, std::string& str,  B1*)
  {
    std::cout << "Triple D23-nonconst ref-B1  Dispatch: " << &str << " - ";
    str += '4';
    return 35;
  }
  static int apply(D23*, std::string& str, D11*)
  {
    std::cout << "Triple D23-nonconst ref-D11 Dispatch: " << &str << " - ";
    str += '5';
    return 36;
  }
  static int apply(D23*, std::string& str, D13*)
  {
    std::cout << "Triple D23-nonconst ref-D13 Dispatch: " << &str << " - ";
    str += '6';
    return 37;
  }
  static int apply(D22*, std::string& str, D14*)
  {
    std::cout << "Triple D22-nonconst ref-D14 Dispatch: " << &str << " - ";
    str += '7';
    return 38;
  }
};

/* ����������� ���������� ���� ���������� */
struct TestDispatch31
{
  static int apply(std::string str1, const std::string& str2, std::string& str3)
  {
    std::cout << "Triple value-const ref-nonconst ref: " << &str1 << ':' << &str2 << ':' << &str3 << " - ";
    str1 += 'a';
    str3 += 'A';
    return 41;
  }
};
struct TestDispatch32
{
  static int apply(const std::string& str1, std::string& str2, std::string str3)
  {
    std::cout << "Triple const ref-nonconst ref-value: " << &str1 << ':' << &str2 << ':' << &str3 << " - ";
    str2 += 'a';
    str3 += 'A';
    return 42;
  }
};
struct TestDispatch33
{
  static int apply(std::string& str1, std::string str2, const std::string& str3)
  {
    std::cout << "Triple nonconst ref-value-const ref: " << &str1 << ':' << &str2 << ':' << &str3 << " - ";
    str1 += 'a';
    str2 += 'A';
    return 43;
  }
};

/* ����������� ���������� ������� ���������� � ������� �������� */
struct TestDispatch41
{
  static int apply(std::string str1, B2*,  const std::string& str2)
  {
    std::cout << "Triple value-B2 -const ref Dispatch: " << &str1 << ':' << &str2 << " - ";
    str1 += 'a';
    return 51;
  }
  static int apply(std::string str1, D21*, const std::string& str2)
  {
    std::cout << "Triple value-D21-const ref Dispatch: " << &str1 << ':' << &str2 << " - ";
    str1 += 'b';
    return 52;
  }
  static int apply(std::string str1, D23*, const std::string& str2)
  {
    std::cout << "Triple value-D23-const ref Dispatch: " << &str1 << ':' << &str2 << " - ";
    str1 += 'c';
    return 53;
  }
};

/* ����������� ���������� ������� ���������� � ������� �������� */
struct TestDispatch42
{
  static int apply(const std::string& str1, B2*,  std::string& str2)
  {
    std::cout << "Triple const ref-B2 -nonconst ref Dispatch: " << &str1 << ':' << &str2 << " - ";
    str2 += 'A';
    return 61;
  }
  static int apply(const std::string& str1, D21*, std::string& str2)
  {
    std::cout << "Triple const ref-D21-nonconst ref Dispatch: " << &str1 << ':' << &str2 << " - ";
    str2 += 'B';                     
    return 62;                       
  }                                  
  static int apply(const std::string& str1, D23*, std::string& str2)
  {                                  
    std::cout << "Triple const ref-D23-nonconst ref Dispatch: " << &str1 << ':' << &str2 << " - ";
    str2 += 'C';
    return 63;
  }
};

/* ����������� ���������� ������� ���������� � ������� �������� */
struct TestDispatch43
{
  static int apply(std::string& str1, B2*,  std::string str2)
  {
    std::cout << "Triple nonconst ref-B2 -value Dispatch: " << &str1 << ':' << &str2 << " - ";
    str1 += 'a';
    str2 += 'A';
    return 61;
  }
  static int apply(std::string& str1, D21*, std::string str2)
  {
    std::cout << "Triple nonconst ref-D21-value Dispatch: " << &str1 << ':' << &str2 << " - ";
    str1 += 'b';
    str2 += 'B';                     
    return 62;                       
  }                                  
  static int apply(std::string& str1, D23*, std::string str2)
  {                                  
    std::cout << "Triple nonconst ref-D23-value Dispatch: " << &str1 << ':' << &str2 << " - ";
    str1 += 'c';
    str2 += 'C';
    return 63;
  }
};

/* ���� ������������� ���������� � ������� ����������������. ������� ������� �������� ������������
   ���������� ����������, �.�. �� ����� ��� �������� ����� � � test2.cpp */
static void testDyna()
{
  B1  o1;
  D11 o11;
  D12 o12;
  D13 o13;
  D14 o14;

  B2  o2;
  D21 o21;
  D22 o22;
  D23 o23;

  std::cout << disp3(&o2 , &o1 , &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o1 , &o21) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o1 , &o22) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o1 , &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o2 , &o11, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o11, &o21) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o11, &o22) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o11, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o2 , &o12, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o12, &o21) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o12, &o22) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o12, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o2 , &o13, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o13, &o21) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o13, &o22) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o13, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o2 , &o14, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o14, &o21) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o14, &o22) << " returned" << std::endl;
  std::cout << disp3(&o2 , &o14, &o23) << " returned" << std::endl;

  std::cout << std::endl << std::endl;

  std::cout << disp3(&o21, &o1 , &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o21, &o1 , &o21) << " returned" << std::endl;
  std::cout << disp3(&o21, &o1 , &o22) << " returned" << std::endl;
  std::cout << disp3(&o21, &o1 , &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o21, &o11, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o21, &o11, &o21) << " returned" << std::endl;
  std::cout << disp3(&o21, &o11, &o22) << " returned" << std::endl;
  std::cout << disp3(&o21, &o11, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o21, &o12, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o21, &o12, &o21) << " returned" << std::endl;
  std::cout << disp3(&o21, &o12, &o22) << " returned" << std::endl;
  std::cout << disp3(&o21, &o12, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o21, &o13, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o21, &o13, &o21) << " returned" << std::endl;
  std::cout << disp3(&o21, &o13, &o22) << " returned" << std::endl;
  std::cout << disp3(&o21, &o13, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o21, &o14, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o21, &o14, &o21) << " returned" << std::endl;
  std::cout << disp3(&o21, &o14, &o22) << " returned" << std::endl;
  std::cout << disp3(&o21, &o14, &o23) << " returned" << std::endl;

  std::cout << std::endl << std::endl;

  std::cout << disp3(&o22, &o1 , &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o22, &o1 , &o21) << " returned" << std::endl;
  std::cout << disp3(&o22, &o1 , &o22) << " returned" << std::endl;
  std::cout << disp3(&o22, &o1 , &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o22, &o11, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o22, &o11, &o21) << " returned" << std::endl;
  std::cout << disp3(&o22, &o11, &o22) << " returned" << std::endl;
  std::cout << disp3(&o22, &o11, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o22, &o12, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o22, &o12, &o21) << " returned" << std::endl;
  std::cout << disp3(&o22, &o12, &o22) << " returned" << std::endl;
  std::cout << disp3(&o22, &o12, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o22, &o13, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o22, &o13, &o21) << " returned" << std::endl;
  std::cout << disp3(&o22, &o13, &o22) << " returned" << std::endl;
  std::cout << disp3(&o22, &o13, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o22, &o14, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o22, &o14, &o21) << " returned" << std::endl;
  std::cout << disp3(&o22, &o14, &o22) << " returned" << std::endl;
  std::cout << disp3(&o22, &o14, &o23) << " returned" << std::endl;

  std::cout << std::endl << std::endl;

  std::cout << disp3(&o23, &o1 , &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o23, &o1 , &o21) << " returned" << std::endl;
  std::cout << disp3(&o23, &o1 , &o22) << " returned" << std::endl;
  std::cout << disp3(&o23, &o1 , &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o23, &o11, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o23, &o11, &o21) << " returned" << std::endl;
  std::cout << disp3(&o23, &o11, &o22) << " returned" << std::endl;
  std::cout << disp3(&o23, &o11, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o23, &o12, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o23, &o12, &o21) << " returned" << std::endl;
  std::cout << disp3(&o23, &o12, &o22) << " returned" << std::endl;
  std::cout << disp3(&o23, &o12, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o23, &o13, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o23, &o13, &o21) << " returned" << std::endl;
  std::cout << disp3(&o23, &o13, &o22) << " returned" << std::endl;
  std::cout << disp3(&o23, &o13, &o23) << " returned" << std::endl;
  std::cout << std::endl;
  std::cout << disp3(&o23, &o14, &o2 ) << " returned" << std::endl;
  std::cout << disp3(&o23, &o14, &o21) << " returned" << std::endl;
  std::cout << disp3(&o23, &o14, &o22) << " returned" << std::endl;
  std::cout << disp3(&o23, &o14, &o23) << " returned" << std::endl;
  std::cout << std::endl << std::endl;
}

/* ���� ������������� ���������� � ������� ���������������� � ������������ ����������.
   ����������� ���������� ����������� ������ ����������, �.�. �� ����� ��� �������� � test2.cpp
   ����� ����������� ����� ���� � ������ ������ ���������� ���������� �� ������ �����������. */
static void testDynaStatDyna()
{
  B1  o1;
  D11 o11;
  D12 o12;
  D13 o13;
  D14 o14;

  B2  o2;
  D21 o21;
  D22 o22;
  D23 o23;

  std::string s(" - it's the string");

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp311(&o2 , s, &o1 ) << " returned" << std::endl;
  std::cout << disp311(&o2 , s, &o11) << " returned" << std::endl;
  std::cout << disp311(&o2 , s, &o12) << " returned" << std::endl;
  std::cout << disp311(&o2 , s, &o13) << " returned" << std::endl;
  std::cout << disp311(&o2 , s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp311(&o21, s, &o1 ) << " returned" << std::endl;
  std::cout << disp311(&o21, s, &o11) << " returned" << std::endl;
  std::cout << disp311(&o21, s, &o12) << " returned" << std::endl;
  std::cout << disp311(&o21, s, &o13) << " returned" << std::endl;
  std::cout << disp311(&o21, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp311(&o22, s, &o1 ) << " returned" << std::endl;
  std::cout << disp311(&o22, s, &o11) << " returned" << std::endl;
  std::cout << disp311(&o22, s, &o12) << " returned" << std::endl;
  std::cout << disp311(&o22, s, &o13) << " returned" << std::endl;
  std::cout << disp311(&o22, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp311(&o23, s, &o1 ) << " returned" << std::endl;
  std::cout << disp311(&o23, s, &o11) << " returned" << std::endl;
  std::cout << disp311(&o23, s, &o12) << " returned" << std::endl;
  std::cout << disp311(&o23, s, &o13) << " returned" << std::endl;
  std::cout << disp311(&o23, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;
  std::cout << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp312(&o2 , s, &o1 ) << " returned" << std::endl;
  std::cout << disp312(&o2 , s, &o11) << " returned" << std::endl;
  std::cout << disp312(&o2 , s, &o12) << " returned" << std::endl;
  std::cout << disp312(&o2 , s, &o13) << " returned" << std::endl;
  std::cout << disp312(&o2 , s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp312(&o21, s, &o1 ) << " returned" << std::endl;
  std::cout << disp312(&o21, s, &o11) << " returned" << std::endl;
  std::cout << disp312(&o21, s, &o12) << " returned" << std::endl;
  std::cout << disp312(&o21, s, &o13) << " returned" << std::endl;
  std::cout << disp312(&o21, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp312(&o22, s, &o1 ) << " returned" << std::endl;
  std::cout << disp312(&o22, s, &o11) << " returned" << std::endl;
  std::cout << disp312(&o22, s, &o12) << " returned" << std::endl;
  std::cout << disp312(&o22, s, &o13) << " returned" << std::endl;
  std::cout << disp312(&o22, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp312(&o23, s, &o1 ) << " returned" << std::endl;
  std::cout << disp312(&o23, s, &o11) << " returned" << std::endl;
  std::cout << disp312(&o23, s, &o12) << " returned" << std::endl;
  std::cout << disp312(&o23, s, &o13) << " returned" << std::endl;
  std::cout << disp312(&o23, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;
  std::cout << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp313(&o2 , s, &o1 ) << " returned" << std::endl;
  std::cout << disp313(&o2 , s, &o11) << " returned" << std::endl;
  std::cout << disp313(&o2 , s, &o12) << " returned" << std::endl;
  std::cout << disp313(&o2 , s, &o13) << " returned" << std::endl;
  std::cout << disp313(&o2 , s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp313(&o21, s, &o1 ) << " returned" << std::endl;
  std::cout << disp313(&o21, s, &o11) << " returned" << std::endl;
  std::cout << disp313(&o21, s, &o12) << " returned" << std::endl;
  std::cout << disp313(&o21, s, &o13) << " returned" << std::endl;
  std::cout << disp313(&o21, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp313(&o22, s, &o1 ) << " returned" << std::endl;
  std::cout << disp313(&o22, s, &o11) << " returned" << std::endl;
  std::cout << disp313(&o22, s, &o12) << " returned" << std::endl;
  std::cout << disp313(&o22, s, &o13) << " returned" << std::endl;
  std::cout << disp313(&o22, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << disp313(&o23, s, &o1 ) << " returned" << std::endl;
  std::cout << disp313(&o23, s, &o11) << " returned" << std::endl;
  std::cout << disp313(&o23, s, &o12) << " returned" << std::endl;
  std::cout << disp313(&o23, s, &o13) << " returned" << std::endl;
  std::cout << disp313(&o23, s, &o14) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s << std::endl;
}

/* ���� ������������� ���������� � ������� ���������������� � ������������ ����������.
   ������� ������� ����������� ����� ���� � ������ ������ ���������� ���������� �� ������ �����������. */
static void testStatDynaStat()
{
  B2  o2;
  D21 o21;
  D22 o22;
  D23 o23;

  std::string s1(" - it's the 1st string ");
  std::string s2(" - it's the 2nd string ");

  std::cout << "Before the passing into multimethod: " << s1 << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << &std::endl;
  std::cout << disp331(s1, &o2 , s2) << " returned" << std::endl;
  std::cout << disp331(s1, &o21, s2) << " returned" << std::endl;
  std::cout << disp331(s1, &o22, s2) << " returned" << std::endl;
  std::cout << disp331(s1, &o23, s2) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s1 << s2 << std::endl;

  s1 = " - it's the 1st string ";
  s2 = " - it's the 2nd string ";

  std::cout << "Before the passing into multimethod: " << s1 << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << &std::endl;
  std::cout << disp332(s1, &o2 , s2) << " returned" << std::endl;
  std::cout << disp332(s1, &o21, s2) << " returned" << std::endl;
  std::cout << disp332(s1, &o22, s2) << " returned" << std::endl;
  std::cout << disp332(s1, &o23, s2) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s1 << s2 << std::endl;

  s1 = " - it's the 1st string ";
  s2 = " - it's the 2nd string ";

  std::cout << "Before the passing into multimethod: " << s1 << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << &std::endl;
  std::cout << disp333(s1, &o2 , s2) << " returned" << std::endl;
  std::cout << disp333(s1, &o21, s2) << " returned" << std::endl;
  std::cout << disp333(s1, &o22, s2) << " returned" << std::endl;
  std::cout << disp333(s1, &o23, s2) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s1 << s2 << std::endl;
}

/* ���� ������������ ���������� � ������� ����������������. ���������� ������. */
static void testStat()
{
  std::string s1(" - it's the 1st string ");
  std::string s2(" - it's the 2nd string ");
  std::string s3(" - it's the 3rd string ");

  std::cout << "Before the passing into multimethod: " << s1 << s2 << s3 << std::endl;
  std::cout << &s1 << ':' << &s2 << ':' << &s3 << std::endl;
  std::cout << disp321(s1, s2, s3) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s1 << s2 << s3 << std::endl;

  s1 = " - it's the 1st string ";
  s2 = " - it's the 2nd string ";
  s3 = " - it's the 3rd string ";

  std::cout << "Before the passing into multimethod: " << s1 << s2 << s3 << std::endl;
  std::cout << &s1 << ':' << &s2 << ':' << &s3 << std::endl;
  std::cout << disp322(s1, s2, s3) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s1 << s2 << s3 << std::endl;

  s1 = " - it's the 1st string ";
  s2 = " - it's the 2nd string ";
  s3 = " - it's the 3rd string ";

  std::cout << "Before the passing into multimethod: " << s1 << s2 << s3 << std::endl;
  std::cout << &s1 << ':' << &s2 << ':' << &s3 << std::endl;
  std::cout << disp323(s1, s2, s3) << " returned" << std::endl;
  std::cout << "Outside a multimethod:       " << s1 << s2 << s3 << std::endl;
  std::cout << std::endl;
}

int main()
{
  testDyna();
  std::cout << std::endl;
  testStat();
  std::cout << std::endl;
  testDynaStatDyna();
  std::cout << std::endl;
  testStatDynaStat();
}
