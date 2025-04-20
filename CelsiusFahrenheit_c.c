/*----------------------------------------------------------------
|   CelsiusFahrenheit.c: rutines de conversió de temperatura 
|						 en format de Coma Flotant E9M22. 
|		Implementació cridant rutines aritmètiques libE9M22.
| ----------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(Febrer-Març 2025)
| ----------------------------------------------------------------*/

#include "E9M22.h"	/* E9M22: tipus Coma Flotant E9M22
                       MAKE_E9M22(real): codifica un valor real en format E9M22
                       E9M22_add/sub/mul: rutines llibreria libE9M22
                    */


/* Celsius2Fahrenheit(): converteix una temperatura en graus Celsius 
                        a la temperatura equivalent en graus Fahrenheit, 
                        usant valors codificats en Coma Flotant E9M22.
    resultat = (input * 9/5) + 32.0;
*/
E9M22_t Celsius2Fahrenheit( E9M22_t input )
{
    E9M22_t resultat;

        // 1a part càlcul: resultat = (input * 9/5)
    resultat = E9M22_mul( input, MAKE_E9M22(9.0/5.0) );

        // 2a part càlcul: resultat = (input * 9/5) + 32.0
    resultat = E9M22_add( resultat, MAKE_E9M22(32.0) );

    return resultat;
}


/* Fahrenheit2Celsius(): converteix una temperatura en graus Fahrenheit 
                        a la temperatura equivalent en graus Celsius, 
                        usant valors codificats en Coma Flotant E9M22.
    resultat = (input - 32.0) * 5/9;
*/
E9M22_t Fahrenheit2Celsius(E9M22_t input)
{
    E9M22_t resultat;

        // 1a part càlcul: resultat = (input - 32.0)
    resultat = E9M22_sub( input, MAKE_E9M22(32.0) );

        // 2a part càlcul: resultat = (input - 32.0) * 5/9
    resultat = E9M22_mul( resultat, MAKE_E9M22(5.0/9.0) );

    return resultat;
}

