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

    for (int i = 0; i < ll; i++)
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

int main()
{
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
    auto s1 = TEXT("Nigga I done steal ya bike tooo") ;

    std::cout << s[1] << std::endl;

    int a = 5, b = 6, v=7;

    a += 7;
    b += a;
    a -= 7;
    b -= 12;

    b = dummy(a);


    OBF_BEGIN

        IF (a == 5)
            IF (b == 8)
                RETURN (N(77))
            ENDIF
        ENDIF

        V(a) = N(1);

        REPEAT
            std::cout << a << std::endl;
            ++ V(a);
        UNTIL(V(a) != N(12));

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


    OBF_END

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

    return 0;

}
