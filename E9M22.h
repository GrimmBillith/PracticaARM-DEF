/*-----------------------------------------------------------------
|   Declaració de tipus, màscares, operacions i macro del format
|	Coma Flotant E9M22 (bits: 1 signe, 9 exponent, 22 mantissa.
| -----------------------------------------------------------------
|	pere.millan@urv.cat
|	(Febrer-Març 2025)
| ----------------------------------------------------------------*/

#ifndef E9M22_H
#define E9M22_H

#include "E9M22_impl.h"

#include <stdint.h>	/* uint32_t */
#include <string.h> /* memcpy */

/* Declaració de tipus Coma Flotant E9M22 
         1 bit  de signe (bit 31), =0 -> positiu, =1 -> negatiu
         9 bits d'exponent (bits 30..22), en excés 255
        22(+1) bits de mantissa (bits 21..0), aprox. 6-7 dígits en base 10
*/
typedef unsigned int E9M22_t;

    /* Número de bits i excés/bias del tipus coma flotant E9M22 */
#define E9M22_e		9				/* quantitat de bits d'exponent: 9 */
#define E9M22_m		(31-E9M22_e)	/* quantitat de bits de mantissa/significand: 22 */
#define E9M22_bias	((1<<( E9M22_e - 1))-1)	/* bias/excés: 255 */

    /* Exponents màxim i mínim del tipus coma flotant E9M22 */
#define E9M22_Emax		E9M22_bias
#define E9M22_Emin		(1-E9M22_bias)


    /* MÀSCARES per als camps de bits de valors E9M22 */
#define E9M22_MASK_FRAC	((1<<E9M22_m)-1)	/* bits 21..0:	fracció/mantissa/significand 0x003FFFFF */
#define E9M22_MASK_EXP	(0x7FFFFFFF ^ E9M22_MASK_FRAC)	
                                            /* bits 30..22:	exponent (en excés 255) 0x7FC00000 */
#define E9M22_MASK_SIGN	(1<<31)				/* bit 31:		signe 0x80000000 */
#define E9M22_1_IMPLICIT_NORMAL (1<<E9M22_m)	/* 1 implícit als valors normals */


/* Macros per esbrinar el tipus de número E9M22 (usable en C en temps de compilació per a literals) */
#define E9M22_IS_NORMAL(e9m22)		(((e9m22 & E9M22_MASK_EXP) != 0) && ((e9m22 & E9M22_MASK_EXP) != E9M22_MASK_EXP))
#define E9M22_IS_DENORMAL(e9m22)	(((e9m22 & E9M22_MASK_EXP) == 0) && ((e9m22 & E9M22_MASK_MANT) != 0))
#define E9M22_IS_ZERO(e9m22)		 ((e9m22 & (E9M22_MASK_EXP | E9M22_MASK_FRAC)) == 0)
#define E9M22_IS_INFINITE(e9m22)	(((e9m22 & E9M22_MASK_EXP) == E9M22_MASK_EXP) && ((e9m22 & E9M22_MASK_FRAC) == 0))
#define E9M22_IS_NAN(e9m22)			(((e9m22 & E9M22_MASK_EXP) == E9M22_MASK_EXP) && ((e9m22 & E9M22_MASK_FRAC) != 0))
#define E9M22_IS_FINITE(e9m22)		 ((e9m22 & E9M22_MASK_EXP) != E9M22_MASK_EXP)	/* Ni ±∞, ni NaN */
#define E9M22_IS_NEGATIVE(e9m22)	  (e9m22 & E9M22_MASK_SIGN)


    /* Alguns valors en coma flotant E9M22 */
#define E9M22_ZERO_POS	0			/* +0,0 en coma flotant E9M22 */
#define E9M22_ZERO_NEG	0x80000000	/* -0,0 en coma flotant E9M22 */
#define E9M22_INF_POS	0x7FC00000	/* +Infinit en coma flotant E9M22 */
#define E9M22_INF_NEG	0xFFC00000	/* -Infinit en coma flotant E9M22 */
#define E9M22_qNAN		0x7FE00000	/* Un possible NaN (quiet) en coma flotant E9M22 */
#define E9M22_sNAN		0x7FD00000	/* Un possible NaN (signaling) en coma flotant E9M22 */

#define E9M22_MAX_NORM	0x7FBFFFFF	// Valor màxim normalitzat (positiu)
#define E9M22_MIN_NORM	0x004FFFFF	// Valor mínim normalitzat (positiu)
#define E9M22_MAX_DNORM	0x003FFFFF	// Valor màxim denormal (positiu)
#define E9M22_MIN_DNORM	0x00000001	// Valor mínim denormal (positiu)

#define E9M22_1			0x3FC00000	// Valor +1,0 en format E9M22
#define E9M22_2			0x40000000	// Valor +2,0 en format E9M22
#define E9M22_5			0x40500000	// Valor +5,0 en format E9M22
#define E9M22_10		0x40900000	// Valor +10,0 en format E9M22
#define E9M22_20		0x40D00000	// Valor +20,0 en format E9M22
#define E9M22_50		0x41240000	// Valor +50,0 en format E9M22
#define E9M22_100		0x41640000	// Valor +100,0 en format E9M22
#define E9M22_200		0x41A40000	// Valor +200,0 en format E9M22
#define E9M22_500		0x41FD0000	// Valor +500,0 en format E9M22

#define E9M22_1000		0x423D0000	// Valor +1.000,0 en format E9M22
#define E9M22_2000		0x427D0000	// Valor +2.000,0 en format E9M22
#define E9M22_5000		0x42CE2000	// Valor +5.000,0 en format E9M22
#define E9M22_10_000	0x430E2000	// Valor +10.000,0 en format E9M22
#define E9M22_20_000	0x434E2000	// Valor +20.000,0 en format E9M22
#define E9M22_50_000	0x43A1A800	// Valor +50.000,0 en format E9M22
#define E9M22_100_000	0x43E1A800	// Valor +100.000,0 en format E9M22
#define E9M22_200_000	0x4421A800	// Valor +200.000,0 en format E9M22
#define E9M22_500_000	0x447A1200	// Valor +500.000,0 en format E9M22

#define E9M22_1_000_000	0x44BA1200	// Valor +1.000.000,0 en format E9M22
#define E9M22_2_000_000	0x44FA1200	// Valor +2.000.000,0 en format E9M22
#define E9M22_5_000_000	0x454C4B40	// Valor +5.000.000,0 en format E9M22

#define E9M22_0_1		0x3EE66666	// Aprox. +0,1 en format E9M22
#define E9M22_0_2		0x3F266666	// Aprox. +0,2 en format E9M22
#define E9M22_0_5		0x3F800000	// Valor +0,5 en format E9M22
#define E9M22_0_01		0x3E11EB85	// Aprox. +0,01 en format E9M22
#define E9M22_0_02		0x3E51EB85	// Aprox. +0,02 en format E9M22
#define E9M22_0_05		0x3EA66666	// Aprox. +0,05 en format E9M22
#define E9M22_0_001		0x3D418937	// Aprox. +0,001 en format E9M22
#define E9M22_0_002		0x3D818937	// Aprox. +0,002 en format E9M22
#define E9M22_0_005		0x3DD1EB85	// Aprox. +0,005 en format E9M22
#define E9M22_0_0001	0x3C68DB8B	// Aprox. +0,0001 en format E9M22
#define E9M22_0_0002	0x3CA8DB8B	// Aprox. +0,0002 en format E9M22
#define E9M22_0_0005	0x3D018937	// Aprox. +0,0005 en format E9M22


/* Macro per convertir valors reals en valors Coma Flotant E9M22
        Exemples:
        MAKE_E9M22(3.1416) --> 0x402487FC
        MAKE_E9M22(-5.125) --> 0xC0520000
*/

    /* Per accedir als bits d'un float  */
#define FLOAT_TO_BITS(f) ({ \
    union { float f; uint32_t u; } _tmp; \
    _tmp.f = (f); \
    _tmp.u; \
})	

    /* Per accedir als bits d'un float (alternativa) */
#define FLOAT_BITS(f) ({                               \
    uint32_t bits;                                     \
    memcpy(&bits, &(float){ (f) }, sizeof(bits));      \
    bits;                                              \
})


    /* Macro per convertir valors float a E9M22 (no considera valors NaN ni infinits) */
    /* 		(només usable dins de codi de funcions) */
#define MAKE_E9M22(float_E8M23) ( !(FLOAT_BITS(float_E8M23) & 0x7F800000) ? /* Si és zero o denormal */ \
 (FLOAT_BITS(float_E8M23) & 0x80000000) : /* codifiquem zero, mantenint el signe*/ \
( (FLOAT_BITS(float_E8M23) & 0x80000000) | \
  ((((FLOAT_BITS(float_E8M23) & 0x7F800000) >> 23) -127 +255) << 22) \
 | ((FLOAT_BITS(float_E8M23) & 0x7FFFFF) >> 1) )	\
 )


/******************************************************/
/* Rutines de CONVERSIÓ de valors E9M22 <-> float/int */
/******************************************************/

    /* E9M22_to_float(): converteix un valor E9M22 a float. */
extern float E9M22_to_float_c_(E9M22_t num);
extern float E9M22_to_float_s(E9M22_t num);

    /* float_to_E9M22(): converteix un valor float a E9M22. */
extern E9M22_t float_to_E9M22_c_(float num);
extern E9M22_t float_to_E9M22_s(float num);

    /* E9M22_to_int(): converteix un valor E9M22 a int. */
extern int E9M22_to_int_c_(E9M22_t num);
extern int E9M22_to_int_s(E9M22_t num);

    /* int_to_E9M22(): converteix un valor int a E9M22. */
extern E9M22_t int_to_E9M22_c_(int num);
extern E9M22_t int_to_E9M22_s(int num);



/*************************************************/
/* Operacions ARITMÈTIQUES en Coma Flotant E9M22 */
/*************************************************/

/* E9M22_add():	calcula i retorna la suma dels 2 operands,
                (num1 + num2) codificats en coma flotant E9M22.
*/
extern E9M22_t E9M22_add_c_(E9M22_t num1, E9M22_t num2);
extern E9M22_t E9M22_add_s(E9M22_t num1, E9M22_t num2);


/* E9M22_sub():	calcula i retorna la diferència dels 2 operands,
                (num1 - num2) codificats en coma flotant E9M22.
*/
extern E9M22_t E9M22_sub_c_(E9M22_t num1, E9M22_t num2);
extern E9M22_t E9M22_sub_s(E9M22_t num1, E9M22_t num2);


/* E9M22_mul():	calcula i retorna el producte dels 2 operands,
                (num1 * num2) codificats en coma flotant E9M22.
*/
extern E9M22_t E9M22_mul_c_(E9M22_t num1, E9M22_t num2);
extern E9M22_t E9M22_mul_s(E9M22_t num1, E9M22_t num2);


/* E9M22_div():	calcula i retorna la divisió dels 2 operands,
                (num1 / num2) codificats en coma flotant E9M22.
*/
extern E9M22_t E9M22_div_c_(E9M22_t num1, E9M22_t num2);
extern E9M22_t E9M22_div_s(E9M22_t num1, E9M22_t num2);


/* E9M22_neg():	canvia el signe (nega) de l'operand num
                codificat en coma flotant E9M22.
*/
extern E9M22_t E9M22_neg_c_(E9M22_t num);
extern E9M22_t E9M22_neg_s(E9M22_t num);


/* E9M22_abs():	calcula i retorna el valor absolut
                de l'operand num codificat en coma flotant E9M22.
*/
extern E9M22_t E9M22_abs_c_(E9M22_t num);
extern E9M22_t E9M22_abs_s(E9M22_t num);


/*************************************************************/
/* Operacions de COMPARACIÓ de números en Coma Flotant E9M22 */
/*************************************************************/

/* Totes les comparacions retornen un valor no-zero si es compleixen */
/* E9M22_are_eq(): indica si num1 == num2, considerant els operands
                   num1 i num2 codificats en coma flotant E9M22.
*/
extern int E9M22_are_eq_c_(E9M22_t num1, E9M22_t num2);
extern int E9M22_are_eq_s(E9M22_t num1, E9M22_t num2);


/* E9M22_are_ne(): indica si num1 != num2, considerant els operands
                   num1 i num2 codificats en coma flotant E9M22.
*/
extern int E9M22_are_ne_c_(E9M22_t num1, E9M22_t num2);
extern int E9M22_are_ne_s(E9M22_t num1, E9M22_t num2);


/* E9M22_are_unordered():
            indica si num1 i num2 no són "ordenables", 
            perquè num1 o num2 són NaN (en coma flotant E9M22).
*/
extern int E9M22_are_unordered_c_(E9M22_t num1, E9M22_t num2);
extern int E9M22_are_unordered_s(E9M22_t num1, E9M22_t num2);


/* E9M22_is_gt(): indica si num1 > num2, considerant els operands
                  num1 i num2 codificats en coma flotant E9M22.
*/
extern int E9M22_is_gt_c_(E9M22_t num1, E9M22_t num2);
extern int E9M22_is_gt_s(E9M22_t num1, E9M22_t num2);


/* E9M22_is_ge(): indica si num1 ≥ num2, considerant els operands
                  num1 i num2 codificats en coma flotant E9M22.
*/
extern int E9M22_is_ge_c_(E9M22_t num1, E9M22_t num2);
extern int E9M22_is_ge_s(E9M22_t num1, E9M22_t num2);


/* E9M22_is_lt(): indica si num1 < num2, considerant els operands
                  num1 i num2 codificats en coma flotant E9M22.
*/
extern int E9M22_is_lt_c_(E9M22_t num1, E9M22_t num2);
extern int E9M22_is_lt_s(E9M22_t num1, E9M22_t num2);


/* E9M22_is_le(): indica si num1 ≤ num2, considerant els operands
                  num1 i num2 codificats en coma flotant E9M22.
*/
extern int E9M22_is_le_c_(E9M22_t num1, E9M22_t num2);
extern int E9M22_is_le_s(E9M22_t num1, E9M22_t num2);


/**********************************************************/
/* Funcions auxiliars: NORMALITZACIÓ i ARRODONIMENT E9M22 */
/**********************************************************/

 	/* Funció per normalitzar i arrodonir al més proper */
extern E9M22_t E9M22_normalize_and_round_c_ (
                unsigned int signe, int exponent, unsigned int mantissa );
extern E9M22_t E9M22_normalize_and_round_s (
                unsigned int signe, int exponent, unsigned int mantissa );


/* Arrodoniment al més proper. Si es troba al mig, al més proper parell */
/* Retorna la mantissa arrodonida (+1) o l'original (truncar) sense desplaçar */
unsigned int E9M22_round_c_ ( unsigned int mantissa, unsigned int nbits_to_shift_right );
unsigned int E9M22_round_s ( unsigned int mantissa, unsigned int nbits_to_shift_right );


/****************************************************************/
/* Funcions AUXILIARS per treballar amb els bits de codificació */
/****************************************************************/

/* count_leading_zeros(): compta quants bits a 0 hi ha 
                          des del bit de més pes (esquerra).
*/
extern unsigned int count_leading_zeros_c_(unsigned int num);
extern unsigned int count_leading_zeros_s(unsigned int num);


/* count_trailing_zeros(): compta quants bits a 0 hi ha 
                           des del bit de menys pes (dreta).
*/
extern unsigned int count_trailing_zeros_c_(unsigned int num);
extern unsigned int count_trailing_zeros_s(unsigned int num);


/*************************************************************/
/* Funcions AUXILIARS per multiplicacions 32 bits -> 64 bits */
/*************************************************************/

/* umul32x32_64():	multiplica 2 operands naturals de 32 bits
                    i retorna el resultat de 64 bits.
*/
extern unsigned long long umul32x32_64(unsigned int a, unsigned int b);

/* umul32x32_2x32(): multiplica 2 operands naturals de 32 bits
                     i retorna el resultat per referència.
*/
extern void umul32x32_2x32(unsigned int a, unsigned int b,
                            unsigned int *mulLow, unsigned int *mulHigh );

#endif /* E9M22_H */
