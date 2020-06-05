/*
 * Copyright (c) 2016, Ferenc Deak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **/

#ifndef INSTR_H
#define INSTR_H

#include <string>
#include <typeinfo>

#include <memory>
#include <random>
#include <vector>
#include <type_traits>
#include <utility>

#ifndef MAX_BOGUS_IMPLEMENTATIONS
#define MAX_BOGUS_IMPLEMENTATIONS 3
#endif

namespace obf
{

// Compile time random number generator from https://github.com/andrivet/ADVobfuscator
/*
 * Written by Sebastien Andrivet
 * Copyright (c) 2010-2015 - Sebastien Andrivet All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
constexpr char time[] = __TIME__;
constexpr int DigitToInt(char c) { return c - '0'; }
const int seed = DigitToInt(time[7]) +
                 DigitToInt(time[6]) * 10 +
                 DigitToInt(time[4]) * 60 +
                 DigitToInt(time[3]) * 600 +
                 DigitToInt(time[1]) * 3600 +
                 DigitToInt(time[0]) * 36000;

template<int N>
struct MetaRandomGenerator final
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
struct MetaRandomGenerator<0> final
{
    static constexpr unsigned value = seed;
};

template<int N, int M>
struct MetaRandom final
{
    static const int value = MetaRandomGenerator<N + 1>::value % M;
};

#define COMP_ASSIGNMENT_OPERATOR(x) \
    refholder<T>& operator x##= (const refholder<T>& ov) { v x##= ov.v; return *this;}  \
    refholder<T>& operator x##= (const refholder<T>&& ov) { v x##= ov.v; return *this;} \
    refholder<T>& operator x##= (const T& ov) { v x##= ov; return *this;}               \
    refholder<T>& operator x##= (const T&& ov) { v x##= ov; return *this;}              \
    refholder<T>& operator x##= (T& ov) { v x##= ov; return *this;}


#define COMPARISON_OPERATOR(x) \
    bool operator x (const T& ov) { return (v x ov); }

/* simple reference holder class, mostly for dealing with holding variables */
template <typename T>
class refholder final
{
public:
    /* Construction, destruction */
    refholder() = delete;
    refholder(T& pv) : v(pv) {}
    refholder(const refholder& ov) : v(ov.v) {}
    refholder(T&&) = delete;

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

    /* Pre increment/decrement operators */
    refholder<T>& operator++() { ++ v; return *this; }
    refholder<T>& operator--() { -- v; return *this; }

    /* post increment/decrement */
    refholder<T> operator++(int) { refholder<T> rv(*this); operator ++(); return rv; }
    refholder<T> operator--(int) { refholder<T> rv(*this); operator --(); return rv; }

    /* Compound assignments */
    COMP_ASSIGNMENT_OPERATOR(+)
    COMP_ASSIGNMENT_OPERATOR(-)
    COMP_ASSIGNMENT_OPERATOR(*)
    COMP_ASSIGNMENT_OPERATOR(/)
    COMP_ASSIGNMENT_OPERATOR(%)
    COMP_ASSIGNMENT_OPERATOR(<<)
    COMP_ASSIGNMENT_OPERATOR(>>)
    COMP_ASSIGNMENT_OPERATOR(&)
    COMP_ASSIGNMENT_OPERATOR(|)
    COMP_ASSIGNMENT_OPERATOR(^)

private:

    /* The root of all evil */
    volatile T& v;
};

/* simple reference holder class, for holding reference of constant variables */
template <typename T>
class refholder <const T> final
{
public:
    /* Construction, destruction */
    refholder() = delete;
    refholder(const T& pv) : v(pv) {}
    refholder(T&&) = delete;

    ~refholder() = default;

    /* Conversion to the real type */
    operator const T() {return v;}

private:

    /* The root of all evil */
    const T& v;
};

/* Binary operators for the value wrappers */
#define DEFINE_BINARY_OPERATOR(x) \
template <class T> refholder<T> operator x (refholder<T>& ls, const T& rs) {refholder<T> rv = ls; ls x##= rs; return rv; } \
template <class T> refholder<T> operator x (refholder<T>& ls, T& rs) {refholder<T> rv = ls; ls x##= rs; return rv; } \
template <class T> refholder<T> operator x (refholder<T>& ls, T&& rs) {refholder<T> rv = ls; ls x##= rs; return rv; } \
template <class T> refholder<T> operator x (refholder<T>& ls, const T&& rs) {refholder<T> rv = ls; ls x##= rs; return rv; } \
template <class T> refholder<T> operator x (refholder<T>& ls, const refholder<T>&& rs) {refholder<T> rv = ls; ls x##= rs; return rv; } \
template <class T> refholder<T> operator x (refholder<T>& ls, refholder<T>&& rs) {refholder<T> rv = ls; ls x##= rs; return rv; } \
template <class T> refholder<T> operator x (refholder<T>& ls, refholder<T>& rs) {refholder<T> rv = ls; ls x##= rs; return rv; } \
template <class T> refholder<T> operator x (refholder<T>& ls, const refholder<T>& rs) {refholder<T> rv = ls; ls x##= rs; return rv; }

DEFINE_BINARY_OPERATOR(+)
DEFINE_BINARY_OPERATOR(-)
DEFINE_BINARY_OPERATOR(*)
DEFINE_BINARY_OPERATOR(/)
DEFINE_BINARY_OPERATOR(%)
DEFINE_BINARY_OPERATOR(&)
DEFINE_BINARY_OPERATOR(|)
DEFINE_BINARY_OPERATOR(<<)
DEFINE_BINARY_OPERATOR(>>)
DEFINE_BINARY_OPERATOR(^)


/* Helping stuff */

struct base_rvholder
{
    virtual ~base_rvholder() = default;

    template<class T>
    operator T () const
    {
        return *reinterpret_cast<const T*>(get());
    }

    template<class T>
    bool operator == (const T& o) const
    {
        return o == operator T ();
    }

    template<class T>
    bool equals(const T& o) const
    {
        return o == *reinterpret_cast<const T*>(get());
    }

    virtual const void* get() const = 0;
};

template<class T>
class rvholder : public base_rvholder
{
public:
    rvholder(T t, T c) :base_rvholder(), v(t), check(c) {}
    ~rvholder() = default;
    virtual const void* get() const override {return reinterpret_cast<const void*>(&v);}
private:
    T v;
    T check;
};


/* what the RETURN/BREAK/CONTINUE will return while running from inside a loop block*/
enum class next_step
{
    ns_break,
    ns_continue,
    ns_return,
    ns_done,
    ns_unused
};

struct next_step_functor_base
{
    virtual ~next_step_functor_base() = default;
    virtual next_step run() = 0;
};

template <class T>
struct next_step_functor final : public next_step_functor_base
{
    next_step_functor(T r) : runner(r) {}
    virtual next_step run() {return runner();}

private:
    T runner;
};

struct bool_functor_base
{
    virtual ~bool_functor_base() = default;
    virtual bool run() = 0;
};

template <class T>
struct bool_functor final : public bool_functor_base
{
    bool_functor(T r) : runner(r) {}
    virtual bool run() {return runner();}

private:
    T runner;
};

struct any_functor_base
{
    virtual ~any_functor_base() = default;
    virtual void run(void*) const = 0;
};

template <class T>
struct any_functor final : public any_functor_base
{
    any_functor(T r) : runner(r) {}

    virtual void run(void* retv) const override
    {
        auto r = runner();
        *reinterpret_cast<decltype(r)*>(retv) = r;
    }


private:
    T runner;

};

/* c++ control structures implementation */

/* supporting implementation for the REPEAT/AS_LONG_AS macros*/

class repeat_wrapper final
{
public:
    repeat_wrapper():body(nullptr), condition(nullptr) {}

    void run()
    {
        do
        {
            try
            {
                body->run();
            }
            catch(next_step& c)
            {
                if(c == next_step::ns_break) break;
                if(c == next_step::ns_continue) continue;
            }
        }
        while( condition->run() );
    }

    template<class T>
    repeat_wrapper& set_body(T lambda) { body.reset(new next_step_functor<T>(lambda)); return *this; }


    template<class T>
    repeat_wrapper& set_condition(T lambda) { condition.reset(new bool_functor<T>(lambda)); return *this; }

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
            try
            {
                body->run();
            }
            catch(next_step& c)
            {
                if(c == next_step::ns_break) break;
                if(c == next_step::ns_continue) continue;
            }
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
            try
            {
                body->run();
            }
            catch(next_step& c)
            {
                if(c == next_step::ns_break) break;
                if(c == next_step::ns_continue) continue;
            }
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

/* select ... case helper */

class case_instruction
{
public:
    case_instruction() = default;
    virtual ~case_instruction() = default;
    virtual next_step execute(const base_rvholder&) const = 0;
};

template<class CT>
class branch final : public case_instruction
{
public:
    template<class T>
    branch(T lambda) {condition.reset(new any_functor<T>(lambda));}

    bool equals(const base_rvholder& rv, CT lv) const
    {
        return rv.equals(lv);
    }

    virtual next_step execute(const base_rvholder& against) const override
    {
        CT retv;
        condition->run( const_cast<void*>(reinterpret_cast<const void*>(&retv) ) );
        // this looks funny, however we cannot use the operator ==
        // due to a visual C++ 2015 compiler bug, which fails to compile and crashes
        return equals(against,retv) ? next_step::ns_done : next_step::ns_continue;
    }

private:
    std::unique_ptr<any_functor_base> condition;
};

class body final : public case_instruction
{
public:
    template<class T>
    body(T lambda) {instructions.reset(new next_step_functor<T>(lambda));}


    virtual next_step execute(const base_rvholder&) const override
    {
        return instructions->run();
    }
private:
    std::unique_ptr<next_step_functor_base> instructions;
};

template <class CT>
class case_wrapper_base
{
public:
    explicit case_wrapper_base(const CT& v) : check(v), default_step(nullptr) {}


    case_wrapper_base& add_entry(const case_instruction& lambda_holder)
    {
        steps.push_back(&lambda_holder);
        return *this;
    }

    case_wrapper_base& add_default(const case_instruction& lambda_holder)
    {
        default_step = &lambda_holder;
        return *this;
    }

    case_wrapper_base& join()
    {
        return *this;
    }

    void run() const
    {
        auto it = steps.begin();
        while(it != steps.end())
        {
            bool increased = false;
            // see if this is a branch
            if(dynamic_cast<const branch<CT>*>(*it) || dynamic_cast<const branch<const CT>*>(*it))
            {
                // branch. Execute it, see if it returns true or false
                next_step enter = (*it)->execute(rvholder<CT>(check,check));
                if(enter == next_step::ns_continue)
                {
                    // step to the next branch
                    ++it;
                    increased = true;
                }
                else
                {
                    // now fast forward and find the first body,
                    // and from that point on run all the bodies, unless one of them breaks
                    while(! dynamic_cast<const body*>(*it)  && it != steps.end() )
                    {
                        ++it;
                        increased = true;
                    }

                    // found the first body.
                    while(it != steps.end())
                    {
                        if(dynamic_cast<const body*>(*it))
                        {
                            (*it)->execute(rvholder<CT>(check,check));
                        }
                        increased = true;
                        ++it;
                    }
                }
            }
            else if(dynamic_cast<const body*>(*it))
            {
                // skip body
            }
            else
            {
                throw(std::string("invalid type:") + typeid(*it).name());
            }

            // just for safety
            if(!increased)
            {
                ++it;
            }
        }

        if(default_step)
        {
            default_step->execute(rvholder<CT>(check,check));
        }
    }

private:

    std::vector<const case_instruction*> steps;
    const CT check;
    const case_instruction* default_step;
};

template<class CT>
class case_wrapper final : public case_wrapper_base<CT>
{
public:
    explicit case_wrapper(const CT& v) : case_wrapper_base<CT>(v) {}
};


template<class CT>
class case_wrapper <const CT> final : public case_wrapper_base<CT>
{
public:
    explicit case_wrapper(const CT& v) : case_wrapper_base<CT>(v) {}
};

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
class extra_xor final : public basic_extra
{
public:
    extra_xor(T& a) : v(a)
    {
		volatile T lv = MetaRandom<__COUNTER__, 4096>::value;
        v ^= lv;
    }
    virtual ~extra_xor()
	{
		volatile T lv = MetaRandom<__COUNTER__ - 1, 4096>::value;
		v ^= lv;
	}

private:
    volatile T& v;
};

template <class T>
class extra_xor <const T> final : public basic_extra
{
public:
    extra_xor(const T&) {}
};

template <class T>
class extra_addition final : public basic_extra
{
public:
    extra_addition(T& a) : v(a)
	{
		volatile T lv = MetaRandom<__COUNTER__, 4096>::value;
		v += lv;
	}
    virtual ~extra_addition()
	{
		volatile T lv = MetaRandom<__COUNTER__ - 1, 4096>::value;
		v -= lv;
	}
private:
    volatile T& v;
};

template <class T>
class extra_addition <const T> final: public basic_extra
{
public:
    extra_addition(const T&) {}
};

template <class T>
class extra_substraction final : public basic_extra
{
public:
    extra_substraction(T& a) : v(a) {v -= MetaRandom<__COUNTER__, 4096>::value; }
    virtual ~extra_substraction() { v += MetaRandom<__COUNTER__ - 1, 4096>::value; }

private:
    volatile T& v;
};

template <class T>
class extra_substraction <const T> final : public basic_extra
{
public:
    extra_substraction(const T&) {}
};

template <typename T, int N>
class extra_chooser final
{
    using type=basic_extra; // intentionally private
};

/* Constant obfuscation implementer */

template<typename T, T n> class Num final
{
public:
    enum { value = ( (n & 0x01)  | ( Num < T , (n >> 1)>::value << 1) ), int64_forcer = 0xffffffffffffffffULL };
    Num() : v(0)
    {
        v = value ^  MetaRandom<32, 4096>::value;
    }
    T get() const { T x = v ^ MetaRandom<32, 4096>::value; return x;}
private:
    volatile T v;
};

struct ObfZero { enum {value = 0}; };
struct ObfOne { enum {value = 1}; };
#define OBF_ZERO(t) template <> struct Num<t,0> final : public ObfZero { t v = value; t get() const { return v;}; };
#define OBF_ONE(t) template <> struct Num<t,1> final : public ObfOne { t v = value; t get() const { return v;}; };
#define OBF_TYPE(t) OBF_ZERO(t) OBF_ONE(t)

OBF_TYPE(bool)

OBF_TYPE(char)
OBF_TYPE(signed char)
OBF_TYPE(unsigned char)
OBF_TYPE(char16_t)
OBF_TYPE(char32_t)
OBF_TYPE(wchar_t)

OBF_TYPE(short int)
OBF_TYPE(unsigned short int)

OBF_TYPE(int)
OBF_TYPE(unsigned int)

OBF_TYPE(long int)
OBF_TYPE(unsigned long int)

OBF_TYPE(long long int)
OBF_TYPE(unsigned long long int)

#if defined OBF_DEBUG

#define OBF_BEGIN
#define OBF_END

#define V(x) x
#define N(x) x

#define RETURN(x) return x;

#define BREAK break;
#define CONTINUE continue;

#define IF(x) if(x) {
#define ELSE } else {
#define ENDIF }

#define FOR(init,cond,inc) for(init;cond;inc) {
#define ENDFOR }

#define WHILE(x) while(x) {
#define ENDWHILE }

#define REPEAT   do {
#define AS_LONG_AS(x) } while ((x));

#define CASE(a) switch (a) {
#define ENDCASE }
#define WHEN(c) case c:
#define DO {
#define DONE }
#define OR
#define DEFAULT default:



#else
#define _JOIN(a,b) a##b
#define N(a) (obf::Num<decltype(a), obf::MetaRandom<__COUNTER__, 4096>::value ^ a>().get() ^ obf::MetaRandom<__COUNTER__ - 1, 4096>::value)
#define DEFINE_EXTRA(N,implementer) template <typename T> struct extra_chooser<T,N> { using type = implementer<T>; }
DEFINE_EXTRA(0, extra_xor);
DEFINE_EXTRA(1, extra_substraction);
DEFINE_EXTRA(2, extra_addition);
#define V(a) ([&](){obf::extra_chooser<std::remove_reference<decltype(a)>::type, obf::MetaRandom<__COUNTER__, \
            MAX_BOGUS_IMPLEMENTATIONS>::value >::type _JOIN(_ec_,__COUNTER__)(a);\
            return obf::stream_helper();}() << a)

#define FOR(init,cond,inc) { std::shared_ptr<obf::base_rvholder> __rvlocal; obf::for_wrapper( [&](){(init); return __crv; },\
           [&]()->bool{return (cond); }, \
           [&](){inc;return __crv;}).set_body( [&]() {
#define ENDFOR return __crv;}).run(); }

#define END return __crv;}).run(); }

#define IF(x) {std::shared_ptr<obf::base_rvholder> __rvlocal; obf::if_wrapper(( [&]()->bool{ return (x); })).set_then( [&]() {
#define ELSE return __crv;}).set_else( [&]() {
#define ENDIF END

#define WHILE(x) {std::shared_ptr<obf::base_rvholder> __rvlocal; obf::while_wrapper([&]()->bool{ return (x); }).set_body( [&]() {
#define ENDWHILE END

#define BREAK __crv = obf::next_step::ns_break; throw __crv;
#define CONTINUE __crv = obf::next_step::ns_continue; throw __crv;

#define RETURN(x) __rvlocal.reset(new obf::rvholder<std::remove_reference<decltype(x)>::type>(x,x));  throw __rvlocal;

#define REPEAT { std::shared_ptr<obf::base_rvholder> __rvlocal; obf::repeat_wrapper().set_body( [&]() {
#define AS_LONG_AS(x) return __crv;}).set_condition([&]()->bool{ return ( (x) ); }).run(); }

#define OBF_BEGIN try { obf::next_step __crv = obf::next_step::ns_done; std::shared_ptr<obf::base_rvholder> __rvlocal; (void)__crv;
#define OBF_END } catch(std::shared_ptr<obf::base_rvholder>& r) { return *r; } catch (...) {throw;}

#define CASE(a) try { std::shared_ptr<obf::base_rvholder> __rvlocal;\
                auto __avholder = a; obf::case_wrapper<std::remove_reference<decltype(a)>::type>(a).
#define ENDCASE run(); } catch(obf::next_step&) {}
#define WHEN(c) add_entry(obf::branch<std::remove_reference<decltype(__avholder)>::type>\
                ( [&,__avholder]() -> std::remove_reference<decltype(__avholder)>::type \
                { std::remove_reference<decltype(__avholder)>::type __c = (c); return __c;} )).
#define DO add_entry( obf::body([&](){
#define DONE return obf::next_step::ns_continue;})).
#define OR join().
#define DEFAULT add_default(obf::body([&](){

#endif

} // namespace obf

#endif // INSTR_H
