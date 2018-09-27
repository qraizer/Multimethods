/**********************************************************\
** Кортежи. В C++03 в STL их нет, а BOOSTовые не подходят **
**                 ввиду иной архитектуры                 **
\**********************************************************/
#ifndef TUPLE_H_CAD83881_D6D4_40ff_B020_BBB5523F6444
#define TUPLE_H_CAD83881_D6D4_40ff_B020_BBB5523F6444

#include "tlist.h"
 
namespace MultiMethods_03
{
 
namespace Tuples
{
 
using Types_Lists::TList;
using Types_Lists::NullType;
 
/* Узел кортежа, одно поле. Первый параметр требуется для уникальной его идентификации,
  если в пределах кортежа T неуникален. */
template <typename Hash, typename T> struct Field
{
  T data;
 
  Field(const T& t) : data(t){}
};
 
/* Генератор кортежа на основе списка типов T.
  В качестве Hash используется текущий срез списка типов. */
template <typename T> struct Tuple;
template <typename L, typename R>
struct Tuple<TList<L, R> >: public Field<TList<L, R>, L>, public Tuple<R>
{
  typedef TList<L, R> TypeList;
  typedef L           DataType;
  typedef Tuple<R>    base_type;
 
  /* Конструкторы нужны, чтобы позволить полям иметь константные и ссылочные типы. */
 
  /* Создание кортежа разным количеством параметров. Без копипаста в C++03 никак, увы. */
  Tuple(const L&  t):                              Field<TList<L, NullType>, L>(t){}
  template <typename T1>
  Tuple(const L&  t0, const T1& t1):               Field<TList<L, R>, L>(t0),
                                                   base_type(const_cast<T1&>(t1)) {}
  template <typename T1, typename T2>
  Tuple(const L&  t0, const T1& t1, const T2& t2): Field<TList<L, R>, L>(t0),
                                                   base_type(const_cast<T1&>(t1), t2) {}
  template <typename T1, typename T2, typename T3>
  Tuple(const L&  t0, const T1& t1, const T2& t2, const T3& t3):
                                                   Field<TList<L, R>, L>(t0),
                                                   base_type(const_cast<T1&>(t1), t2, t3) {}
  template <typename T1, typename T2, typename T3, typename T4>
  Tuple(const L&  t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4):
                                                   Field<TList<L, R>, L>(t0),
                                                   base_type(const_cast<T1&>(t1), t2, t3, t4) {}
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  Tuple(const L&  t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4,
        const T5& t5):                             Field<TList<L, R>, L>(t0),
                                                   base_type(const_cast<T1&>(t1), t2, t3, t4, t5) {}
};
template <>
struct Tuple<NullType>
{
};
 
/* Получить срез кортежа Tpl по номеру поля I от начала */
template <int I, typename Tpl> struct GetTupleSlice
{
  typedef typename GetTupleSlice<I-1, typename Tpl::base_type>::type type;
};
template <typename Tpl>        struct GetTupleSlice<0, Tpl>
{
  typedef Tpl type;
};
 
/* Получить поле I кортежа Tpl */
template <int I, typename Tpl>
typename GetTupleSlice<I, Tpl>::type::DataType& GetField(Tpl& tuple)
{
  typedef typename GetTupleSlice<I, Tpl>::type TupleSlice;
 
  return static_cast<Field<typename TupleSlice::TypeList,
                           typename TupleSlice::DataType>&>(tuple).data;
}
 
} // Tuples
 
} // MultiMethods_03
 
#endif // TUPLE_H_CAD83881_D6D4_40ff_B020_BBB5523F6444
