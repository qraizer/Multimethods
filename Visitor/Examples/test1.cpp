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

/* Список типов иерархии. Типы могут быть как угодно вперемежку. */
typedef MM::MakeTList<D13, D12, D11, B1, D14> Param1List;

class TestDispatch1;

class TestDispatch11;
class TestDispatch12;
class TestDispatch13;

/* Создаём диспетчеры */
typedef MM::Prototype<TestDispatch1,  const int&, Param1List*> Proto1;

typedef MM::Prototype<TestDispatch11, const int&,       std::string>  Proto11;
typedef MM::Prototype<TestDispatch12, const int&, const std::string&> Proto12;
typedef MM::Prototype<TestDispatch13, const int&,       std::string&> Proto13;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Для теста возврата ссылок */
int rets[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

/* Определяем классы */
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

/* Определяем наши мультиметоды и их перекрытия */
/* Динамическое связывание */
struct TestDispatch1
{
  static const int& apply(B1*  o)                     // базовый
  {
    std::cout << "Single B1"  << '\t';
    return rets[0];
  }
  static const int& apply(D12* o)                     // перекрывающий
  {
    std::cout << "Single D12" << '\t';
    return rets[1];
  }
  static const int& apply(D13* o)                     // ещё один перекрывающий
  {
    std::cout << "Single D13" << '\t';
    return rets[2];
  }
};

/* Статическое связывание: предельные случаи - нет динамики */
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

MM::Dispatcher<Proto1> disp1;

MM::Dispatcher<Proto11> disp11;
MM::Dispatcher<Proto12> disp12;
MM::Dispatcher<Proto13> disp13;

int main()
{
  B1  o;
  D11 o1;
  D12 o2;
  D13 o3;
  D14 o4;

  /* Тест позднего связывания */
  std::cout << &disp1(&o ) - rets << '\n';
  std::cout << &disp1(&o1) - rets << '\n';
  std::cout << &disp1(&o2) - rets << '\n';
  std::cout << &disp1(&o3) - rets << '\n';
  std::cout << &disp1(&o4) - rets << '\n';

  std::cout << std::endl;

  /* Тест раннего связывания */
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
