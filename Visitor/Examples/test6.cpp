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
typedef MM::MakeTList<B2, D21, D22, D23> ParamType;

/* "Прототип" мультиметода */
struct TestDispatch;

typedef MM::Prototype<TestDispatch, void, ParamType*, ParamType*, ParamType*,
                                          ParamType*, ParamType*, ParamType*> Proto;

/*******************************************************\
**  Теперь все определения                             **
\*******************************************************/

/* Определяем классы */
class B2
{
public:
  MAKE_ACCEPTABLE(Proto, ParamType, B2);
};

class D21 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto, ParamType, D21);
};

class D22 : public D21
{
public:
  MAKE_ACCEPTABLE(Proto, ParamType, D22);
};

class D23 : public B2
{
public:
  MAKE_ACCEPTABLE(Proto, ParamType, D23);
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

/* Создаём диспетчеры */
MM::Dispatcher<Proto> disp;

#include <cmath>
#include <cstdlib>

int main()
{
  B2  o;
  D21 o1;
  D22 o2;
  D23 o3;

  B2       *objs[MM::Length<ParamType>::value] = { &o, &o1, &o2, &o3 };
  const int bound = static_cast<int>(pow(static_cast<double>(
                                MM::Length<ParamType>::value), 6.0)+0.1);

  for (int i=0; i<bound; ++i)
  {
    int j = i, k = 0;
    int idxs[6];

    for (int l=0; l<6; ++l)
    {
      div_t pair = div(j, MM::Length<ParamType>::value);

      j         = pair.quot;
      idxs[k++] = pair.rem;
    }
    disp(objs[idxs[0]], objs[idxs[1]], objs[idxs[2]], objs[idxs[3]], objs[idxs[4]], objs[idxs[5]]);
  }
}
