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

// Определение иерархии
typedef MM::MakeTList<D13, D12, D11, B1, D14> ParamList;

// Объявление мультиметодов
class TestDispatch1;
class TestDispatch2;
class TestDispatch3;
class TestDispatch4;
class TestDispatch5;
class TestDispatch6;
class TestDispatch7;

// Прототипы мультиметодов
typedef MM::Prototype<TestDispatch1, void, ParamList&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&> Proto1;
typedef MM::Prototype<TestDispatch2, void, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, ParamList&&> Proto2;
typedef MM::Prototype<TestDispatch3, void, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, ParamList&&, std::string> Proto3;
typedef MM::Prototype<TestDispatch4, void, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, ParamList&&, std::string, std::string&> Proto4;
typedef MM::Prototype<TestDispatch5, void, const volatile std::string&, volatile std::string&, std::string&&, ParamList&&, std::string, std::string&, const std::string&> Proto5;
typedef MM::Prototype<TestDispatch6, void, volatile std::string&, std::string&&, ParamList&&, std::string, std::string&, const std::string&, const volatile std::string&> Proto6;
typedef MM::Prototype<TestDispatch7, void, std::string&&, ParamList&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&> Proto7;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Определяем классы */
/* Первая иерархия */
class B1
{
public:
  MAKE_ACCEPTABLE(Proto1, ParamList, B1);
};

class D11 : public B1
{
public:
  MAKE_ACCEPTABLE(Proto1, ParamList, D11);
};

class D12 : public B1
{
public:
  MAKE_ACCEPTABLE(Proto1, ParamList, D12);
};

class D13 : public D11
{
public:
  MAKE_ACCEPTABLE(Proto1, ParamList, D13);
};

class D14 : public D12
{
public:
  MAKE_ACCEPTABLE(Proto1, ParamList, D14);
};

/* Определяем наши мультиметоды и их перекрытия */
struct TestDispatch1
{
  static void apply(B1&&,  std::string, std::string& s1, const std::string&, const volatile std::string&, volatile std::string&, std::string&& s2)
  {
    std::cout << "Simple B1  Dispatch" << std::endl;
  }
  static void apply(D11&&, std::string, std::string& s1, const std::string&, const volatile std::string&, volatile std::string&, std::string&&)
  {
    std::cout << "Simple D11 Dispatch" << std::endl;
  }
  static void apply(D14&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&& s2)
  {
    std::cout << "Simple D14 Dispatch" << std::endl;
  }
};

struct TestDispatch2
{
  static void apply(std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, B1&&)
  {
    std::cout << "Simple B1  Dispatch" << std::endl;
  }
  static void apply(std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, D11&&)
  {
    std::cout << "Simple D11 Dispatch" << std::endl;
  }
  static void apply(std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, D14&&)
  {
    std::cout << "Simple D14 Dispatch" << std::endl;
  }
};

struct TestDispatch3
{
  static void apply(std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, B1&& , std::string)
  {
    std::cout << "Simple B1  Dispatch" << std::endl;
  }
  static void apply(std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, D11&&, std::string)
  {
    std::cout << "Simple D11 Dispatch" << std::endl;
  }
  static void apply(std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, D14&&, std::string)
  {
    std::cout << "Simple D14 Dispatch" << std::endl;
  }
};

struct TestDispatch4
{
  static void apply(const std::string&, const volatile std::string&, volatile std::string&, std::string&&, B1&& , std::string, std::string&)
  {                                                                                                                                                      
    std::cout << "Simple B1  Dispatch" << std::endl;                                                                                                     
  }                                                                                                                                                      
  static void apply(const std::string&, const volatile std::string&, volatile std::string&, std::string&&, D11&&, std::string, std::string&)
  {                                                                                                                                                      
    std::cout << "Simple D11 Dispatch" << std::endl;                                                                                                     
  }                                                                                                                                                      
  static void apply(const std::string&, const volatile std::string&, volatile std::string&, std::string&&, D14&&, std::string, std::string&)
  {
    std::cout << "Simple D14 Dispatch" << std::endl;
  }
};

struct TestDispatch5
{
  static void apply(const volatile std::string&, volatile std::string&, std::string&&, B1&& , std::string, std::string&, const std::string&)
  {
    std::cout << "Simple B1  Dispatch" << std::endl;
  }
  static void apply(const volatile std::string&, volatile std::string&, std::string&&, D11&&, std::string, std::string&, const std::string&)
  {
    std::cout << "Simple D11 Dispatch" << std::endl;
  }
  static void apply(const volatile std::string&, volatile std::string&, std::string&&, D14&&, std::string, std::string&, const std::string&)
  {
    std::cout << "Simple D14 Dispatch" << std::endl;
  }
};

struct TestDispatch6
{
  static void apply(volatile std::string&, std::string&&, B1&& , std::string, std::string&, const std::string&, const volatile std::string&)
  {                                                                                       
    std::cout << "Simple B1  Dispatch" << std::endl;                                      
  }                                                                                       
  static void apply(volatile std::string&, std::string&&, D11&&, std::string, std::string&, const std::string&, const volatile std::string&)
  {                                                                                       
    std::cout << "Simple D11 Dispatch" << std::endl;                                      
  }                                                                                       
  static void apply(volatile std::string&, std::string&&, D14&&, std::string, std::string&, const std::string&, const volatile std::string&)
  {
    std::cout << "Simple D14 Dispatch" << std::endl;
  }
};

struct TestDispatch7
{
  static void apply(std::string&&, B1&& , std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&)
  {                                                                
    std::cout << "Simple B1  Dispatch" << std::endl;               
  }                                                                
  static void apply(std::string&&, D11&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&)
  {                                                                
    std::cout << "Simple D11 Dispatch" << std::endl;               
  }                                                                
  static void apply(std::string&&, D14&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&)
  {
    std::cout << "Simple D14 Dispatch" << std::endl;
  }
};

/* Создаём диспетчеры */
MM::Dispatcher<Proto1> disp1;
MM::Dispatcher<Proto2> disp2;
MM::Dispatcher<Proto3> disp3;
MM::Dispatcher<Proto4> disp4;
MM::Dispatcher<Proto5> disp5;
MM::Dispatcher<Proto6> disp6;
MM::Dispatcher<Proto7> disp7;

int main()
{
  B1  o1;
  D11 o11;
  D12 o12;
  D13 o13;
  D14 o14;
  B1 *b1s[] = { &o1, &o11, &o12, &o13, &o14 };

  std::string str = "It's a string";

  for (auto i : b1s)
  {
    disp1(std::move(*i), "It's another string", str, "It's more another string", str, str, "It's string again");
    disp2("It's another string", str, "It's more another string", str, str, "It's string again", std::move(*i));
    disp3(str, "It's more another string", str, str, "It's string again", std::move(*i), "It's another string");
    disp4("It's more another string", str, str, "It's string again", std::move(*i), "It's another string", str);
    disp5(str, str, "It's string again", std::move(*i), "It's another string", str, "It's more another string");
    disp6(str, "It's string again", std::move(*i), "It's another string", str, "It's more another string", str);
    disp7("It's string again", std::move(*i), "It's another string", str, "It's more another string", str, str);
    std::cout << '\n';
  }
}
