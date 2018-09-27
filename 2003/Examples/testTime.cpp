#include <fstream>
#include <chrono>
#
#include <stdexcept>
#include <iostream>
#include "mmethod.h"

namespace MM = MultiMethods_03;

using MM::NullType;
using MM::TList;

class B1;
class D11;
class D12;
class D13;
class D14;

class B2;
class D21;
class D22;
class D23;

typedef TList<D13, TList<D12, TList<D11, TList<B1, TList<D14, NullType> > > > > Param1List;
typedef TList<B2,  TList<D21, TList<D22, TList<D23,           NullType> > > >   Param2List;

typedef MM::LinkDynamic<Param1List, int> Dyna1ParamType;
typedef MM::LinkDynamic<Param2List, int> Dyna2ParamType;

typedef MM::LinkStatic<std::chrono::high_resolution_clock::time_point&, int> StatParam1Type; // по ссылке
typedef MM::LinkStatic<std::ostream&,                                   int> StatParam2Type; // по ссылке

typedef TList<Dyna2ParamType,                                   /* два крайних параметра одного */
        TList<StatParam1Type,                                   /* типа и средний иного         */
        TList<Dyna1ParamType,                                   /* типа и средний иного         */
        TList<Dyna2ParamType,
        TList<StatParam1Type,                                   /* типа и средний иного         */
        TList<StatParam2Type, NullType>>>>>> Dyna1Dyna2Dyna1;   /* все с поздним связыванием    */

struct TestDispatch1;

MM::Dispatcher<TestDispatch1,  Dyna1Dyna2Dyna1, int> disp3;

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

/* Вторая иерархия */
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


template <typename Ch, typename Tr>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& ostream,
                                       std::chrono::high_resolution_clock::duration time)
{
  return ostream << std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
}

struct TestDispatch1
{
  static int apply(B2*,  std::chrono::high_resolution_clock::time_point& started, B1*,
                   B2*,  std::chrono::high_resolution_clock::time_point& start,   std::ostream& out)
  {
    auto diff = std::chrono::high_resolution_clock::now() - started;

    out << "Triple B2 -B1 -B2  Dispatch: " << diff << '\n';
    start = std::chrono::high_resolution_clock::now();

    return 1;
  }
  static int apply(D21*, std::chrono::high_resolution_clock::time_point& started, B1*,
                   B2*,  std::chrono::high_resolution_clock::time_point& start,   std::ostream& out)
  {
    auto diff = std::chrono::high_resolution_clock::now() - started;

    out << "Triple D21-B1 -B2  Dispatch: " << diff << '\n';
    start = std::chrono::high_resolution_clock::now();

    return 2;
  }
  static int apply(D21*, std::chrono::high_resolution_clock::time_point& started, D12*,
                   D22*, std::chrono::high_resolution_clock::time_point& start,   std::ostream& out)
  {
    auto diff = std::chrono::high_resolution_clock::now() - started;

    out << "Triple D21-D12-D22 Dispatch: " << diff << '\n';
    start = std::chrono::high_resolution_clock::now();

    return 3;
  }
  static int apply(D22*, std::chrono::high_resolution_clock::time_point& started, B1*,
                   D23*, std::chrono::high_resolution_clock::time_point& start,   std::ostream& out)
  {
    auto diff = std::chrono::high_resolution_clock::now() - started;

    out << "Triple D22-B1 -D23 Dispatch: " << diff << '\n';
    start = std::chrono::high_resolution_clock::now();

    return 4;
  }
  static int apply(D23*, std::chrono::high_resolution_clock::time_point& started, B1* ,
                   D21*, std::chrono::high_resolution_clock::time_point& start,   std::ostream& out)
  {
    auto diff = std::chrono::high_resolution_clock::now() - started;

    out << "Triple D23-B1 -D21 Dispatch: " << diff << '\n';
    start = std::chrono::high_resolution_clock::now();

    return 5;
  }
  static int apply(D23*, std::chrono::high_resolution_clock::time_point& started, D11*,
                   D23*, std::chrono::high_resolution_clock::time_point& start,   std::ostream& out)
  {
    auto diff = std::chrono::high_resolution_clock::now() - started;

    out << "Triple D23-D11-D23 Dispatch: " << diff << '\n';
    start = std::chrono::high_resolution_clock::now();

    return 6;
  }
  static int apply(D23*, std::chrono::high_resolution_clock::time_point& started, D13*,
                   D22*, std::chrono::high_resolution_clock::time_point& start,   std::ostream& out)
  {
    auto diff = std::chrono::high_resolution_clock::now() - started;

    out << "Triple D23-D13-D22 Dispatch: " << diff << '\n';
    start = std::chrono::high_resolution_clock::now();

    return 7;
  }
  static int apply(D23*, std::chrono::high_resolution_clock::time_point& started, D14*,
                   D22*, std::chrono::high_resolution_clock::time_point& start,   std::ostream& out)
  {
    auto diff = std::chrono::high_resolution_clock::now() - started;

    out << "Triple D23-D14-D22 Dispatch: " << diff << '\n';
    start = std::chrono::high_resolution_clock::now();

    return 8;
  }
};

int foo(B2* d1, B1* d2, B2* d3, std::ostream& file)
{
  auto            start = std::chrono::high_resolution_clock::now();
  decltype(start) end;
  auto            ret   = disp3(d1, start, d2, d3, end, file);
  auto            diff  = std::chrono::high_resolution_clock::now() - end;

  file << ret << " returned: " << diff << std::endl;
  return ret;
}

static void testDyna()
{
  std::ofstream outFile("res.txt");

  B1  o1;
  D11 o11;
  D12 o12;
  D13 o13;
  D14 o14;

  B2  o2;
  D21 o21;
  D22 o22;
  D23 o23;

  foo(&o2 , &o1 , &o2 , outFile);
  foo(&o2 , &o1 , &o21, outFile);
  foo(&o2 , &o1 , &o22, outFile);
  foo(&o2 , &o1 , &o23, outFile);
  outFile << std::endl;
  foo(&o2 , &o11, &o2 , outFile);
  foo(&o2 , &o11, &o21, outFile);
  foo(&o2 , &o11, &o22, outFile);
  foo(&o2 , &o11, &o23, outFile);
  outFile << std::endl;
  foo(&o2 , &o12, &o2 , outFile);
  foo(&o2 , &o12, &o21, outFile);
  foo(&o2 , &o12, &o22, outFile);
  foo(&o2 , &o12, &o23, outFile);
  outFile << std::endl;
  foo(&o2 , &o13, &o2 , outFile);
  foo(&o2 , &o13, &o21, outFile);
  foo(&o2 , &o13, &o22, outFile);
  foo(&o2 , &o13, &o23, outFile);
  outFile << std::endl;
  foo(&o2 , &o14, &o2 , outFile);
  foo(&o2 , &o14, &o21, outFile);
  foo(&o2 , &o14, &o22, outFile);
  foo(&o2 , &o14, &o23, outFile);

  outFile << std::endl << std::endl;

  foo(&o21, &o1 , &o2 , outFile);
  foo(&o21, &o1 , &o21, outFile);
  foo(&o21, &o1 , &o22, outFile);
  foo(&o21, &o1 , &o23, outFile);
  outFile << std::endl;
  foo(&o21, &o11, &o2 , outFile);
  foo(&o21, &o11, &o21, outFile);
  foo(&o21, &o11, &o22, outFile);
  foo(&o21, &o11, &o23, outFile);
  outFile << std::endl;
  foo(&o21, &o12, &o2 , outFile);
  foo(&o21, &o12, &o21, outFile);
  foo(&o21, &o12, &o22, outFile);
  foo(&o21, &o12, &o23, outFile);
  outFile << std::endl;
  foo(&o21, &o13, &o2 , outFile);
  foo(&o21, &o13, &o21, outFile);
  foo(&o21, &o13, &o22, outFile);
  foo(&o21, &o13, &o23, outFile);
  outFile << std::endl;
  foo(&o21, &o14, &o2 , outFile);
  foo(&o21, &o14, &o21, outFile);
  foo(&o21, &o14, &o22, outFile);
  foo(&o21, &o14, &o23, outFile);

  outFile << std::endl << std::endl;

  foo(&o22, &o1 , &o2 , outFile);
  foo(&o22, &o1 , &o21, outFile);
  foo(&o22, &o1 , &o22, outFile);
  foo(&o22, &o1 , &o23, outFile);
  outFile << std::endl;
  foo(&o22, &o11, &o2 , outFile);
  foo(&o22, &o11, &o21, outFile);
  foo(&o22, &o11, &o22, outFile);
  foo(&o22, &o11, &o23, outFile);
  outFile << std::endl;
  foo(&o22, &o12, &o2 , outFile);
  foo(&o22, &o12, &o21, outFile);
  foo(&o22, &o12, &o22, outFile);
  foo(&o22, &o12, &o23, outFile);
  outFile << std::endl;
  foo(&o22, &o13, &o2 , outFile);
  foo(&o22, &o13, &o21, outFile);
  foo(&o22, &o13, &o22, outFile);
  foo(&o22, &o13, &o23, outFile);
  outFile << std::endl;
  foo(&o22, &o14, &o2 , outFile);
  foo(&o22, &o14, &o21, outFile);
  foo(&o22, &o14, &o22, outFile);
  foo(&o22, &o14, &o23, outFile);

  outFile << std::endl << std::endl;

  foo(&o23, &o1 , &o2 , outFile);
  foo(&o23, &o1 , &o21, outFile);
  foo(&o23, &o1 , &o22, outFile);
  foo(&o23, &o1 , &o23, outFile);
  outFile << std::endl;
  foo(&o23, &o11, &o2 , outFile);
  foo(&o23, &o11, &o21, outFile);
  foo(&o23, &o11, &o22, outFile);
  foo(&o23, &o11, &o23, outFile);
  outFile << std::endl;
  foo(&o23, &o12, &o2 , outFile);
  foo(&o23, &o12, &o21, outFile);
  foo(&o23, &o12, &o22, outFile);
  foo(&o23, &o12, &o23, outFile);
  outFile << std::endl;
  foo(&o23, &o13, &o2 , outFile);
  foo(&o23, &o13, &o21, outFile);
  foo(&o23, &o13, &o22, outFile);
  foo(&o23, &o13, &o23, outFile);
  outFile << std::endl;
  foo(&o23, &o14, &o2 , outFile);
  foo(&o23, &o14, &o21, outFile);
  foo(&o23, &o14, &o22, outFile);
  foo(&o23, &o14, &o23, outFile);

  outFile << std::endl << std::endl;
}

int main()
{
  try{
    testDyna();
  }catch(const std::exception& exc){std::cerr << exc.what() << std::endl;}
}
