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

/* Для теста возврата ссылок */
int rets[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

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
  static int& apply(B2*,  B2*)
  {
    std::cout << "Double B2 -B2  Dispatch" << std::endl;
    return rets[0];
  }
  static int& apply(D21*, B2*)
  {
    std::cout << "Double D21-B2  Dispatch" << std::endl;
    return rets[1];
  }
  static int& apply(D21*, D22*)
  {
    std::cout << "Double D21-D22 Dispatch" << std::endl;
    return rets[2];
  }
  static int& apply(D22*, D23*)
  {
    std::cout << "Double D22-D23 Dispatch" << std::endl;
    return rets[3];
  }
  static int& apply(D23*, D22*)
  {
    std::cout << "Double D23-D22 Dispatch" << std::endl;
    return rets[4];
  }
  static int& apply(D23*, D23*)
  {
    std::cout << "Double D23-D23 Dispatch" << std::endl;
    return rets[5];
  }
};

/* Статическое связывание: предельные случаи - нет динамики */
struct TestDispatch21
{
  static int& apply(std::string str1, const std::string& str2)
  {
    std::cout << "Double value - const ref: " << &str1 << ':' << &str2 << std::endl;
    str1 = "changing inside the multimethod";
    return rets[6];
  }
};
struct TestDispatch22
{
  static int& apply(const std::string& str1, std::string& str2)
  {
    std::cout << "Double const ref - nonconst ref: " << &str1 << ':' << &str2 << std::endl;
    str2 = "changing inside the multimethod";
    return rets[7];
  }
};
struct TestDispatch23
{
  static int& apply(std::string& str1, std::string str2)
  {
    std::cout << "Double nonconst ref - value: " << &str1 << ':' << &str2 << std::endl;
    str1 = "changing inside a multimethod";
    str2 = "changing inside a multimethod";
    return rets[8];
  }
};

/* Создаём диспетчеры */
MM::Dispatcher<TestDispatch2,  int&, ParamList*, ParamList*> disp2;

MM::Dispatcher<TestDispatch21, int&,       std::string,  const std::string&> disp21;
MM::Dispatcher<TestDispatch22, int&, const std::string&,       std::string&> disp22;
MM::Dispatcher<TestDispatch23, int&,       std::string&,       std::string>  disp23;

static void more();

#include <algorithm>
#include <iterator>

int main()
{
  B2  o;
  D21 o1;
  D22 o2;
  D23 o3;

  disp2(&o , &o ) += 10;
  disp2(&o , &o1) += 10;
  disp2(&o , &o2) += 10;
  disp2(&o , &o3) += 10;

  std::cout << std::endl;

  disp2(&o1, &o ) += 10;
  disp2(&o1, &o1) += 10;
  disp2(&o1, &o2) += 10;
  disp2(&o1, &o3) += 10;

  std::cout << std::endl;

  disp2(&o2, &o ) += 10;
  disp2(&o2, &o1) += 10;
  disp2(&o2, &o2) += 10;
  disp2(&o2, &o3) += 10;

  std::cout << std::endl;

  disp2(&o3, &o ) += 10;
  disp2(&o3, &o1) += 10;
  disp2(&o3, &o2) += 10;
  disp2(&o3, &o3) += 10;

  std::cout << std::endl << std::endl;

  std::string s1(" - it's the 1st string");
  std::string s2(" - it's the 2nd string");

  std::cout << "Before the passing into multimethod: " << s1 << '\t' << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << std::endl;
  disp21(s1, s2) += 10;
  std::cout << "Outside a multimethod:       " << s1 << '\t' << s2 << std::endl << std::endl;

  s1 = " - it's the 1st string";
  s2 = " - it's the 2nd string";

  std::cout << "Before the passing into multimethod: " << s1 << '\t' << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << std::endl;
  disp22(s1, s2) += 10;
  std::cout << "Outside a multimethod:       " << s1 << '\t' << s2 << std::endl << std::endl;

  s1 = " - it's the 1st string";
  s2 = " - it's the 2nd string";

  std::cout << "Before the passing into multimethod: " << s1 << '\t' << s2 << std::endl;
  std::cout << &s1 << ':' << &s2 << std::endl;
  disp23(s1, s2) += 10;
  std::cout << "Outside a multimethod:       " << s1 << '\t' << s2 << std::endl << std::endl;

  std::copy(rets, rets + sizeof(rets)/sizeof(*rets), std::ostream_iterator<int>(std::cout, "\t"));
  std::cout << std::endl << std::endl;

  more();
}

/*******************************************************\
**  Тест смешанных мультиметодов                       **
\*******************************************************/

struct TestDispatch211
{
  static int& apply(B2*  o, const std::string& str)
  {
    std::cout << "Double B2  - string: " << &str << '\t';
    return rets[0];
  }
  static int& apply(D22* o, const std::string& str)
  {
    std::cout << "Double D22 - string: " << &str << '\t';
    return rets[1];
  }
  static int& apply(D23* o, const std::string& str)
  {
    std::cout << "Double D23 - string: " << &str << '\t';
    return rets[2];
  }
};

struct TestDispatch212
{
  static int& apply(B2*  o, std::string& str)
  {
    std::cout << "Double B2  - string: " << &str << '\t';
    str += "0";
    return rets[3];
  }
  static int& apply(D23* o, std::string& str)
  {
    std::cout << "Double D23 - string: " << &str << '\t';
    str += "1";
    return rets[4];
  }
};

struct TestDispatch213
{
  static int& apply(B2*  o, std::string str)
  {
    std::cout << "Double B2  - string: " << &str << '\t';
    str += "0";
    return rets[5];
  }
  static int& apply(D22* o, std::string str)
  {
    std::cout << "Double D22 - string: " << &str << '\t';
    str += "1";
    return rets[6];
  }
};

struct TestDispatch221
{
  static int& apply(std::string str, B2*  o)
  {
    std::cout << "Double B2  - string: " << &str << '\t';
    str += "0";
    return rets[7];
  }
  static int& apply(std::string str, D22* o)
  {
    std::cout << "Double D22 - string: " << &str << '\t';
    str += "1";
    return rets[8];
  }
  static int& apply(std::string str, D23* o)
  {
    std::cout << "Double D23 - string: " << &str << '\t';
    str += "2";
    return rets[9];
  }
};

struct TestDispatch222
{
  static int& apply(const std::string& str, B2*  o)
  {
    std::cout << "Double B2  - string: " << &str << '\t';
    return rets[10];
  }
  static int& apply(const std::string& str, D23* o)
  {
    std::cout << "Double D23 - string: " << &str << '\t';
    return rets[11];
  }
};

struct TestDispatch223
{
  static int& apply(std::string& str, B2*  o)
  {
    std::cout << "Double B2  - string: " << &str << '\t';
    str += "0";
    return rets[12];
  }
  static int& apply(std::string& str, D22* o)
  {
    std::cout << "Double D22 - string: " << &str << '\t';
    str += "1";
    return rets[13];
  }
};

MM::Dispatcher<TestDispatch211, int&, ParamList*, const std::string&> disp211;
MM::Dispatcher<TestDispatch212, int&, ParamList*,       std::string&> disp212;
MM::Dispatcher<TestDispatch213, int&, ParamList*,       std::string>  disp213;
                                    
MM::Dispatcher<TestDispatch221, int&,       std::string,  ParamList*> disp221;
MM::Dispatcher<TestDispatch222, int&, const std::string&, ParamList*> disp222;
MM::Dispatcher<TestDispatch223, int&,       std::string&, ParamList*> disp223;

static void more()
{
  B2  o;
  D21 o1;
  D22 o2;
  D23 o3;

  std::string s(" - it's the string");

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << &disp211(&o , s) - rets << '\n';
  std::cout << &disp211(&o1, s) - rets << '\n';
  std::cout << &disp211(&o2, s) - rets << '\n';
  std::cout << &disp211(&o3, s) - rets << '\n';
  std::cout << "Outside a multimethod:       " << s << std::endl;
  s = " - it's the string";
  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << &disp212(&o , s) - rets << '\n';
  std::cout << &disp212(&o1, s) - rets << '\n';
  std::cout << &disp212(&o2, s) - rets << '\n';
  std::cout << &disp212(&o3, s) - rets << '\n';
  std::cout << "Outside a multimethod:       " << s << std::endl;
  s = " - it's the string";
  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << &disp213(&o , s) - rets << '\n';
  std::cout << &disp213(&o1, s) - rets << '\n';
  std::cout << &disp213(&o2, s) - rets << '\n';
  std::cout << &disp213(&o3, s) - rets << '\n';
  std::cout << "Outside a multimethod:       " << s << std::endl << std::endl;

  s = " - it's the string";

  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << &disp221(s, &o ) - rets << '\n';
  std::cout << &disp221(s, &o1) - rets << '\n';
  std::cout << &disp221(s, &o2) - rets << '\n';
  std::cout << &disp221(s, &o3) - rets << '\n';
  std::cout << "Outside a multimethod:       " << s << std::endl;
  s = " - it's the string";
  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << &disp222(s, &o ) - rets << '\n';
  std::cout << &disp222(s, &o1) - rets << '\n';
  std::cout << &disp222(s, &o2) - rets << '\n';
  std::cout << &disp222(s, &o3) - rets << '\n';
  std::cout << "Outside a multimethod:       " << s << std::endl;
  s = " - it's the string";
  std::cout << "Before the passing into multimethod: " << s << std::endl;
  std::cout << &s << std::endl;
  std::cout << &disp223(s, &o ) - rets << '\n';
  std::cout << &disp223(s, &o1) - rets << '\n';
  std::cout << &disp223(s, &o2) - rets << '\n';
  std::cout << &disp223(s, &o3) - rets << '\n';
  std::cout << "Outside a multimethod:       " << s << std::endl << std::endl;
}
