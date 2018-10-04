#include <iostream>
#include <iomanip>
#include <string>
#
#include "mmethod.h"

namespace MM = MultiMethods_RTTI;

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

/* Список типов иерархии. */
typedef MM::MakeTList<B2, D21, D22, D23> ParamList;
/* Создаём диспетчеры */
MM::Dispatcher<TestDispatch, void, ParamList*, ParamList*, ParamList*,
                                   ParamList*, ParamList*, ParamList*> disp;

#include <cmath>
#include <cstdlib>

int main()
{
  B2  o;
  D21 o1;
  D22 o2;
  D23 o3;

  B2       *objs[MM::Length<ParamList>::value] = { &o, &o1, &o2, &o3 };
  const int bound = static_cast<int>(pow(MM::Length<ParamList>::value, 6.0)+0.1);

  for (int i=0; i<bound; ++i)
  {
    int j = i, k = 0;
    int idxs[6];

    for (int l = 0; l < 6; ++l)
    {
      div_t pair = div(j, MM::Length<ParamList>::value);

      j         = pair.quot;
      idxs[k++] = pair.rem;
    }
    disp(objs[idxs[0]], objs[idxs[1]], objs[idxs[2]], objs[idxs[3]], objs[idxs[4]], objs[idxs[5]]);
  }
}
