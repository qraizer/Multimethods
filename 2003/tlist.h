/*******************************************************\
** Списки типов, заменяющие variadic templates в C++0x **
\*******************************************************/
#ifndef TLIST_H_CAD83881_D6D4_40ff_B020_BBB5523F6444
#define TLIST_H_CAD83881_D6D4_40ff_B020_BBB5523F6444
 
namespace MultiMethods_03
{
 
namespace Types_Lists
{
 
/* Терминальный тип */
struct NullType {};
 
/* Список типов.
  L - голова, любой тип, кроме терминального и списка.
  R - хвост, либо терминальный тип, либо список */
template <typename L, typename R> struct TList
{
  typedef L Head;
  typedef R Tail;
};
 
/* Генератор веерной иерархии на основе списка типов T.
  Все типы из списка T применяются к шаблону Node. Первый его параметр - параметризированный
  тип Head из списка, второй - тип возвращаемого значения мультиметода.
  Все получившиеся Node<T::Head> не состоят в родстве относительно друг друга, что
  обеспечивает их равноправность при перекрытии. Получившая веерная иерархия будет т.о.
  содержать в каждом луче веера декларацию интерфейса абстрактного акцептора для отдельного
  типа из списка. От базы (самого производного класса GenHierarchy<>) можно однозначно добраться
  до любого узла путём обычного восходящего приведения типов. */
template <template <typename, typename> class Node, typename Ret, typename T> struct GenHierarchy;
template <template <typename, typename> class Node, typename Ret, typename L, typename R>
struct GenHierarchy<Node, Ret, TList<L, R> >: public Node<L, Ret>, public GenHierarchy<Node, Ret, R>
{
};
template <template <typename, typename> class Node, typename Ret>
struct GenHierarchy<Node, Ret, NullType>
{
};
 
/* Определение наследования по методу Рани Шарони.
  Первый параметр - предположительно производный класс, второй - предположительно базовый. */
template <typename D, typename B>
class IsDerived
{
  typedef char (&small)[1];
  typedef char (&big)  [2];
 
  template <typename T>
  static small check(const volatile D&, T);
  static big   check(const volatile B&, int);
 
  struct Proxy
  {
    operator const volatile B&() const;
    operator const volatile D&();
  };
 
public:
  enum { value = sizeof(check(Proxy(), 0)) == 1 /* small */ };
};
 
/* compile-time if() */
template <typename D, typename B, bool> struct If             { typedef D type; };
template <typename D, typename B>       struct If<D, B, true> { typedef B type; };
 
/* Метафункция получения самого базового класса из списка типов */
template <typename TL>            struct MostBase;
template <typename L, typename R> struct MostBase<TList<L, R> >
{
  typedef typename If<L, typename MostBase<R>::type,
            IsDerived<L, typename MostBase<R>::type>::value>::type type;
};
template <typename T>             struct MostBase<TList<T, NullType> >
{
  typedef T type;
};
 
/* Метафункция получения длины списка */
template <typename TL>            struct Length;
template <typename L, typename R> struct Length<TList<L, R> >
{
  enum { value = Length<R>::value + 1 };
};
template <>                       struct Length<NullType>
{
  enum { value = 0 };
};
 
} // Types_Lists
 
} // MultiMethods_03
 
#endif // TLIST_H_CAD83881_D6D4_40ff_B020_BBB5523F6444
