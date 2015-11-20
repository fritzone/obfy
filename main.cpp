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
    auto s = _T("Nigga I done steal ya bike") ;

    std::cout << s[1] << std::endl;

    int a = 5, b = 6, v=7;

    a += 7;
    b += a;
    a -= 7;
    b -= 12;

    b = dummy(a);

    if(a == 5)
    {
        if(b == 6)
        {
            return 77;
        }
    }


    OBF_BEGIN

        IF(a == 5)
            IF(b == 6)
                RETURN(_N(77))
            ENDIF
        ENDIF

    OBF_END

    std::cout << "BEFOR:" << a<< std::endl;

    IF( _(a) == _N(9) )

         _(b) = a + _N(5);
    ELSE
         _(a) = _N(9);
         _(b) = a + b;
    ENDIF

    std::cout << "AF:" << a << " " << b<< std::endl;

    std::cout << "BALBLALA:" << a<< std::endl;

    _(a) = 1;
    WHILE( _(a)  < _N(10) )

        std::cout << "IN:" << a<< std::endl;


         v = _(a);

        _(a) += _N(1);
    ENDWHILE

    std::cout << "AFTER:" << a << " " << b << " " << v << std::endl;

    IF( _(b)  == _N(1) )

        _(b) += _N(50);
        _(a) = _N(66);

        _(a) = _(b);

    ENDIF

    FOR( ( _(a)=_N(0), _(b)=_N(10) ) , (_(a) < _N(10) ), ( ++ _(a), -- _(b)) )
        std::cout << "INFOR:" << a <<" B=" << b<< std::endl;
    ENDFOR

    return 0;

}
