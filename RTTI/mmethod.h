#ifndef MULTIMETHODS_H_E689D195_5567_4BB4_8DFA_F4B111F69D92
#define MULTIMETHODS_H_E689D195_5567_4BB4_8DFA_F4B111F69D92

#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <utility>
#
#include "tlist.h"
#include "tuple.h"

namespace MultiMethods_RTTI
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

/* Класс-обёртка над std::type_info, позволяющая работать с ними в семантике значений.
   Хранит ссылку (точнее, указатель, чтобы не заморачиваться спец.методами и позволить
   компилятору самому их сгенерировать) на оригинальный std::type_info. Это в рамках
   Стандарта, т.к. объекты std::type_info имеют время жизни до конца программы. */
class TypeInfo
{
  const std::type_info *info;
        int             ind;    // исходный индекс в списке типов, ибо после сортировки
                                // экземпляры в массиве перемешаются
public:
           TypeInfo(): /* DefaultConstructable */         info(&typeid(void)), ind(0)   {}
  explicit TypeInfo(const std::type_info& ti, int idx=0): info(&ti),           ind(idx) {}

  bool operator==(const      TypeInfo&  ti) const { return *info == *ti.info; }
  bool operator!=(const      TypeInfo&  ti) const { return *info != *ti.info; }
  bool operator==(const std::type_info& ti) const { return *info ==  ti;      }
  bool operator!=(const std::type_info& ti) const { return *info !=  ti;      }
  bool operator< (const      TypeInfo&  ti) const { return info->before(*ti.info); }
  bool operator< (const std::type_info& ti) const { return info->before( ti);      }
  int  index() const { return ind; }
};

/********************************************************************************\
***************            Акцепторы. Всего их две штуки.           **************
\********************************************************************************/

/* Динамический акцептор. Обрабатывает динамически связываемые параметры.
   Первый параметр - список типов, должен включать базовый и все производные от него классы,
                     которые предполагается передавать аргументом в мультиметод,
   второй          - тип возвращаемого значения мультиметода. */
template <typename TL, typename Ret> class LinkDynamic
{
public:
  typedef TL TypeList;

  /* Генератор конкретных акцепторов, реализующих чистые методы абстрактных акцепторов.
     Всего их два разных: один для последнего акцептора в списке и один для остальных.
     Этот генератор теоретически может иметь разный вид для разных абстрактных акцепторов,
     но в итоге этого не понадобилось, так что они выглядят одинаково. */
  template <typename T,
            template <typename, typename, typename, typename, typename, typename, typename> class GA,
            typename ATL, typename CTL, typename TPL, typename SRC, typename SHT>
  struct MakeAcceptor
  {
    typedef GA<T, TypeList, ATL, CTL, TPL, SRC, SHT> type;
  };

  /* Используется details::MakeParamTypes<> для получения типа параметра мультиметода.
     Для динамического акцептора это указатель на тип Head из TL. */
  template <typename T> struct ParamType { typedef T *type; };

  /* Получить RTTI параметра. Для динамического акцептора объекты хранятся по указателю,
     соответственно параметр нужно разыменовать, иначе вернётся RTTI указателя. */
  template <typename T>
  static const std::type_info& deref(T* &&t)                                     { return typeid(*t); }
  /* Для динамически связываемых параметров тип может и не найтись из-за ошибки программиста,
     так что проверка на точное соответствие важна. */
  static       bool            check(const TypeInfo& l, const std::type_info& r) { return l == r;     }
};

/* Статическый акцептор. Обрабатывает статически связываемые параметры.
   Первый параметр - непосредственно тип аргумента мультиметода,
   второй          - тип возвращаемого значения мультиметода. */
template <typename Type, typename Ret> class LinkStatic
{
public:
  typedef TList<Type, NullType> TypeList;

  /* Такой же генератор конкретных акцепторов.
     Предполагалось, что они с динамическим акцептором будут разными, однако не пригодилось.
     Оставлено как есть на всякий случай для будущих модификаций. */
  template <typename T,
            template <typename, typename, typename, typename, typename, typename, typename> class GA,
            typename ATL, typename CTL, typename TPL, typename SRC, typename SHT>
  struct MakeAcceptor
  {
    typedef GA<T, TypeList, ATL, CTL, TPL, SRC, SHT> type;
  };

  /* Используется details::MakeParamTypes<> для получения типа параметра мультиметода.
     Для статического акцептора это rvalue ссылка на тип Type. */
  template <typename T> struct ParamType { typedef T&& type; };

  /* Получить RTTI параметра. Для статического акцептора объекты хранятся по rvalue ссылке,
     соответственно разыменовывать ничего не надо, а rvalue ref скрывает любые cv-квалификаторы,
     поэтому не надо опасаться, что вместо RTTI параметра будет возвращено RTTI на его cv- или ref,
     главное только не форвардить. */
  template <typename T>
  static const std::type_info& deref(T &&t)                                  { return typeid(t); }
  /* В списке типов только один тип, а его несоответствие, ежели вдруг, будет выявлено
     ещё при компиляции. */
  static       bool            check(const TypeInfo&, const std::type_info&) { return true;      }
};

/********************************************************************************\
**************      Создание внутренней VMT, основанной на RTTI.     *************
**************                           Часть I                     *************
\********************************************************************************/

/* Построение массива на экземплярах RTTI-класса. Рекурсию можно было бы переписать на вариадиках,
   однако сейчас это уже невозможно, т.к. исходные вариадики заменены на список типов. Кроме того,
   исходно вариадики тут могут быть задействованы только с C++17, т.к. ранее fold-expression
   в языке не было.
   первый параметр - исключительно для завершения рекурсии; его можно было бы не включать в список
                     аргументов, ограничившись только шаблонным параметром, но тогда рекурсию
                     можно было бы остановить только частичной специализацией, однако увы, для
                     функций частичных специализаций не предусмотрено, а полная специализация
                     была бы возможна, если исключить последний параметр, но для этого makeSheet()
                     нужно было бы сделать методом Sheet, где явные специализации запрещены
                     Стандартом;
   второй параметр - индекс в списке типов и он же начальный индекс в массие; последний может быть
                     изменён после сортировки массива, так что связь с исходным индексом в списке
                     типа приходится сохранять в самом экземпляре RTTI-класса;
   третий параметр - массив экземпляров RTTI-класса. */
template <typename Tl, typename T, size_t N> void makeSheet(Tl, int idx, T (&row)[N])
{
  row[idx] = TypeInfo(typeid(typename Tl::Head), idx);
  makeSheet(typename Tl::Tail(), idx+1, row);
}
// Стоп рекурсии. Можно вписать assert для контроля N... если вы параноик.
template<typename T, size_t N> void makeSheet(NullType, int, T (&)[N]) {}

/* Класс-обёртка над массивами экземпляров RTTI-класса.
   Хранит по массиву на каждый параметр мультиметода: массив для следующего параметра лежит в
   базовом классе массива для текущего. */
template <typename Tl>            class Sheet;
template <typename L, typename R> class Sheet<TList<L, R>>: public Sheet<R>
{
  TypeInfo row[Length<typename L::TypeList>::value];

public:
  typedef Sheet<R> base_type;

  Sheet()               // базовые классы уже построили и отсортировали свои массивы
  {
    makeSheet(typename L::TypeList(), 0, row);  // строим массив
    std::sort(std::begin(row), std::end(row));  // и сортируем
  }

  // Страшная строчка. Константный метод возвращает константную ссылку на массив экземпляров
  // RTTI-классов известной размерности.
  // Можно было бы ограничиться публикацией row[], однако хочется его константности, а делать
  // row[] константным нельзя, т.к. он инициализируется в теле конструктора, а не инициализатором.
  const TypeInfo (&get()const)[Length<typename L::TypeList>::value] { return row; }
};
template <> class Sheet<NullType>{};

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

/* Первый параметр - пользовательский класс с реализациями перекрытых мультиметодов,
   второй    - тип возвращаемого мультиметодом значения,
   третий... - типы параметров. */
template <typename DI, typename ret_type, typename ...Args>
class Dispatcher
{
  // преобразовать типы параметров мультиметода в аргументы
  typedef typename details::MakeParams<ret_type, Args...>::type TTl;
  typedef          details::Sheet<TTl>                   sheet_type;    // тип RTTI-"матрицы"

  // массивы экземпляров RTTI-класса, хранящие RTTI всех классов каждого параметра из ...Args
  const sheet_type sheet;

  /* Акцепторы. IterXXX обрабатывает все параметры, кроме последнего, LastXXX обрабатывает
     последний параметр. YYYYStart нужен для CRTP: методы всех YYYYAcceptorCaller приводятся
     к методам YYYYStart, что делает их имеющих единый квалифицирующий класс. */
  template <typename T, typename ATL, typename CTL, typename PTL,
                        typename TPL, typename SRC, typename SHT>               struct IterStart;
  template <typename T, typename ATL, typename CTL, typename PTL,
                        typename TPL, typename SRC, typename SHT, typename MD>  struct IterAcceptorCaller;
  template <typename T, typename ATL, typename CTL, typename PTL,
                        typename TPL, typename SRC, typename SHT>               struct LastStart;
  template <typename T, typename ATL, typename CTL, typename PTL,
                        typename TPL, typename SRC, typename SHT, typename MD>  struct LastAcceptorCaller;

  /* Генератор очередного акцептора на основе очередного среза списка типов акцепторов. */
  template <typename T, typename ATL, typename PTL, typename TPL,
                        typename SRC, typename SHT>               struct AcceptorMaker;
  // эта специализация является очередной итерацией цикла и восстанавливает очередной тип
  template <typename T, typename AL,  typename AR, typename PTL,
                        typename TPL, typename SRC, typename SHT> struct AcceptorMaker
                                                        <T, TList<AL, AR>, PTL, TPL, SRC, SHT>
  {
    typedef typename AL::template MakeAcceptor<T, IterStart,
                                        TList<AL, AR>,       PTL, TPL, SRC, SHT>::type type;
  };
  // эта специализация является последней итерацией цикла
  template <typename T, typename AL,  typename PTL, typename TPL,
                        typename SRC, typename SHT>               struct AcceptorMaker
                                                        <T, TList<AL, NullType>, PTL, TPL, SRC, SHT>
  {
    typedef typename AL::template MakeAcceptor<T, LastStart,
                                        TList<AL, NullType>, PTL, TPL, SRC, SHT>::type type;
  };

/********************************************************************************\
**************      Создание внутренней VMT, основанной на RTTI.     *************
**************                         Часть II.                     *************
\********************************************************************************/

  /* Реализация акцепторов. Общий принцип заключается в генерации линейной иерархии акцепторов
     на основе списка типов иерархии параметра (для статически связываемых параметров список
     состоит из одного типа), заполнении массива методов всех таких акцепторов для каждого типа
     в списке, восстановлении динамического типа параметра по его RTTI и вызову того акцептора,
     который сопоставлен этому классу, для чего используются связь по индексам массива методов и
     исходного (несортированного) массива RTTI-классов.
     Параметры:
     - тип возвращаемого значения;
     - срез списка типов иерархии своего параметра;
     - срез списка типов параметров диспетчера;
     - список восстановленных динамических типов предыдущих параметров (в обратном порядке);
     - срез списка типов параметров мультиметода;
     - тип исходного кортежа с параметрами doIt();
     - срез тип RTTI-"матрицы";
     - самый производный акцептор (YYYYStart). */

  /* Реализация акцептора последнего параметра мультиметода. Особенности:
     - срез списка типов параметров мультиметода не используется, ибо тут он уже пуст.
     - вызывает конкретный диспетчер вместо следующего акцептора. */

  // это все апцепторы в иерархии, кроме самого базового
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC,
            typename SHT, typename MD>
  struct LastAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT, MD>:
                        public LastAcceptorCaller<ret_type, R, CTL, PTL, TPL, SRC, SHT, MD>
  {
    typedef LastAcceptorCaller<ret_type, R,  CTL, PTL, TPL, SRC, SHT, MD> base_type;
    typedef typename CTL::Head::template ParamType<L>::type               arg_type;

    // конструктор строит вариадик из указателей на методы Accept()
    template <typename ...A>
    LastAcceptorCaller(SRC& ptl, TPL& src, const SHT& sht, A&&... args):
        base_type(ptl, src, sht, std::forward<A>(args)...,
                static_cast<ret_type (MD::*)(param_type)>(&LastAcceptorCaller::Accept)) {}

    // восстановление типа последнего параметра и вызов нужной специализации конкретного диспетчера
    typedef typename MostBase<typename CTL::Head::TypeList>::type   most_base;
    typedef typename CTL::Head::template ParamType<most_base>::type param_type;
    ret_type Accept(param_type obj)
    {
      arg_type arg = static_cast<arg_type>(obj);        // восстановленый тип

      // вызвать конкретный диспетчер; т.к. восстановленные типы в PTL лежат в обратном порядке,
      // последний восстановленный тип должен быть обработан специальным образом, поэтому
      // передаётся отдельно от RTL и кортежа.
      return CallConcrete<DI, ret_type, Args...>::template apply<SRC, PTL, arg_type>::
                                        call(base_type::m_Params, std::forward<arg_type>(arg));
    }
  };
  // это базовый в иерархии акцептор; весь список типов иерархии параметра уже обработан производными
  // классами, так что этот с ними уже не работает, он только обслуживает внутреннюю VMT
  template <typename CTL, typename PTL, typename TPL, typename SRC, typename SHT, typename MD>
  struct LastAcceptorCaller<ret_type, NullType, CTL, PTL, TPL, SRC, SHT, MD>
  {
    typedef typename MostBase<typename CTL::Head::TypeList>::type   most_base;
    typedef typename CTL::Head::template ParamType<most_base>::type param_type;

    /* Построение массива методов для восстановления типа динамически связываемого параметра.
       Фактически, это внутренняя VMT. Для статически связываемого параметра массив состоит
       из одного элемента.
       В C++17 и позже этот метод может быть реализован проще посредством fold-expression.
       Ниже приводится его возможная реализация.

       template <std::size_t ...Ints, typename ...Args>
       void getArgs(std::index_sequence<Ints...>, Args ...args)
       {
         ((accepts[Ints] = args), ...);
       }

       В более ранних C++1x приходится делать рекурсией, останавливаемой перегрузкой. */
    template <int I, typename ...A> void getArgs(ret_type (MD::*at)(param_type), A&&... args)
    {
      accepts[I] = at;
      getArgs<I+1>(std::forward<A>(args)...);
    }
    template <int I>                void getArgs(ret_type (MD::*at)(param_type))
    {
      accepts[I] = at;
    }

    // конструктор строит внутреннюю VMT; закоменченный параметр тоже имеет отношение к fold-expression
    template <typename ...A>
    LastAcceptorCaller(SRC& ptl, TPL&, const SHT&, A&&... args): m_Params(ptl)
    {
      getArgs<0>(/*std::index_sequence_for<Args...>(), */std::forward<A>(args)...);
    }

    SRC &m_Params;      // исходный кортеж
    MD  *self;          // LastStart
    // внутренняя VMT - массив методов Accept();
    // в отличие от RTTI-"матрицы" sheet, у каждого параметра мультиметода она своя и потому
    // должна строиться в run-time, чем конструкторы LastAcceptorCaller и занимаются
    ret_type (MD::*accepts[Length<typename CTL::Head::TypeList>::value])(param_type);
  };

  // самый производный в иерархии акцептор; нужен для реализации CRTP, т.к. только тут все методы
  // из базовых классов-акцепьтров можно свести к единому производному квалифицирующему классу
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC, typename SHT>
  struct LastStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>:
                public LastAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT,
                                          LastStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>>
  {
    typedef LastAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT,
                LastStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>> base_type;

    // поле self лежит в самом базовом акцепторе
    LastStart(SRC& ptl, TPL& src, const SHT& sht): base_type(ptl, src, sht) { this->self = this; }
  };

  /* Реализация акцептора всех параметров мультиметода, кроме последнего. Особенности:
     - используется, если мультиметод имеет более 1 параметра. */

  // это все апцепторы в иерархии, кроме самого базового
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC,
            typename SHT, typename MD>
  struct IterAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT, MD> :
                        public IterAcceptorCaller<ret_type, R, CTL, PTL, TPL, SRC, SHT, MD>
  {
    typedef IterAcceptorCaller<ret_type, R,  CTL, PTL, TPL, SRC, SHT, MD> base_type;
    typedef typename CTL::Head::template ParamType<L>::type               arg_type;

    // конструктор строит вариадик из указателей на методы Accept()
    template <typename ...A>
    IterAcceptorCaller(SRC& ptl, TPL& src, const SHT& sht, A&&... args):
        base_type(ptl, src, sht, std::forward<A>(args)...,
                  static_cast<ret_type (MD::*)(param_type)>(&IterAcceptorCaller::Accept)) {}

    /* восстановление типа очередного параметра, генерация следующего акцептора, передача ему
       следующего среза списка акцепторов, базовых типов параметров мультиметода, RTTI-матрицы итп
       и накопленной восстановленной информации о динамических типах и его вызов. */
    typedef typename MostBase<typename CTL::Head::TypeList>::type   most_base;
    typedef typename CTL::Head::template ParamType<most_base>::type param_type;
    ret_type Accept(param_type arg)
    {
      typedef typename SHT::base_type SheetType;
      typedef typename AcceptorMaker<ret_type, typename CTL::Tail, TList<arg_type, PTL>,
                                               typename TPL::base_type, SRC, SheetType>::type acceptor;
      // следующий акцептор
      acceptor a(base_type::m_Params, static_cast<typename TPL::base_type&>(base_type::m_Args),
                 static_cast<const SheetType&>(base_type::m_Sht));
      // поиск в RTTI-массиве элемента, соответствующего динамическому типу пришедшего в диспетчер
      // аргумента, и получение его индекса в исходном списке типов, описывающем его иерархию
      auto idx = std::lower_bound(std::begin(base_type::m_Sht.get()), std::end(base_type::m_Sht.get()),
                                  details::TypeInfo(CTL::Tail::Head::deref(GetField<0>(base_type::m_Args))));

      // т.к. поиск не обязан давать точное совпадение и может только указать место в сортированном
      // контейнере, где ему было бы самое место, нужно самостоятельно убедиться в том, что элемент
      // успешно найден; в противном случае это означает ошибку в описании списка типов в иерархии
      if ( idx == std::end(base_type::m_Sht.get()) ||
          !CTL::Tail::Head::check(*idx, CTL::Tail::Head::deref(GetField<0>(base_type::m_Args))))
        throw std::invalid_argument("IterAcceptorCaller<> error: unlisted parameter");
      // индексируем внутреннюю VMT, вызывая Accept() того акцептора, чей индекс вернул поиск
      // в RTTI-массиве, и передавая аргумент мультиметода, т.с. восстанавливая его тип;
      // в результате итерируемся к обработке следующего параметра
      return (a.self ->* a.accepts[idx->index()])(GetField<0>(base_type::m_Args));
    }
  };
  // это базовый в иерархии акцептор; весь список типов иерархии параметра уже обработан производными
  // классами, так что этот с ними уже не работает, он только обслуживает внутреннюю VMT
  template <typename CTL, typename PTL, typename TPL, typename SRC, typename SHT, typename MD>
  struct IterAcceptorCaller<ret_type, NullType, CTL, PTL, TPL, SRC, SHT, MD>
  {
    typedef typename MostBase<typename CTL::Head::TypeList>::type   most_base;
    typedef typename CTL::Head::template ParamType<most_base>::type param_type;

    // Построение массива методов для восстановления типа динамически связываемого параметра.
    // См. также коммент к подобному методу в LastAcceptorCaller.
    template <int I, typename ...A>
    void getArgs(ret_type (MD::*at)(param_type), A&&... args)
    {
      accepts[I] = at;
      getArgs<I+1>(std::forward<A>(args)...);
    }
    template <int I>
    void getArgs(ret_type (MD::*at)(param_type))
    {
      accepts[I] = at;
    }

    // конструктор, строит внутреннюю VMT
    template <typename ...A>
    IterAcceptorCaller(SRC& ptl, TPL& src, const SHT& sht, A&&... args):
                                                        m_Params(ptl), m_Args(src), m_Sht(sht)
    {
      getArgs<0>(std::forward<A>(args)...);
    }

    SRC &m_Params;              // исходный кортеж
    TPL &m_Args;                // срез кортежа по текущий аргумент
    const SHT &m_Sht;           // срез RTTI-"матрицы"

    MD *self;                   // IterStart
    // внутренняя VMT - массив методов Accept();
    // см. также коммент к подобному массиву в LastAcceptorCaller
    ret_type (MD::*accepts[Length<typename CTL::Head::TypeList>::value])(param_type);
  };

  // самый производный в иерархии акцептор; нужен для реализации CRTP, см. коммент к LastStart
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC, typename SHT>
  struct IterStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT> :
                        public IterAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT,
                                                  IterStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>>
  {
    typedef IterAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT,
                IterStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>> base_type;

    // поле self лежит в самом базовом акцепторе
    IterStart(SRC& ptl, TPL& src, const SHT& sht): base_type(ptl, src, sht) { this->self = this; }
  };

  /* Обобщённая точка входа. Создаёт акцептор первого параметра и вызывает его.
     См. комменты к IterAcceptorCaller::Accept(). */
  template <typename Tpl>
  ret_type doIt(Tpl& tpl)
  {
    typedef typename Tpl::base_type        TupleType;   // срез кортежа
    typedef typename sheet_type::base_type SheetType;   // срез RTTI-"матрицы"
    typedef typename AcceptorMaker<ret_type, TTl, NullType, TupleType, Tpl, SheetType>::type acceptor;

    acceptor a(tpl, static_cast<TupleType&>(tpl), static_cast<const SheetType&>(sheet));
    auto idx = std::lower_bound(std::begin(sheet.get()), std::end(sheet.get()),
                                details::TypeInfo(TTl::Head::deref(GetField<0>(tpl))));

    if ( idx == std::end(sheet.get()) ||
        !TTl::Head::check(*idx, TTl::Head::deref(GetField<0>(tpl))))
      throw std::invalid_argument("doIt() error: unlisted parameter");
    return (a.self ->* a.accepts[idx->index()])(GetField<0>(tpl));
  }

  /* Промежуточный метод перед doIt().
     Выполняет вторую фазу обработки параметров: создание и заполнение кортежа. После этого этапа
     вариадиков больше нет, а все параметры прочеканы и предобработны. */
  ret_type decay(typename details::MakeArg<Args>::type&&... args)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl>::type> TupleType;

    TupleType tuple(std::forward<typename details::MakeArg<Args>::type>(args)...);
    return doIt(tuple);
  }

public:
  /* Точка вызова диспетчера. Аргументами могут прийти любые типы, т.к. они не обязаны строго
     совпадать с формальными параметрами мультиметода. Их соответствие проеряется по ходу дела,
     и ежели что, либо будет выполнен неявный каст, либо компилятор сообщит о проблеме.
     В отличие от реализации для C++03, тут возможно сохранить полную Стандатную семантику
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
template <typename DI, typename ret_type>
struct Dispatcher<DI, ret_type>
{
  ret_type operator ()()
  {
    return CallConcrete<DI, ret_type, NullType>::template apply<NullType, NullType, NullType>::call();
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

} // MultiMethods_RTTI

#endif // MULTIMETHODS_H_E689D195_5567_4BB4_8DFA_F4B111F69D92
