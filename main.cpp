#include <iostream>
#include <stdio.h>

#include "instr.h"
#include <type_traits>
#include <iostream>
#include <memory>
#include <random>


int dummy(int &a)
{
    a = 5;
    return 6;
}

int main()
{
    auto s = TEXT("Nigga I done steal ya bike tooo") ;

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
