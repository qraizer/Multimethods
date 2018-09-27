#ifndef MULTIMETHODS_H_0A0E1659_FD72_4B5B_A091_EF59C8050584
#define MULTIMETHODS_H_0A0E1659_FD72_4B5B_A091_EF59C8050584

#include "tlist.h"
#include "tuple.h"

namespace MultiMethods_Visitor
{

/* Фактически списки типов и кортежи разрабатывались для мультиметодов, и их использование
   без них вряд ли имеет смысл. Если нужны, лучше заюзать стандартные из буста или C++1x.
   Так как мультиметоды очень плотно связаны как с теми, так и с другими, нет смысла скрывать эту
   связь. */
using namespace Types_Lists;
using namespace Tuples;

/* Внутренности реализации (приватный интерфейс). */
namespace details
{

/* Узел веерной иерархии абстрактного акцептора. Документирует интерфейс Visitor-а */
template <typename T, typename Ret> class Acceptor
{
public:
  virtual Ret Accept(T*&& obj) = 0;
};

/* Строит список типов параметров "базового" мультиметода на основе
   акцепторов абстрактного диспетчера и типов параметров, с которыми вызван его operator().
   Заменяет списки типов, описывающих иерархии динамически связываемых аргументов, на самые
   базовые классы в этих иерархиях. Применяя эту же операция для статически связваемых
   аргументов, фактически ничего не меняет, т.к. там всего один тип в списке.*/
template <typename>                 struct MakeParamTypes;
template <typename Tl, typename Tr> struct MakeParamTypes<TList<Tl, Tr>>
{
  typedef TList<typename Tl::template ParamType<typename MostBase<typename Tl::TypeList>::type>::type,
                typename MakeParamTypes<Tr>::type> type;
};
template <>                         struct MakeParamTypes<NullType>
{
  typedef NullType type;
};

/********************************************************************************\
***************      Абстрактные акцепторы. Всего их две штуки.     **************
\********************************************************************************/

/* Динамический акцептор. Обрабатывает динамически связываемые параметры.
   Первый параметр - список типов, должен включать базовый и все производные от него классы,
                     которые предполагается передавать аргументом в мультиметод,
   второй          - тип возвращаемого значения мультиметода. */
template <typename TL, typename Ret> class LinkDynamic: public GenHierarchy<details::Acceptor, Ret, TL>
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

/* Статическый акцептор. Обрабатывает статически связываемые параметры.
   Первый параметр - непосредственно тип аргумента мультиметода,
   второй          - тип возвращаемого значения мультиметода. */
template <typename Type, typename Ret> class LinkStatic
{
public:
  typedef TList<Type, NullType> TypeList;

  // вызывается конкретным акцептором; восстанавливать нечего, просто вызывает следующий акцептор
  template <typename T, typename X>
  Ret apply(T&& t, X& acceptor)
  {
    return acceptor.Accept(std::forward<T>(t));
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
     Для статического акцептора это rvalue ссылка на тип Type. */
  template <typename T> struct ParamType { typedef T&& type; };
};

/********************************************************************************\
*************        Стратегии работы с параметрами диспетчера.       ************
*******       Увы, <type_traits> не поможет: там типы, а тут шаблоны       *******
\********************************************************************************/

/* Преобразование типа аргумента диспетчера в тип его параметра. */

// Статически связываемые параметры. Типы совпадают.
template <typename T> struct MakeArg
{
  typedef T type;
};

// Динамически связываемые параметры.
// Список типов пребразуется в cv-неквалифицированный тип указателя на самый базовый класс.
template <typename L, typename R> struct MakeArg<TList<L, R>>;                  // намерено не реализован
template <typename L, typename R> struct MakeArg<TList<L, R>*>                  // *
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
template <typename L, typename R> struct MakeArg<const TList<L, R>*>            // const *
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
template <typename L, typename R> struct MakeArg<const volatile TList<L, R>*>   // const volatile *
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
template <typename L, typename R> struct MakeArg<volatile TList<L, R>*>         // volatile *
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
template <typename L, typename R> struct MakeArg<TList<L, R>&>                  // &
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
template <typename L, typename R> struct MakeArg<const TList<L, R>&>            // const &
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
template <typename L, typename R> struct MakeArg<const volatile TList<L, R>&>   // const volatile &
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
template <typename L, typename R> struct MakeArg<volatile TList<L, R>&>         // volatile &
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
template <typename L, typename R> struct MakeArg<TList<L, R>&&>                 // &&
{
  typedef typename MostBase<TList<L, R>>::type *type;
};
// Технически тут могут быть cv-квалифицированные варианты для &&, но кому они нужны


/* Выбор политики для обработки диспетчером его параметров */

// Статически связываемые параметры. Политика статического связывания.
template <typename T, typename RT> struct MakeParam
{
  typedef details::LinkStatic<T, RT> type;
};

// Динамически связываемые параметры. Политика динамического связывания.
// Все варианты как и у MakeArg<>.
template <typename L, typename R, typename RT> struct MakeParam<TList<L, R>, RT>;
template <typename L, typename R, typename RT> struct MakeParam<TList<L, R>*, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};
template <typename L, typename R, typename RT> struct MakeParam<const TList<L, R>*, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};
template <typename L, typename R, typename RT> struct MakeParam<const volatile TList<L, R>*, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};
template <typename L, typename R, typename RT> struct MakeParam<volatile TList<L, R>*, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};
template <typename L, typename R, typename RT> struct MakeParam<TList<L, R>&, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};
template <typename L, typename R, typename RT> struct MakeParam<const TList<L, R>&, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};
template <typename L, typename R, typename RT> struct MakeParam<const volatile TList<L, R>&, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};
template <typename L, typename R, typename RT> struct MakeParam<volatile TList<L, R>&, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};
template <typename L, typename R, typename RT> struct MakeParam<TList<L, R>&&, RT>
{
  typedef details::LinkDynamic<TList<L, R>, RT> type;
};

/* Построить список типов политик для всех параметров диспетчера.
   RT - return type, перенесён в начало списка параметров для бесконфликности с вариадиком. */
template <typename RT, typename T, typename ...List> struct MakeParams
{
  typedef TList<typename MakeParam<T, RT>::type, typename MakeParams<RT, List...>::type> type;
};
template <typename RT, typename T>                   struct MakeParams<RT, T>
{
  typedef TList<typename MakeParam<T, RT>::type, NullType> type;
};

/* Предобработка параметра, переданного диспетчеру.
   Нужна частичная специализация, т.к. работа основана на вариадике диспетчера, куда замешаны
   списки типов вместо фактических типов. А чтобы не заморачиваться MostBase<>, внутри нужен и
   простой шаблонный метод для обработки собственно параметра. */

// Статически связываемый параметр. Просто форвард.
template <typename A>             struct mkref
{
  template <typename T>
  static T&& doIt(T&& t) { return std::forward<T>(t); }
};

// Динамически связываемый параметр. Нужно убрать cv-квалификаторы и преобразовать в указатель.
// Все варианты как и у MakeArg<>.
template <typename L, typename R> struct mkref<TList<L, R>>;
template <typename L, typename R> struct mkref<TList<L, R>*>
{
  template <typename T>
  static T* doIt(T* t)                { return t;                  }
};
template <typename L, typename R> struct mkref<const TList<L, R>*>
{
  template <typename T>
  static T* doIt(const T* t)          { return const_cast<T*>(t);  }
};
template <typename L, typename R> struct mkref<const volatile TList<L, R>*>
{
  template <typename T>
  static T* doIt(const volatile T* t) { return const_cast<T*>(t);  }
};
template <typename L, typename R> struct mkref<volatile TList<L, R>*>
{
  template <typename T>
  static T* doIt(volatile T* t)       { return const_cast<T*>(t);  }
};
template <typename L, typename R> struct mkref<TList<L, R>&>
{
  template <typename T>
  static T* doIt(T& t)                { return &t;                 }
};
template <typename L, typename R> struct mkref<const TList<L, R>&>
{
  template <typename T>
  static T* doIt(const T& t)          { return &const_cast<T&>(t); }
};
template <typename L, typename R> struct mkref<const volatile TList<L, R>&>
{
  template <typename T>
  static T* doIt(const volatile T& t) { return &const_cast<T&>(t); }
};
template <typename L, typename R> struct mkref<volatile TList<L, R>&>
{
  template <typename T>
  static T* doIt(volatile T& t)       { return &const_cast<T&>(t); }
};
template <typename L, typename R> struct mkref<TList<L, R>&&>
{
  template <typename T>
  static T* doIt(T&& t)               { return &t;                 }
};

/* Постобработка параметра, переданного диспетчеру.
   Нужна частичная специализация, т.к. работа основана на вариадике диспетчера, куда замешаны
   списки типов вместо фактических типов + там же лежат исходные cv- и ptr/rev квалификаторы.
   А чтобы не заморачиваться MostBase<>, внутри нужен и простой шаблонный метод для обработки
   собственно параметра. */

// Статически связываемый параметр. Всё могло быть проще, мог быть просто форвард. Но реальность
// такова, что для просто форварда нет разницы между rvalue и rvalue ref, в результате объекты,
// принимаемые мультиметодом по значению, переносятся вместо того, чтобы копироваться.
template <typename X>            struct dcref
{
  template <typename T>
  static T                 doIt(T&& t)               { return t; }
};
template <typename X>            struct dcref<X&>
{
  template <typename T>
  static T&                doIt(T& t)                { return t; }
};
template <typename X>            struct dcref<const X&>
{
  template <typename T>
  static const T&          doIt(const T& t)          { return t; }
};
template <typename X>            struct dcref<const volatile X&>
{
  template <typename T>
  static const volatile T& doIt(const volatile T& t) { return t; }
};
template <typename X>            struct dcref<volatile X&>
{
  template <typename T>
  static volatile T&       doIt(volatile T& t)       { return t; }
};
// вот ради этого весь сыр-бор
template <typename X>            struct dcref<X&&>
{
  template <typename T>
  static T&&               doIt(T&& t){return std::forward<T>(t);}
};

// Динамически связываемый параметр. Нужно вернуть исходные его cv- и ptr/ref квалификаторы.
// Все варианты как и у MakeArg<>.
template <typename L, typename R> struct dcref<TList<L, R>>;
template <typename L, typename R> struct dcref<TList<L, R>*>
{
  template <typename T>
  static       T* doIt(T* t)          { return t;                                 }
};
template <typename L, typename R> struct dcref<const TList<L, R>*>
{
  template <typename T>
  static const T* doIt(T* t)          { return const_cast<const T*>(t);           }
};
template <typename L, typename R> struct dcref<const volatile TList<L, R>*>
{
  template <typename T>
  static const volatile T* doIt(T* t) { return const_cast<const volatile T*>(t);  }
};
template <typename L, typename R> struct dcref<volatile TList<L, R>*>
{
  template <typename T>
  static volatile T* doIt(T* t)       { return const_cast<volatile T*>(t);        }
};
template <typename L, typename R> struct dcref<TList<L, R>&>
{
  template <typename T>
  static       T& doIt(T* t)          { return *t;                                }
};
template <typename L, typename R> struct dcref<const TList<L, R>&>
{
  template <typename T>
  static const T& doIt(T* t)          { return const_cast<const T&>(*t);          }
};
template <typename L, typename R> struct dcref<const volatile TList<L, R>&>
{
  template <typename T>
  static const volatile T& doIt(T* t) { return const_cast<const volatile T&>(*t); }
};
template <typename L, typename R> struct dcref<volatile TList<L, R>&>
{
  template <typename T>
  static volatile T& doIt(T* t)       { return const_cast<volatile T&>(*t);       }
};
template <typename L, typename R> struct dcref<TList<L, R>&&>
{
  template <typename T>
  static T&& doIt(T* t)               { return std::move(*t);                     }
};

} // details

/********************************************************************************\
*****************              Абстрактный диспетчер              ****************
\********************************************************************************/

/* Конкретный диспетчер */
template <typename, typename, typename ...> class CallConcrete;

/* Прототип диспетчера. Должен быть отделён от диспетчера, иначе получается циклическая связь между
   определениями абстрактных акцепторов и объявлениями visit-методов в классах пользователя.
   Первый параметр - пользовательский класс с реализациями перекрытых мультиметодов,
   второй          - тип возвращаемого мультиметодом значения,
   третий...       - типы параметров. */
template <typename DI, typename RetType, typename ...Args>
struct Prototype
{
protected:
  // преобразовать типы параметров мультиметода в аргументы
  // фактически это список абстрактных акцепторов
  typedef typename details::MakeParams<RetType, Args...>::type TTl;

public:
  typedef RetType ret_type;

  /* Получить тип абстрактного акцептора по списку типов иерархии.
     Требуется в visit-методах пользовательских классов, чтобы от него выбрать свой интерфейс.
     В версии для C++03 абстрактные акцепторы создавались пользователем, тут их создаёт сам диспетчер,
     так что пользовать диспетчер стало проще, а реализовать visit-метод сложнее. */
  template <typename TL>            struct getArg;
  template <typename H, typename T> struct getArg<TList<H, T>>
  {
    // Найти список типов иерархии в аргументах мультиметода
    template <typename X, typename Y, typename Tl>            struct findType;
    template <typename X, typename Y, typename L, typename R> struct findType<X, Y, TList<L, R>>
    {
      typedef typename findType<X, typename R::Head::TypeList, R>::type type;
    };
    template <typename X, typename L, typename R>             struct findType<X, X, TList<L, R>>
    {
      typedef L type;
    };
    // Найти LinkDynamic<> запрошенного TList<> в TTl
    typedef typename findType<TList<H, T>, typename TTl::Head::TypeList, TTl>::type type;
  };
};
// Специализация для беспараметрических мультиметодов
template <typename DI, typename RetType> struct Prototype<DI, RetType> {};

/* Абстрактный диспетчер. Принимает свой прототип. */
template <typename Proto> class Dispatcher;

template <typename DI, typename RetType, typename ...Args> 
class Dispatcher<Prototype<DI, RetType, Args...>>: Prototype<DI, RetType, Args...>
{
  using typename Prototype<DI, RetType, Args...>::TTl;

/********************************************************************************\
***************       Конкретные акцепторы. Всего их две штуки      **************
\********************************************************************************/

  /* Конкретные акцепторы. IterAcceptorCaller обрабатывает все параметры, кроме последнего,
     LastAcceptorCaller обрабатывает последний параметр. */
  template <typename T, typename ATL, typename CTL,
                        typename PTL, typename TPL, typename SRC> struct IterAcceptorCaller;
  template <typename T, typename ATL, typename CTL,
                        typename PTL, typename TPL, typename SRC> struct LastAcceptorCaller;

  /* Генератор очередного конкретного акцептора на основе очередного среза списка типов
     абстрактных акцепторов. */
  template <typename T, typename ATL, typename PTL, typename TPL,
                        typename SRC>               struct AcceptorMaker;
  // эта специализация является очередной итерацией цикла и генерирует очередной акцептор
  template <typename T, typename AL,  typename AR, typename PTL,
                        typename TPL, typename SRC> struct AcceptorMaker<T, TList<AL, AR>, PTL, TPL, SRC>
  {
    typedef typename AL::template MakeAcceptor<T, IterAcceptorCaller,
                                        TList<AL, AR>,       PTL, TPL, SRC>::type type;
  };
  // эта специализация является последней итерацией цикла
  template <typename T, typename AL,  typename PTL, typename TPL,
                        typename SRC>               struct AcceptorMaker<T, TList<AL, NullType>, PTL, TPL, SRC>
  {
    typedef typename AL::template MakeAcceptor<T, LastAcceptorCaller,
                                        TList<AL, NullType>, PTL, TPL, SRC>::type type;
  };

  /* Реализация конкретных акцепторов. Общий принцип заключается в генерации линейной иерархии
     на основе абстрактного акцептора. Очередной узел в иерархии является производным от предыдущего
     узла и реализует интерфейс очередного Visitor-а из веерной иерархии. В совокупности вся линейная
     иерархия конкретного акцептора реализует все интерфейсы этого Visitor-а. Последний узел
     производится непосредственно от абстрактного акцептора, который эти интерфейсы документировал.
     В целом, т.к. и конкретный, и абстрактный акцепторы строятся на основе одинаковых списков типов,
     реализованным оказывается каждый узел из веерной иерархии, и каждый - в своём узле линейной
     иерархии. В результате позднее связываение метода Accept() вызовет нужный узел, которому
     известен его тип в списке, тем самым динамический тип восстанавливается, и тип параметра может
     быть приведён к восстановленному без dynamic_cast<>. Восстановленные типы накапливаются в
     списке восстановленных типов. В заключение вызывается следующий конкретный акцептор.
     Параметры:
     - тип возвращаемого значения;
     - срез списка типов иерархии своего параметра;
     - срез списка типов параметров диспетчера (точнее, его прототипа);
     - список восстановленных динамических типов предыдущих параметров (в обратном порядке);
     - срез списка типов параметров мультиметода;
     - тип исходного кортежа с параметрами doIt(). */

  /* Реализация конкретного акцептора последнего параметра мультиметода. Особенности:
     - срез списка типов параметров мультиметода не используется, ибо тут он уже пуст.
     - вызывает конкретный диспетчер вместо следующего конкретного акцептора. */

  // это все апцепторы в иерархии, кроме самого базового
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC>
  struct LastAcceptorCaller<RetType, TList<L, R>, CTL, PTL, TPL, SRC>:
                                        public LastAcceptorCaller<RetType, R, CTL, PTL, TPL, SRC>
  {
    typedef LastAcceptorCaller<RetType, R,  CTL, PTL, TPL, SRC> base_type;
    typedef typename CTL::Head::template ParamType<L>::type     arg_type;

    LastAcceptorCaller(SRC& ptl, TPL& src): base_type(ptl, src) {}

    // Восстанавливает тип последнего параметра и вызывает нужную специализацию конкретного
    // диспетчера для преобразования кортежа обратно в вариадик; последний параметр нет смысла
    // включать в кортеж, т.к. это потребует также модификации списков типов, потому
    // его проще передать явно вместе с его восстановленным типом.
    RetType Accept(arg_type&& obj)
    {
      return CallConcrete<DI, RetType, Args...>::template apply<SRC, PTL, arg_type>
                                        ::call(base_type::m_Params, std::forward<arg_type>(obj));
    }
  };
  // это базовый в иерархии акцептор; весь список типов иерархии параметра уже обработан производными
  // классами, так что этот с ними уже не работает, он только хранит общую для них информацию и
  // наследует интерфейсы Visitor-а от абстрактного акцептора
  template <typename CTL, typename PTL, typename TPL, typename SRC>
  struct LastAcceptorCaller<RetType, NullType, CTL, PTL, TPL, SRC> : public CTL::Head
  {
    LastAcceptorCaller(SRC& ptl, TPL&): m_Params(ptl) {}

    SRC &m_Params;
  };

  /* Реализация конкретного акцептора всех параметров мультиметода, кроме последнего. Особенности:
     - используется, если мультиметод имеет более 1 параметра. */
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC>
  struct IterAcceptorCaller<RetType, TList<L, R>, CTL, PTL, TPL, SRC> :
                                        public IterAcceptorCaller<RetType, R, CTL, PTL, TPL, SRC>
  {
    typedef IterAcceptorCaller<RetType, R,  CTL, PTL, TPL, SRC> base_type;
    typedef typename CTL::Head::template ParamType<L>::type     arg_type;

    IterAcceptorCaller(SRC& ptl, TPL& src): base_type(ptl, src) {}

    /* восстановление типа очередного параметра, генерация следующего конкретного акцептора,
       передача ему следующего среза списка абстрактных акцепторов, базовых типов параметров
       мультиметода и накопленной восстановленной информации о динамических типах и его вызов. */
    // выглядит странным, но да, сам параметр метода не нужен, важен лишь его тип для того, чтобы
    // динамический тип был правильно восстановлен; (точнее, даже он нужен лишь самому последнему
    // акцептору, а тип нужен лишь для помещения в список восстановленных типов); сами параметры
    // всё равно лежат в кортеже, им нужен только лишь static_cast<>, который позже сделает
    // конкретный диспетчер
    RetType Accept(arg_type&&)
    {
      typedef decltype(GetField<0>(base_type::m_Args)) realType;
      typedef typename AcceptorMaker<RetType, typename CTL::Tail, TList<arg_type, PTL>,
                                              typename TPL::base_type, SRC>::type acceptor;

      acceptor a(base_type::m_Params, static_cast<typename TPL::base_type&>(base_type::m_Args));

      return a.apply(std::forward<realType>(GetField<0>(base_type::m_Args)), a);
    }
  };
  // это базовый в иерархии акцептор; весь список типов иерархии параметра уже обработан производными
  // классами, так что этот с ними уже не работает, он только хранит общую для них информацию и
  // наследует интерфейсы Visitor-а от абстрактного акцептора
  template <typename CTL, typename PTL, typename TPL, typename SRC>
  struct IterAcceptorCaller<RetType, NullType, CTL, PTL, TPL, SRC> : public CTL::Head
  {
    IterAcceptorCaller(SRC& ptl, TPL& src): m_Params(ptl), m_Args(src) {}

    SRC &m_Params;
    TPL &m_Args;
  };

  /* Обобщённая точка входа. Создаёт акцептор первого параметра и вызывает его.
     См. комменты к IterAcceptorCaller::Accept(). */
  template <typename Tpl>
  static RetType doIt(Tpl& tpl)
  {
    typedef decltype(GetField<0>(tpl)) realType;
    typedef typename Tpl::base_type TupleType;
    typedef typename AcceptorMaker<RetType, TTl, NullType, TupleType, Tpl>::type acceptor;

    acceptor a(tpl, static_cast<TupleType&>(tpl));

    return a.apply(std::forward<realType>(GetField<0>(tpl)), a);
  }

  /* Промежуточный метод перед doIt().
     Выполняет вторую фазу обработки параметров: создание и заполнение кортежа. После этого этапа
     вариадиков больше нет, а все параметры прочеканы и предобработны. */
  RetType decay(typename details::MakeArg<Args>::type&&... args)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl>::type> TupleType;

    TupleType tuple(std::forward<typename details::MakeArg<Args>::type>(args)...);
    return doIt(tuple);
  }

public:
  using typename Prototype<DI, RetType, Args...>::ret_type;

  /* Точка вызова диспетчера. Аргументами могут прийти любые типы, т.к. они не обязаны строго
     совпадать с формальными параметрами мультиметода. Их соответствие проеряется по ходу дела,
     и ежели что, либо будет выполнен неявный каст, либо компилятор сообщит о проблеме.
     В отличие от реализации для C++03, тут возможно сохранить полную Стандартную семантику
     соответствия типов фактических параметров типам заявленных формальных. */
  template <typename ...A>
  ret_type operator ()(A&&... args)
  {
    // Предобработка параметров, в ходе которой динамически связываемые сводятся к указателям на
    // cv-неквалифицированные значения, и выполняется проверка соответствия фатических параметров
    // формальным. Важно, чтобы static_cast<> выполнял каст к rvalue ref. Во-первых, это не даёт
    // ему выполнять явный каст типов там, где неявный бы провалился (например, каст explicit
    // конструктором), во-вторых, он сводит динамически связываемые параметры к их базовым классам,
    // что, вообще говоря, является его главным назначением.
    return decay(static_cast<typename details::MakeArg<Args>::type&&>(
                                details::mkref<Args>::doIt(std::forward<A>(args))) ...);
  }
};

/* Это тривиальный случай - у мультиметода параметры отсутствуют. */
template <typename DI, typename RetType>
struct Dispatcher<Prototype<DI, RetType>>
{
  RetType operator ()()
  {
    return CallConcrete<DI, RetType, NullType>::template apply<NullType, NullType, NullType>::call();
  }
};

/*******************************************************************************\
*****************              Конкретный диспетчер              ****************
\*******************************************************************************/

/* Первый параметр - пользовательский класс с реализациями перекрытых мультиметодов,
   второй          - тип возвращаемого мультиметодом значения,
   третий          - исходный вариадик формальных параметров, по которому нужно будет выполнить
                     постобработку фактических. */
template <typename UI, typename Ret, typename ...Args>
struct CallConcrete
{
  /* Преобразование кортежа обратно в вариадик с учётом восстановленных динамических типов.
     Обрабатывает все элементы кортежа, кроме последнего, который пришёл отдельно и уже обработан.
       SRC - исходный кортеж;
       PL  - список восстановленных типов (в обратном порядке);
       Head- последний восстановленный тип.
     И тут тоже пригодились бы fold-expression из C++17. */
  template <typename SRC, typename PL, typename Head> struct apply
  {
    // обрабатывает очередной элемент кортежа
    //   pl   - исходный кортеж;
    //   data - последний параметр мультиметода; в касте не нуждается, т.к. передаётся вне контежа,
    //          и его тип уже целевой;
    //   args - уже обработанные параметры (при первом вызове пуст)
    template <typename ...A>
    static Ret call(SRC& pl, Head&& data, A&& ...args)
    {
      // взять из кортежа очередной элемент (в обратном порядке), преобразовать к восстановленному
      // типу из PL и передать следующему преобразователю вместе с последним параметром и исходным
      // кортежем; так потихоньку наполняется вариадик
      return apply<SRC, typename PL::Tail, Head>::call(pl, std::forward<Head>(data),
                static_cast<typename PL::Head>(GetField<Length<PL>::value-1>(pl)),
                std::forward<A>(args)...);
    }
  };
  /* Сюда попадаем, когда элементов кортежа больше нет (или и не было).
     Обрабатывает все элементы кортежа, кроме последнего.
       SRC - исходный кортеж;
       PL  - список восстановленных типов (в обратном порядке);
       Head- последний восстановленный тип.
     И тут тоже пригодились бы fold-expression из C++17. */
  template <typename SRC, typename Head>              struct apply<SRC, NullType, Head>
  {
    // вызывается предыдущим apply<>::call(); все параметры восстановлены, осталось только
    // поставить на место последний параметр...
    // вообще говоря, эти два метода можно было бы объединить в один, если не то факт, что длины
    // вариадиков ...Args и ...A разные; поэтому сначала их sizeof... выравниваются
    template <typename ...A>
    static Ret call(SRC& pl, Head&& data, A&& ...args)
    {
      return   call(std::forward<A>(args)..., std::forward<Head>(data));
    }
    // ...выполнить постобработку каждого из них в соответствии с его политикой из ...Args
    // и вызвать пользовательский перекрытый мультиметод
    template <typename ...A>
    static Ret call(A&& ...args)
    {
      return UI::apply(details::dcref<Args>::doIt(std::forward<A>(args))...);
    }
    // отдельная специализация для беспараметрических мультиметодов
    static Ret call()
    {
      return UI::apply();
    }
  };
};

} // MultiMethods_Visitor

/* Метод, который должен иметь любой полиморфный класс в иерархии, желающий участвовать в динамическом
   связывании. Реализован как макро для удобства.
   Первый параметр - прототип диспетчера; из него будет вытащен список параметров мультиметода;
   второй параметр - список типов иерархии, которой принадлежит полиморфный класс, он будет искаться
                     в списке параметров в прототипе;
   третий параметр - тип, на который указывает this, т.е. сам класс.
   Метод является visit-методом в паттерне Visitor. Он получает свой конкретный акцептор, должен
   запросить (static_cast<>-ом) у него конкретно свой интерфейс и вызвать его, сообщив т.с. свой
   динамический тип. Фактически static_cast<> отбирает из всего дерева интерфейсов свой, что и
   является восстановлением его динамического типа, а вызов Accept() сообщает его конкретному
   акцептору, бросая управление в реализующий этот интерфейс метод где-то в линейной иерархии его
   узлов.
   Проблема тут в том, чтобы создать для visit-метода правильную сигнатуру. Пользователький код её
   не знает, т.к. типы абстрактных акцепторов генерируется самим прототипом. Поэтому приходится
   задействовать метакод для получения недостающих данных. */

// Тело visit-метода
#define MAKE_ACCEPTABLE_BODY(Proto, PList, Concrete)                                            \
      {                                                                                         \
        return static_cast<MultiMethods_Visitor::details::Acceptor<Concrete,                    \
                                                  typename Proto::ret_type>&>(a).Accept(this);  \
      }

// Объявление visit-метода в классе
#define MAKE_ACCEPTABLE_DECL(Proto, PList, Concrete)                                            \
      virtual typename Proto::ret_type Accept(typename Proto::template getArg<PList>::type &a);

// Внутриклассное определение visit-метода
#define MAKE_ACCEPTABLE(Proto, PList, Concrete)                                                 \
      virtual typename Proto::ret_type Accept(typename Proto::template getArg<PList>::type &a)  \
      MAKE_ACCEPTABLE_BODY(Proto, PList, Concrete)

// Внеклассное определение visit-метода
#define MAKE_ACCEPTABLE_IMPL(Proto, PList, Concrete)                                                    \
      typename Proto::ret_type Concrete::Accept(typename Proto::template getArg<PList>::type &a)\
      MAKE_ACCEPTABLE_BODY(Proto, PList, Concrete)

#endif // MULTIMETHODS_H_0A0E1659_FD72_4B5B_A091_EF59C8050584
