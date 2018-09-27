/*****************************************************************\
** ������ �����. �������� �� ������� variadic templates � C++1x, **
**     ������� ������ ��������� ������� � ���������� ����,       **
**           ��� �������� ������ �� ����� �������.              **
\*****************************************************************/

#ifndef TLIST_H_E689D195_5567_4BB4_8DFA_F4B111F69D92
#define TLIST_H_E689D195_5567_4BB4_8DFA_F4B111F69D92

#include <type_traits>

namespace Types_Lists
{

/* ������������ ��� */
struct NullType {};

/* ������ �����.
   L - ������, ����� ���, ����� ������������� � ������.
   R - �����, ���� ������������ ���, ���� ������ */
template <typename L, typename R> struct TList
{
  typedef L Head;
  typedef R Tail;
};

/* �������� ������ ����� ���������. */
template <typename L, typename ...R> struct MakeTypeList
{
  typedef TList<L, typename MakeTypeList<R...>::type> type;
};
template <typename T> struct MakeTypeList<T>
{
  typedef TList<T, NullType> type;
};

template <typename L, typename ...R>
using MakeTList = typename MakeTypeList<L, R...>::type;

/* ��������� ������� �������� �� ������ ������ ����� T.
   ��� ���� �� ������ T ����������� � ������� Node. ������ ��� �������� - �������������������
   ��� Head �� ������, ������ - ��� ������������� �������� ������������.
   ��� ������������ Node<T::Head> �� ������� � ������� ������������ ���� �����, ���
   ������������ �� �������������� ��� ����������. ���������� ������� �������� ����� �.�.
   ��������� � ������ ���� ����� ���������� ���������� ������������ ��������� ��� ����������
   ���� �� ������. �� ���� (������ ������������ ������ GenHierarchy<>) ����� ���������� ���������
   �� ������ ���� ���� �������� ����������� ���������� �����. */
template <template <typename, typename> class Node, typename Ret, typename T> struct GenHierarchy;
template <template <typename, typename> class Node, typename Ret, typename L, typename R>
struct GenHierarchy<Node, Ret, TList<L, R> >: public Node<L, Ret>, public GenHierarchy<Node, Ret, R>
{
};
template <template <typename, typename> class Node, typename Ret>
struct GenHierarchy<Node, Ret, NullType>
{
};

/* ����������� ��������� ������ �������� ������ �� ������ ����� */
template <typename TL>            struct MostBase;
template <typename L, typename R> struct MostBase<TList<L, R> >
{
  typedef typename MostBase<R>::type                                                    nextType;
  typedef typename std::conditional<std::is_base_of<L, nextType>::value, L, nextType>::type type;
};
template <typename T>             struct MostBase<TList<T, NullType> >
{
  typedef T type;
};

/* ����������� ��������� ����� ������ */
template <typename TL>            struct Length;
template <typename L, typename R> struct Length<TList<L, R> >
{
  enum { value = Length<R>::value + 1 };
};
template <>                       struct Length<NullType>
{
  enum { value = 0 };
};

/* ����������� ��������� ���� �� ������ �� ��� ������ */
template <typename TL, int I>            struct GetType;
template <typename L, typename R, int I> struct GetType<TList<L, R>, I>
{
  typedef typename GetType<R, I-1>::type type;
};
template <typename L, typename R>        struct GetType<TList<L, R>, 0>
{
  typedef L type;
};

} // Types_Lists

#endif // TLIST_H_E689D195_5567_4BB4_8DFA_F4B111F69D92
