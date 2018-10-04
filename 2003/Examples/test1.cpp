#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_03;

using MM::TList;
using MM::NullType;

/* Наши тестовые классы */
class B1;
class D11;
class D12;
class D13;
class D14;

/* Список типов иерархии. Типы могут быть как угодно вперемежку. */
typedef TList<D13, TList<D12, TList<D11, TList<B1, TList<D14, NullType> > > > > Param1List;

/* Параметры мультиметода: единственный с поздним связыванием; возвращает void */
typedef MM::LinkDynamic<Param1List, const int&> DynaParam1Type;
/* "Прототип" мультиметода - один параметр */
typedef TList<DynaParam1Type, NullType> Dyna1Simple;

/* Параметры мультиметодов: с ранним связыванием, возвращают void */
typedef MM::LinkStatic<      std::string,  const int&> StatParam1Type;        // по значению
typedef MM::LinkStatic<const std::string&, const int&> StatParam2Type;        // по константной ссылке
typedef MM::LinkStatic<      std::string&, const int&> StatParam3Type;        // по неконстантной ссылке
/* "Прототипы" мультиметодов - по одному параметру */
typedef TList<StatParam1Type, NullType> Stat1Simple;
typedef TList<StatParam2Type, NullType> Stat2Simple;
typedef TList<StatParam3Type, NullType> Stat3Simple;

/* Объявляем структуры с мультиметодами и их перекрытиями */
struct TestDispatch1;

struct TestDispatch11;
struct TestDispatch12;
struct TestDispatch13;

/* Создаём диспетчеры */
MM::Dispatcher<TestDispatch1,  Dyna1Simple, const int&> disp1;

MM::Dispatcher<TestDispatch11, Stat1Simple, const int&> disp11;
MM::Dispatcher<TestDispatch12, Stat2Simple, const int&> disp12;
MM::Dispatcher<TestDispatch13, Stat3Simple, const int&> disp13;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Для теста возврата ссылок */
int rets[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

/* Определяем классы */
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
