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
class refholder final
{
public:
    /* Construction, destruction */
    refholder() = delete;
    refholder(T& pv) : v(pv) {}
    ~refholder() = default;

    /* Assignment */
    refholder<T>& operator = (const T& ov) { v = ov; return *this;}
    refholder<T>& operator = (const refholder<T>& ov ) { v = ov.v; return *this; }

    /* Comparison */
    bool operator == (const T& ov) { return !(v ^ ov); }
    bool operator != (const T& ov) { return !operator ==(ov); }
    COMPARISON_OPERATOR(>=)
    COMPARISON_OPERATOR(<=)
    COMPARISON_OPERATOR(>)
    COMPARISON_OPERATOR(<)

    /* Conversion to the real type */
    operator T() {return v;}

    /* Pre increment/decrement only */
    refholder<T>& operator++() { ++ v; return *this; }
    refholder<T>& operator--() { -- v; return *this; }

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

    /* The root of all evil */
    T& v;

    /* All binary ops. disabled */
    DISABLE_BINARY_OPERATOR(+);
    DISABLE_BINARY_OPERATOR(-);
    DISABLE_BINARY_OPERATOR(*);
    DISABLE_BINARY_OPERATOR(/);
    DISABLE_BINARY_OPERATOR(%);
    DISABLE_BINARY_OPERATOR(&);
    DISABLE_BINARY_OPERATOR(|);
    DISABLE_BINARY_OPERATOR(<<);
    DISABLE_BINARY_OPERATOR(>>);

    /* Disable post increment/decrement */
    refholder<T>& operator++(int);
    refholder<T>& operator--(int);

};


/* Helping stuff */

struct base_rvholder
{
    template<class T>
    operator T ()
    {
        return *reinterpret_cast<T*>(get());
    }

    virtual void* get() = 0;
};

template<class T>
struct rvholder : public base_rvholder
{
    rvholder(T t) :base_rvholder(), v(t) {}
    virtual void* get() {return &v;}
    T v;
};


/* what the RETURN/BREAK/CONTINUE will return while running from inside a loop block*/
enum class next_step
{
    ns_break,
    ns_continue,
    ns_return,
    ns_done
};

struct next_step_functor_base
{
    virtual next_step run() = 0;

};

template <class T>
struct next_step_functor final : public next_step_functor_base {

    next_step_functor(T r) : runner(r) {}
    virtual next_step run() {return runner();}

private:
    T runner;
};

struct bool_functor_base
{
    virtual bool run() = 0;
};

template <class T>
struct bool_functor final : public bool_functor_base {

    bool_functor(T r) : runner(r) {}
    virtual bool run() {return runner();}

private:
    T runner;
};

/* c++ control structures implementation */

/* supporting implementation for the REPEAT/UNTIL macros*/

class repeat_wrapper
{
public:
    repeat_wrapper():body(nullptr), condition(nullptr) {}

private:

    std::unique_ptr<next_step_functor_base> body;
    std::unique_ptr<bool_functor_base> condition;
};

/* supporting implementation for the FOR macro */

class for_wrapper final
{
public:

    template<class INIT, class COND, class INCR>
    explicit for_wrapper(INIT lambda_init, COND lambda_cond, INCR lambda_incr)
    {
        condition.reset(new bool_functor<COND>(lambda_cond));
        initializer.reset(new next_step_functor<INIT>(lambda_init));
        increment.reset(new next_step_functor<INCR>(lambda_incr));
    }

    void run()
    {
        for( initializer->run(); condition->run(); increment->run())
        {
            next_step c = body->run();
            if(c == next_step::ns_break) break;
            if(c == next_step::ns_continue) continue;
        }
    }

    ~for_wrapper() noexcept = default;

    template<class T>
    for_wrapper& set_body(T lambda) { body.reset(new next_step_functor<T>(lambda)); return *this; }
private:
    std::unique_ptr<next_step_functor_base> initializer;
    std::unique_ptr<bool_functor_base> condition;
    std::unique_ptr<next_step_functor_base> increment;
    std::unique_ptr<next_step_functor_base> body;


};

/* supporting implementation for the WHILE macro */

class while_wrapper final
{
public:
    template<class T>
    explicit while_wrapper(T lambda) :body(nullptr), condition(nullptr)
    {condition.reset(new bool_functor<T>(lambda));}

    void run()
    {
        while( condition->run() )
        {
            next_step c = body->run();
            if(c == next_step::ns_break) break;
            if(c == next_step::ns_continue) continue;
        }
    }

    ~while_wrapper() noexcept = default;

    template<class T>
    while_wrapper& set_body(T lambda) { body.reset(new next_step_functor<T>(lambda)); return *this; }

private:
    std::unique_ptr<next_step_functor_base> body;
    std::unique_ptr<bool_functor_base> condition;
};

/* supporting implementation for the IF macro */

class if_wrapper final
{
public:
    template<class T>
    if_wrapper(T lambda) {condition.reset(new bool_functor<T>(lambda));}

    void run()
    {
        if(condition->run()) { if(thens) {
            thens->run();
        }}
        else { if(elses) {
            elses->run();
        }}

    }

    ~if_wrapper() noexcept = default;

    template<class T>
    if_wrapper& set_then(T lambda) { thens.reset(new next_step_functor<T>(lambda)); return *this; }
    template<class T>
    if_wrapper& set_else(T lambda) { elses.reset(new next_step_functor<T>(lambda)); return *this; }

private:
    std::unique_ptr<bool_functor_base> condition;
    std::unique_ptr<next_step_functor_base> thens;
    std::unique_ptr<next_step_functor_base> elses;
};

template<typename X>
refholder<X> $ (X& a)
{
    return refholder<X>(a);
}

/* syntactic sugar */
class stream_helper {};
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
    extra_addition(T& a) : v(a) {$(v) += 1; }
    virtual ~extra_addition() { $(v) -= 1; }

private:
    T& v;
};


template <class T>
class extra_substraction : public basic_extra
{
public:
    extra_substraction(T& a) : v(a) {$(v) -= 1; }
    virtual ~extra_substraction() { $(v) += 1; }

private:
    T& v;
};


template <typename T, int N>
class extra_chooser
{
    using type=basic_extra; // intentionally private
};

/* Number obfuscation implementer */

template<typename T>
struct Num
{
    template <T n, int D = 0>
    struct VH
    {
        enum { value = ( (n & 0x01)  | ( Num < T >::VH<(n >> 1)>::value << 1) ) };
        T v;
        T rv;
        VH() : v(0), rv(0)
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(1.0, 10.0);
            rv = dist(mt);
            v = value ^ rv;
        }
        T get() const { return v ^ rv;}
    };
};

template <typename T> template<int D> struct Num<T>::VH<0,D>
{
    enum {value = 0};
    T v = value;
};

template <typename T> template <int D> struct Num<T>::VH<1,D>
{
    enum {value = 1};
    T v = value;
};

#define _N(a) (Num<decltype(a)>::VH<MetaRandom<__COUNTER__, 4096>::value ^ a>().get() ^ MetaRandom<__COUNTER__ - 1, 4096>::value)
#define DEFINE_EXTRA(N,implementer) template <typename T> struct extra_chooser<T,N> { using type = implementer<T>; }
DEFINE_EXTRA(0, extra_xor);
DEFINE_EXTRA(1, extra_substraction);
DEFINE_EXTRA(2, extra_addition);
#define _(a) ([&](){extra_chooser<decltype(a), MetaRandom<__COUNTER__, MAX_BOGUS_IMPLEMENTATIONS>::value >::type _ec_##__COUNTER__(a);\
            return stream_helper();}() << a)
#define IF(x) { std::shared_ptr<base_rvholder> rvlocal; if_wrapper(( [&] () { return (x); })).set_then( [&]() {
#define ELSE return next_step::ns_done;}).set_else( [&]() {
#define FOR(init,cond,inc) { std::shared_ptr<base_rvholder> rvlocal; for_wrapper( [&](){init;return next_step::ns_done;}, [&](){return cond;}, [&](){inc;return next_step::ns_done;}).set_body( [&]() {
#define END return next_step::ns_done;}).run(); }
#define ENDIF END
#define ENDWHILE END
#define ENDFOR END
#define BREAK return next_step::ns_break;
#define RETURN(x) rvlocal.reset(new rvholder<decltype(x)>(x));  throw rvlocal;
#define CONTINUE return next_step::ns_continue;
#define WHILE(x) {std::shared_ptr<base_rvholder> rvlocal; while_wrapper([&] () { return (x); }).set_body( [&]() {
#define REPEAT
#define UNTIL

#define OBF_BEGIN try {
#define OBF_END } catch(std::shared_ptr<base_rvholder>& r) { return *r; }

#endif // INSTR_H
