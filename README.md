# Multimethods
A multimethods implementation for C++03 (Visitor) and C++14 (Visitor and RTTI).

This library is result of manyyears work for fun. I have great interest to multimethods theme which attracted me from C++03 age. Because C++17 and even C++20 don't provide native multimethods implementation I decide to share these.
Main characteristics:
- header only lib;
- unbounded N-arity (except C++03, but very simply extendable);
- separate linkage type (static or dynamic) for each param;
- static linkage allow any types, any indirection (r-value refs included, excepting C++03 of course) and any cv-qualification (excepting r-value refs, I'm lazy);
- dynamic linkage also allow same, excepting param-by-value of course (but C++03 allow only cv-nonqualified pointers); you can use same or different class hierarchies for different dynamic params;
- simple and intuitive definition of dispatcher (C++03 is more comprehensive);
- dynamic linkage uses rules of Standard overload rules; you will get a compile-time error on ambiguous or undecidability;
- base multimethod and all its overrides are encapsulated within one scope;
- no reinterpret_cast<>, no dynamic_cast<>;
- full params type control for static linkage params on entire path from dispatcher call to getting execution by multimethod; only Standard implicit conversions are allowed; (C++03 has some troubles there); no unneeded copies (but not for return value, sorry);
- great performance; Visitor (both of them) extremely close to native implementation (whether it existed).

Ideas of these implementations relatively simple but their descriptions take many space. Thus I keep this readme without of it. But I can give URLs to russian forum topics where these design ideas detaily described. Also this lib contain many comments on russian, sorry.
- http://forum.sources.ru/index.php?showtopic=413422 – C++1x on Visitor pattern;  
- http://forum.sources.ru/index.php?showtopic=413381 – C++1x on RTTI;
- http://forum.sources.ru/index.php?showtopic=396031 – C++03 on Visitor pattern (limited features).

Welcome to discussions!
