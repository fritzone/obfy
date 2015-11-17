#include <iostream>
#include <stdio.h>

#include "instr.h"

#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>

//

int main()
{
    int bb = _N(452);
    /*
    std::cout << "VALUE1:" << bb << std::endl;

    int bb2 = MetaRandom<1,255>::value ;
    std::cout << "VALUE2:" << bb2 << std::endl;

    char s[123];
    sprintf(s, "%03X", 45);
    std::cout << s << std::endl;


    std::cout << "VALUE:" << bb << std::endl;


        int a = 5, b = 6, v = 0;

    std::cout << "BEFOR:" << a<< std::endl;

    /*
    IF( _(a) == 9 )

         _(a)  = 6;

         IF ( _(b) == 8 )
             _(a)  = 11;
         END

         _(b)  = 12;
         _(b) = a + 5;
     ELSE
          _(a) = 9;
         _(b) = a + b;
     END;

    WHILE( _(a)  < 10)

        std::cout << "IN:" << a<< std::endl;

        IF(_(a) == 9 && _(b) == 15)
            _(a) = 7;
            _(b) = 1;
        END

         v = _(a);

        _(a) += 1;

    END;

    if( a == 5)
    {
        b = 9;
    }


    std::cout << "AFTER:" << a << " " << b << " " << v << std::endl;


    IF( _(b)  == 9)

        _(b) += 50;
        _(a) = 66;

    END

    std::cout << "REALLY AFTER:" << a << " " << b << " " << v << std::endl;
*/
    return 0;

}
