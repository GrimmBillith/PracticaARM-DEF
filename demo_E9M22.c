/*-----------------------------------------------------------------------
|   Description: a program to check some fixed point arithmetic functions
|				implemented in "libE9M22.a"; 
|				specifically, it computes the area of a trapezium, 
|				given the key parameters (long base, short base, height).
|------------------------------------------------------------------------
|	Author: Santiago Romaní, Pere Millán (DEIM, URV)
|	Date:   April/2021, March/2022-2024, February-March/2025 
| -----------------------------------------------------------------------*/

#include "E9M22.h"	/* external declarations of types, defines, macros
                    and function prototypes for operating with E9M22 numbers */

	// Alguns valors E9M22 emprats
#define E9M22_29_99			0x40F7F5C2	// Valor 29.99 
#define E9M22_30_75			0x40FB0000	// Valor 30.75 
#define E9M22_45_12			0x411A3D70	// Valor 45.12 
#define E9M22_1137_67065	0x42471ABB	// Valor 1137.67065 

	/* global variables as inputs and output, to compute the areas */
E9M22_t long_base = E9M22_45_12;		// B = 45.12 cm
E9M22_t short_base = E9M22_30_75;		// b = 30.75 cm
E9M22_t height = E9M22_29_99;			// h = 29.99 cm
E9M22_t trapezium_area;		// expected value: (B+b)*h/2 = 1137.67065 cm^2
float f_trapezium_area;

int main(void)
{
    E9M22_t pr;					/* partial result */
    
        // trapezium_area = (long_base + short_base)*height / 2.0
    pr = E9M22_add( long_base, short_base );
    pr = E9M22_mul( pr, height );
    pr = E9M22_div( pr, E9M22_2 );

    trapezium_area = pr;	// update output global variables
    f_trapezium_area = E9M22_to_float(pr);

/* TESTING POINT: check the result 
    (gdb) p /x trapezium_area	-> 0x42471ABA
    (gdb) p f_trapezium_area	-> 1137.67041
*/

/* BREAKPOINT */
    return 0;
}
