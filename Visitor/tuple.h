/****************************************************************\
**                          �������.                            **
** ����������� �� std �� ���� ������ ��� ��������� �� ��������. **
\****************************************************************/

#ifndef TUPLE_H_E689D195_5567_4BB4_8DFA_F4B111F69D92
#define TUPLE_H_E689D195_5567_4BB4_8DFA_F4B111F69D92

#include "tlist.h"

namespace Tuples
{

using Types_Lists::TList;
using Types_Lists::NullType;

/* ���� �������, ���� ����. ������ �������� ��������� ��� ���������� ��� �������������,
   ���� � �������� ������� T ����������. */
template <typename Hash, typename T> struct Field
{
  T&& data;

  Field(T&& t) : data(std::forward<T>(t)) {}
};

/* ��������� ������� �� ������ ������ ����� T.
   � �������� Hash ������������ ������� ���� ������ �����. */
template <typename T> struct Tuple;
template <typename L, typename R>
struct Tuple<TList<L, R> >: public Field<TList<L, R>, L>, public Tuple<R>
{
  typedef TList<L, R> TypeList;
  typedef L           DataType;
  typedef Tuple<R>    base_type;
  
  /* ������������ �����, ����� ��������� ����� ����� ����������� � ��������� ����. */
  Tuple(L&& l):                 Field<TList<L, R>, L>(std::forward<L>(l)) {}

  template <typename ...Args>
  Tuple(L&& l, Args&&... args): Field<TList<L, R>, L>(std::forward<L>(l)),
                                base_type(std::forward<Args>(args)...)    {}
};
template <>
struct Tuple<NullType>
{
};

/* ������������ ����������. */
namespace details
{

/* �������� ���� ������� Tpl �� ������ ���� I �� ������ */
template <int I, typename Tpl> struct GetTupleSlice
{
  typedef typename GetTupleSlice<I-1, typename Tpl::base_type>::type type;
};
template <typename Tpl>        struct GetTupleSlice<0, Tpl>
{
  typedef Tpl type;
};

} // details

/* �������� ���� I ������� Tpl */
template <int I, typename Tpl>
typename details::GetTupleSlice<I, Tpl>::type::DataType&& GetField(Tpl& tuple)
{
  typedef typename details::GetTupleSlice<I, Tpl>::type TupleSlice;

  return std::forward<typename TupleSlice::DataType>(
                static_cast<Field<typename TupleSlice::TypeList,
                                  typename TupleSlice::DataType>&>(tuple).data);
}

} // Tuples

#endif // TUPLE_H_E689D195_5567_4BB4_8DFA_F4B111F69D92
