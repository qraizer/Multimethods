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

/* ���������� ������ ����� � ������� ��������������� ��� �������������, � �� �������������
   ��� ��� ���� �� ����� �����. ���� �����, ����� ������� ����������� �� ����� ��� C++1x.
   ��� ��� ������������ ����� ������ ������� ��� � ����, ��� � � �������, ��� ������ �������� ���
   �����. */
using namespace Types_Lists;
using namespace Tuples;

/* ������������ ���������� (��������� ���������). */
namespace details
{

/* ������ ������ ����� ���������� "��������" ������������ �� ������
   ���������� ������������ ���������� � ����� ����������, � �������� ������ ��� operator().
   �������� ������ �����, ����������� �������� ����������� ����������� ����������, �� �����
   ������� ������ � ���� ���������. �������� ��� �� �������� ��� ���������� ����������
   ����������, ���������� ������ �� ������, �.�. ��� ����� ���� ��� � ������.*/
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

/* �����-������ ��� std::type_info, ����������� �������� � ���� � ��������� ��������.
   ������ ������ (������, ���������, ����� �� �������������� ����.�������� � ���������
   ����������� ������ �� �������������) �� ������������ std::type_info. ��� � ������
   ���������, �.�. ������� std::type_info ����� ����� ����� �� ����� ���������. */
class TypeInfo
{
  const std::type_info *info;
        int             ind;    // �������� ������ � ������ �����, ��� ����� ����������
                                // ���������� � ������� ������������
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
***************            ���������. ����� �� ��� �����.           **************
\********************************************************************************/

/* ������������ ��������. ������������ ����������� ����������� ���������.
   ������ �������� - ������ �����, ������ �������� ������� � ��� ����������� �� ���� ������,
                     ������� �������������� ���������� ���������� � �����������,
   ������          - ��� ������������� �������� ������������. */
template <typename TL, typename Ret> class LinkDynamic
{
public:
  typedef TL TypeList;

  /* ��������� ���������� ����������, ����������� ������ ������ ����������� ����������.
     ����� �� ��� ������: ���� ��� ���������� ��������� � ������ � ���� ��� ���������.
     ���� ��������� ������������ ����� ����� ������ ��� ��� ������ ����������� ����������,
     �� � ����� ����� �� ������������, ��� ��� ��� �������� ���������. */
  template <typename T,
            template <typename, typename, typename, typename, typename, typename, typename> class GA,
            typename ATL, typename CTL, typename TPL, typename SRC, typename SHT>
  struct MakeAcceptor
  {
    typedef GA<T, TypeList, ATL, CTL, TPL, SRC, SHT> type;
  };

  /* ������������ details::MakeParamTypes<> ��� ��������� ���� ��������� ������������.
     ��� ������������� ��������� ��� ��������� �� ��� Head �� TL. */
  template <typename T> struct ParamType { typedef T *type; };

  /* �������� RTTI ���������. ��� ������������� ��������� ������� �������� �� ���������,
     �������������� �������� ����� ������������, ����� ������� RTTI ���������. */
  template <typename T>
  static const std::type_info& deref(T* &&t)                                     { return typeid(*t); }
  /* ��� ����������� ����������� ���������� ��� ����� � �� ������� ��-�� ������ ������������,
     ��� ��� �������� �� ������ ������������ �����. */
  static       bool            check(const TypeInfo& l, const std::type_info& r) { return l == r;     }
};

/* ����������� ��������. ������������ ���������� ����������� ���������.
   ������ �������� - ��������������� ��� ��������� ������������,
   ������          - ��� ������������� �������� ������������. */
template <typename Type, typename Ret> class LinkStatic
{
public:
  typedef TList<Type, NullType> TypeList;

  /* ����� �� ��������� ���������� ����������.
     ��������������, ��� ��� � ������������ ���������� ����� �������, ������ �� �����������.
     ��������� ��� ���� �� ������ ������ ��� ������� �����������. */
  template <typename T,
            template <typename, typename, typename, typename, typename, typename, typename> class GA,
            typename ATL, typename CTL, typename TPL, typename SRC, typename SHT>
  struct MakeAcceptor
  {
    typedef GA<T, TypeList, ATL, CTL, TPL, SRC, SHT> type;
  };

  /* ������������ details::MakeParamTypes<> ��� ��������� ���� ��������� ������������.
     ��� ������������ ��������� ��� rvalue ������ �� ��� Type. */
  template <typename T> struct ParamType { typedef T&& type; };

  /* �������� RTTI ���������. ��� ������������ ��������� ������� �������� �� rvalue ������,
     �������������� �������������� ������ �� ����, � rvalue ref �������� ����� cv-�������������,
     ������� �� ���� ���������, ��� ������ RTTI ��������� ����� ���������� RTTI �� ��� cv- ��� ref,
     ������� ������ �� ����������. */
  template <typename T>
  static const std::type_info& deref(T &&t)                                  { return typeid(t); }
  /* � ������ ����� ������ ���� ���, � ��� ��������������, ����� �����, ����� ��������
     ��� ��� ����������. */
  static       bool            check(const TypeInfo&, const std::type_info&) { return true;      }
};

/********************************************************************************\
**************      �������� ���������� VMT, ���������� �� RTTI.     *************
**************                           ����� I                     *************
\********************************************************************************/

/* ���������� ������� �� ����������� RTTI-������. �������� ����� ���� �� ���������� �� ����������,
   ������ ������ ��� ��� ����������, �.�. �������� ��������� �������� �� ������ �����. ����� ����,
   ������� ��������� ��� ����� ���� ������������� ������ � C++17, �.�. ����� fold-expression
   � ����� �� ����.
   ������ �������� - ������������� ��� ���������� ��������; ��� ����� ���� �� �� �������� � ������
                     ����������, ������������� ������ ��������� ����������, �� ����� ��������
                     ����� ���� �� ���������� ������ ��������� ��������������, ������ ���, ���
                     ������� ��������� ������������� �� �������������, � ������ �������������
                     ���� �� ��������, ���� ��������� ��������� ��������, �� ��� ����� makeSheet()
                     ����� ���� �� ������� ������� Sheet, ��� ����� ������������� ���������
                     ����������;
   ������ �������� - ������ � ������ ����� � �� �� ��������� ������ � ������; ��������� ����� ����
                     ������ ����� ���������� �������, ��� ��� ����� � �������� �������� � ������
                     ���� ���������� ��������� � ����� ���������� RTTI-������;
   ������ �������� - ������ ����������� RTTI-������. */
template <typename Tl, typename T, size_t N> void makeSheet(Tl, int idx, T (&row)[N])
{
  row[idx] = TypeInfo(typeid(typename Tl::Head), idx);
  makeSheet(typename Tl::Tail(), idx+1, row);
}
// ���� ��������. ����� ������� assert ��� �������� N... ���� �� ��������.
template<typename T, size_t N> void makeSheet(NullType, int, T (&)[N]) {}

/* �����-������ ��� ��������� ����������� RTTI-������.
   ������ �� ������� �� ������ �������� ������������: ������ ��� ���������� ��������� ����� �
   ������� ������ ������� ��� ��������. */
template <typename Tl>            class Sheet;
template <typename L, typename R> class Sheet<TList<L, R>>: public Sheet<R>
{
  TypeInfo row[Length<typename L::TypeList>::value];

public:
  typedef Sheet<R> base_type;

  Sheet()               // ������� ������ ��� ��������� � ������������� ���� �������
  {
    makeSheet(typename L::TypeList(), 0, row);  // ������ ������
    std::sort(std::begin(row), std::end(row));  // � ���������
  }

  // �������� �������. ����������� ����� ���������� ����������� ������ �� ������ �����������
  // RTTI-������� ��������� �����������.
  // ����� ���� �� ������������ ����������� row[], ������ ������� ��� �������������, � ������
  // row[] ����������� ������, �.�. �� ���������������� � ���� ������������, � �� ���������������.
  const TypeInfo (&get()const)[Length<typename L::TypeList>::value] { return row; }
};
template <> class Sheet<NullType>{};

/********************************************************************************\
*************        ��������� ������ � ����������� ����������.       ************
*******       ���, <type_traits> �� �������: ��� ����, � ��� �������       *******
\********************************************************************************/

/* �������������� ���� ��������� ���������� � ��� ��� ���������. */

// ���������� ����������� ���������. ���� ���������.
template <typename T> struct MakeArg
{
  typedef T type;
};

// ����������� ����������� ���������.
// ������ ����� ������������ � cv-������������������� ��� ��������� �� ����� ������� �����.
template <typename L, typename R> struct MakeArg<TList<L, R>>;                  // �������� �� ����������
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
// ���������� ��� ����� ���� cv-����������������� �������� ��� &&, �� ���� ��� �����


/* ����� �������� ��� ��������� ����������� ��� ���������� */

// ���������� ����������� ���������. �������� ������������ ����������.
template <typename T, typename RT> struct MakeParam
{
  typedef details::LinkStatic<T, RT> type;
};

// ����������� ����������� ���������. �������� ������������� ����������.
// ��� �������� ��� � � MakeArg<>.
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

/* ��������� ������ ����� ������� ��� ���� ���������� ����������.
   RT - return type, �������� � ������ ������ ���������� ��� ��������������� � ����������. */
template <typename RT, typename T, typename ...List> struct MakeParams
{
  typedef TList<typename MakeParam<T, RT>::type, typename MakeParams<RT, List...>::type> type;
};
template <typename RT, typename T>                   struct MakeParams<RT, T>
{
  typedef TList<typename MakeParam<T, RT>::type, NullType> type;
};

/* ������������� ���������, ����������� ����������.
   ����� ��������� �������������, �.�. ������ �������� �� ��������� ����������, ���� ��������
   ������ ����� ������ ����������� �����. � ����� �� �������������� MostBase<>, ������ ����� �
   ������� ��������� ����� ��� ��������� ���������� ���������. */

// ���������� ����������� ��������. ������ �������.
template <typename A>             struct mkref
{
  template <typename T>
  static T&& doIt(T&& t) { return std::forward<T>(t); }
};

// ����������� ����������� ��������. ����� ������ cv-������������� � ������������� � ���������.
// ��� �������� ��� � � MakeArg<>.
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

/* ������������� ���������, ����������� ����������.
   ����� ��������� �������������, �.�. ������ �������� �� ��������� ����������, ���� ��������
   ������ ����� ������ ����������� ����� + ��� �� ����� �������� cv- � ptr/rev �������������.
   � ����� �� �������������� MostBase<>, ������ ����� � ������� ��������� ����� ��� ���������
   ���������� ���������. */

// ���������� ����������� ��������. �� ����� ���� �����, ��� ���� ������ �������. �� ����������
// ������, ��� ��� ������ �������� ��� ������� ����� rvalue � rvalue ref, � ���������� �������,
// ����������� ������������� �� ��������, ����������� ������ ����, ����� ������������.
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
// ��� ���� ����� ���� ���-���
template <typename X>            struct dcref<X&&>
{
  template <typename T>
  static T&&               doIt(T&& t){return std::forward<T>(t);}
};

// ����������� ����������� ��������. ����� ������� �������� ��� cv- � ptr/ref �������������.
// ��� �������� ��� � � MakeArg<>.
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
*****************              ����������� ���������              ****************
\********************************************************************************/

/* ���������� ��������� */
template <typename, typename, typename ...> class CallConcrete;

/* ������ �������� - ���������������� ����� � ������������ ���������� �������������,
   ������    - ��� ������������� ������������� ��������,
   ������... - ���� ����������. */
template <typename DI, typename ret_type, typename ...Args>
class Dispatcher
{
  // ������������� ���� ���������� ������������ � ���������
  typedef typename details::MakeParams<ret_type, Args...>::type TTl;
  typedef          details::Sheet<TTl>                   sheet_type;    // ��� RTTI-"�������"

  // ������� ����������� RTTI-������, �������� RTTI ���� ������� ������� ��������� �� ...Args
  const sheet_type sheet;

  /* ���������. IterXXX ������������ ��� ���������, ����� ����������, LastXXX ������������
     ��������� ��������. YYYYStart ����� ��� CRTP: ������ ���� YYYYAcceptorCaller ����������
     � ������� YYYYStart, ��� ������ �� ������� ������ ��������������� �����. */
  template <typename T, typename ATL, typename CTL, typename PTL,
                        typename TPL, typename SRC, typename SHT>               struct IterStart;
  template <typename T, typename ATL, typename CTL, typename PTL,
                        typename TPL, typename SRC, typename SHT, typename MD>  struct IterAcceptorCaller;
  template <typename T, typename ATL, typename CTL, typename PTL,
                        typename TPL, typename SRC, typename SHT>               struct LastStart;
  template <typename T, typename ATL, typename CTL, typename PTL,
                        typename TPL, typename SRC, typename SHT, typename MD>  struct LastAcceptorCaller;

  /* ��������� ���������� ��������� �� ������ ���������� ����� ������ ����� ����������. */
  template <typename T, typename ATL, typename PTL, typename TPL,
                        typename SRC, typename SHT>               struct AcceptorMaker;
  // ��� ������������� �������� ��������� ��������� ����� � ��������������� ��������� ���
  template <typename T, typename AL,  typename AR, typename PTL,
                        typename TPL, typename SRC, typename SHT> struct AcceptorMaker
                                                        <T, TList<AL, AR>, PTL, TPL, SRC, SHT>
  {
    typedef typename AL::template MakeAcceptor<T, IterStart,
                                        TList<AL, AR>,       PTL, TPL, SRC, SHT>::type type;
  };
  // ��� ������������� �������� ��������� ��������� �����
  template <typename T, typename AL,  typename PTL, typename TPL,
                        typename SRC, typename SHT>               struct AcceptorMaker
                                                        <T, TList<AL, NullType>, PTL, TPL, SRC, SHT>
  {
    typedef typename AL::template MakeAcceptor<T, LastStart,
                                        TList<AL, NullType>, PTL, TPL, SRC, SHT>::type type;
  };

/********************************************************************************\
**************      �������� ���������� VMT, ���������� �� RTTI.     *************
**************                         ����� II.                     *************
\********************************************************************************/

  /* ���������� ����������. ����� ������� ����������� � ��������� �������� �������� ����������
     �� ������ ������ ����� �������� ��������� (��� ���������� ����������� ���������� ������
     ������� �� ������ ����), ���������� ������� ������� ���� ����� ���������� ��� ������� ����
     � ������, �������������� ������������� ���� ��������� �� ��� RTTI � ������ ���� ���������,
     ������� ����������� ����� ������, ��� ���� ������������ ����� �� �������� ������� ������� �
     ��������� (����������������) ������� RTTI-�������.
     ���������:
     - ��� ������������� ��������;
     - ���� ������ ����� �������� ������ ���������;
     - ���� ������ ����� ���������� ����������;
     - ������ ��������������� ������������ ����� ���������� ���������� (� �������� �������);
     - ���� ������ ����� ���������� ������������;
     - ��� ��������� ������� � ����������� doIt();
     - ���� ��� RTTI-"�������";
     - ����� ����������� �������� (YYYYStart). */

  /* ���������� ��������� ���������� ��������� ������������. �����������:
     - ���� ������ ����� ���������� ������������ �� ������������, ��� ��� �� ��� ����.
     - �������� ���������� ��������� ������ ���������� ���������. */

  // ��� ��� ��������� � ��������, ����� ������ ��������
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC,
            typename SHT, typename MD>
  struct LastAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT, MD>:
                        public LastAcceptorCaller<ret_type, R, CTL, PTL, TPL, SRC, SHT, MD>
  {
    typedef LastAcceptorCaller<ret_type, R,  CTL, PTL, TPL, SRC, SHT, MD> base_type;
    typedef typename CTL::Head::template ParamType<L>::type               arg_type;

    // ����������� ������ �������� �� ���������� �� ������ Accept()
    template <typename ...A>
    LastAcceptorCaller(SRC& ptl, TPL& src, const SHT& sht, A&&... args):
        base_type(ptl, src, sht, std::forward<A>(args)...,
                static_cast<ret_type (MD::*)(param_type)>(&LastAcceptorCaller::Accept)) {}

    // �������������� ���� ���������� ��������� � ����� ������ ������������� ����������� ����������
    typedef typename MostBase<typename CTL::Head::TypeList>::type   most_base;
    typedef typename CTL::Head::template ParamType<most_base>::type param_type;
    ret_type Accept(param_type obj)
    {
      arg_type arg = static_cast<arg_type>(obj);        // �������������� ���

      // ������� ���������� ���������; �.�. ��������������� ���� � PTL ����� � �������� �������,
      // ��������� ��������������� ��� ������ ���� ��������� ����������� �������, �������
      // ��������� �������� �� RTL � �������.
      return CallConcrete<DI, ret_type, Args...>::template apply<SRC, PTL, arg_type>::
                                        call(base_type::m_Params, std::forward<arg_type>(arg));
    }
  };
  // ��� ������� � �������� ��������; ���� ������ ����� �������� ��������� ��� ��������� ������������
  // ��������, ��� ��� ���� � ���� ��� �� ��������, �� ������ ����������� ���������� VMT
  template <typename CTL, typename PTL, typename TPL, typename SRC, typename SHT, typename MD>
  struct LastAcceptorCaller<ret_type, NullType, CTL, PTL, TPL, SRC, SHT, MD>
  {
    typedef typename MostBase<typename CTL::Head::TypeList>::type   most_base;
    typedef typename CTL::Head::template ParamType<most_base>::type param_type;

    /* ���������� ������� ������� ��� �������������� ���� ����������� ������������ ���������.
       ����������, ��� ���������� VMT. ��� ���������� ������������ ��������� ������ �������
       �� ������ ��������.
       � C++17 � ����� ���� ����� ����� ���� ���������� ����� ����������� fold-expression.
       ���� ���������� ��� ��������� ����������.

       template <std::size_t ...Ints, typename ...Args>
       void getArgs(std::index_sequence<Ints...>, Args ...args)
       {
         ((accepts[Ints] = args), ...);
       }

       � ����� ������ C++1x ���������� ������ ���������, ��������������� �����������. */
    template <int I, typename ...A> void getArgs(ret_type (MD::*at)(param_type), A&&... args)
    {
      accepts[I] = at;
      getArgs<I+1>(std::forward<A>(args)...);
    }
    template <int I>                void getArgs(ret_type (MD::*at)(param_type))
    {
      accepts[I] = at;
    }

    // ����������� ������ ���������� VMT; ������������� �������� ���� ����� ��������� � fold-expression
    template <typename ...A>
    LastAcceptorCaller(SRC& ptl, TPL&, const SHT&, A&&... args): m_Params(ptl)
    {
      getArgs<0>(/*std::index_sequence_for<Args...>(), */std::forward<A>(args)...);
    }

    SRC &m_Params;      // �������� ������
    MD  *self;          // LastStart
    // ���������� VMT - ������ ������� Accept();
    // � ������� �� RTTI-"�������" sheet, � ������� ��������� ������������ ��� ���� � ������
    // ������ ��������� � run-time, ��� ������������ LastAcceptorCaller � ����������
    ret_type (MD::*accepts[Length<typename CTL::Head::TypeList>::value])(param_type);
  };

  // ����� ����������� � �������� ��������; ����� ��� ���������� CRTP, �.�. ������ ��� ��� ������
  // �� ������� �������-���������� ����� ������ � ������� ������������ ���������������� ������
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC, typename SHT>
  struct LastStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>:
                public LastAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT,
                                          LastStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>>
  {
    typedef LastAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT,
                LastStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>> base_type;

    // ���� self ����� � ����� ������� ���������
    LastStart(SRC& ptl, TPL& src, const SHT& sht): base_type(ptl, src, sht) { this->self = this; }
  };

  /* ���������� ��������� ���� ���������� ������������, ����� ����������. �����������:
     - ������������, ���� ����������� ����� ����� 1 ���������. */

  // ��� ��� ��������� � ��������, ����� ������ ��������
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC,
            typename SHT, typename MD>
  struct IterAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT, MD> :
                        public IterAcceptorCaller<ret_type, R, CTL, PTL, TPL, SRC, SHT, MD>
  {
    typedef IterAcceptorCaller<ret_type, R,  CTL, PTL, TPL, SRC, SHT, MD> base_type;
    typedef typename CTL::Head::template ParamType<L>::type               arg_type;

    // ����������� ������ �������� �� ���������� �� ������ Accept()
    template <typename ...A>
    IterAcceptorCaller(SRC& ptl, TPL& src, const SHT& sht, A&&... args):
        base_type(ptl, src, sht, std::forward<A>(args)...,
                  static_cast<ret_type (MD::*)(param_type)>(&IterAcceptorCaller::Accept)) {}

    /* �������������� ���� ���������� ���������, ��������� ���������� ���������, �������� ���
       ���������� ����� ������ ����������, ������� ����� ���������� ������������, RTTI-������� ���
       � ����������� ��������������� ���������� � ������������ ����� � ��� �����. */
    typedef typename MostBase<typename CTL::Head::TypeList>::type   most_base;
    typedef typename CTL::Head::template ParamType<most_base>::type param_type;
    ret_type Accept(param_type arg)
    {
      typedef typename SHT::base_type SheetType;
      typedef typename AcceptorMaker<ret_type, typename CTL::Tail, TList<arg_type, PTL>,
                                               typename TPL::base_type, SRC, SheetType>::type acceptor;
      // ��������� ��������
      acceptor a(base_type::m_Params, static_cast<typename TPL::base_type&>(base_type::m_Args),
                 static_cast<const SheetType&>(base_type::m_Sht));
      // ����� � RTTI-������� ��������, ���������������� ������������� ���� ���������� � ���������
      // ���������, � ��������� ��� ������� � �������� ������ �����, ����������� ��� ��������
      auto idx = std::lower_bound(std::begin(base_type::m_Sht.get()), std::end(base_type::m_Sht.get()),
                                  details::TypeInfo(CTL::Tail::Head::deref(GetField<0>(base_type::m_Args))));

      // �.�. ����� �� ������ ������ ������ ���������� � ����� ������ ������� ����� � �������������
      // ����������, ��� ��� ���� �� ����� �����, ����� �������������� ��������� � ���, ��� �������
      // ������� ������; � ��������� ������ ��� �������� ������ � �������� ������ ����� � ��������
      if ( idx == std::end(base_type::m_Sht.get()) ||
          !CTL::Tail::Head::check(*idx, CTL::Tail::Head::deref(GetField<0>(base_type::m_Args))))
        throw std::invalid_argument("IterAcceptorCaller<> error: unlisted parameter");
      // ����������� ���������� VMT, ������� Accept() ���� ���������, ��� ������ ������ �����
      // � RTTI-�������, � ��������� �������� ������������, �.�. �������������� ��� ���;
      // � ���������� ����������� � ��������� ���������� ���������
      return (a.self ->* a.accepts[idx->index()])(GetField<0>(base_type::m_Args));
    }
  };
  // ��� ������� � �������� ��������; ���� ������ ����� �������� ��������� ��� ��������� ������������
  // ��������, ��� ��� ���� � ���� ��� �� ��������, �� ������ ����������� ���������� VMT
  template <typename CTL, typename PTL, typename TPL, typename SRC, typename SHT, typename MD>
  struct IterAcceptorCaller<ret_type, NullType, CTL, PTL, TPL, SRC, SHT, MD>
  {
    typedef typename MostBase<typename CTL::Head::TypeList>::type   most_base;
    typedef typename CTL::Head::template ParamType<most_base>::type param_type;

    // ���������� ������� ������� ��� �������������� ���� ����������� ������������ ���������.
    // ��. ����� ������� � ��������� ������ � LastAcceptorCaller.
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

    // �����������, ������ ���������� VMT
    template <typename ...A>
    IterAcceptorCaller(SRC& ptl, TPL& src, const SHT& sht, A&&... args):
                                                        m_Params(ptl), m_Args(src), m_Sht(sht)
    {
      getArgs<0>(std::forward<A>(args)...);
    }

    SRC &m_Params;              // �������� ������
    TPL &m_Args;                // ���� ������� �� ������� ��������
    const SHT &m_Sht;           // ���� RTTI-"�������"

    MD *self;                   // IterStart
    // ���������� VMT - ������ ������� Accept();
    // ��. ����� ������� � ��������� ������� � LastAcceptorCaller
    ret_type (MD::*accepts[Length<typename CTL::Head::TypeList>::value])(param_type);
  };

  // ����� ����������� � �������� ��������; ����� ��� ���������� CRTP, ��. ������� � LastStart
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC, typename SHT>
  struct IterStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT> :
                        public IterAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT,
                                                  IterStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>>
  {
    typedef IterAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT,
                IterStart<ret_type, TList<L, R>, CTL, PTL, TPL, SRC, SHT>> base_type;

    // ���� self ����� � ����� ������� ���������
    IterStart(SRC& ptl, TPL& src, const SHT& sht): base_type(ptl, src, sht) { this->self = this; }
  };

  /* ���������� ����� �����. ������ �������� ������� ��������� � �������� ���.
     ��. �������� � IterAcceptorCaller::Accept(). */
  template <typename Tpl>
  ret_type doIt(Tpl& tpl)
  {
    typedef typename Tpl::base_type        TupleType;   // ���� �������
    typedef typename sheet_type::base_type SheetType;   // ���� RTTI-"�������"
    typedef typename AcceptorMaker<ret_type, TTl, NullType, TupleType, Tpl, SheetType>::type acceptor;

    acceptor a(tpl, static_cast<TupleType&>(tpl), static_cast<const SheetType&>(sheet));
    auto idx = std::lower_bound(std::begin(sheet.get()), std::end(sheet.get()),
                                details::TypeInfo(TTl::Head::deref(GetField<0>(tpl))));

    if ( idx == std::end(sheet.get()) ||
        !TTl::Head::check(*idx, TTl::Head::deref(GetField<0>(tpl))))
      throw std::invalid_argument("doIt() error: unlisted parameter");
    return (a.self ->* a.accepts[idx->index()])(GetField<0>(tpl));
  }

  /* ������������� ����� ����� doIt().
     ��������� ������ ���� ��������� ����������: �������� � ���������� �������. ����� ����� �����
     ���������� ������ ���, � ��� ��������� ��������� � �������������. */
  ret_type decay(typename details::MakeArg<Args>::type&&... args)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl>::type> TupleType;

    TupleType tuple(std::forward<typename details::MakeArg<Args>::type>(args)...);
    return doIt(tuple);
  }

public:
  /* ����� ������ ����������. ����������� ����� ������ ����� ����, �.�. ��� �� ������� ������
     ��������� � ����������� ����������� ������������. �� ������������ ���������� �� ���� ����,
     � ����� ���, ���� ����� �������� ������� ����, ���� ���������� ������� � ��������.
     � ������� �� ���������� ��� C++03, ��� �������� ��������� ������ ���������� ���������
     ������������ ����� ����������� ���������� ����� ���������� ����������. */
  template <typename ...A>
  ret_type operator ()(A&&... args)
  {
    // ������������� ����������, � ���� ������� ����������� ����������� �������� � ���������� ��
    // cv-������������������� ��������, � ����������� �������� ������������ ���������� ����������
    // ����������. �����, ����� static_cast<> �������� ���� � rvalue ref. ��-������, ��� �� ���
    // ��� ��������� ����� ���� ����� ���, ��� ������� �� ���������� (��������, ���� explicit
    // �������������), ��-������, �� ������ ����������� ����������� ��������� � �� ������� �������,
    // ���, ������ ������, �������� ��� ������� �����������.
    return decay(static_cast<typename details::MakeArg<Args>::type&&>(
                                details::mkref<Args>::doIt(std::forward<A>(args))) ...);
  }
};

/* ��� ����������� ������ - � ������������ ��������� �����������. */
template <typename DI, typename ret_type>
struct Dispatcher<DI, ret_type>
{
  ret_type operator ()()
  {
    return CallConcrete<DI, ret_type, NullType>::template apply<NullType, NullType, NullType>::call();
  }
};

/*******************************************************************************\
*****************              ���������� ���������              ****************
\*******************************************************************************/

/* ������ �������� - ���������������� ����� � ������������ ���������� �������������,
   ������          - ��� ������������� ������������� ��������,
   ������          - �������� �������� ���������� ����������, �� �������� ����� ����� ���������
                     ������������� �����������. */
template <typename UI, typename Ret, typename ...Args>
struct CallConcrete
{
  /* �������������� ������� ������� � �������� � ������ ��������������� ������������ �����.
     ������������ ��� �������� �������, ����� ����������, ������� ������ �������� � ��� ���������.
       SRC - �������� ������;
       PL  - ������ ��������������� ����� (� �������� �������);
       Head- ��������� ��������������� ���.
     � ��� ���� ����������� �� fold-expression �� C++17. */
  template <typename SRC, typename PL, typename Head> struct apply
  {
    // ������������ ��������� ������� �������
    //   pl   - �������� ������;
    //   data - ��������� �������� ������������; � ����� �� ���������, �.�. ��������� ��� �������,
    //          � ��� ��� ��� �������;
    //   args - ��� ������������ ��������� (��� ������ ������ ����)
    template <typename ...A>
    static Ret call(SRC& pl, Head&& data, A&& ...args)
    {
      // ����� �� ������� ��������� ������� (� �������� �������), ������������� � ����������������
      // ���� �� PL � �������� ���������� ��������������� ������ � ��������� ���������� � ��������
      // ��������; ��� ���������� ����������� ��������
      return apply<SRC, typename PL::Tail, Head>::call(pl, std::forward<Head>(data),
                static_cast<typename PL::Head>(GetField<Length<PL>::value-1>(pl)),
                std::forward<A>(args)...);
    }
  };
  /* ���� ��������, ����� ��������� ������� ������ ��� (��� � �� ����).
     ������������ ��� �������� �������, ����� ����������.
       SRC - �������� ������;
       PL  - ������ ��������������� ����� (� �������� �������);
       Head- ��������� ��������������� ���.
     � ��� ���� ����������� �� fold-expression �� C++17. */
  template <typename SRC, typename Head>              struct apply<SRC, NullType, Head>
  {
    // ���������� ���������� apply<>::call(); ��� ��������� �������������, �������� ������
    // ��������� �� ����� ��������� ��������...
    // ������ ������, ��� ��� ������ ����� ���� �� ���������� � ����, ���� �� �� ����, ��� �����
    // ���������� ...Args � ...A ������; ������� ������� �� sizeof... �������������
    template <typename ...A>
    static Ret call(SRC& pl, Head&& data, A&& ...args)
    {
      return   call(std::forward<A>(args)..., std::forward<Head>(data));
    }
    // ...��������� ������������� ������� �� ��� � ������������ � ��� ��������� �� ...Args
    // � ������� ���������������� ���������� �����������
    template <typename ...A>
    static Ret call(A&& ...args)
    {
      return UI::apply(details::dcref<Args>::doIt(std::forward<A>(args))...);
    }
    // ��������� ������������� ��� ������������������ �������������
    static Ret call()
    {
      return UI::apply();
    }
  };
};

} // MultiMethods_RTTI

#endif // MULTIMETHODS_H_E689D195_5567_4BB4_8DFA_F4B111F69D92
