#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_RTTI;

/* Наши тестовые классы */
class B2;
class D21;
class D22;
class D23;

/* Список типов иерархии. Типы могут быть как угодно вперемежку. */
typedef MM::MakeTList<B2, D21, D22, D23> ParamList;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Определяем классы */
class B2
{
public:
  virtual ~B2(){}
};

class D21 : public B2 {};
class D22 : public D21{};
class D23 : public B2 {};

/* Определяем наши мультиметоды и их перекрытия */
/* Динамическое связывание */
struct TestDispatch2
{
  static void apply(int, B2*,  B2*, int)
  {
    std::cout << "Double B2 -B2  Dispatch" << std::endl;
  }
  static void apply(int, D21*, B2*, int)
  {
    std::cout << "Double D21-B2  Dispatch" << std::endl;
  }
  static void apply(int, D21*, D22*, int)
  {
    std::cout << "Double D21-D22 Dispatch" << std::endl;
  }
  static void apply(int, D22*, D23*, int)
  {
    std::cout << "Double D22-D23 Dispatch" << std::endl;
  }
  static void apply(int, D23*, D22*, int)
  {
    std::cout << "Double D23-D22 Dispatch" << std::endl;
  }
  static void apply(int, D23*, D23*, int)
  {
    std::cout << "Double D23-D23 Dispatch" << std::endl;
  }
};

/* Статическое связывание: предельные случаи - нет динамики */
struct TestDispatch21
{
  static void apply(int, std::string str1, const std::string& str2, int)
  {
    std::cout << "Double value - const ref: " << &str1 << ':' << &str2 << std::endl;
    str1 = "changing inside the multimethod";
  }
};
struct TestDispatch22
{
  static void apply(int, const std::string& str1, std::string& str2, int)
  {
    std::cout << "Double const ref - nonconst ref: " << &str1 << ':' << &str2 << std::endl;
    str2 = "changing inside the multimethod";
  }
};
struct TestDispatch23
{
  static void apply(int, std::string& str1, std::string str2, int)
  {
    std::cout << "Double nonconst ref - value: " << &str1 << ':' << &str2 << std::endl;
    str1 = "changing inside a multimethod";
    str2 = "changing inside a multimethod";
  }
};

/* Создаём диспетчеры */
MM::Dispatcher<TestDispatch2,  void, int, ParamList*, ParamList*, int> disp2;

MM::Dispatcher<TestDispatch21, void, int,       std::string,  const std::string&, int> disp21;
MM::Dispatcher<TestDispatch22, void, int, const std::string&,       std::string&, int> disp22;
MM::Dispatcher<TestDispatch23, void, int,       std::string&,       std::string,  int> disp23;

static void more();

int main()
{
  B2  o;
  D21 o1;
  D22 o2;
  D23 o3;

  disp2(123, &o , &o , 321);
  disp2(123, &o , &o1, 321);
  disp2(123, &o , &o2, 321);
  disp2(123, &o , &o3, 321);

  std::cout << std::endl;

  disp2(123, &o1, &o , 321);
  disp2(123, &o1, &o1, 321);
  disp2(123, &o1, &o2, 321);
  disp2(123, &o1, &o3, 321);

  std::cout << std::endl;

  disp2(123, &o2, &o , 321);
  disp2(123, &o2, &o1, 321);
  disp2(123, &o2, &o2, 321);
  disp2(123, &o2, &o3, 321);

  std::cout << std::endl;

  disp2(123, &o3, &o , 321);
  disp2(123, &o3, &o1, 321);
  disp2(123, &o3, &o2, 321);
  disp2(123, &o3, &o3, 321);

  std::cout << std::endl << std::endl;

  std::string s1(" - it's the 1st string");
  std::string s2(" - it's the 2nd string");

  std::cout << "Before the passing into multimethod: " << s1 << '\t' << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << std::endl;
  disp21(123, s1, s2, 321);
  std::cout << "Outside a multimethod:       " << s1 << '\t' << s2 << std::endl << std::endl;
  
  s1 = " - it's the 1st string";
  s2 = " - it's the 2nd string";

  std::cout << "Before the passing into multimethod: " << s1 << '\t' << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << std::endl;
  disp22(123, s1, s2, 321);
  std::cout << "Outside a multimethod:       " << s1 << '\t' << s2 << std::endl << std::endl;

  s1 = " - it's the 1st string";
  s2 = " - it's the 2nd string";

  std::cout << "Before the passing into multimethod: " << s1 << '\t' << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << std::endl;
  disp23(123, s1, s2, 321);
  std::cout << "Outside a multimethod:       " << s1 << '\t' << s2 << std::endl << std::endl;

  more();
}

/*******************************************************\
**  Тест смешанных мультиметодов                       **
\*******************************************************/

struct TestDispatch211
{
  static void apply(int, B2*  o, const std::string& str, int)
  {
    std::cout << "Double B2  - string: " << &str << std::endl;
  }
  static void apply(int, D22* o, const std::string& str, int)
  {
    std::cout << "Double D22 - string: " << &str << std::endl;
  }
  static void apply(int, D23* o, const std::string& str, int)
  {
    std::cout << "Double D23 - string: " << &str << std::endl;
  }
};

struct TestDispatch212
{
  static void apply(int, B2*  o, std::string& str, int)
  {
    std::cout << "Double B2  - string: " << &str << std::endl;
    str += "0";
  }
  static void apply(int, D23* o, std::string& str, int)
  {
    std::cout << "Double D23 - string: " << &str << std::endl;
    str += "1";
  }
};

struct TestDispatch213
{
  static void apply(int, B2*  o, std::string str, int)
  {
    std::cout << "Double B2  - string: " << &str << std::endl;
    str += "0";
  }
  static void apply(int, D22* o, std::string str, int)
  {
    std::cout << "Double D22 - string: " << &str << std::endl;
    str += "1";
  }
};

struct TestDispatch221
{
  static void apply(int, std::string str, B2*  o, int)
  {
    std::cout << "Double B2  - string: " << &str << std::endl;
    str += "0";
  }
  static void apply(int, std::string str, D22* o, int)
  {
    std::cout << "Double D22 - string: " << &str << std::endl;
    str += "1";
  }
  static void apply(int, std::string str, D23* o, int)
  {
    std::cout << "Double D23 - string: " << &str << std::endl;
    str += "2";
  }
};

struct TestDispatch222
{
  static void apply(int, const std::string& str, B2*  o, int)
  {
    std::cout << "Double B2  - string: " << &str << std::endl;
  }
  static void apply(int, const std::string& str, D23* o, int)
  {
    std::cout << "Double D23 - string: " << &str << std::endl;
  }
};

struct TestDispatch223
{
  static void apply(int, std::string& str, B2*  o, int)
  {
    std::cout << "Double B2  - string: " << &str << std::endl;
    str += "0";
  }
  static void apply(int, std::string& str, D22* o, int)
  {
    std::cout << "Double D22 - string: " << &str << std::endl;
    str += "1";
  }
};

MM::Dispatcher<TestDispatch211, void, int, ParamList*, const std::string&, int> disp211;
MM::Dispatcher<TestDispatch212, void, int, ParamList*,       std::string&, int> disp212;
MM::Dispatcher<TestDispatch213, void, int, ParamList*,       std::string,  int> disp213;

MM::Dispatcher<TestDispatch221, void, int,       std::string,  ParamList*, int> disp221;
MM::Dispatcher<TestDispatch222, void, int, const std::string&, ParamList*, int> disp222;
MM::Dispatcher<TestDispatch223, void, int,       std::string&, ParamList*, int> disp223;

static void more()
{
  B2  o;
  D21 o1;
  D22 o2;
  D23 o3;

  std::string s(" - it's the string");

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  disp211(123, &o , s, 321);
  disp211(123, &o1, s, 321);
  disp211(123, &o2, s, 321);
  disp211(123, &o3, s, 321);
  std::cout << "Outside a multimethod:       " << s << std::endl;
  s = " - it's the string";
  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  disp212(123, &o , s, 321);
  disp212(123, &o1, s, 321);
  disp212(123, &o2, s, 321);
  disp212(123, &o3, s, 321);
  std::cout << "Outside a multimethod:       " << s << std::endl;
  s = " - it's the string";
  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  disp213(123, &o , s, 321);
  disp213(123, &o1, s, 321);
  disp213(123, &o2, s, 321);
  disp213(123, &o3, s, 321);
  std::cout << "Outside a multimethod:       " << s << std::endl << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  disp221(123, s, &o , 321);
  disp221(123, s, &o1, 321);
  disp221(123, s, &o2, 321);
  disp221(123, s, &o3, 321);
  std::cout << "Outside a multimethod:       " << s << std::endl;
  s = " - it's the string";
  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  disp222(123, s, &o , 321);
  disp222(123, s, &o1, 321);
  disp222(123, s, &o2, 321);
  disp222(123, s, &o3, 321);
  std::cout << "Outside a multimethod:       " << s << std::endl;
  s = " - it's the string";
  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  disp223(123, s, &o , 321);
  disp223(123, s, &o1, 321);
  disp223(123, s, &o2, 321);
  disp223(123, s, &o3, 321);
  std::cout << "Outside a multimethod:       " << s << std::endl << std::endl;
}
