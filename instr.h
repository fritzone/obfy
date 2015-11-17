#ifndef INSTR_H
#define INSTR_H

#include <memory>
#include <random>

#ifndef MAX_BOGUS_IMPLEMENTATIONS
#define MAX_BOGUS_IMPLEMENTATIONS 3
#endif

// random generator reusing from https://github.com/andrivet/ADVobfuscator
// Copyright (c) 2010-2014, Sebastien Andrivet
// All rights reserved.
namespace
{
    constexpr char time[] = __TIME__;
    constexpr int DigitToInt(char c) { return c - '0'; }
    const int seed = DigitToInt(time[7]) +
                     DigitToInt(time[6]) * 10 +
                     DigitToInt(time[4]) * 60 +
                     DigitToInt(time[3]) * 600 +
                     DigitToInt(time[1]) * 3600 +
                     DigitToInt(time[0]) * 36000;
}
template<int N>
struct MetaRandomGenerator
{
private:
    static constexpr unsigned a = 16807;
    static constexpr unsigned m = 2147483647;
    static constexpr unsigned s = MetaRandomGenerator<N - 1>::value;
    static constexpr unsigned lo = a * (s & 0xFFFF);
    static constexpr unsigned hi = a * (s >> 16);
    static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16);
    static constexpr unsigned hi2 = hi >> 15;
    static constexpr unsigned lo3 = lo2 + hi;

public:
    static constexpr unsigned max = m;
    static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
};

template<>
struct MetaRandomGenerator<0>
{
    static constexpr unsigned value = seed;
};

template<int N, int M>
struct MetaRandom
{
    static const int value = MetaRandomGenerator<N + 1>::value % M;
};

#define COMP_ASSIGNMENT_OPERATOR(x) \
    refholder<T>& operator x##= (const refholder<T>& ov) { v x##= ov.v; return *this;}  \
    refholder<T>& operator x##= (const refholder<T>&& ov) { v x##= ov.v; return *this;} \
    refholder<T>& operator x##= (const T& ov) { v x##= ov; return *this;}               \
    refholder<T>& operator x##= (const T&& ov) { v x##= ov; return *this;}              \
    refholder<T>& operator x##= (T& ov) { v x##= ov; return *this;}                     \
    refholder<T>& operator x##= (T&& ov) { v x##= ov; return *this;}

#define COMPARISON_OPERATOR(x) \
    bool operator x (const T& ov) { return (v x ov); }

#define DISABLE_BINARY_OPERATOR(x) \
    refholder& operator x (const refholder<T>&);        \
    refholder& operator x (const refholder<T>&&);       \
    refholder& operator x (refholder<T>&&);             \
    refholder& operator x (refholder<T>&);              \
    refholder& operator x (const T&);                   \
    refholder& operator x (const T&&)

/* simple reference holder class, mostly for dealing with numbers */
template <typename T>
class refholder
{
public:
    refholder(T& pv) : v(pv) {}

    /* Comparison operator */
    COMPARISON_OPERATOR(==)
    COMPARISON_OPERATOR(>=)
    COMPARISON_OPERATOR(<=)
    COMPARISON_OPERATOR(!=)
    COMPARISON_OPERATOR(>)
    COMPARISON_OPERATOR(<)

    /* Conversion to the real type */
    operator T() {return v;}

    /* Assignment */
    refholder<T>& operator = (const T& ov) { v = ov; return *this;}

    /* Compound assignments */
    COMP_ASSIGNMENT_OPERATOR(+)
    COMP_ASSIGNMENT_OPERATOR(-)
    COMP_ASSIGNMENT_OPERATOR(*)
    COMP_ASSIGNMENT_OPERATOR(/)
    COMP_ASSIGNMENT_OPERATOR(<<)
    COMP_ASSIGNMENT_OPERATOR(>>)
    COMP_ASSIGNMENT_OPERATOR(&)
    COMP_ASSIGNMENT_OPERATOR(|)
    COMP_ASSIGNMENT_OPERATOR(^)

private:
    T& v;

    DISABLE_BINARY_OPERATOR(+);
    DISABLE_BINARY_OPERATOR(-);
    DISABLE_BINARY_OPERATOR(*);
    DISABLE_BINARY_OPERATOR(/);
    DISABLE_BINARY_OPERATOR(%);
    DISABLE_BINARY_OPERATOR(&);
    DISABLE_BINARY_OPERATOR(|);
    DISABLE_BINARY_OPERATOR(<<);
    DISABLE_BINARY_OPERATOR(>>);

};

struct void_functor_base
{
    virtual void run() = 0;
};

template <class T>
struct functor : public void_functor_base {

    functor(T r) : runner(r) {}
    virtual void run() {runner();}

private:
    T runner;
};

struct bool_functor_base
{
    virtual bool run() = 0;
};

template <class T>
struct bool_functor : public bool_functor_base {

    bool_functor(T r) : runner(r) {}
    virtual bool run() {return runner();}

private:
    T runner;
};


class while_wrapper
{
public:
    template<class T>
    while_wrapper(T lambda) {condition.reset(new bool_functor<T>(lambda));}

    ~while_wrapper()
    {
        while( condition->run() )
        {
            body->run();
        }
    }

    template<class T>
    void set_body(T lambda) { body.reset(new functor<T>(lambda)); }

private:
    std::unique_ptr<void_functor_base> body;
    std::unique_ptr<bool_functor_base> condition;
};

class if_wrapper
{
public:
    template<class T>
    if_wrapper(T lambda) {condition.reset(new bool_functor<T>(lambda));}

    ~if_wrapper()
    {
        if(condition->run()) { if(thens) {
            thens->run();
        }}
        else { if(elses) {
            elses->run();
        }}
    }

    template<class T>
    if_wrapper& set_then(T lambda) { thens.reset(new functor<T>(lambda)); return *this; }
    template<class T>
    void set_else(T lambda) { elses.reset(new functor<T>(lambda)); }

private:
    std::unique_ptr<bool_functor_base> condition;
    std::unique_ptr<void_functor_base> thens;
    std::unique_ptr<void_functor_base> elses;
};



template<typename X>
refholder<X> $ (X& a)
{
    return refholder<X>(a);
}

class stream_helper
{
};

template <typename T>
refholder<T> operator << (stream_helper, T& a)
{
    return refholder<T>(a);
}

/* The bogus operations allowed */
class basic_extra
{
public:
    basic_extra() = default;
    ~basic_extra() = default;
    basic_extra(const basic_extra&) = delete;
    basic_extra& operator = (const basic_extra&) = delete;
    basic_extra(const basic_extra&&) = delete;
    basic_extra& operator = (const basic_extra&&) = delete;
};

template <class T>
class extra_xor : public basic_extra
{
public:
    extra_xor(T& a) : v(a), rv(0)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(1.0, 10.0);
        rv = dist(mt);
        $(v) ^=  rv;
        std::cerr <<"XOR " << rv << std::endl;

    }
    virtual ~extra_xor() { $(v) ^=  rv; }

private:
    T& v;
    T rv;
};

template <class T>
class extra_addition : public basic_extra
{
public:
    extra_addition(T& a) : v(a) {std::cerr <<"ADD\n";  $(v) += 1; }
    virtual ~extra_addition() { $(v) -= 1; }

private:
    T& v;
};


template <class T>
class extra_substraction : public basic_extra
{
public:
    extra_substraction(T& a) : v(a) {std::cerr <<"SUB\n";  $(v) -= 1; }
    virtual ~extra_substraction() { $(v) += 1; }

private:
    T& v;
};


template <typename T, int N>
class extra_chooser
{
    using type=basic_extra; // intentionally private
};

#define DEFINE_EXTRA(N,implementer) template <typename T> struct extra_chooser<T,N> { using type = implementer<T>; }

DEFINE_EXTRA(0, extra_xor);
DEFINE_EXTRA(1, extra_substraction);
DEFINE_EXTRA(2, extra_addition);

#define _(a) [&](){extra_chooser<decltype(a), MetaRandom<__COUNTER__, MAX_BOGUS_IMPLEMENTATIONS>::value >::type _ec_##__COUNTER__(a);  return stream_helper();}() << a

#define IF(x) if_wrapper(( [&] () { return x; })).set_then( [&]() {
#define ELSE }).set_else( [&]() {
#define END });
#define WHILE(x) while_wrapper([&] () { return x; }).set_body( [&]() {
#define END });


#endif // INSTR_H
