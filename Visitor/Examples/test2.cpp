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

/* Объявляем прототипы */
class TestDispatch2;
class TestDispatch21;
class TestDispatch22;
class TestDispatch23;

typedef MM::Prototype<TestDispatch2,  int&, ParamList*, ParamList*> Proto2;

typedef MM::Prototype<TestDispatch21, int&,       std::string,  const std::string&> Proto21;
typedef MM::Prototype<TestDispatch22, int&, const std::string&,       std::string&> Proto22;
typedef MM::Prototype<TestDispatch23, int&,       std::string&,       std::string>  Proto23;

class TestDispatch211;
class TestDispatch212;
class TestDispatch213;

class TestDispatch221;
class TestDispatch222;
class TestDispatch223;

typedef MM::Prototype<TestDispatch211, int&, ParamList*, const std::string&> Proto211;
typedef MM::Prototype<TestDispatch212, int&, ParamList*,       std::string&> Proto212;
typedef MM::Prototype<TestDispatch213, int&, ParamList*,       std::string>  Proto213;
                                    
typedef MM::Prototype<TestDispatch221, int&,       std::string,  ParamList*> Proto221;
typedef MM::Prototype<TestDispatch222, int&, const std::string&, ParamList*> Proto222;
typedef MM::Prototype<TestDispatch223, int&,       std::string&, ParamList*> Proto223;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Для теста возврата ссылок */
int rets[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

/* Определяем классы */
class B2
{
public:
  MAKE_ACCEPTABLE(Proto2,   ParamList, B2);
};

class D21 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto2,   ParamList, D21);
};

class D22 : public D21
{
public:
  MAKE_ACCEPTABLE(Proto2,   ParamList, D22);
};

class D23 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto2,   ParamList, D23);
};

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
MM::Dispatcher<Proto2> disp2;

MM::Dispatcher<Proto21> disp21;
MM::Dispatcher<Proto22> disp22;
MM::Dispatcher<Proto23> disp23;

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
