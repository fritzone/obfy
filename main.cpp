#if !defined( WIN32 )
    #ifdef UNIT_TESTS
        #define BOOST_TEST_MAIN
        #define BOOST_TEST_DYN_LINK
        #include <boost/test/auto_unit_test.hpp>
    #endif
#endif

#include "instr.h"

int simple_if()
{
    int n = 42;
    OBF_BEGIN
       IF( n == 42)
            n = 43;
       ELSE
            n = 44;
       ENDIF
    OBF_END
    return n;
}

int numeric_wrapper_returner()
{
    int n;
    OBF_BEGIN
       n = N(42);
    OBF_END
    return n;
}

int simple_variable_wrapper_returner()
{
	int n;
	OBF_BEGIN
		V(n) = N(42);
	OBF_END
	return n;
}

int simple_variable_wrapper_min_eq()
{
	int n = 64;
	OBF_BEGIN
		V(n) -= n << N(2);
	OBF_END
	return n;
}


int variable_wrapper_returner()
{
    int n = numeric_wrapper_returner();
    OBF_BEGIN
        V(n) ++;
        V(n) --;
    OBF_END
    return n;
}

int variable_wrapper_operations()
{
    int n = numeric_wrapper_returner();
    int v;
    OBF_BEGIN
        V(v) = V(n) + N(1);
        V(v) = V(v) - N(1);
        V(v) = V(v) * N(2);
        V(v) = V(v) / N(2);
        V(v) = V(v) | N(1);
        V(v) = V(v) & N(42);
        V(v) = V(v) << N(1);
        V(v) = V(v) >> N(1);
        V(v) = V(v) ^ N(55);
        V(v) = V(v) ^ N(55);
        V(v) = V(v) % N(41);

        V(v) += N(41);
        V(v) -= N(1);
        V(v) *= N(2);
        V(v) /= N(2);
        V(v) |= N(1);
        V(v) &= N(42);
        V(v) <<= N(1);
        V(v) >>= N(1);
        V(v) ^= N(55);
        V(v) ^= N(55);
        V(v) %= N(41);
        V(v) += N(2);

    OBF_END
    return v;
}

int for_loop_test()
{
    int a;
    int n = 0;
    int x;
    V(x) = N(556);
    OBF_BEGIN

        FOR(V(a) = 0, V(a) < N(10), V(a)++)
            n ++;
        ENDFOR

    OBF_END
    return n;
}

int for_loop_test_break()
{
    int a;
    int n = 0;
    OBF_BEGIN

        FOR(V(a) = 0, V(a) < N(10), V(a)++)
            V(n) ++;
            IF(V(a) == N(5))
                BREAK;
            ENDIF
        ENDFOR

    OBF_END
    return n;
}

int for_loop_test_continue()
{
    int a;
    int n = 0;
    OBF_BEGIN

        FOR(V(a) = 0, V(a) < N(10), V(a)++)
            IF(V(a) == N(5))
                CONTINUE;
            ENDIF
            V(n) ++;
        ENDFOR

    OBF_END
    return n;
}

int while_loop_test()
{
    int a = 0;
    int n = 0;
    OBF_BEGIN

        WHILE(V(a) < 10)
            V(n) ++;
            V(a) = V(n);
        ENDWHILE

    OBF_END
    return n;
}

int while_loop_test_break()
{
    int a = 0;
    int n = 0;
    OBF_BEGIN

        WHILE(V(a) < 10)
            V(n) ++;
            V(a) = V(n);
            IF (V(a) > N(5) )
                BREAK;
            ENDIF
        ENDWHILE

    OBF_END
    return n;
}

int while_loop_test_continue()
{
    int a = 0;
    int n = 0;
    OBF_BEGIN

        WHILE(V(a) < 10)
            V(a) ++;
            IF (V(a) >= N(5) )
                CONTINUE;
            ENDIF
            V(n) ++;

        ENDWHILE

    OBF_END
    return n;
}

int repeat_loop_test()
{
    int a = 0;
    int n = 0;

    OBF_BEGIN

        REPEAT
            ++V(a);
            V(n) = V(a);
        AS_LONG_AS ( V(a) <= N(10))

    OBF_END
    return n;
}

int repeat_loop_test_break()
{
    int a = 0;
    int n = 0;

    OBF_BEGIN

        REPEAT
            IF (V(n) > N(5))
                BREAK;
            ELSE
                ++V(a);
                V(n) = V(a);
        ENDIF
        AS_LONG_AS ( V(a) <= N(10))

    OBF_END
    return n;
}

int repeat_loop_test_continue()
{
    int a = 0;
    int n = 0;

    OBF_BEGIN

        REPEAT
            ++V(a);

            IF (V(a) < N(7))
                CONTINUE;
            ENDIF

            V(n) ++;
        AS_LONG_AS ( V(a) < N(10))

    OBF_END
    return n;
}

int case_tester()
{
    const std::string something = "B";
    int n =


#ifndef OBF_DEBUG
            0;

    OBF_BEGIN

        CASE ( something )

            WHEN("Y") DO
                BREAK;
            DONE

            WHEN("X") DO
                BREAK;
            DONE

            WHEN("A") OR WHEN("B") DO
                V(n) = N(42);
                BREAK;
            DONE

            WHEN("C") DO
                V(n) = N(43);
                BREAK;
            DONE

            DEFAULT
                V(n) = 44;
            DONE

        ENDCASE

    OBF_END
#else
            42;
        #endif
    return n;
}

int case_tester_fallthrough()
{
    std::string something = "B";
    int n =

#ifndef OBF_DEBUG
            0;
    OBF_BEGIN

        CASE (something)

            WHEN( "A" ) OR WHEN("B") DO
                V(n) = N(42);
            DONE

            WHEN("C") DO
                V(n) ++;
            DONE

            DEFAULT
                V(n) ++;
            DONE

        ENDCASE

    OBF_END
#else
    44;
#endif

    return n;
}

int returner()
{
    OBF_BEGIN
        RETURN(42);
    OBF_END
}

struct ATest
{
    int x = 42;
};

ATest class_test(int& a)
{
    OBF_BEGIN
        IF(V(a) == 5)
            V(a) = 42;
        ENDIF
        RETURN (ATest()) ;
    OBF_END
}

#if !defined (WIN32) && defined (UNIT_TESTS)

#include <stdint.h>

BOOST_AUTO_TEST_CASE(test_wrappers)
{
    BOOST_CHECK_EQUAL(numeric_wrapper_returner(), 42);
    BOOST_CHECK_EQUAL(simple_variable_wrapper_min_eq(), -192);

    BOOST_CHECK_EQUAL(variable_wrapper_returner(), 42);
    BOOST_CHECK_EQUAL(variable_wrapper_operations(), 42);
}

BOOST_AUTO_TEST_CASE(for_loops)
{
    BOOST_CHECK_EQUAL(for_loop_test(), 10);
    BOOST_CHECK_EQUAL(for_loop_test_break(), 6);
    BOOST_CHECK_EQUAL(for_loop_test_continue(), 9);
}

BOOST_AUTO_TEST_CASE(while_loops)
{
    BOOST_CHECK_EQUAL(while_loop_test(), 10);
    BOOST_CHECK_EQUAL(while_loop_test_break(), 6);
    BOOST_CHECK_EQUAL(while_loop_test_continue(), 4);
}

BOOST_AUTO_TEST_CASE(repeat_loops)
{
    BOOST_CHECK_EQUAL(repeat_loop_test(), 11);
    BOOST_CHECK_EQUAL(repeat_loop_test_break(), 6);
    BOOST_CHECK_EQUAL(repeat_loop_test_continue(), 4);
}

BOOST_AUTO_TEST_CASE(case_test)
{
    BOOST_CHECK_EQUAL(case_tester(), 42);
    BOOST_CHECK_EQUAL(case_tester_fallthrough(), 44);
}

BOOST_AUTO_TEST_CASE(return_test)
{
    BOOST_CHECK_EQUAL(returner(), 42);
    int a = 5;
    ATest x = class_test(a);
    BOOST_CHECK_EQUAL(a, 42);
    BOOST_CHECK_EQUAL(x.x, 42);
}

BOOST_AUTO_TEST_CASE(bignumber)
{
    int64_t bigNumber;
    V(bigNumber) = N(1537232811123);
    BOOST_CHECK_EQUAL(bigNumber,1537232811123);
}

#else
#include <iostream>
int main()
{
	std::cout << numeric_wrapper_returner() << std::endl << simple_variable_wrapper_returner();
}

#endif
