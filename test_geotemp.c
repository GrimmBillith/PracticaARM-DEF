/*----------------------------------------------------------------
|   Testing temperature data processing;
| ----------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(April 2021, March 2022-2024, March 2025)
| ----------------------------------------------------------------*/

#include "E9M22.h"				/* external declarations of types, defines and
                                   macro for dealing with E9M22 numbers */
#include "avgmaxmintemp.h"		/* mmres: return type from avgmaxmin routines */
#include "divmod.h"				/* rutina div_mod() de divisió entera */

#define NUM_TEST_ROWS	4

    // Nombrosos valors E9M22 emprats
#define E9M22_0_3	0x3F4CCCCD	// Valor 0.3 
#define E9M22_0_7	0x3F999999	// Valor 0.7 
#define E9M22_0_8	0x3FA66666	// Valor 0.8 
#define E9M22_2_1	0x40033333	// Valor 2.1 
#define E9M22_3_8	0x40399999	// Valor 3.8 
#define E9M22_7_3	0x4074CCCD	// Valor 7.3 
#define E9M22_13_4	0x40AB3333	// Valor 13.4 
#define E9M22_17_0	0x40C40000	// Valor 17.0 
#define E9M22_19_675 0x40CEB333	// Valor 19.675 
#define E9M22_19_8	0x40CF3333	// Valor 19.8 
#define E9M22_20_0	0x40D00000	// Valor 20.0 
#define E9M22_25_9	0x40E79999	// Valor 25.9 
#define E9M22_26_6	0x40EA6666	// Valor 26.6 
#define E9M22_28_0	0x40F00000	// Valor 28.0 
#define E9M22_28_04	0x40F028F6	// Valor 28.04 
#define E9M22_30_38	0x40F9851E	// Valor 30.38 
#define E9M22_33_44	0x4102E147	// Valor 33.44 
#define E9M22_56_1	0x41303333	// Valor 56.1 
#define E9M22_56_12	0x41303D70	// Valor 56.12 
#define E9M22_56_7	0x41316666	// Valor 56.7 
#define E9M22_68_0	0x41440000	// Valor 68.0 
#define E9M22_78_6	0x414E9999	// Valor 78.6 
#define E9M22_78_62	0x414E9EB8	// Valor 78.62 
#define E9M22_134_0	0x41830000	// Valor 134.0 
#define E9M22_n0_7	0xBF999999	// Valor -0.7 
#define E9M22_n0_9	0xBFB33333	// Valor -0.9 
#define E9M22_n1_8	0xBFF33333	// Valor -1.8 
#define E9M22_n1_84	0xBFF5C28F	// Valor -1.84 
#define E9M22_n2_2	0xC0066666	// Valor -2.2 
#define E9M22_n3_0	0xC0200000	// Valor -3.0 
#define E9M22_n3_5	0xC0300000	// Valor -3.5 
#define E9M22_n5_7	0xC05B3333	// Valor -5.7 
#define E9M22_n5_8	0xC05CCCCD	// Valor -5.8 
#define E9M22_n7_5	0xC0780000	// Valor -7.5 
#define E9M22_n9_8	0xC08E6666	// Valor -9.8 
#define E9M22_n10_3	0xC0926666	// Valor -10.3 
#define E9M22_n11_5	0xC09C0000	// Valor -11.5 
#define E9M22_n14_9	0xC0B73333	// Valor -14.9 
#define E9M22_n15_4	0xC0BB3333	// Valor -15.4 
#define E9M22_n16_25 0xC0C10000	// Valor -16.25 
#define E9M22_n18_5	0xC0CA0000	// Valor -18.5 
#define E9M22_n18_8	0xC0CB3333	// Valor -18.8 
#define E9M22_n20_7	0xC0D2CCCD	// Valor -20.7 
#define E9M22_n21_025 0xC0D41999	// Valor -21.025 
#define E9M22_n89_2	0xC1593333	// Valor -89.2 
#define E9M22_n128_6 0xC1804CCD	// Valor -128.6 


E9M22_t test_data[NUM_TEST_ROWS][12]; 		// To be set by converting from float
float f_test_data[NUM_TEST_ROWS][12] = {	// float temperatures to be converted to E9M22 values
    {13.4, 13.4, 20.0, 13.4,  20.0,  20.0,  25.9,  25.9,  20.0,  20.0, 20.0, 25.9},	// several replicated min and max
    {-2.2, -3.5, -5.8, -7.5, -11.5, -15.4, -18.8, -18.5, -14.9,	-10.3, -5.7, -3.0},	// all negatives
    { 0.1,  0.3,  0.7,  0.8,   0.8,  -0.9,  -0.7,   0.5,   0.0,   0.7,  0.5, -0.9},	// all values around 0° Celsius
    {56.7, 56.7, 56.7, 56.7, -89.2, -89.2, -89.2, -89.2, -89.2, -89.2, 56.7, 56.7}	// extreme temperatures on Earth
};

/* type definition of the structured record that holds the test case values */
typedef struct {
    unsigned short id;	/* index to be checked (city or month) */
    E9M22_t xavg;		/* expected average */
    maxmin_t xmm;		/* expected max-min results */
} test_struct;


/*********************************/
/* the lists of test case values */
/*********************************/

    /* Tests for cities */
test_struct test_cases_cities[] =
{{0, E9M22_19_8,					/*  0: several replicated min and max */
    {E9M22_13_4,  E9M22_25_9,
     E9M22_56_12, E9M22_78_62,
     0, 6}},
 {1, E9M22_n9_8,					/*  1: all negatives */
    {E9M22_n18_8, E9M22_n2_2,
     E9M22_n1_84, E9M22_28_04,
     6, 0}},
 {2, E9M22_0_2,						/*  2: all values around 0° Celsius */
    {E9M22_n0_9,  E9M22_0_8,
     E9M22_30_38, E9M22_33_44,
     5, 3}},
 {3, E9M22_n16_25,					/*  3: extreme temperatures on Earth duplicated */
    {E9M22_n89_2,  E9M22_56_7,
     E9M22_n128_6, E9M22_134_0,
     4, 0}},

	// To-do? Més tests estudiants?

};


    /* Tests  for months */
test_struct test_cases_months[] =
{{0, E9M22_17_0,					/*  0: first column (January) */
    {E9M22_n2_2, E9M22_56_7,
     E9M22_28_0, E9M22_134_0,
     1, 3}},
 {6, E9M22_n20_7,					/*  1: middle column (July) */
    {E9M22_n89_2, E9M22_25_9,
     E9M22_n128_6, E9M22_78_6,
     3, 0}},
 {8, E9M22_n21_025,					/*  2: middle 2nd half column (September) */
    {E9M22_n89_2, E9M22_20_0,
     E9M22_n128_6, E9M22_68_0,
     3, 0}},
 {11, E9M22_19_675,					/*  3: last column (December) */
    {E9M22_n3_0, E9M22_56_7,
     E9M22_26_6, E9M22_134_0,
     1, 3}}

	// To-do? Més tests estudiants?

};


/* ini_temperatures(): funció per a convertir valor float a la taula ftemp
            en valors E9M22 a la taula ttemp.
    Paràmetres:
        ftemp[][12]	→ taula amb valors float de temperatures;
        ttemp[][12]	→ taula amb valors E9M22 corresponents als valor float de ftemp;
        num_cities	→ número de ciutats, per a ambdues taules ftemp[][] i ttemp[][];
    Resultats:
        ttemp[][12]	→ les temperatures float de ftemp convertides E9M22.
*/
void ini_temperatures(float ftemp[][12], E9M22_t ttemp[][12], unsigned short num_cities)
{
    unsigned short i, j;
    
    for (i = 0; i < num_cities; i++)	// recorregut de totes les ciutats
        for (j = 0; j < 12; j++)		// recorregut de tots els mesos
            ttemp[i][j] = float_to_E9M22(ftemp[i][j]);
}


/* error_bits(): funció per detectar errors en valors retornats per rutines 
            avgmaxmin_city i avgmaxmin_month.
    Paràmetres:
        avg		→ valor calculat/resultat de mitjana de temperatures;
        maxmin	→ resta de valors calculats/resultats;
        xavg	→ valor esperat de mitjana de temperatures;
        maxmin	→ resta de valors esperats (calculats/resultats);
    Resultat:
        valor amb bits actius en cas d'error:
        • bit 0: error en avg
        • bit 1: error en tmin_C
        • bit 2: error en tmax_C
        • bit 3: error en tmin_F
        • bit 4: error en tmax_F
        • bit 5: error en id_min
        • bit 6: error en id_max
*/
unsigned char error_bits(E9M22_t avg, maxmin_t *maxmin, E9M22_t xavg, maxmin_t *xmm)
{
    unsigned char nerr = 0;
    E9M22_t error;						/* marginal error */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"	// Evitar warning xavg_f, etc.
    float error_f, avg_f, xavg_f;		/* float values for debug */

    avg_f = E9M22_to_float_c_(avg);
    xavg_f = E9M22_to_float_c_(xavg);

    error = E9M22_abs_c_( E9M22_sub_c_(avg , xavg) );
    error_f = E9M22_to_float_c_(error);
    if (E9M22_is_gt_c_(error, E9M22_0_05))	/* average divergence error */
        nerr |= 1;						// set bit 0

    error = E9M22_abs_c_( E9M22_sub_c_(maxmin->tmin_C , xmm->tmin_C) );
    error_f = E9M22_to_float_c_(error);
    if (E9M22_is_gt_c_(error, E9M22_0_0001))	/* min temp. (°C) divergence error */
        nerr |= 2;						// set bit 1

    error = E9M22_abs_c_( E9M22_sub_c_(maxmin->tmax_C , xmm->tmax_C) );
    error_f = E9M22_to_float_c_(error);
    if (E9M22_is_gt_c_(error, E9M22_0_0001))	/* max temp. (°C) divergence error */
        nerr |= 4;						// set bit 2
    
    error = E9M22_abs_c_( E9M22_sub_c_(maxmin->tmin_F , xmm->tmin_F) );
    error_f = E9M22_to_float_c_(error);
    if (E9M22_is_gt_c_(error, E9M22_0_1))	/* min temp. (°F) divergence error */
        nerr |= 8;						// set bit 3
    
    error = E9M22_abs_c_( E9M22_sub_c_(maxmin->tmax_F , xmm->tmax_F) );
    error_f = E9M22_to_float_c_(error);
    if (E9M22_is_gt_c_(error, E9M22_0_1))	/* max temp. (°F) divergence error */
        nerr |= 16;						// set bit 4
    
    if (maxmin->id_min != xmm->id_min) /* min index wrong error */
        nerr |= 32;						// set bit 5
    
    if (maxmin->id_max != xmm->id_max) /* max index wrong error */
        nerr |= 64;						// set bit 6

    return(nerr);
}


	/* Function to apply test cases; returns percentage of OK's */
int apply_tests( E9M22_t (*func_to_test)		// Funció a testejar (i declaració de paràmetres)
					(E9M22_t[][12], unsigned short, unsigned short, maxmin_t *), 
				 test_struct test_cases[],		// Vector de casos a provar
				 unsigned int num_cases			// Número de casos al vector
				 )							// retorna el percentatge de casos correctes
{
    unsigned int i;						/* loop index */
    E9M22_t avg;						/* routine results */
    maxmin_t maxmin;
    unsigned int num_ok = 0;			/* number of right tests */
    unsigned int num_ko = 0;			/* number of wrong tests */
    unsigned int percent_ok, quo, res;
    unsigned char errors;

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        avg = func_to_test(test_data, NUM_TEST_ROWS, test_cases[i].id, &maxmin);
        
        errors = error_bits(avg, &maxmin, test_cases[i].xavg, &test_cases[i].xmm);
        if ( !errors )
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
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
    unsigned int perc_ok_cities = 0;	/* percentage of right tests avgmaxmin_city */
    unsigned int perc_ok_months = 0;	/* percentage of right tests avgmaxmin_month */

    ini_temperatures(f_test_data, test_data, NUM_TEST_ROWS);	// dades float → E9M22

		// Jocs de proves avgmaxmin_city
	perc_ok_cities = apply_tests( avgmaxmin_city,	// routine to test
					test_cases_cities, NUM_ELEMS(test_cases_cities) );	// test cases

		// Jocs de proves avgmaxmin_month
	perc_ok_months = apply_tests( avgmaxmin_month,	// routine to test
					test_cases_months, NUM_ELEMS(test_cases_months) );	// test cases


/* TESTING POINT: check if all percentages of OK tests 
                  are greater than 70% required
				  to deliver the FC practice.
    (gdb) p perc_ok_cities
    (gdb) p perc_ok_months
*/

/* BREAKPOINT */
    return(0);
}
