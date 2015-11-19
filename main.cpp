#include <iostream>
#include <stdio.h>

#include "instr.h"

#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>

#include <type_traits>
//

int main()
{

    int a = 5, b = 6, v=7;

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
