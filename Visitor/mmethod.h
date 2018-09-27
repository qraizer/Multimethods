#ifndef MULTIMETHODS_H_0A0E1659_FD72_4B5B_A091_EF59C8050584
#define MULTIMETHODS_H_0A0E1659_FD72_4B5B_A091_EF59C8050584

#include "tlist.h"
#include "tuple.h"

namespace MultiMethods_Visitor
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

/* ���� ������� �������� ������������ ���������. ������������� ��������� Visitor-� */
template <typename T, typename Ret> class Acceptor
{
public:
  virtual Ret Accept(T*&& obj) = 0;
};

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

/********************************************************************************\
***************      ����������� ���������. ����� �� ��� �����.     **************
\********************************************************************************/

/* ������������ ��������. ������������ ����������� ����������� ���������.
   ������ �������� - ������ �����, ������ �������� ������� � ��� ����������� �� ���� ������,
                     ������� �������������� ���������� ���������� � �����������,
   ������          - ��� ������������� �������� ������������. */
template <typename TL, typename Ret> class LinkDynamic: public GenHierarchy<details::Acceptor, Ret, TL>
{
public:
  typedef TL TypeList;

  // ���������� ���������� ����������, ��������������� ������������ ��� t
  template <typename T, typename X>
  Ret apply(T* t, X&)
  {
    return t->Accept(*this);   // ������������ �������������� ���� ������ t
  }
  
  /* ��������� ���������� ����������, ����������� ������ ������ ����������� ����������.
     ����� �� ��� ������. */
  template <typename T,
            template <typename, typename, typename, typename, typename, typename> class GA,
            typename ATL, typename CTL, typename TPL, typename SRC>
  struct MakeAcceptor
  {
    typedef GA<T, TL, ATL, CTL, TPL, SRC> type;
  };

  /* ������������ details::MakeParamTypes<> ��� ��������� ���� ��������� ������������.
     ��� ������������� ��������� ��� ��������� �� ��� Head �� TL. */
  template <typename T> struct ParamType { typedef T* type; };
};

/* ����������� ��������. ������������ ���������� ����������� ���������.
   ������ �������� - ��������������� ��� ��������� ������������,
   ������          - ��� ������������� �������� ������������. */
template <typename Type, typename Ret> class LinkStatic
{
public:
  typedef TList<Type, NullType> TypeList;

  // ���������� ���������� ����������; ��������������� ������, ������ �������� ��������� ��������
  template <typename T, typename X>
  Ret apply(T&& t, X& acceptor)
  {
    return acceptor.Accept(std::forward<T>(t));
  }

  /* ����� �� ��������� ���������� ����������. */
  template <typename T,
            template <typename, typename, typename, typename, typename, typename> class GA,
            typename ATL, typename CTL, typename TPL, typename SRC>
  struct MakeAcceptor
  {
    typedef GA<T, TypeList, ATL, CTL, TPL, SRC> type;
  };

  /* ������������ details::MakeParamTypes<> ��� ��������� ���� ��������� ������������.
     ��� ������������ ��������� ��� rvalue ������ �� ��� Type. */
  template <typename T> struct ParamType { typedef T&& type; };
};

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

/* �������� ����������. ������ ���� ������ �� ����������, ����� ���������� ����������� ����� �����
   ������������� ����������� ���������� � ������������ visit-������� � ������� ������������.
   ������ �������� - ���������������� ����� � ������������ ���������� �������������,
   ������          - ��� ������������� ������������� ��������,
   ������...       - ���� ����������. */
template <typename DI, typename RetType, typename ...Args>
struct Prototype
{
protected:
  // ������������� ���� ���������� ������������ � ���������
  // ���������� ��� ������ ����������� ����������
  typedef typename details::MakeParams<RetType, Args...>::type TTl;

public:
  typedef RetType ret_type;

  /* �������� ��� ������������ ��������� �� ������ ����� ��������.
     ��������� � visit-������� ���������������� �������, ����� �� ���� ������� ���� ���������.
     � ������ ��� C++03 ����������� ��������� ����������� �������������, ��� �� ������ ��� ���������,
     ��� ��� ���������� ��������� ����� �����, � ����������� visit-����� �������. */
  template <typename TL>            struct getArg;
  template <typename H, typename T> struct getArg<TList<H, T>>
  {
    // ����� ������ ����� �������� � ���������� ������������
    template <typename X, typename Y, typename Tl>            struct findType;
    template <typename X, typename Y, typename L, typename R> struct findType<X, Y, TList<L, R>>
    {
      typedef typename findType<X, typename R::Head::TypeList, R>::type type;
    };
    template <typename X, typename L, typename R>             struct findType<X, X, TList<L, R>>
    {
      typedef L type;
    };
    // ����� LinkDynamic<> ������������ TList<> � TTl
    typedef typename findType<TList<H, T>, typename TTl::Head::TypeList, TTl>::type type;
  };
};
// ������������� ��� ������������������ �������������
template <typename DI, typename RetType> struct Prototype<DI, RetType> {};

/* ����������� ���������. ��������� ���� ��������. */
template <typename Proto> class Dispatcher;

template <typename DI, typename RetType, typename ...Args> 
class Dispatcher<Prototype<DI, RetType, Args...>>: Prototype<DI, RetType, Args...>
{
  using typename Prototype<DI, RetType, Args...>::TTl;

/********************************************************************************\
***************       ���������� ���������. ����� �� ��� �����      **************
\********************************************************************************/

  /* ���������� ���������. IterAcceptorCaller ������������ ��� ���������, ����� ����������,
     LastAcceptorCaller ������������ ��������� ��������. */
  template <typename T, typename ATL, typename CTL,
                        typename PTL, typename TPL, typename SRC> struct IterAcceptorCaller;
  template <typename T, typename ATL, typename CTL,
                        typename PTL, typename TPL, typename SRC> struct LastAcceptorCaller;

  /* ��������� ���������� ����������� ��������� �� ������ ���������� ����� ������ �����
     ����������� ����������. */
  template <typename T, typename ATL, typename PTL, typename TPL,
                        typename SRC>               struct AcceptorMaker;
  // ��� ������������� �������� ��������� ��������� ����� � ���������� ��������� ��������
  template <typename T, typename AL,  typename AR, typename PTL,
                        typename TPL, typename SRC> struct AcceptorMaker<T, TList<AL, AR>, PTL, TPL, SRC>
  {
    typedef typename AL::template MakeAcceptor<T, IterAcceptorCaller,
                                        TList<AL, AR>,       PTL, TPL, SRC>::type type;
  };
  // ��� ������������� �������� ��������� ��������� �����
  template <typename T, typename AL,  typename PTL, typename TPL,
                        typename SRC>               struct AcceptorMaker<T, TList<AL, NullType>, PTL, TPL, SRC>
  {
    typedef typename AL::template MakeAcceptor<T, LastAcceptorCaller,
                                        TList<AL, NullType>, PTL, TPL, SRC>::type type;
  };

  /* ���������� ���������� ����������. ����� ������� ����������� � ��������� �������� ��������
     �� ������ ������������ ���������. ��������� ���� � �������� �������� ����������� �� �����������
     ���� � ��������� ��������� ���������� Visitor-� �� ������� ��������. � ������������ ��� ��������
     �������� ����������� ��������� ��������� ��� ���������� ����� Visitor-�. ��������� ����
     ������������ ��������������� �� ������������ ���������, ������� ��� ���������� ��������������.
     � �����, �.�. � ����������, � ����������� ��������� �������� �� ������ ���������� ������� �����,
     ������������� ����������� ������ ���� �� ������� ��������, � ������ - � ���� ���� ��������
     ��������. � ���������� ������� ����������� ������ Accept() ������� ������ ����, ��������
     �������� ��� ��� � ������, ��� ����� ������������ ��� �����������������, � ��� ��������� �����
     ���� ������� � ���������������� ��� dynamic_cast<>. ��������������� ���� ������������� �
     ������ ��������������� �����. � ���������� ���������� ��������� ���������� ��������.
     ���������:
     - ��� ������������� ��������;
     - ���� ������ ����� �������� ������ ���������;
     - ���� ������ ����� ���������� ���������� (������, ��� ���������);
     - ������ ��������������� ������������ ����� ���������� ���������� (� �������� �������);
     - ���� ������ ����� ���������� ������������;
     - ��� ��������� ������� � ����������� doIt(). */

  /* ���������� ����������� ��������� ���������� ��������� ������������. �����������:
     - ���� ������ ����� ���������� ������������ �� ������������, ��� ��� �� ��� ����.
     - �������� ���������� ��������� ������ ���������� ����������� ���������. */

  // ��� ��� ��������� � ��������, ����� ������ ��������
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC>
  struct LastAcceptorCaller<RetType, TList<L, R>, CTL, PTL, TPL, SRC>:
                                        public LastAcceptorCaller<RetType, R, CTL, PTL, TPL, SRC>
  {
    typedef LastAcceptorCaller<RetType, R,  CTL, PTL, TPL, SRC> base_type;
    typedef typename CTL::Head::template ParamType<L>::type     arg_type;

    LastAcceptorCaller(SRC& ptl, TPL& src): base_type(ptl, src) {}

    // ��������������� ��� ���������� ��������� � �������� ������ ������������� �����������
    // ���������� ��� �������������� ������� ������� � ��������; ��������� �������� ��� ������
    // �������� � ������, �.�. ��� ��������� ����� ����������� ������� �����, ������
    // ��� ����� �������� ���� ������ � ��� ��������������� �����.
    RetType Accept(arg_type&& obj)
    {
      return CallConcrete<DI, RetType, Args...>::template apply<SRC, PTL, arg_type>
                                        ::call(base_type::m_Params, std::forward<arg_type>(obj));
    }
  };
  // ��� ������� � �������� ��������; ���� ������ ����� �������� ��������� ��� ��������� ������������
  // ��������, ��� ��� ���� � ���� ��� �� ��������, �� ������ ������ ����� ��� ��� ���������� �
  // ��������� ���������� Visitor-� �� ������������ ���������
  template <typename CTL, typename PTL, typename TPL, typename SRC>
  struct LastAcceptorCaller<RetType, NullType, CTL, PTL, TPL, SRC> : public CTL::Head
  {
    LastAcceptorCaller(SRC& ptl, TPL&): m_Params(ptl) {}

    SRC &m_Params;
  };

  /* ���������� ����������� ��������� ���� ���������� ������������, ����� ����������. �����������:
     - ������������, ���� ����������� ����� ����� 1 ���������. */
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC>
  struct IterAcceptorCaller<RetType, TList<L, R>, CTL, PTL, TPL, SRC> :
                                        public IterAcceptorCaller<RetType, R, CTL, PTL, TPL, SRC>
  {
    typedef IterAcceptorCaller<RetType, R,  CTL, PTL, TPL, SRC> base_type;
    typedef typename CTL::Head::template ParamType<L>::type     arg_type;

    IterAcceptorCaller(SRC& ptl, TPL& src): base_type(ptl, src) {}

    /* �������������� ���� ���������� ���������, ��������� ���������� ����������� ���������,
       �������� ��� ���������� ����� ������ ����������� ����������, ������� ����� ����������
       ������������ � ����������� ��������������� ���������� � ������������ ����� � ��� �����. */
    // �������� ��������, �� ��, ��� �������� ������ �� �����, ����� ���� ��� ��� ��� ����, �����
    // ������������ ��� ��� ��������� ������������; (������, ���� �� ����� ���� ������ ����������
    // ���������, � ��� ����� ���� ��� ��������� � ������ ��������������� �����); ���� ���������
    // �� ����� ����� � �������, �� ����� ������ ���� static_cast<>, ������� ����� �������
    // ���������� ���������
    RetType Accept(arg_type&&)
    {
      typedef decltype(GetField<0>(base_type::m_Args)) realType;
      typedef typename AcceptorMaker<RetType, typename CTL::Tail, TList<arg_type, PTL>,
                                              typename TPL::base_type, SRC>::type acceptor;

      acceptor a(base_type::m_Params, static_cast<typename TPL::base_type&>(base_type::m_Args));

      return a.apply(std::forward<realType>(GetField<0>(base_type::m_Args)), a);
    }
  };
  // ��� ������� � �������� ��������; ���� ������ ����� �������� ��������� ��� ��������� ������������
  // ��������, ��� ��� ���� � ���� ��� �� ��������, �� ������ ������ ����� ��� ��� ���������� �
  // ��������� ���������� Visitor-� �� ������������ ���������
  template <typename CTL, typename PTL, typename TPL, typename SRC>
  struct IterAcceptorCaller<RetType, NullType, CTL, PTL, TPL, SRC> : public CTL::Head
  {
    IterAcceptorCaller(SRC& ptl, TPL& src): m_Params(ptl), m_Args(src) {}

    SRC &m_Params;
    TPL &m_Args;
  };

  /* ���������� ����� �����. ������ �������� ������� ��������� � �������� ���.
     ��. �������� � IterAcceptorCaller::Accept(). */
  template <typename Tpl>
  static RetType doIt(Tpl& tpl)
  {
    typedef decltype(GetField<0>(tpl)) realType;
    typedef typename Tpl::base_type TupleType;
    typedef typename AcceptorMaker<RetType, TTl, NullType, TupleType, Tpl>::type acceptor;

    acceptor a(tpl, static_cast<TupleType&>(tpl));

    return a.apply(std::forward<realType>(GetField<0>(tpl)), a);
  }

  /* ������������� ����� ����� doIt().
     ��������� ������ ���� ��������� ����������: �������� � ���������� �������. ����� ����� �����
     ���������� ������ ���, � ��� ��������� ��������� � �������������. */
  RetType decay(typename details::MakeArg<Args>::type&&... args)
  {
    typedef Tuple<typename details::MakeParamTypes<TTl>::type> TupleType;

    TupleType tuple(std::forward<typename details::MakeArg<Args>::type>(args)...);
    return doIt(tuple);
  }

public:
  using typename Prototype<DI, RetType, Args...>::ret_type;

  /* ����� ������ ����������. ����������� ����� ������ ����� ����, �.�. ��� �� ������� ������
     ��������� � ����������� ����������� ������������. �� ������������ ���������� �� ���� ����,
     � ����� ���, ���� ����� �������� ������� ����, ���� ���������� ������� � ��������.
     � ������� �� ���������� ��� C++03, ��� �������� ��������� ������ ����������� ���������
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
template <typename DI, typename RetType>
struct Dispatcher<Prototype<DI, RetType>>
{
  RetType operator ()()
  {
    return CallConcrete<DI, RetType, NullType>::template apply<NullType, NullType, NullType>::call();
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

} // MultiMethods_Visitor

/* �����, ������� ������ ����� ����� ����������� ����� � ��������, �������� ����������� � ������������
   ����������. ���������� ��� ����� ��� ��������.
   ������ �������� - �������� ����������; �� ���� ����� ������� ������ ���������� ������������;
   ������ �������� - ������ ����� ��������, ������� ����������� ����������� �����, �� ����� ��������
                     � ������ ���������� � ���������;
   ������ �������� - ���, �� ������� ��������� this, �.�. ��� �����.
   ����� �������� visit-������� � �������� Visitor. �� �������� ���� ���������� ��������, ������
   ��������� (static_cast<>-��) � ���� ��������� ���� ��������� � ������� ���, ������� �.�. ����
   ������������ ���. ���������� static_cast<> �������� �� ����� ������ ����������� ����, ��� �
   �������� ��������������� ��� ������������� ����, � ����� Accept() �������� ��� �����������
   ���������, ������ ���������� � ����������� ���� ��������� ����� ���-�� � �������� �������� ���
   �����.
   �������� ��� � ���, ����� ������� ��� visit-������ ���������� ���������. ��������������� ��� �
   �� �����, �.�. ���� ����������� ���������� ������������ ����� ����������. ������� ����������
   ������������� ������� ��� ��������� ����������� ������. */

// ���� visit-������
#define MAKE_ACCEPTABLE_BODY(Proto, PList, Concrete)                                            \
      {                                                                                         \
        return static_cast<MultiMethods_Visitor::details::Acceptor<Concrete,                    \
                                                  typename Proto::ret_type>&>(a).Accept(this);  \
      }

// ���������� visit-������ � ������
#define MAKE_ACCEPTABLE_DECL(Proto, PList, Concrete)                                            \
      virtual typename Proto::ret_type Accept(typename Proto::template getArg<PList>::type &a);

// �������������� ����������� visit-������
#define MAKE_ACCEPTABLE(Proto, PList, Concrete)                                                 \
      virtual typename Proto::ret_type Accept(typename Proto::template getArg<PList>::type &a)  \
      MAKE_ACCEPTABLE_BODY(Proto, PList, Concrete)

// ����������� ����������� visit-������
#define MAKE_ACCEPTABLE_IMPL(Proto, PList, Concrete)                                                    \
      typename Proto::ret_type Concrete::Accept(typename Proto::template getArg<PList>::type &a)\
      MAKE_ACCEPTABLE_BODY(Proto, PList, Concrete)

#endif // MULTIMETHODS_H_0A0E1659_FD72_4B5B_A091_EF59C8050584
