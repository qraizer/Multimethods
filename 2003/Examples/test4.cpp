#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_03;

using MM::TList;
using MM::NullType;

/* Наши тестовые классы */
class B2;
class D21;
class D22;
class D23;

/* Список типов иерархии. Типы могут быть как угодно вперемежку. */
typedef TList<B2, TList<D21, TList<D22, TList<D23, NullType> > > > ParamList;
/* Параметры мультиметода: оба одного типа с поздним связыванием; возвращает void */
typedef MM::LinkDynamic<ParamList, void> DynaParamType;
/* "Прототип" мультиметода - два параметра одного типа */
typedef TList<MM::LinkStatic<int, void>,
        TList<DynaParamType,
        TList<DynaParamType,
        TList<MM::LinkStatic<int, void>, NullType> > > > DynaDyna;

/* Параметры мультиметодов: с ранним связыванием, возвращают void */
typedef MM::LinkStatic<      std::string,  void> StatParam1Type;        // по значению
typedef MM::LinkStatic<const std::string&, void> StatParam2Type;        // по константной ссылке
typedef MM::LinkStatic<      std::string&, void> StatParam3Type;        // по неконстантной ссылке
/* "Прототипы" мультиметодов - по два разных параметра */
typedef TList<MM::LinkStatic<int, void>,
        TList<StatParam1Type,
        TList<StatParam2Type,
        TList<MM::LinkStatic<int, void>, NullType> > > > Stat1Stat;
typedef TList<MM::LinkStatic<int, void>,
        TList<StatParam2Type,
        TList<StatParam3Type,
        TList<MM::LinkStatic<int, void>, NullType> > > > Stat2Stat;
typedef TList<MM::LinkStatic<int, void>,
        TList<StatParam3Type,
        TList<StatParam1Type,
        TList<MM::LinkStatic<int, void>, NullType> > > > Stat3Stat;

/* Объявляем структуры с мультиметодами и их перекрытиями */
struct TestDispatch2;

struct TestDispatch21;
struct TestDispatch22;
struct TestDispatch23;

/* Создаём диспетчеры */
MM::Dispatcher<TestDispatch2,  DynaDyna> disp2;

MM::Dispatcher<TestDispatch21, Stat1Stat> disp21;
MM::Dispatcher<TestDispatch22, Stat2Stat> disp22;
MM::Dispatcher<TestDispatch23, Stat3Stat> disp23;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Определяем классы */
class B2
{
public:
  MAKE_ACCEPTABLE(void, DynaParamType, B2);
};

class D21 : public B2
{
public:
  MAKE_ACCEPTABLE(void, DynaParamType, D21);
};

class D22 : public D21
{
public:
  MAKE_ACCEPTABLE(void, DynaParamType, D22);
};

class D23 : public B2
{
public:
  MAKE_ACCEPTABLE(void, DynaParamType, D23);
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

typedef TList<MM::LinkStatic<int, void>,
        TList<DynaParamType,
        TList<StatParam2Type, 
        TList<MM::LinkStatic<int, void>, NullType> > > > Dyna1Stat;
typedef TList<MM::LinkStatic<int, void>,
        TList<DynaParamType,
        TList<StatParam3Type, 
        TList<MM::LinkStatic<int, void>, NullType> > > > Dyna2Stat;
typedef TList<MM::LinkStatic<int, void>,
        TList<DynaParamType,
        TList<StatParam1Type, 
        TList<MM::LinkStatic<int, void>, NullType> > > > Dyna3Stat;

typedef TList<MM::LinkStatic<int, void>,
        TList<StatParam1Type,
        TList<DynaParamType, 
        TList<MM::LinkStatic<int, void>, NullType> > > > Stat1Dyna;
typedef TList<MM::LinkStatic<int, void>,
        TList<StatParam2Type,
        TList<DynaParamType, 
        TList<MM::LinkStatic<int, void>, NullType> > > > Stat2Dyna;
typedef TList<MM::LinkStatic<int, void>,
        TList<StatParam3Type,
        TList<DynaParamType, 
        TList<MM::LinkStatic<int, void>, NullType> > > > Stat3Dyna;

struct TestDispatch211;
struct TestDispatch212;
struct TestDispatch213;

struct TestDispatch221;
struct TestDispatch222;
struct TestDispatch223;

MM::Dispatcher<TestDispatch211, Dyna1Stat> disp211;
MM::Dispatcher<TestDispatch212, Dyna2Stat> disp212;
MM::Dispatcher<TestDispatch213, Dyna3Stat> disp213;

MM::Dispatcher<TestDispatch221, Stat1Dyna> disp221;
MM::Dispatcher<TestDispatch222, Stat2Dyna> disp222;
MM::Dispatcher<TestDispatch223, Stat3Dyna> disp223;

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
