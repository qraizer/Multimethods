#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_Visitor;

/* Наши тестовые классы */
class B1;
class D11;
class D12;
class D13;
class D14;

class B2;
class D21;
class D22;
class D23;

// Определение иерархий
typedef MM::MakeTList<D13, D12, D11, B1, D14> Param1List;
typedef MM::MakeTList<B2,  D21, D22, D23>     Param2List;

// Объявление мультиметодов
class TestDispatch1;
class TestDispatch2;
class TestDispatch3;
class TestDispatch4;

// Прототипы мультиметодов
typedef MM::Prototype<TestDispatch1, void, Param1List*, const Param2List*, const volatile Param1List*, volatile Param2List*> Proto1;
typedef MM::Prototype<TestDispatch2, void, const Param1List*, const volatile Param2List*, volatile Param1List*, Param2List*> Proto2;
typedef MM::Prototype<TestDispatch3, void, const volatile Param1List*, volatile Param2List*, Param1List*, const Param2List*> Proto3;
typedef MM::Prototype<TestDispatch4, void, volatile Param1List*, Param2List*, const Param1List*, const volatile Param2List*> Proto4;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Определяем классы */
/* Первая иерархия */
class B1
{
public:
  MAKE_ACCEPTABLE(Proto1, Param1List, B1);
};

class D11 : public B1
{
public:
  MAKE_ACCEPTABLE(Proto1, Param1List, D11);
};

class D12 : public B1
{
public:
  MAKE_ACCEPTABLE(Proto1, Param1List, D12);
};

class D13 : public D11
{
public:
  MAKE_ACCEPTABLE(Proto1, Param1List, D13);
};

class D14 : public D12
{
public:
  MAKE_ACCEPTABLE(Proto1, Param1List, D14);
};

/* Вторая иерархия */
class B2
{
public:
  MAKE_ACCEPTABLE(Proto1, Param2List, B2);
};

class D21 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto1, Param2List, D21);
};

class D22 : public D21
{
public:
  MAKE_ACCEPTABLE(Proto1, Param2List, D22);
};

class D23 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto1, Param2List, D23);
};

/* Определяем наши мультиметоды и их перекрытия */
/* Динамическое связывание */
struct TestDispatch1
{
  static void apply(B1*, const B2*, const volatile B1*, volatile B2*)
  {
    std::cout << "Quadruple B1 -B2 -B1 -B2  Dispatch" << std::endl;
  }
  static void apply(D11*,const B2*, const volatile D12*,volatile B2*)
  {
    std::cout << "Quadruple D11-B2 -D12-B2  Dispatch" << std::endl;
  }
  static void apply(D11*,const D22*,const volatile D12*,volatile B2*)
  {
    std::cout << "Quadruple D11-D22-D12-B2  Dispatch" << std::endl;
  }
  static void apply(D12*,const D21*,const volatile D13*,volatile D22*)
  {
    std::cout << "Quadruple D12-D21-D13-D22 Dispatch" << std::endl;
  }
  static void apply(D12*,const D22*,const volatile D13*,volatile D23*)
  {
    std::cout << "Quadruple D12-D22-D13-D23 Dispatch" << std::endl;
  }
  static void apply(D13*,const D23*,const volatile D13*,volatile D23*)
  {
    std::cout << "Quadruple D13-D23-D13-D23 Dispatch" << std::endl;
  }
};

struct TestDispatch2
{
  static void apply(const B1*, const volatile B2*, volatile B1*, B2*)
  {
    std::cout << "Quadruple B1 -B2 -B1 -B2  Dispatch" << std::endl;
  }
  static void apply(const D11*,const volatile B2*, volatile D12*,B2*)
  {
    std::cout << "Quadruple D11-B2 -D12-B2  Dispatch" << std::endl;
  }
  static void apply(const D11*,const volatile D22*,volatile D12*,B2*)
  {
    std::cout << "Quadruple D11-D22-D12-B2  Dispatch" << std::endl;
  }
  static void apply(const D12*,const volatile D21*,volatile D13*,D22*)
  {
    std::cout << "Quadruple D12-D21-D13-D22 Dispatch" << std::endl;
  }
  static void apply(const D12*,const volatile D22*,volatile D13*,D23*)
  {
    std::cout << "Quadruple D12-D22-D13-D23 Dispatch" << std::endl;
  }
  static void apply(const D13*,const volatile D23*,volatile D13*,D23*)
  {
    std::cout << "Quadruple D13-D23-D13-D23 Dispatch" << std::endl;
  }
};

struct TestDispatch3
{
  static void apply(const volatile B1*, volatile B2*, B1*, const B2*)
  {
    std::cout << "Quadruple B1 -B2 -B1 -B2  Dispatch" << std::endl;
  }
  static void apply(const volatile D11*,volatile B2*, D12*,const B2*)
  {
    std::cout << "Quadruple D11-B2 -D12-B2  Dispatch" << std::endl;
  }
  static void apply(const volatile D11*,volatile D22*,D12*,const B2*)
  {
    std::cout << "Quadruple D11-D22-D12-B2  Dispatch" << std::endl;
  }
  static void apply(const volatile D12*,volatile D21*,D13*,const D22*)
  {
    std::cout << "Quadruple D12-D21-D13-D22 Dispatch" << std::endl;
  }
  static void apply(const volatile D12*,volatile D22*,D13*,const D23*)
  {
    std::cout << "Quadruple D12-D22-D13-D23 Dispatch" << std::endl;
  }
  static void apply(const volatile D13*,volatile D23*,D13*,const D23*)
  {
    std::cout << "Quadruple D13-D23-D13-D23 Dispatch" << std::endl;
  }
};

struct TestDispatch4
{
  static void apply(volatile B1*, B2*, const B1*, const volatile B2*)
  {
    std::cout << "Quadruple B1 -B2 -B1 -B2  Dispatch" << std::endl;
  }
  static void apply(volatile D11*,B2*, const D12*,const volatile B2*)
  {
    std::cout << "Quadruple D11-B2 -D12-B2  Dispatch" << std::endl;
  }
  static void apply(volatile D11*,D22*,const D12*,const volatile B2*)
  {
    std::cout << "Quadruple D11-D22-D12-B2  Dispatch" << std::endl;
  }
  static void apply(volatile D12*,D21*,const D13*,const volatile D22*)
  {
    std::cout << "Quadruple D12-D21-D13-D22 Dispatch" << std::endl;
  }
  static void apply(volatile D12*,D22*,const D13*,const volatile D23*)
  {
    std::cout << "Quadruple D12-D22-D13-D23 Dispatch" << std::endl;
  }
  static void apply(volatile D13*,D23*,const D13*,const volatile D23*)
  {
    std::cout << "Quadruple D13-D23-D13-D23 Dispatch" << std::endl;
  }
};

/* Создаём диспетчеры */
MM::Dispatcher<Proto1> disp1;
MM::Dispatcher<Proto2> disp2;
MM::Dispatcher<Proto3> disp3;
MM::Dispatcher<Proto4> disp4;

B1  o1;
D11 o11;
D12 o12;
D13 o13;
D14 o14;

B2  o2;
D21 o21;
D22 o22;
D23 o23;

B1 *b1s[] = { &o1, &o11, &o12, &o13, &o14 };
B2 *b2s[] = { &o2, &o21, &o22, &o23 };

template <typename Disp>
void run(Disp&& disp)
{
  for (auto i : b1s)
  {
    for (auto j : b2s)
    {
      for (auto k : b1s)
      {
        for (auto l : b2s)
          disp(i, j, k, l);
        std::cout << '\n';
      }
      std::cout << '\n';
    }
    std::cout << '\n';
  }
}

int main()
{
  run(disp1);
  run(disp2);
  run(disp3);
  run(disp4);
}
