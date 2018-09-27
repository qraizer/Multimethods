#ifndef MULTIMETHODS_H_CAD83881_D6D4_40FF_B020_BBB5523F6444
#define MULTIMETHODS_H_CAD83881_D6D4_40FF_B020_BBB5523F6444

#include "tlist.h"
#include "tuple.h"

namespace MultiMethods_03
{

/* ���������� ������ ����� � ������� ��������������� ��� �������������, � �� �������������
   ��� ��� ���� �� ����� �����. ���� �����, ����� ������� ����������� �� ����� ��� C++11.
   ��� ��� ������������ ����� ������ ������� ��� � ����, ��� � � �������, ��� ������ �������� ���
   �����. */
using namespace Types_Lists;
using namespace Tuples;

/* ���� ������� �������� ������������ ��������� */
template <typename T, typename Ret> class Acceptor
{
public:
  virtual Ret Accept(T* obj) = 0;
};

namespace details
{

/* ������ ������ ����� ���������� "��������" ������������ �� ������
   �r�������� ������������ ���������� � ����� ����������, � �������� ������ ��� operator().
   ��������� ������� ������� ��� ��������� ���������������� ����������. */
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
***************      ����������� ���������. ����� �� ��� �����.     **************
\********************************************************************************/


/* ������������ ��������. ������������ ����������������� ���������.
   ������ �������� - ������ �����, ������ �������� ������� � ��� ����������� �� ���� ������,
                     ������� �������������� ���������� ���������� � �����������,
   ������          - ��� ������������� �������� ������������. */
template <typename TL, typename Ret> class LinkDynamic: public GenHierarchy<Acceptor, Ret, TL>
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

/* ����������� ��������. ������������ ���������������� ���������.
   ������ �������� - ��������������� ��� ��������� ������������,
   ������          - ��� ������������� �������� ������������. */
template <typename Type, typename Ret> class LinkStatic
{
public:
  typedef TList<Type, NullType> TypeList;

  // ���������� ���������� ����������; ��������������� ������, ������ �������� ��������� ��������
  template <typename T, typename X>
  Ret apply(T& t, X& acceptor)
  {
    return acceptor.Accept(t);
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
     ��� ������������ ��������� ��� ����������� ������ �� ��� Type. */
  template <typename T> struct ParamType { typedef const T& type; };
};

/********************************************************************************\
*****************              ����������� ���������              ****************
\********************************************************************************/

/* ���������� ��������� */
template <typename, typename> class CallConcrete;

/* ������ �������� - ���������������� ����� � ������������ ���������� �������������,
   ������ - ������ ����� ����������� ���������� ��� ������� ��������� ������������,
   ������ - ��� ������������� ������������� ��������. */
template <typename DI, typename TTl, typename ret_type = void>
struct Dispatcher
{
  /* ���������� ��������� ���� ������������� � ���������� ���������� ������������
     ��������������. ��, �� ����� ���. */
  template <typename T, typename ATL, typename CTL,
                        typename PTL, typename TPL, typename SRC> struct IterAcceptorCaller;
  template <typename T, typename ATL, typename CTL,
                        typename PTL, typename TPL, typename SRC> struct LastAcceptorCaller;

  /* ��������� ���������� ����������� ��������� �� ������ ���������� ����� ������ �����
     ���������� ����������. */
  template <typename T, typename ATL, typename PTL, typename TPL,
                        typename SRC>               struct AcceptorMaker;
  // ��� ������������� �������� ��������� ��������� ����� � ��������������� ��������� ���
  template <typename T, typename AL,  typename AR, typename PTL,
                        typename TPL, typename SRC> struct AcceptorMaker<T, TList<AL, AR>, PTL, TPL, SRC>
  {
    typedef typename AL::template MakeAcceptor<T, IterAcceptorCaller,
                                        TList<AL, AR>,       PTL, TPL, SRC>::type type;
  };
  // ��� ������������� �������� ��������� ��������� �����;
  template <typename T, typename AL,  typename PTL, typename TPL,
                        typename SRC>               struct AcceptorMaker<T, TList<AL, NullType>, PTL, TPL, SRC>
  {
    typedef typename AL::template MakeAcceptor<T, LastAcceptorCaller,
                                        TList<AL, NullType>, PTL, TPL, SRC>::type type;
  };

  /* ���������� ���������� ����������. ����� ������� ����������� � ��������� �������� ��������
     �� ������ ������������ ���������. ��������� ���� � �������� �������� ����������� �� �����������
     ���� � ��������� ������ ����� ���������� ������������ ��������� �� ������� ��������. ���������
     ���� ������������ ��������������� �� ������������ ���������.
     � �����, �.�. � ����������, � ����������� ��������� �������� �� ������ ���������� ������� �����,
     ������������� ����������� ������ ���� �� ������� ��������, � ������ - � ���� ���� ��������
     ��������. � ���������� ������� ����������� ������ Accept() ������� ������ ����, ��������
     �������� ��� ��� � ������, ��� ����� ������������ ��� �����������������, � ��� ��������� �����
     ���� ������� � ���������������� ��� dynamic_cast<>. ��������������� ���� ������������� �
     ������ ��������������� �����. � ���������� ���������� ��������� ���������� ��������.
     ���������:
     - ��� ������������� ��������;
     - ���� ������ ����� �������� ������ ���������;
     - ���� ������ ����� ����������� ����������;
     - ������ ��������������� ������������ ����� ���������� ����������;
     - ���� ������ "�������" ����� ���������� ������������;
     - ��� ��������� ������� � ����������� doIt(). */

  /* ���������� ����������� �������� ���������� ��������� ������������. �����������:
     - ���� ������ "�������" ����� ���������� ������������ �� ������������, ��� ��� �� ��� ����.
     - �������� ���������� ��������� ������ ���������� ����������� ���������. */
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC>
  struct LastAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC>:
                                        public LastAcceptorCaller<ret_type, R, CTL, PTL, TPL, SRC>
  {
    typedef LastAcceptorCaller<ret_type, R,  CTL, PTL, TPL, SRC> base_type;
    typedef typename CTL::Head::template ParamType<L>::type       arg_type;

    LastAcceptorCaller(SRC& ptl, TPL& src): base_type(ptl, src) {}

    /* ��������������� ��� ���������� ��������� � �������� ������ ������������� �����������
       ����������, � ����������� �� ����� ������ ���������� ������������. */
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

  /* ���������� ����������� ��������� �������������� ��������� ������������. �����������:
     - �� ������������, ���� ����������� ����� ������ 1 ��������. */
  template <typename L, typename R, typename CTL, typename PTL, typename TPL, typename SRC>
  struct IterAcceptorCaller<ret_type, TList<L, R>, CTL, PTL, TPL, SRC> : public IterAcceptorCaller<ret_type, R, CTL, PTL, TPL, SRC>
  {
    typedef IterAcceptorCaller<ret_type, R,  CTL, PTL, TPL, SRC> base_type;
    typedef typename CTL::Head::template ParamType<L>::type       arg_type;

    IterAcceptorCaller(SRC& ptl, TPL& src): base_type(ptl, src) {}

    /* ��������������� ��� ���������� ���������, ���������� ��������� ���������� ��������,
       ��������� ��� ��������� ���� ������ ����������� ���������� � "�������" ����� ���������� �
       ����������� ��������������� ���������� � �������� ���. */
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

  /* ���������� ����� �����. ������ ���������� �������� ������� ��������� � �������� ���. */
  template <typename Tpl>
  static ret_type doIt(Tpl& tpl)
  {
    typedef typename Tpl::base_type TupleType;
    typedef typename AcceptorMaker<ret_type, TTl, NullType, TupleType, Tpl>::type acceptor;

    acceptor a(tpl, static_cast<TupleType&>(tpl));

    return a.apply(GetField<0>(tpl), a);
  }
  /* ����������������� ����� ����� ��� ������������������ ������������� */
  static ret_type doIt(Tuple<NullType>&)
  {
    return CallConcrete<DI, ret_type>::template apply<NullType, NullType, -1>::call();
  }

  /* ��, ��� ����� ���������. ���������� ����� ����� ��� ������������� � ������ ����������� ����������.
     ���������� ����� ����� � C++03 ��� ����������. �� ���� �� ������� ���������� "�������" ����,
     ������� ��� ������ ����� �������� �� ������ ����������, ��� �������� ����������,
     ���� operator() ����� �����������, �.�. � ���� ������, ����������� ��������, ���������� �����
     �������� �������� �� TTl::Tail:: ... Tail::Head::TypeList, ��� ����� ��������� ����������
     ���������� ������������. �������� �������� ��������� ����� �������� �������� ���������
     ���������� operator(). ������� �� SFINAE �������, ����.
     ������ ������ "�������" ����� ����������, ��������� ��� � �������� ���������� ����� �����
     � ��� ��������. */
  /* ��� ����������� ������ - ��������� �����������. */
  ret_type operator ()()
  {
    typedef Tuple<typename details::MakeParamTypes<TTl,
                  NullType>::type> TupleType;
    TupleType tuple;
    return doIt(tuple);
  }
  /* ����� ���� ������� ��������, �� ������ �� ������ ���������� ����������.
     ���� �� 6, �� ��� ����������� ����������� ����������, ���� ���������. */
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
*****************              ���������� ���������              ****************
\*******************************************************************************/

/* ������ �������� - ���������������� ����� � ������������ ���������� �������������,
   ������          - ��� ������������� ������������� ��������. */
template <typename UI, typename T>
struct CallConcrete
{
  typedef T ret_type;

  /* � ��� ����� ���������. �� ��� �� �������.
     ���������� ��������� ����������� ���������� ������������ ����������. ��������� ������ ��������
     � ������������������ ���������� ������������, ��������� �� ������ ��������������� �����. �����
     ����, ��� ��� ���� �������������, ��� ������ �������� static_cast<>.
     ������������� ������� ����������, � �������� ������� ���������� ������� ������� ���� ��
     ���������� ����������, ��������� ������� ������� �� ����������� �������� ����� ���������� ��
     ����������� ������ � ���������� �� ������� (��� ����������������� ����������). ��� �����������
     ���������� �������� ������ �������� ����������� ������ ��� ��� ������������ (�������), ��� �
     ��� ������������� (��������) ����������. */
  template <typename SRC, typename PL, int N> struct apply;
  template <typename SRC, typename PL>        struct apply<SRC, PL, -1> // ����������� ������
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
      return UI::apply(static_cast<typename PL::Tail::Tail::Head>(GetField<0>(pl)),//���� � PL ����� �
                       static_cast<typename PL::Tail      ::Head>(GetField<1>(pl)),//�������� �������
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

// �������� ������������� ����������
enum { implemented = 6 };

} // MultiMethods_03

/* �����, ������� ������ ����� ����� ����������� ����� � ��������, �������� ����������� � �������
   ���������� �������������. ���������� ��� ����� ��� ��������.
   ������ �������� - ��� ������������� ������������� ��������;
   ������ �������� - �������� �� ������������ ���������� � �������� ������� �� ����������� ��������;
   ������ �������� - ���, �� ������� ��������� this, �.�. ��� �����.
   ����� ������ ����������� ����������� �������� � ��� ����, ������������������ ����� *this,
   ������� ��� ����� Accept(), ������� ���������� ��������� �� ����, �.�. this, � ������� ��������
   ��� ���������� � �������� ������ ����������. */
#define MAKE_ACCEPTABLE(Ret, Abstract, Concrete)                                           \
          virtual Ret Accept(Abstract& a)                                                  \
          {                                                                                \
            return static_cast<MultiMethods_03::Acceptor<Concrete, Ret>&>(a).Accept(this); \
          }

#endif // MULTIMETHODS_H_CAD83881_D6D4_40FF_B020_BBB5523F6444
