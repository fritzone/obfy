#include <iostream>
#include <stdio.h>

#define OBF_DEBUG

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
    size_t l = strlen(user), lic_ctr = 0;

    size_t add = 0, i =0;

    FOR (i = 0, i < ll, i++)
        IF (users_license[i] != '-')
            license += users_license[i];
        ENDIF
    ENDFOR

    WHILE (V(lic_ctr) < license.length() )

        size_t i = lic_ctr;
        V(i) %= l;
        int current = 0;
        WHILE(i < l)
            V(current) += user[i ++];
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

int main()
{

    int a = 7;



    OBF_BEGIN


    a = 5;

//    std::string something = "D";
//    std::string something_else = "D";

//    CASE (something)

//        WHEN("A") OR WHEN("B") DO
//            std::cout <<"Hurra, something is " << something << std::endl;
//            BREAK;
//        DONE

//        WHEN("C") DO
//            std::cout <<"Too bad, something is " << something << std::endl;
//            BREAK;
//        DONE

//        WHEN(something_else) DO
//            std::cout <<"Interesing, something is " << something_else << std::endl;
//            BREAK;
//        DONE

//        DEFAULT
//            std::cout << "something is neither A, B or C, but:" << something << std::endl;
//        DONE

//    ENDCASE



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
    WHILE(a < 5)
        std::cout << "counter before=" << a << std::endl;
        IF(a == 2)
             a++;
             CONTINUE
        ENDIF
        std::cout << "counter after=" << a << std::endl;
        a++;
    ENDFOR





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



    REPEAT
        std::cout <<"LLPP:" << a << std::endl;
        ++a;
    UNTIL (a < 12)

    V(a) = N(1);
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

            OBF_END
    return 0;

}
