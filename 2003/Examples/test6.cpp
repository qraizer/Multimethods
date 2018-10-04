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
typedef MM::LinkDynamic<ParamList, void> ParamType;

/* "Прототип" мультиметода */
typedef TList<ParamType,
        TList<ParamType,
        TList<ParamType,
        TList<ParamType,
        TList<ParamType,
        TList<ParamType, NullType> > > > > > Proto;

/* Параметры мультиметодов: с ранним связыванием, возвращают void */
struct TestDispatch;

/* Создаём диспетчеры */
MM::Dispatcher<TestDispatch, Proto> disp;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Определяем классы */
class B2
{
public:
  MAKE_ACCEPTABLE(void, ParamType, B2);
  virtual ~B2(){}
};

class D21 : public B2
{
public:
  MAKE_ACCEPTABLE(void, ParamType, D21);
};

class D22 : public D21
{
public:
  MAKE_ACCEPTABLE(void, ParamType, D22);
};

class D23 : public B2
{
public:
  MAKE_ACCEPTABLE(void, ParamType, D23);
};

/* Определяем наши мультиметоды и их перекрытия */
/* Динамическое связывание */
struct TestDispatch
{
  static void apply(B2*,  B2*,  B2*,  B2*,  B2*,  B2*)
  {
    std::cout << "B2 -B2 -B2 -B2 -B2 -B2 " << std::endl;
  }
  static void apply(D21*, B2*,  D21*, B2*,  D21*, B2*)
  {
    std::cout << "D21-B2 -D21-B2 -D21-B2 " << std::endl;
  }
  static void apply(D21*, D22*, D21*, D22*, D21*, D22*)
  {
    std::cout << "D21-D22-D21-D22-D21-D22" << std::endl;
  }
  static void apply(D22*, D23*, D22*, D23*, D22*, D23*)
  {
    std::cout << "D22-D23-D22-D23-D22-D23" << std::endl;
  }
  static void apply(D23*, D22*, D23*, D22*, D23*, D22*)
  {
    std::cout << "D23-D22-D23-D22-D23-D22" << std::endl;
  }
  static void apply(D23*, D23*, D23*, D23*, D23*, D23*)
  {
    std::cout << "D23-D23-D23-D23-D23-D23" << std::endl;
  }
};

#include <cmath>
#include <cstdlib>

int main()
{
  B2  o;
  D21 o1;
  D22 o2;
  D23 o3;

  B2       *objs[MM::Length<ParamList>::value] = { &o, &o1, &o2, &o3 };
  const int bound = static_cast<int>(pow(static_cast<double>(
                                MM::Length<ParamList>::value), MM::implemented)+0.1);

  for (int i=0; i<bound; ++i)
  {
    int j = i, k = 0;
    int idxs[MM::implemented];

    for (int l=0; l<MM::implemented; ++l)
    {
      div_t pair = div(j, MM::Length<ParamList>::value);

      j         = pair.quot;
      idxs[k++] = pair.rem;
    }
    disp(objs[idxs[0]], objs[idxs[1]], objs[idxs[2]], objs[idxs[3]], objs[idxs[4]], objs[idxs[5]]);
  }
}
