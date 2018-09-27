#ifndef MULTIMETHODS_H_CAD83881_D6D4_40FF_B020_BBB5523F6444
#define MULTIMETHODS_H_CAD83881_D6D4_40FF_B020_BBB5523F6444

#include "tlist.h"
#include "tuple.h"

namespace MultiMethods_03
{

/* Фактически списки типов и кортежи разрабатывались для мультиметодов, и их использование
   без них вряд ли имеет смысл. Если нужны, лучше заюзать стандартные из буста или C++11.
   Так как мультиметоды очень плотно связаны как с теми, так и с другими, нет смысла скрывать эту
   связь. */
using namespace Types_Lists;
using namespace Tuples;

/* Узел веерной иерархии абстрактного акцептора */
template <typename T, typename Ret> class Acceptor
{
public:
  virtual Ret Accept(T* obj) = 0;
};

namespace details
{

/* Строит список типов параметров "базового" мультиметода на основе
   аrцепторов абстрактного диспетчера и типов параметров, с которыми вызван его operator().
   Требуется главным образом для поддержки раннесвязываемых аргументов. */
template <typename,    typename>    struct MakeParamTypes;
template <typename Tl, typename Rl,
          typename Tr, typename Rr> struct MakeParamTypes<TList<Tl, Tr>, TList<Rl, Rr> >
{
  typedef TList<typename Tl::template ParamType<Rl>::type,
                typename MakeParamTypes<Tr, Rr>    ::type> type;
};
template <>                         struct MakeParamTypes<NullType, NullType>
{
  typedef NullType type;
};

} // details

/********************************************************************************\
***************      Абстрактные акцепторы. Всего их две штуки.     **************
\********************************************************************************/


/* Динамический акцептор. Обрабатывает позднесвязываемые параметры.
   Первый параметр - список типов, должен включать базовый и все производные от него классы,
                     которые предполагается передавать аргументом в мультиметод,
   второй          - тип возвращаемого значения мультиметода. */
template <typename TL, typename Ret> class LinkDynamic: public GenHierarchy<Acceptor, Ret, TL>
{
public:
  typedef TL TypeList;

  // вызывается конкретным акцептором, восстановливает динамический тип t
  template <typename T, typename X>
  Ret apply(T* t, X&)
  {
    return t->Accept(*this);   // делегировать восстановление типа самому t
  }
  
  /* Генератор конкретных акцепторов, реализующих чистые методы абстрактных акцепторов.
     Всего их два разных. */
  template <typename T,
            template <typename, typename, typename, typename, typename, typename> class GA,
            typename ATL, typename CTL, typename TPL, typename SRC>
  struct MakeAcceptor
  {
    typedef GA<T, TL, ATL, CTL, TPL, SRC> type;
  };

  /* Используется details::MakeParamTypes<> для получения типа параметра мультиметода.
     Для динамического акцептора это указатель на тип Head из TL. */
  template <typename T> struct ParamType { typedef T* type; };
};

/* Статическый акцептор. Обрабатывает раннесвязываемые параметры.
   Первый параметр - непосредственно тип аргумента мультиметода,
   второй          - тип возвращаемого значения мультиметода. */
template <typename Type, typename Ret> class LinkStatic
{
public:
  typedef TList<Type, NullType> TypeList;

  // вызывается конкретным акцептором; восстанавливать нечего, просто вызывает следующий акцептор
  template <typename T, typename X>
  Ret apply(T& t, X& acceptor)
  {
    return acceptor.Accept(t);
  }

  /* Такой же генератор конкретных акцепторов. */
  template <typename T,
            template <typename, typename, typename, typename, typename, typename> class GA,
            typename ATL, typename CTL, typename TPL, typename SRC>
  struct MakeAcceptor
  {
    typedef GA<T, TypeList, ATL, CTL, TPL, SRC> type;
  };

  /* Используется details::MakeParamTypes<> для получения типа параметра мультиметода.
     Для статического акцептора это константная ссылка на тип Type. */
  template <typename T> struct ParamType { typedef const T& type; };
};

/********************************************************************************\
*****************              Абстрактный диспетчер              ****************
\********************************************************************************/

/* Конкретный диспетчер */
template <typename, typename> class CallConcrete;

/* Первый параметр - пользовательский класс с реализациями перекрытых мультиметодов,
   второй - список типов абстрактных акцепторов для каждого параметра мультиметода,
   третий - тип возвращаемого мультиметодом значения. */
template <typename DI, typename TTl, typename ret_type = void>
struct Dispatcher
{
  /* Конкретные акцепторы всех промежуточних и последнего параметров мультиметода
     соответственно. Да, их ровно два. */
  template <typename T, typename ATL, typename CTL,
                        typename PTL, typename TPL, typename SRC> struct IterAcceptorCaller;
  template <typename T, typename ATL, typename CTL,
                        typename PTL, typename TPL, typename SRC> struct LastAcceptorCaller;

  /* Генератор очередного конкретного акцептора на основе очередного среза списка типов
     конкретных акцепторов. */
  template <typename T, typename ATL, typename PTL, typename TPL,
                        typename SRC>               struct AcceptorMaker;
  // эта специализация является очередной итерацией цикла и восстанавливает очередной тип
  template <typename T, typename AL,  typename AR, typename PTL,
                        typename TPL, typename SRC> struct AcceptorMaker<T, TList<AL, AR>, PTL, TPL, SRC>
  {
    typedef typename AL::template MakeAcceptor<T, IterAcceptorCaller,
                                        TList<AL, AR>,       PTL, TPL, SRC>::type type;
  };
  // эта специализация является последней итерацией цикла;
  template <typename T, typename AL,  typename PTL, typename TPL,
                        typename SRC>               struct AcceptorMaker<T, TList<AL, NullType>, PTL, TPL, SRC>
  {
    typedef typename AL::template MakeAcceptor<T, LastAcceptorCaller,
                                        TList<AL, NullType>, PTL, TPL, SRC>::type type;
  };

  /* Реализация конкретных акцепторов. Общий принцип заключается в генерации линейной иерархии
     на основе абстрактного акцептора. Очередной узел в иерархии является производным от предыдущего
     узла и реализует чистый метод очередного абстрактного акцептора из веерной иерархии. Последний
     узел производится непосредственно от абстрактного акцептора.
     В целом, т.к. и конкретный, и абстрактный акцепторы строятся на основе одинаковых списков типов,
     реализованным оказывается каждый узел из веерной иерархии, и каждый - в своём узле линейной
     иерархии. В результате позднее связываение метода Accept() вызовет нужный узел, которому
     известен его тип в списке, тем самым динамический тип восстанавливается, и тип параметра может
     быть приведён к восстановленному без dynamic_cast<>. Восстановленные типы накапливаются в
     списке восстановленных типов. В заключение вызывается следующий конкретный акцептор.
     Параметры:
     - тип возвращаемого значения;
     - срез списка типов иерархии своего параметра;
     - срез списка типов абстрактных акцепторов;
     - список восстановленных динамических типов предыдущих параметров;
     - срез списка "базовых" типов параметров мультиметода;
     - тип исходного кортежа с параметрами doIt(). */

  /* Реализация конкретного акцептор последнего параметра мультиметода. Особенности:
     - срез списка "базовых" типов параметров мультиметода не используется, ибо тут он уже пуст.
     - вызывает конкретный диспетчер вместо следующего конкретного акцептора. */
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC>
  struct LastAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC>:
                                        public LastAcceptorCaller<ret_type, R, CTL, PTL, TPL, SRC>
  {
    typedef LastAcceptorCaller<ret_type, R,  CTL, PTL, TPL, SRC> base_type;
    typedef typename CTL::Head::template ParamType<L>::type       arg_type;

    LastAcceptorCaller(SRC& ptl, TPL& src): base_type(ptl, src) {}

    /* Восстанавливает тип последнего параметра и вызывает нужную специализацию конкретного
       диспетчера, в зависимости от длины списка параметров мультиметода. */
    ret_type Accept(arg_type obj)
    {
      return CallConcrete<DI, ret_type>::template apply<SRC,
                    TList<arg_type, PTL>, Length<PTL>::value>::call(base_type::m_Params, obj);
    }
  };
  template <typename L, typename CTL, typename PTL, typename TPL, typename SRC>
  struct LastAcceptorCaller<ret_type, TList<L, NullType>, CTL, PTL, TPL, SRC> : public CTL::Head
  {
    typedef typename CTL::Head::template ParamType<L>::type arg_type;

    LastAcceptorCaller(SRC& ptl, TPL&): m_Params(ptl) {}

    ret_type Accept(arg_type obj)
    {
      return CallConcrete<DI, ret_type>::template apply<SRC,
                    TList<arg_type, PTL>, Length<PTL>::value>::call(m_Params, obj);
    }

    SRC &m_Params;
  };

  /* Реализация конкретного акцептора промежуточного параметра мультиметода. Особенности:
     - не используется, если мультиметод имеет только 1 параметр. */
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC>
  struct IterAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC> : public IterAcceptorCaller<ret_type, R, CTL, PTL, TPL, SRC>
  {
    typedef IterAcceptorCaller<ret_type, R,  CTL, PTL, TPL, SRC> base_type;
    typedef typename CTL::Head::template ParamType<L>::type       arg_type;

    IterAcceptorCaller(SRC& ptl, TPL& src): base_type(ptl, src) {}

    /* Восстанавливает тип очередного параметра, генерирует следующий конкретный акцептор,
       передавая ему следующий срез списка абстрактных акцепторов и "базовых" типов параметров и
       накопленную восстановленную информацию и вызывает его. */
    ret_type Accept(arg_type)
    {
      typedef typename AcceptorMaker<ret_type, typename CTL::Tail, TList<arg_type, PTL>,
                                               typename TPL::base_type, SRC>::type acceptor;
      acceptor a(base_type::m_Params, static_cast<typename TPL::base_type&>(base_type::m_Args));

      return a.apply(GetField<0>(base_type::m_Args), a);
    }
  };
  template <typename L, typename CTL, typename PTL, typename TPL, typename SRC>
  struct IterAcceptorCaller<ret_type, TList<L, NullType>, CTL, PTL, TPL, SRC> : public CTL::Head
  {
    typedef typename CTL::Head::template ParamType<L>::type arg_type;

    IterAcceptorCaller(SRC& ptl, TPL& src): m_Params(ptl), m_Args(src) {}

    ret_type Accept(arg_type)
    {
      typedef typename AcceptorMaker<ret_type, typename CTL::Tail, TList<arg_type, PTL>,
                                               typename TPL::base_type, SRC>::type acceptor;
      acceptor a(m_Params, static_cast<typename TPL::base_type&>(m_Args));

      return a.apply(GetField<0>(m_Args), a);
    }

    SRC &m_Params;
    TPL &m_Args;
  };

  /* Обобщённая точка входа. Создаёт конкретный акцептор первого параметра и вызывает его. */
  template <typename Tpl>
  static ret_type doIt(Tpl& tpl)
  {
    typedef typename Tpl::base_type TupleType;
    typedef typename AcceptorMaker<ret_type, TTl, NullType, TupleType, Tpl>::type acceptor;

    acceptor a(tpl, static_cast<TupleType&>(tpl));

    return a.apply(GetField<0>(tpl), a);
  }
  /* Специализированая точка входа для беспараметрических мультиметодов */
  static ret_type doIt(Tuple<NullType>&)
  {
    return CallConcrete<DI, ret_type>::template apply<NullType, NullType, -1>::call();
  }

  /* Да, тут много копипаста. Раздельные точки входа для мультиметодов с разным количеством параметров.
     Обобщённая точка входа в C++03 тут невозможна. По этой же причине конкретные "базовые" типы,
     которые уже сейчас можно получить из списка акцепторов, тут привести невозможно,
     если operator() будет нешаблонным, т.к. в этом случае, инстанцируя прототип, компилятор будет
     ругаться ошибками на TTl::Tail:: ... Tail::Head::TypeList, чьи длины превышают количество
     параметров мультиметода. Проблема решается переносом таких сигнатур вовнутрь шаблонных
     реализаций operator(). Вариант со SFINAE сложнее, ИМХО.
     Создаёт кортеж "базовых" типов параметров, заполняет его и вызывает обобщённую точку входа
     с этм кортежем. */
  /* Это тривиальный случай - параметры отсутствуют. */
  ret_type operator ()()
  {
    typedef Tuple<typename details::MakeParamTypes<TTl,
                  NullType>::type> TupleType;
    TupleType tuple;
    return doIt(tuple);
  }
  /* Далее идут обычные варианты, по одному на каждое количество параметров.
     Пока до 6, но это элементарно расширяется копипастом, если требуется. */
  template <typename T>
  ret_type operator ()(const T& p)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl,
                  TList<typename MostBase<typename TTl::Head::TypeList>::type,
                  NullType> >::type> TupleType;
    TupleType tuple(const_cast<T&>(p));
    return doIt(tuple);
  }
  template <typename T0, typename T1>
  ret_type operator ()(const T0& p0, const T1& p1)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl,
                  TList<typename MostBase<typename TTl::      Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Head::TypeList>::type,
                  NullType> > >::type> TupleType;
    TupleType tuple(const_cast<T0&>(p0), const_cast<T1&>(p1));
    return doIt(tuple);
  }
  template <typename T0, typename T1, typename T2>
  ret_type operator ()(const T0& p0, const T1& p1, const T2& p2)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl,
                  TList<typename MostBase<typename TTl::            Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::      Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::Head::TypeList>::type,
                  NullType> > > >::type> TupleType;
    TupleType tuple(const_cast<T0&>(p0), const_cast<T1&>(p1), const_cast<T2&>(p2));
    return doIt(tuple);
  }
  template <typename T0, typename T1, typename T2, typename T3>
  ret_type operator ()(const T0& p0, const T1& p1, const T2& p2, const T3& p3)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl,
                  TList<typename MostBase<typename TTl::                  Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::            Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::      Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::Tail::Head::TypeList>::type,
                  NullType> > > > >::type> TupleType;
    TupleType tuple(const_cast<T0&>(p0), const_cast<T1&>(p1), const_cast<T2&>(p2), const_cast<T3&>(p3));
    return doIt(tuple);
  }
  template <typename T0, typename T1, typename T2, typename T3, typename T4>
  ret_type operator ()(const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl,
                  TList<typename MostBase<typename TTl::                        Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::                  Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::            Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::Tail::      Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::Tail::Tail::Head::TypeList>::type,
                  NullType> > > > > >::type> TupleType;
    TupleType tuple(const_cast<T0&>(p0), const_cast<T1&>(p1), const_cast<T2&>(p2), const_cast<T3&>(p3),
                    const_cast<T4&>(p4));
    return doIt(tuple);
  }
  template <typename T0, typename T1, typename T2, typename T3, typename T4,
            typename T5>
  ret_type operator ()(const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl,
                  TList<typename MostBase<typename TTl::                        Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::                  Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::            Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::Tail::      Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::Tail::Tail::Head::TypeList>::type,
                  TList<typename MostBase<typename TTl::Tail::Tail::Tail::Tail::
                                                                          Tail::Head::TypeList>::type,
                  NullType> > > > > > >::type> TupleType;
    TupleType tuple(const_cast<T0&>(p0), const_cast<T1&>(p1), const_cast<T2&>(p2), const_cast<T3&>(p3),
                    const_cast<T4&>(p4), const_cast<T5&>(p5));
    return doIt(tuple);
  }
};

/*******************************************************************************\
*****************              Конкретный диспетчер              ****************
\*******************************************************************************/

/* Первый параметр - пользовательский класс с реализациями перекрытых мультиметодов,
   второй          - тип возвращаемого мультиметодом значения. */
template <typename UI, typename T>
struct CallConcrete
{
  typedef T ret_type;

  /* И тут много копипаста. По той же причине.
     Вызывается последним абстрактным акцептором абстрактного диспетчера. Переводит кортеж значений
     в последовательность параметров мультиметода, базируясь на списке восстановленных типов. Ввиду
     того, что все типы восстановлены, тут вполне подходит static_cast<>.
     Задействуется обычная перегрузка, в процессе которой компилятор пробует вызвать один из
     подходящих перекрытий, базируясь главным образом на возможности неявного каста указателей на
     производные классы к указателям на базовые (для позднесвязываемых параметров). Это гарантирует
     одинаковые принципы отбора наиболее подходящего метода как для статического (раннего), так и
     для динамического (позднего) связывания. */
  template <typename SRC, typename PL, int N> struct apply;
  template <typename SRC, typename PL>        struct apply<SRC, PL, -1> // тривиальный случай
  {
    static T call()
    {
      return UI::apply();
    }
  };
  template <typename SRC, typename PL>        struct apply<SRC, PL, 0>
  {
    static T call(SRC&, typename PL::Head& data)
    {
      return UI::apply(data);
    }
  };
  template <typename SRC, typename PL>        struct apply<SRC, PL, 1>
  {
    static T call(SRC& pl, typename PL::Head& data)
    {
      return UI::apply(static_cast<typename PL::Tail::Head>(GetField<0>(pl)),
                       data);
    }
  };
  template <typename SRC, typename PL>        struct apply<SRC, PL, 2>
  {
    static T call(SRC& pl, typename PL::Head& data)
    {
      return UI::apply(static_cast<typename PL::Tail::Tail::Head>(GetField<0>(pl)),//типы в PL лежат в
                       static_cast<typename PL::Tail      ::Head>(GetField<1>(pl)),//обратном порядке
                       data);
    }
  };
  template <typename SRC, typename PL>        struct apply<SRC, PL, 3>
  {
    static T call(SRC& pl, typename PL::Head& data)
    {
      return UI::apply(static_cast<typename PL::Tail::Tail::Tail::Head>(GetField<0>(pl)),
                       static_cast<typename PL::Tail::Tail      ::Head>(GetField<1>(pl)),
                       static_cast<typename PL::Tail            ::Head>(GetField<2>(pl)),
                       data);
    }
  };
  template <typename SRC, typename PL>        struct apply<SRC, PL, 4>
  {
    static T call(SRC& pl, typename PL::Head& data)
    {
      return UI::apply(static_cast<typename PL::Tail::Tail::Tail::Tail::Head>(GetField<0>(pl)),
                       static_cast<typename PL::Tail::Tail::Tail      ::Head>(GetField<1>(pl)),
                       static_cast<typename PL::Tail::Tail            ::Head>(GetField<2>(pl)),
                       static_cast<typename PL::Tail                  ::Head>(GetField<3>(pl)),
                       data);
    }
  };
  template <typename SRC, typename PL>        struct apply<SRC, PL, 5>
  {
    static T call(SRC& pl, typename PL::Head& data)
    {
      return UI::apply(static_cast<typename PL::Tail::Tail::Tail::Tail::Tail::Head>(GetField<0>(pl)),
                       static_cast<typename PL::Tail::Tail::Tail::Tail      ::Head>(GetField<1>(pl)),
                       static_cast<typename PL::Tail::Tail::Tail            ::Head>(GetField<2>(pl)),
                       static_cast<typename PL::Tail::Tail                  ::Head>(GetField<3>(pl)),
                       static_cast<typename PL::Tail                        ::Head>(GetField<4>(pl)),
                       data);
    }
  };
};

// максимум реализованных параметров
enum { implemented = 6 };

} // MultiMethods_03

/* Метод, который должен иметь любой полиморфный класс в иерархии, желающий участвовать в позднем
   связывании мультиметодов. Реализован как макро для удобства.
   Первый параметр - тип возвращаемого мультиметодом значения;
   второй параметр - приходит от абстрактного диспетчера и является ссылкой на абстрактный акцептор;
   третий параметр - тип, на который указывает this, т.е. сам класс.
   Метод должен откастовать абстрактный акцептор к его узлу, параметризованному типом *this,
   вызвать его метод Accept(), передав параметром указатель на себя, т.е. this, и вернуть значение
   его результата в качестве своего результата. */
#define MAKE_ACCEPTABLE(Ret, Abstract, Concrete)                                           \
          virtual Ret Accept(Abstract& a)                                                  \
          {                                                                                \
            return static_cast<MultiMethods_03::Acceptor<Concrete, Ret>&>(a).Accept(this); \
          }

#endif // MULTIMETHODS_H_CAD83881_D6D4_40FF_B020_BBB5523F6444
