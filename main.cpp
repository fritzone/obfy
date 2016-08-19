#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/auto_unit_test.hpp>

#include <iostream>
#include <stdio.h>


#include "instr.h"
#include <type_traits>
#include <iostream>
#include <memory>
#include <random>
#include <string.h>

int dummy(int &a)
{
    a = 5;
    return 6;
}

static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

bool check_license(const char* user, const char* users_license)
{
    std::string license;
    size_t ll = strlen(users_license);
    size_t l = strlen(user), lic_ctr = 0;
    int add = 0;

    for (size_t i = 0; i < ll; i++)
        if (users_license[i] != '-')
            license += users_license[i];

    while (lic_ctr < license.length() )
    {
        size_t i = lic_ctr;
        i %= l;
        int current = 0;
        while (i < l) current += user[i ++];
        current += add;
        add++;

        if (license[lic_ctr] != letters[current % sizeof letters])
            return false;

        lic_ctr++;
    }

    return true;
}

bool check_license1(const char* user, const char* users_license)
{
    OBF_BEGIN
    std::string license;
    size_t ll = strlen(users_license);
    size_t l = strlen(user), lic_ctr = N(0);

    size_t add = N(0), i =N(0);

    FOR (V(i) = N(0), V(i) < V(ll), V(i)++)
        IF ( V(users_license[i]) != N(45) )
            license += users_license[i];
        ENDIF
    ENDFOR

    WHILE (V(lic_ctr) < license.length() )

        size_t i = lic_ctr;
        V(i) %= l;
        int current = 0;
        WHILE(V(i) < V(l) )
            V(current) += user[V(i)++];
        ENDWHILE
        V(current) += V(add);
        ++V(add);

        IF ( (license [lic_ctr] != letters[current % sizeof letters]) )
            RETURN(false);
        ENDIF

        lic_ctr++;
    ENDWHILE

    RETURN (true);

    OBF_END
}


std::string generate_license(const char* user)
{
    // the license will contain only these character
    // 16 chars + 0
    char result[17] = { 0 };
    size_t l = strlen(user), lic_ctr = 0;
    int add = 0;
    while (lic_ctr < 16)
    {
        size_t i = lic_ctr;
        i %= l;
        int current = 0;
        while (i < l)
        {
            current += user[i];
            i++;
        }
        current += add;
        add++;

        result[lic_ctr] = letters[current % sizeof letters];
        lic_ctr++;
    }

    return std::string(result);
}

int stuffer()
{
    return 1;
}

struct X
{
    int f;
};

int method()
{
    OBF_BEGIN
        RETURN(1)
    OBF_END
}

int some_fun()
{
OBF_BEGIN
    RETURN(42)
OBF_END
}


int something()
{

    int a = some_fun();
    std::cout << a;


    OBF_BEGIN


    a = 5;

    FOR(a = 0, a < 10, a++)
       std::cout << "counter=" << a << std::endl;
       IF(a == 1)
            BREAK
       ENDIF
    ENDFOR

    int xs[12] = {0};
    V(xs[1]) = N(2);
    V(xs[1]) ++;
    int z = V(xs[1]) + N(9);

    std::cout << xs[1] << std::endl;

//    FOR(a = 0, a < 5, a++)
//       std::cout << "counter before=" << a << std::endl;
//       IF(a == 2)
//            CONTINUE
//       ENDIF
//       std::cout << "counter after=" << a << std::endl;
//    ENDFOR

    a = 0;
    WHILE( some_fun() )
        std::cout << "counter before=" << a << std::endl;
        IF(a == 2)
             a++;
             BREAK;
        ENDIF
        std::cout << "counter after=" << a << std::endl;
        a++;
    ENDWHILE





    std::string s = generate_license("Ferenc Deak");
    std::string license = "";
    int current_size = 0;
    for (size_t i = 0; i < s.length() ; i++)
    {
        license += s[i];
        if (++current_size == 4 && i < s.length() - 1)
        {
            license += "-";
            current_size = 0;
        }
    }
    std::cout << "User:" << license  << std::endl;

    std::cout << "Check:" << check_license1("Ferenc Deak", license.c_str()) << std::endl;

    int b = N(96), v=7;



    do
    { std::cout << "LLXX:" << a << std::endl;
        ++a;
    }while(a < 12);


    IF (a == 5)
        RETURN (77)
    ENDIF



    V(a) = V(a) - V(b);
    std::cout << "A=" << a << std::endl;


    std::cout << "A=" << a << std::endl;





        a = 6;

        CASE (a)

            WHEN(5) OR WHEN(4) DO
                std::cout <<"Hurra, 5" << std::endl;
                BREAK;
            DONE

            WHEN(6) DO
                std::cout <<"Hurra, 6" << std::endl;
            DONE

        ENDCASE




    std::cout << "BEFOR:" << a<< std::endl;

    IF( V(a) == N(9) )

         V(b) = a + N(5);
    ELSE
         V(a) = N(9);
         V(b) = a + b;
    ENDIF

    std::cout << "AF:" << a << " " << b<< std::endl;

    std::cout << "BALBLALA:" << a<< std::endl;

    V(a) = 1;
    WHILE( V(a)  < N(10) )

        std::cout << "IN:" << a<< std::endl;
        v = V(a);

        V(a) += N(1);
    ENDWHILE

    std::cout << "AFTER:" << a << " " << b << " " << v << std::endl;

    IF( V(b)  == N(1) )

        V(b) += N(50);
        V(a) = N(66);

        V(a) = V(b);

    ENDIF

    FOR( ( V(a)=N(0), V(b)=N(10) ) , (V(a) < N(10) ), ( ++ V(a), -- V(b)) )
        std::cout << "INFOR:" << a <<" B=" << b<< std::endl;
    ENDFOR

    const int ssx=66;
    V(a) = V(ssx);
    int x = N(5) - V(a);
    std::cout << x;

    if (V(a) == 66)
    {
        std::cout << "\nVA is 66";
    }

    //float xxx;
    //V(xxx) = N(12.34);

    OBF_END


}

int numeric_wrapper_returner()
{
    int n;
    OBF_BEGIN
       n = N(42);
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
        UNTIL ( V(a) <= N(10))

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
        UNTIL ( V(a) <= N(10))

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
        UNTIL ( V(a) < N(10))

    OBF_END
    return n;
}

int case_tester()
{
    const std::string something = "B";
    int n = 0;

    OBF_BEGIN

        CASE ( something )

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

    return n;
}

int case_tester_fallthrough()
{
    std::string something = "B";
    int n = 0;
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

    return n;
}

BOOST_AUTO_TEST_CASE(test_wrappers)
{
    BOOST_CHECK_EQUAL(numeric_wrapper_returner(), 42);
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

    float d = 1.2;
    V(d) = 4.5;
}
