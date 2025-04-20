/*---------------------------------------------------------------------
|   Declaració de prototipus de funcions de conversió de temperatures,
|	en format Coma Flotant E9M22.
| ---------------------------------------------------------------------
|	pere.millan@urv.cat
|	santiago.romani@urv.cat
|	(Febrer-Març 2025)
| ---------------------------------------------------------------------*/

#ifndef CELSIUSFAHRENHEIT_H
#define CELSIUSFAHRENHEIT_H

#include "E9M22.h"		// E9M22_t: tipus Coma Flotant E9M22

/* Celsius2Fahrenheit(): converteix una temperatura en graus Celsius 
                         a la temperatura equivalent en graus Fahrenheit, 
                         usant valors codificats en Coma Flotant E9M22.
    output = (input * 9/5) + 32.0;
*/
extern E9M22_t Celsius2Fahrenheit(E9M22_t input);

/* Fahrenheit2Celsius(): converteix una temperatura en graus Fahrenheit 
                         a la temperatura equivalent en graus Celsius, 
                         usant valors codificats en Coma Flotant E9M22.
    output = (input - 32.0) * 5/9;
*/
extern E9M22_t Fahrenheit2Celsius(E9M22_t input);


#endif /* CELSIUSFAHRENHEIT_H */
