/*----------------------------------------------------------------
|   Testing temperature-scale conversion routines;
| ----------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(Abril 2021, Març 2022, Març 2023, Febrer 2024, Març 2025)
| ----------------------------------------------------------------
|	Programador/a 1: guillem.requeno@estudiants.urv.cat
|	Programador/a 2: alexxavier.jurado@estudiants.urv.cat
|-----------------------------------------------------------------*/

#include "E9M22.h"				/* external declarations of types, defines and
                                   macro for dealing with E9M22 numbers */
#include "CelsiusFahrenheit.h"	/* external declarations of function
                                   prototypes for temperature conversions */
#include "divmod.h"				/* rutina div_mod() de divisió entera */

#define MAX_ERROR_PERCENTAGE 2	/* 2% maximum allowable error percentage */


/* type definition of the structured record that holds the test case values */
typedef struct {
    float f_input;		/* input temperature value for conversion (to be converted to E9M22) */
    float f_xresult;	/* Expected result of the operation (to be converted to E9M22) */
} test_ops_struct;


/*********************************/
/* the lists of test case values */
/*********************************/

    /* Tests Celsius -> Fahrenheit */
test_ops_struct test_cases_Cels2Fahr[] =
{{ -273.15,	-459.67}, 		/*  0: absolute min. °C temperature */
 { -40.0,	-40.0},			/*  1: equal °C and °F */
 { -17.7777777777778, 0.0},	/*  2: conversion to 0 °F */	
 { -17.777,	0.0014},		/*  3: nearby conversion to 0 °F */
 { -17.6,	0.32},			/*  4: another nearby conversion to 0 °F */
 { -1.125,	29.975},		/*  5: small negative °C (positive °F) */
 { 0.0,		32.0},			/*  6: 0.0 °C = 32.0 °F */
 { 1.0,		33.8}, 			/*  7: unit °C */
 {	31.1,	87.98},			/*  8: common positive °C */
 { 5500.0,	9932.0},		/*  9: Sun's photosphere temperature in °C */
 { 1.0E15,	1.8E15}, 		/* 10: max. °C temperature at LHC (Large Hadron Collider) */

	// To-do? Més tests estudiants?

};

    /* Tests Fahrenheit -> Celsius */
test_ops_struct test_cases_Fahr2Cels[] =
{{ -459.67,	-273.15},		/*  0: absolute min. °F temperature */
 { -40.0,	-40.0},			/*  1: equal °F and °C */
 { 0.0,	-17.7777777777778},	/*  2: 0.0 °F = -17.777... °C */
 { 1.0, -17.2222222222222},	/*  3: unit °F */
 { 31.999,	 -0.0006},		/*  4: small converted °C */
							/*  5: small converted °C */
 { 31.9997802734384, -0.000122070311999857}, 
 { 32.0,	0.0}, 			/*  6: 0.0 °C */
							/*  7: small °C */
 { 32.009,	0.00500000000000019}, 
 { 59.0,	15.0}, 			/*  8: common positive °F */
 { 451.0,	232.77777778},	/*  9: temperature for burning books */
 { 9932.0,	5500.0},		/* 10: Sun's photosphere temperature in °F */
 { 1.8E15,	1.0E15},		/* 11: max. °F temperature at LHC (Large Hadron Collider) */

	// To-do? Més tests estudiants?

};

	/* "Forward" declaration */
unsigned int error_percentage(E9M22_t actual_result, E9M22_t expected_result);


	/* Function to apply test cases; returns percentage of OK's */
int apply_tests( E9M22_t (*func_to_test)(E9M22_t),	// Funció a testejar
				 test_ops_struct test_cases[],		// Vector de casos a provar
				 unsigned int num_cases				// Número de casos al vector
				 )							// retorna el percentatge de casos correctes
{
    unsigned int i;						/* loop index */
    E9M22_t result;						/* routine result */
    unsigned int num_ok = 0;			/* number of right tests */
    unsigned int num_ko = 0;			/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    E9M22_t input;		/* input temperature value for conversion */
    E9M22_t xresult;	/* Expected result of the operation */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"	// Evitar warning f_result
    float f_result;		/* Actual result of the operation (float) */

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
            // float --> E9M22 conversions
        input = float_to_E9M22_c_(test_cases[i].f_input); 		// input value
        xresult = float_to_E9M22_c_(test_cases[i].f_xresult); 	// expected result

			// Call the function to test with the current test case
		result = func_to_test(input);

            // E9M22 --> float conversion
        f_result = E9M22_to_float_c_(result); 	// actual result (float)

        error_percent = error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult ; f_result != test_cases[i].f_xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}

#define NUM_ELEMS(VECTOR) (sizeof(VECTOR) / sizeof(VECTOR[0]))

int main(void)
{
    unsigned int perc_ok_Cels2Fahr = 0;	/* percentage of right tests Celsius2Fahrenheit */
    unsigned int perc_ok_Fahr2Cels = 0;	/* percentage of right tests Fahrenheit2Celsius */

		// Jocs de proves Celsius2Fahrenheit
	perc_ok_Cels2Fahr = apply_tests( Celsius2Fahrenheit,	// routine to test
					test_cases_Cels2Fahr, NUM_ELEMS(test_cases_Cels2Fahr) );	// test cases

		// Jocs de proves Fahrenheit2Celsius
	perc_ok_Fahr2Cels = apply_tests( Fahrenheit2Celsius,	// routine to test
					test_cases_Fahr2Cels, NUM_ELEMS(test_cases_Fahr2Cels) );	// test cases


/* TESTING POINT: check if all percentages of OK tests 
                  are greater than 70% required
				  to deliver the FC practice.
    (gdb) p perc_ok_Cels2Fahr
    (gdb) p perc_ok_Fahr2Cels
*/

/* BREAKPOINT */
    return(0);
}



unsigned int error_percentage(E9M22_t actual_result, E9M22_t expected_result)
{
    E9M22_t error_percentage, min_val, diff, div, abs;
    unsigned int int_error;

    min_val = E9M22_0_001;

    if (E9M22_is_lt_c_(E9M22_abs_c_(expected_result), min_val) 
        && E9M22_is_lt_c_(E9M22_abs_c_(actual_result), min_val))
    {
        int_error = 0;	// Small values considered correct
    }
    else
    {
            // %error = 100 * Abs((Eexpected - Eactual) / Eexpected);
        diff = E9M22_sub_c_(expected_result, actual_result);
        div = E9M22_div_c_(diff, actual_result);
        abs = E9M22_abs_c_(div);
        error_percentage = E9M22_mul_c_ ( E9M22_100, abs );
        
        int_error = E9M22_to_int_c_(error_percentage);
        
        if ( (int_error==0) && ! E9M22_IS_ZERO(error_percentage) )
            int_error = 1;	/* Notify very small error */
        }

    return int_error;
}



