#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_RTTI;

/* Определяем классы */
/* Первая иерархия */
class B1
{
public:
  virtual ~B1(){}
};

class D11 : public B1 {};
class D12 : public B1 {};
class D13 : public D11{};
class D14 : public D12{};

/* Определяем наши мультиметоды и их перекрытия */
struct TestDispatch1
{
  static void apply(B1&&,  std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&)
  {
    std::cout << "Simple B1  Dispatch" << std::endl;
  }
  static void apply(D11&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&)
  {
    std::cout << "Simple D11 Dispatch" << std::endl;
  }
  static void apply(D14&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&)
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

// Определение иерархии
typedef MM::MakeTList<D13, D12, D11, B1, D14> ParamList;

/* Создаём диспетчеры */
MM::Dispatcher<TestDispatch1, void, ParamList&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&> disp1;
MM::Dispatcher<TestDispatch2, void, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, ParamList&&> disp2;
MM::Dispatcher<TestDispatch3, void, std::string&, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, ParamList&&, std::string> disp3;
MM::Dispatcher<TestDispatch4, void, const std::string&, const volatile std::string&, volatile std::string&, std::string&&, ParamList&&, std::string, std::string&> disp4;
MM::Dispatcher<TestDispatch5, void, const volatile std::string&, volatile std::string&, std::string&&, ParamList&&, std::string, std::string&, const std::string&> disp5;
MM::Dispatcher<TestDispatch6, void, volatile std::string&, std::string&&, ParamList&&, std::string, std::string&, const std::string&, const volatile std::string&> disp6;
MM::Dispatcher<TestDispatch7, void, std::string&&, ParamList&&, std::string, std::string&, const std::string&, const volatile std::string&, volatile std::string&> disp7;

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
