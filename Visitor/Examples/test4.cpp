#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_Visitor;

/* Наши тестовые классы */
class B2;
class D21;
class D22;
class D23;

/* Список типов иерархии. Типы могут быть как угодно вперемежку. */
typedef MM::MakeTList<B2, D21, D22, D23> ParamList;

class TestDispatch211;
class TestDispatch212;
class TestDispatch213;

class TestDispatch221;
class TestDispatch222;
class TestDispatch223;

typedef MM::Prototype<TestDispatch211, void, int, ParamList*, const std::string&, int> Proto211;
typedef MM::Prototype<TestDispatch212, void, int, ParamList*,       std::string&, int> Proto212;
typedef MM::Prototype<TestDispatch213, void, int, ParamList*,       std::string,  int> Proto213;

typedef MM::Prototype<TestDispatch221, void, int,       std::string,  ParamList*, int> Proto221;
typedef MM::Prototype<TestDispatch222, void, int, const std::string&, ParamList*, int> Proto222;
typedef MM::Prototype<TestDispatch223, void, int,       std::string&, ParamList*, int> Proto223;

class TestDispatch2;
class TestDispatch21;
class TestDispatch22;
class TestDispatch23;

typedef MM::Prototype<TestDispatch2,  void, int, ParamList*, ParamList*, int> Proto2;

typedef MM::Prototype<TestDispatch21, void, int,       std::string,  const std::string&, int> Proto21;
typedef MM::Prototype<TestDispatch22, void, int, const std::string&,       std::string&, int> Proto22;
typedef MM::Prototype<TestDispatch23, void, int,       std::string&,       std::string,  int> Proto23;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Определяем классы */
class B2
{
public:
  MAKE_ACCEPTABLE(Proto211, ParamList, B2);
};

class D21 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto211, ParamList, D21);
};

class D22 : public D21
{
public:
  MAKE_ACCEPTABLE(Proto211, ParamList, D22);
};

class D23 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto211, ParamList, D23);
};

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
MM::Dispatcher<Proto2> disp2;

MM::Dispatcher<Proto21> disp21;
MM::Dispatcher<Proto22> disp22;
MM::Dispatcher<Proto23> disp23;

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

MM::Dispatcher<Proto211> disp211;
MM::Dispatcher<Proto212> disp212;
MM::Dispatcher<Proto213> disp213;

MM::Dispatcher<Proto221> disp221;
MM::Dispatcher<Proto222> disp222;
MM::Dispatcher<Proto223> disp223;

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
