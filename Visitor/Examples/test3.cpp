#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_Visitor;

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

typedef MM::MakeTList<D13, D12, D11, B1, D14> Param1List;
typedef MM::MakeTList<B2,  D21, D22, D23>     Param2List;

class TestDispatch1;
class TestDispatch21;
class TestDispatch22;
class TestDispatch23;
class TestDispatch31;
class TestDispatch32;
class TestDispatch33;
class TestDispatch41;
class TestDispatch42;
class TestDispatch43;

typedef MM::Prototype<TestDispatch1,  int, Param2List*,        Param1List*,        Param2List*> Proto3;
typedef MM::Prototype<TestDispatch21, int, Param2List*,        std::string,        Param1List*> Proto311;
typedef MM::Prototype<TestDispatch22, int, Param2List*,        const std::string&, Param1List*> Proto312;
typedef MM::Prototype<TestDispatch23, int, Param2List*,        std::string&,       Param1List*> Proto313;
typedef MM::Prototype<TestDispatch31, int, std::string,  const std::string&,      std::string&> Proto321;
typedef MM::Prototype<TestDispatch32, int, const std::string&, std::string&,      std::string > Proto322;
typedef MM::Prototype<TestDispatch33, int, std::string&,       std::string, const std::string&> Proto323;
typedef MM::Prototype<TestDispatch41, int, std::string,        Param2List*,       std::string&> Proto331;
typedef MM::Prototype<TestDispatch42, int, const std::string&, Param2List*,       std::string&> Proto332;
typedef MM::Prototype<TestDispatch43, int, std::string&,       Param2List*,       std::string > Proto333;

/*******************************************************\
**  ������ ��� �����������                             **
\*******************************************************/

/* ���������� ������ */
/* ������ �������� */
class B1
{
public:
  MAKE_ACCEPTABLE(Proto3, Param1List, B1);
};

class D11 : public B1
{
public:
  MAKE_ACCEPTABLE(Proto3, Param1List, D11);
};

class D12 : public B1
{
public:
  MAKE_ACCEPTABLE(Proto3, Param1List, D12);
};

class D13 : public D11
{
public:
  MAKE_ACCEPTABLE(Proto3, Param1List, D13);
};

class D14 : public D12
{
public:
  MAKE_ACCEPTABLE(Proto3, Param1List, D14);
};

/* ������ �������� */
class B2
{
public:
  MAKE_ACCEPTABLE(Proto3, Param2List, B2);
};

class D21 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto3, Param2List, D21);
};

class D22 : public D21
{
public:
  MAKE_ACCEPTABLE(Proto3, Param2List, D22);
};

class D23 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto3, Param2List, D23);
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

MM::Dispatcher<Proto3>   disp3;
MM::Dispatcher<Proto311> disp311;
MM::Dispatcher<Proto312> disp312;
MM::Dispatcher<Proto313> disp313;
MM::Dispatcher<Proto321> disp321;
MM::Dispatcher<Proto322> disp322;
MM::Dispatcher<Proto323> disp323;
MM::Dispatcher<Proto331> disp331;
MM::Dispatcher<Proto332> disp332;
MM::Dispatcher<Proto333> disp333;

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
