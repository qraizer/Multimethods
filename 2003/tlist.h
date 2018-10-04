/*******************************************************\
** ���᪨ ⨯��, �������騥 variadic templates � C++0x **
\*******************************************************/
#ifndef TLIST_H_CAD83881_D6D4_40ff_B020_BBB5523F6444
#define TLIST_H_CAD83881_D6D4_40ff_B020_BBB5523F6444
 
namespace MultiMethods_03
{
 
namespace Types_Lists
{
 
/* ��ନ����� ⨯ */
struct NullType {};
 
/* ���᮪ ⨯��.
  L - ������, �� ⨯, �஬� �ନ���쭮�� � ᯨ᪠.
  R - 墮��, ���� �ନ����� ⨯, ���� ᯨ᮪ */
template <typename L, typename R> struct TList
{
  typedef L Head;
  typedef R Tail;
};
 
/* ������� ���୮� ����娨 �� �᭮�� ᯨ᪠ ⨯�� T.
  �� ⨯� �� ᯨ᪠ T �ਬ������� � 蠡���� Node. ���� ��� ��ࠬ��� - ��ࠬ��ਧ�஢����
  ⨯ Head �� ᯨ᪠, ��ன - ⨯ �����頥���� ���祭�� ���⨬�⮤�.
  �� ����稢訥�� Node<T::Head> �� ����� � த�⢥ �⭮�⥫쭮 ��� ��㣠, ��
  ���ᯥ稢��� �� ࠢ���ࠢ����� �� ��४��⨨. ����稢�� ���ୠ� ������ �㤥� �.�.
  ᮤ�ঠ�� � ������ ��� ���� �������� ����䥩� ����ࠪ⭮�� ��楯�� ��� �⤥�쭮��
  ⨯� �� ᯨ᪠. �� ���� (ᠬ��� �ந�������� ����� GenHierarchy<>) ����� �������筮 ��������
  �� ��� 㧫� ���� ���筮�� ���室�饣� �ਢ������ ⨯��. */
template <template <typename, typename> class Node, typename Ret, typename T> struct GenHierarchy;
template <template <typename, typename> class Node, typename Ret, typename L, typename R>
struct GenHierarchy<Node, Ret, TList<L, R> >: public Node<L, Ret>, public GenHierarchy<Node, Ret, R>
{
};
template <template <typename, typename> class Node, typename Ret>
struct GenHierarchy<Node, Ret, NullType>
{
};
 
/* ��।������ ��᫥������� �� ��⮤� ���� ��஭�.
  ���� ��ࠬ��� - �।������⥫쭮 �ந������ �����, ��ன - �।������⥫쭮 ������. */
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
 
/* ����㭪�� ����祭�� ᠬ��� �������� ����� �� ᯨ᪠ ⨯�� */
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
 
/* ����㭪�� ����祭�� ����� ᯨ᪠ */
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
