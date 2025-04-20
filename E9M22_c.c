/*------------------------------------------------------------------
|   E9M22_c.c: operacions amb números en format Coma Flotant E9M22. 
| ------------------------------------------------------------------
|	pere.millan@urv.cat
|	(Febrer-Març 2025)
| ------------------------------------------------------------------*/

#include "E9M22.h"	/* E9M22_t: tipus Coma Flotant E9M22
                       MAKE_E9M22(real): codifica un valor real en format E9M22
                       E9M22_MASK_SIGN: màscara per obtenir el bit de signe
                    */
#include "divmod.h"		/* rutina div_mod() de divisió entera */


/******************************************************/
/* Rutines de CONVERSIÓ de valors E9M22 <-> float/int */
/******************************************************/

    /* Paràmetres del tipus C float (IEEE754 binary32) */
#define FLOAT_e		8				/* quantitat de bits d'exponent: 8 */
#define FLOAT_m		(31-FLOAT_e)	/* quantitat de bits de mantissa/significand: 23 */
#define FLOAT_bias	((1<<( FLOAT_e - 1))-1)	/* fracció/bias/excés: 127 */

    /* MÀSCARES per als camps de bits de valors float */
#define FLOAT_MASK_FRAC	((1<<FLOAT_m)-1)	/* bits 22..0:	fracció/mantissa/significand 0x007FFFFF */
#define FLOAT_MASK_EXP	(0x7FFFFFFF ^ FLOAT_MASK_FRAC)	
                                            /* bits 30..23:	exponent (en excés 127) */
#define FLOAT_MASK_SIGN	(1<<31)				/* bit 31:		signe 0x80000000 */
#define FLOAT_1_IMPLICIT_NORMAL (1<<FLOAT_m)	/* 1 implícit als valors normals */

    /* Exponents màxim i mínim del tipus C float (IEEE754 binary32) */
#define FLOAT_EXP_MIN	(1-FLOAT_bias)
#define FLOAT_EXP_MAX	FLOAT_bias



    /* E9M22_to_float(): converteix un valor E9M22 a float. */
float E9M22_to_float_c_(E9M22_t num)
{
    union { float f; uint32_t u; } resultat;
    unsigned int signe;
    int exponent, despl;

    signe = num & E9M22_MASK_SIGN;

    if ( E9M22_IS_NAN(num) )
    {	// generar NaN mantenint payload i bits q/s
        resultat.u = signe | FLOAT_MASK_EXP 
                    | ((num & 0x00300000) << 1) | (num & 0x0000FFFF);
    }
    else
    {
        exponent = ((num & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias;
        if ( E9M22_IS_INFINITE(num) || (exponent > FLOAT_EXP_MAX) )
        {	// Infinit o exponent massa gran (overflow)
            resultat.u = signe | FLOAT_MASK_EXP;	// Generar codificació ±∞
        }
        else
        {
            if ( E9M22_IS_ZERO(num) || (exponent < FLOAT_EXP_MIN) )
            {	// Zero o exponent massa petit (denormal/underflow)
                if ( exponent >= (FLOAT_EXP_MIN - FLOAT_m) )
                {
                    despl = -exponent + FLOAT_EXP_MIN -1;
                    resultat.u = signe | 
                                ( (E9M22_1_IMPLICIT_NORMAL | (num & E9M22_MASK_FRAC) ) >> despl );
                }
                else
                {	// Underflow --> zero
                    resultat.u = signe;
                }
            }
            else
            {	// Normalitzat
                resultat.u = signe | ((exponent + FLOAT_bias) << FLOAT_m) 
                            | ((num <<1) & FLOAT_MASK_FRAC) ;
            }
        }
    }

    return resultat.f;
}



    /* float_to_E9M22(): converteix un valor float a E9M22. */
E9M22_t float_to_E9M22_c_(float num)
{
    E9M22_t resultat;
    unsigned int signe, bits_float;
    int exponent, leading_zeros;

    bits_float = FLOAT_TO_BITS(num);
    signe = bits_float & FLOAT_MASK_SIGN;
    
    if ( (bits_float & FLOAT_MASK_EXP) == FLOAT_MASK_EXP )	/* Infinit o NaN */
    {
        if ( (bits_float & FLOAT_MASK_FRAC) != 0 )	/* NaN, mantenint bits q/s i payload */
        {
            resultat = signe | E9M22_MASK_EXP 
                        | ( (bits_float & 0x00600000) >> 1)	// reposicionar bits qNaN/sNaN
                        | (bits_float & 0x000FFFFF) ;		// possible payload NaN
        }
        else	/* Infinit (amb signe) */
        {
            resultat = signe | E9M22_MASK_EXP; // Bits fracció/mantisa/significand a 0
        }
    }
    else	/* Normal, Zero o Denormal */
    {
        if ( (bits_float & FLOAT_MASK_EXP) != 0 )	/* Normalitzat */
        {
                /* Calcular nou exponent codificat */
            exponent = ((bits_float & FLOAT_MASK_EXP) >> FLOAT_m ) - FLOAT_bias + E9M22_bias;
            resultat = signe | (exponent << E9M22_m ) | ((bits_float >> 1) & E9M22_MASK_FRAC) ;
        }
        else	/* Denormal o Zero */
        {
            if ( (bits_float & FLOAT_MASK_FRAC) == 0 )	/* ±Zero */
            {
                resultat = bits_float;	/* El ±Zero es codifica igual en els 2 formats */
            }
            else	/* Denormal. Codificable com a Normal en E9M22 */
            {
                exponent = FLOAT_EXP_MIN;	// -126
                resultat = bits_float & FLOAT_MASK_FRAC;
                leading_zeros = count_leading_zeros_c_(resultat);	/* On es troba el 1r bit a 1? */
                resultat <<= (leading_zeros - 8);	/* Moure 1r bit a 1 a posició de bit implícit normalitzat */
                exponent -= (leading_zeros - 8);	/* Ajustar valor de l'exponent */
                resultat = signe 		/* Combinar signe + exponent + fracció */
                            | ((exponent + E9M22_bias) << E9M22_m ) 
                            | (resultat & E9M22_MASK_FRAC);
            }
        }
    }

    return resultat;
}



    /* E9M22_to_int(): converteix un valor E9M22 a int. */
int E9M22_to_int_c_(E9M22_t num)
{
    int exponent, despl, resultat;

    exponent = ((num & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias ;

    if ( E9M22_IS_FINITE(num) && (exponent <= 30) )
    {
        if ( E9M22_IS_NORMAL(num) && (exponent >= 0) )
        {
            resultat = E9M22_1_IMPLICIT_NORMAL | (num & E9M22_MASK_FRAC);

            if (exponent != E9M22_m)
            {
                if (exponent < E9M22_m)
                {
                    despl = (E9M22_m - exponent);
                    resultat = E9M22_round_c_(resultat, despl);
                    resultat >>= despl;
                }
                else
                    resultat <<= (exponent - E9M22_m);
            }
        }
        else /* Massa petit, Denormal o Zero --> 0 */
            resultat = 0;
    }
    else
    {	// num és NaN o ±∞ o massa gran: retorna valor màxim
        resultat = 0x7FFFFFFF;
    }

    if ( E9M22_IS_NEGATIVE(num) ) resultat = -resultat;

    return resultat;
}



    /* int_to_E9M22(): converteix un valor int a E9M22. */
E9M22_t int_to_E9M22_c_(int num)
{
    E9M22_t resultat;
    int exponent;
    unsigned int signe = 0, leading_zeros, lz2, despl;

    if ( num < 0)
    {
        num = -num;
        signe = E9M22_MASK_SIGN;
    }

    leading_zeros = count_leading_zeros_c_(num);

    if (leading_zeros < 32)
    {
        exponent = 31 - leading_zeros;
        if (leading_zeros < E9M22_e)
        {	// Cal desplaçar a la dreta
            despl = E9M22_e - leading_zeros;
            resultat = E9M22_round_c_(num, despl);
            lz2 = count_leading_zeros_c_(resultat);
            if (lz2 < leading_zeros)
            {
                despl++;
                exponent++;
            }
            resultat >>= despl;
        }
        else if (leading_zeros > E9M22_e)
        {	// Cal desplaçar a l'esquerra
            despl = leading_zeros - E9M22_e;
            resultat = num << despl;
        }
        else
        {	// No cal desplaçar
            // exponent = E9M22_m;
			resultat = num;
        }
            // Combinar components E9M22:
        resultat = signe | ((exponent + E9M22_bias) << E9M22_m) | (resultat & E9M22_MASK_FRAC);
    }
    else
    {	// És el zero (positiu)
        resultat = 0;
    }

    return resultat;
}


/*************************************************/
/* Operacions ARITMÈTIQUES en Coma Flotant E9M22 */
/*************************************************/

/* E9M22_add():	calcula i retorna la suma dels 2 operands,
                (num1 + num2) codificats en coma flotant E9M22.

  # Casos "especials" amb la suma:
    • Si algun operand és NaN, la suma és NaN
    • +∞ + +∞ = +∞ / -∞ + -∞ = -∞ / +∞ + -∞ = NaN
    • ±∞ + finit = ±∞
*/
E9M22_t E9M22_add_c_(E9M22_t num1, E9M22_t num2)
{
    E9M22_t suma;
    unsigned int signe1, signe2, signe_suma, despl, dif_exp;
    int exp1, exp2, exp_suma, mant1, mant2, mant_suma;

    signe1 = num1 & E9M22_MASK_SIGN;
    signe2 = num2 & E9M22_MASK_SIGN;

    if ( E9M22_IS_FINITE(num1) && E9M22_IS_FINITE(num2) )
    {
        if ( ! E9M22_IS_ZERO(num1) && ! E9M22_IS_ZERO(num2) )
        {	/* num1 i num2 són normals o denormals */
            if ((num1 & E9M22_MASK_EXP) != 0)
            {		/* num1 normal */
                exp1 = ((num1 & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias;
                mant1 = E9M22_1_IMPLICIT_NORMAL | (num1 & E9M22_MASK_FRAC);
            }
            else
            {		/* num1 denormal */
                exp1 = E9M22_Emin;
                mant1 = num1 & E9M22_MASK_FRAC;
            }

            if ((num2 & E9M22_MASK_EXP) != 0)
            {		/* num2 normal */
                exp2 = ((num2 & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias;
                mant2 = E9M22_1_IMPLICIT_NORMAL | (num2 & E9M22_MASK_FRAC);
            }
            else
            {		/* num2 denormal */
                exp2 = E9M22_Emin;
                mant2 = num2 & E9M22_MASK_FRAC;
            }

            /* Exponents diferents? Cal ALINEAR mantisses? */
            if ( exp1 != exp2 )
            {
                if (exp1 < exp2)
                {
                    dif_exp = exp2 - exp1;
                    if (dif_exp < E9M22_e)
                    {	/* Alinear despl. esq. mant2 */
                        mant2 <<= dif_exp;
                        exp2 -= dif_exp;
                    }
                    else
                    {	/* Alinear despl. esq. mant2 ... */
                        despl = E9M22_e - 1;
                        mant2 <<= despl;
                        exp2 -= despl;

                        /* ... i despl. drt. mant1 */
                        dif_exp = exp2 - exp1;
                        mant1 >>= dif_exp;
                        exp1 += dif_exp;
                    }
                }
                else /* exp1 > exp2 */
                {
                    dif_exp = exp1 - exp2;
                    if (dif_exp < E9M22_e)
                    {	/* Alinear despl. esq. mant1 */
                        mant1 <<= dif_exp;
                        exp1 -= dif_exp;
                    }
                    else
                    {	/* Alinear despl. esq. mant1 ... */
                        despl = E9M22_e - 1;
                        mant1 <<= despl;
                        exp1 -= despl;

                        /* ... i despl. drt. mant2 */
                        dif_exp = exp1 - exp2;
                        mant2 >>= dif_exp;
                        exp2 += dif_exp;
                    }
                }
            }

            if (mant1 == 0)
                suma = num2;
            else if (mant2 == 0)
                suma = num1;
            else
            {	/* Sumar mantisses no 0, amb signe (Ca2) si tenen signe diferent */
                if (signe1 != signe2)
                {	/* Fer Ca2/negar el que sigui negatiu */
                    if ( E9M22_IS_NEGATIVE(num1) )
                        mant1 = -mant1;
                    else
                        mant2 = -mant2;
                }

                mant_suma = mant1 + mant2;	// Sumar mantisses (amb signe, Ca2)
                if (mant_suma < 0)
                {
                    mant_suma = -mant_suma;			// mantissa natural (no negativa)
                }

                    // Calcular signe resultat
                if ( (signe1 == signe2) || (E9M22_abs_c_(num1) >= E9M22_abs_c_(num2)) )
                    signe_suma = signe1;
                else
                    signe_suma = signe2;

                exp_suma = exp1; // serveix també exp2, estan alineats

                suma = E9M22_normalize_and_round_c_(signe_suma, exp_suma, mant_suma);
            }
        }
        else
        {	/* num1 o num2 són zero */
            if ( E9M22_IS_ZERO(num1) )
                suma = num2;
            else
                suma = num1;
        }
    }
    else /* num1 o num2 són NaN o ±∞ */
    {
        if ( E9M22_IS_NAN(num1) )
            suma = num1;	// NaN
        else if ( E9M22_IS_NAN(num2) )
            suma = num2;	// NaN
        else if ( E9M22_IS_INFINITE(num1) )
             {
                if ( E9M22_IS_INFINITE(num2) )
                {
                    if (signe1 == signe2)
                        suma = num1;		// ±∞ + ±∞ = ±∞
                    else
                        suma = E9M22_qNAN;	// +∞ + -∞ = NaN
                }
                else
                {
                    suma = num1;		// ±∞ + finit = ±∞
                }
             }
        else 
            suma = num2;		// finit + ±∞ = ±∞
    }

    return suma;
}


/* E9M22_sub():	calcula i retorna la diferència dels 2 operands,
                (num1 - num2) codificats en coma flotant E9M22.
                Aplica la suma amb el 2n operand negat.
*/
E9M22_t E9M22_sub_c_(E9M22_t num1, E9M22_t num2)
{
    E9M22_t resta, num2negat;

    num2negat = E9M22_neg_c_(num2);	/* ⚠️ Aplicant màscares seria més òptim */ 

        /* Restem sumant num2 negat a num1 */
    resta = E9M22_add_c_ (num1, num2negat);

    return resta;
}


/* E9M22_mul():	calcula i retorna el producte dels 2 operands,
                (num1 × num2) codificats en coma flotant E9M22.

  # Casos "especials" amb el producte:
    • Si algun operand és NaN, el producte és NaN
    • ±∞ × ±∞ = ±∞ / ±∞ × ±0 = ±NaN
    • ±∞ × finit(≠0) = ±∞
*/
E9M22_t E9M22_mul_c_(E9M22_t num1, E9M22_t num2)
{
    E9M22_t producte;
    unsigned int prod64lo, prod64hi;		// Resultat de la multiplicació (64 bits)
    unsigned int signe1, signe2, signe_prod, despl;
    unsigned int num_trailing_zeros, num_leading_zeros, sticky, mask_sticky;
    int exp1, exp2, exp_prod, mant1, mant2, mant_prod;

    signe1 = num1 & E9M22_MASK_SIGN;
    signe2 = num2 & E9M22_MASK_SIGN;
    if (signe1 == signe2)		/* ⚠️ Hi ha implementacions més eficients 🤔 */
        signe_prod = 0;	// Positiu
    else
        signe_prod = E9M22_MASK_SIGN;	// Negatiu


    if ( E9M22_IS_FINITE(num1) && E9M22_IS_FINITE(num2) )
    {
        if ( ! E9M22_IS_ZERO(num1) && ! E9M22_IS_ZERO(num2) )
        {	/* num1 i num2 són normals o denormals */
            if ((num1 & E9M22_MASK_EXP) != 0)
            {		/* num1 normal */
                exp1 = ((num1 & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias;
                mant1 = E9M22_1_IMPLICIT_NORMAL | (num1 & E9M22_MASK_FRAC);
            }
            else
            {		/* num1 denormal */
                exp1 = E9M22_Emin;
                mant1 = num1 & E9M22_MASK_FRAC;
            }

            if ((num2 & E9M22_MASK_EXP) != 0)
            {		/* num2 normal */
                exp2 = ((num2 & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias;
                mant2 = E9M22_1_IMPLICIT_NORMAL | (num2 & E9M22_MASK_FRAC);
            }
            else
            {		/* num2 denormal */
                exp2 = E9M22_Emin;
                mant2 = num2 & E9M22_MASK_FRAC;
            }

            /* Ajustar mantisses per minimitzar bits resultat producte */
            num_trailing_zeros = count_trailing_zeros_c_(mant1);
            if (num_trailing_zeros > 0)
            {
                mant1 >>= num_trailing_zeros;	// Eliminar zeros per la dreta
                exp1 += num_trailing_zeros;		// Ajustar exponent 1
            }

            num_trailing_zeros = count_trailing_zeros_c_(mant2);
            if (num_trailing_zeros > 0)
            {
                mant2 >>= num_trailing_zeros;	// Eliminar zeros per la dreta
                exp2 += num_trailing_zeros;		// Ajustar exponent 2
            }

                // multiplicació de 64 bits, podem tenir fins a 2*23 = 46 bits
            umul32x32_2x32(mant1, mant2, &prod64lo, &prod64hi);
            exp_prod = exp1 + exp2 - E9M22_m;	// Exponent (ajustat) producte

            // Verificar si el resultat supera els 32 bits (32 bits alts != 0):
            if ( prod64hi !=0 )
            {
                num_leading_zeros = count_leading_zeros_c_(prod64hi);
                despl = 32 - num_leading_zeros;
                    /* Detectar si els bits que "es perdran" amb el desplaçament són 0 o no-0 */
                mask_sticky = (1<<despl) - 1;	// Màscara per accedir als bits que es "perdran"
                sticky = ((prod64lo & mask_sticky) != 0) ? 1 : 0;
                    /* Convertir resultat a 32 bits i ajustar exponent: */
                mant_prod = (prod64hi << num_leading_zeros) | (prod64lo >> despl);
                exp_prod += despl;
                    /* Afegir informació dels bits "perduts": */
                mant_prod |= sticky;
                
            }
            else	// El resultat de la multiplicació ja es troba als 32 bits baixos de prod64
            {
                mant_prod = prod64lo;	// Agafar els 32 bits baixos de prod64 (els 32 alts són 0)
            }

            producte = E9M22_normalize_and_round_c_(signe_prod, exp_prod, mant_prod);
        }
        else
        {	/* num1 o num2 són zero */
            producte = signe_prod;	// ±0
        }
    }
    else /* num1 o num2 són NaN o ±∞ */
    {
        if ( E9M22_IS_NAN(num1) )
            producte = num1;	// NaN
        else if ( E9M22_IS_NAN(num2) )
            producte = num2;	// NaN
        else if ( E9M22_IS_INFINITE(num1) )
             {
                if ( E9M22_IS_ZERO(num2) )
                {		// ±∞ × ±0 = ±NaN
                    producte = signe_prod | E9M22_qNAN;
                }
                else
                {		// ±∞ × (≠0) = ±∞
                    producte = signe_prod | E9M22_INF_POS;
                }
             }
        else // num2 = ±∞
             {
                if ( E9M22_IS_ZERO(num1) )
                {		// ±∞ × ±0 = ±NaN
                    producte = signe_prod | E9M22_qNAN;
                }
                else
                {		// (≠0) × ±∞ = ±∞
                    producte = signe_prod | E9M22_INF_POS;
                }
             }
    }

    return producte;
}


/* E9M22_div():	calcula i retorna la divisió dels 2 operands,
                (num1 ÷ num2) codificats en coma flotant E9M22.

  # Casos "especials" amb la divisió:
    • Si algun operand és NaN, la divisió és NaN
    • ±∞ ÷ ±∞ / ±0 ÷ ±0 = ±NaN
    • num ÷ ±∞ = ±0
    • ±∞ ÷ num = ±∞
    • finit(≠0) ÷ ±0 = ±∞
*/
E9M22_t E9M22_div_c_(E9M22_t num1, E9M22_t num2)
{
    E9M22_t divisio;
    unsigned int quo, res;	// Quocient i residu de div_mod()
    unsigned int signe1, signe2, signe_div;
    unsigned int num_trailing_zeros, num_leading_zeros;
    int exp1, exp2, exp_div, mant1, mant2, mant_div;

    signe1 = num1 & E9M22_MASK_SIGN;
    signe2 = num2 & E9M22_MASK_SIGN;
    if (signe1 == signe2)		/* ⚠️ Hi ha implementacions més eficients 🤔 */
        signe_div = 0;	// Positiu
    else
        signe_div = E9M22_MASK_SIGN;	// Negatiu


    if ( E9M22_IS_FINITE(num1) && E9M22_IS_FINITE(num2) )
    {
        if ( ! E9M22_IS_ZERO(num1) && ! E9M22_IS_ZERO(num2) )
        {	/* num1 i num2 són normals o denormals */
            if ((num1 & E9M22_MASK_EXP) != 0)
            {		/* num1 normal */
                exp1 = ((num1 & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias;
                mant1 = E9M22_1_IMPLICIT_NORMAL | (num1 & E9M22_MASK_FRAC);
            }
            else
            {		/* num1 denormal */
                exp1 = E9M22_Emin;
                mant1 = num1 & E9M22_MASK_FRAC;
            }

            if ((num2 & E9M22_MASK_EXP) != 0)
            {		/* num2 normal */
                exp2 = ((num2 & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias;
                mant2 = E9M22_1_IMPLICIT_NORMAL | (num2 & E9M22_MASK_FRAC);
            }
            else
            {		/* num2 denormal */
                exp2 = E9M22_Emin;
                mant2 = num2 & E9M22_MASK_FRAC;
            }

            /* Ajustar mantisses per maximitzar bits resultat divisió:
                numerador màxim (lsl), denominador mínim (lsr) */
            num_leading_zeros = count_leading_zeros_c_(mant1);
            mant1 <<= num_leading_zeros;	// Eliminar zeros per l'esquerra
            exp1 -= num_leading_zeros;		// Ajustar exponent 1

            num_trailing_zeros = count_trailing_zeros_c_(mant2);
            if (num_trailing_zeros > 0)
            {
                mant2 >>= num_trailing_zeros;	// Eliminar zeros per la dreta
                exp2 += num_trailing_zeros;		// Ajustar exponent 2
            }

            div_mod(mant1, mant2, &quo, &res);
            mant_div = quo;

            exp_div = exp1 - exp2 + E9M22_m;	// Exponent (ajustat) divisio

            divisio = E9M22_normalize_and_round_c_(signe_div, exp_div, mant_div);
        }
        else
        {	/* num1 o num2 són zero */
            if (E9M22_IS_ZERO(num1))
            {	// Numerador és ±0
                if (E9M22_IS_ZERO(num2))
                    divisio = signe_div | E9M22_qNAN; // ±0 ÷ ±0 = ±NaN
                else
                    divisio = signe_div | E9M22_ZERO_POS; // ±0 ÷ (≠0) = ±0
            }
            else // num2 és ±0 i num1 no
            {
                divisio = signe_div | E9M22_INF_POS; //  (≠0) ÷ ±0 = ±∞
            }
        }
    }
    else /* num1 o num2 són NaN o ±∞ */
    {
        if ( E9M22_IS_NAN(num1) )
            divisio = num1;	// NaN
        else if ( E9M22_IS_NAN(num2) )
            divisio = num2;	// NaN
        else if ( E9M22_IS_INFINITE(num1) )
             {
                if ( E9M22_IS_INFINITE(num2) )
                {		// ±∞ ÷ ±∞ = ±NaN
                    divisio = signe_div | E9M22_qNAN;
                }
                else
                {		// ±∞ ÷ (≠∞) = ±∞
                    divisio = signe_div | E9M22_INF_POS;
                }
             }
        else // num2 = ±∞ i num1 no
             {		// (≠∞) ÷ ±∞ = ±0
                divisio = signe_div | E9M22_ZERO_POS;
             }
    }

    return divisio;
}


/* E9M22_neg():	canvia el signe (nega) de l'operand num
                codificat en coma flotant E9M22.
*/
E9M22_t E9M22_neg_c_(E9M22_t num)
{
    E9M22_t resultat;

        /* ⚠️ Funciona, però hi ha formes més òptimes de fer-ho */ 
    if ( E9M22_IS_NEGATIVE(num) )
            /* Posar a 0 el bit de signe (negatiu → positiu) */
        resultat = num & ~E9M22_MASK_SIGN;
    else
            /* Posar a 1 el bit de signe (positiu → negatiu) */
        resultat = num | E9M22_MASK_SIGN;
    
    return resultat;
}


/* E9M22_abs():	calcula i retorna el valor absolut
                de l'operand num codificat en coma flotant E9M22.
*/
E9M22_t E9M22_abs_c_(E9M22_t num)
{
    E9M22_t resultat;

        /* ⚠️ Funciona, però hi ha formes més òptimes de fer-ho */ 
    if ( E9M22_IS_NEGATIVE(num) )
            /* Posar a 0 el bit de signe (negatiu → positiu) */
        resultat = num & ~E9M22_MASK_SIGN;
    else
            /* num ja és positiu */
        resultat = num;

    return resultat;
}


/*************************************************************/
/* Operacions de COMPARACIÓ de números en Coma Flotant E9M22 */
/*************************************************************/

/* Regles generals d'ordenació de números en coma flotant:
    • NaN no és comparable amb res (ni amb un altre NaN: NaN ≠ NaN).
    • +0.0 = -0.0
    • -∞ < -Normals < -Denormals < -0.0 = +0.0 < +Denormals < +Normals < +∞
*/


/* E9M22_are_eq(): indica si num1 == num2, considerant els operands
                   num1 i num2 codificats en coma flotant E9M22.
                   Retorna un valor no-zero si num1 == num2.
*/
int E9M22_are_eq_c_(E9M22_t num1, E9M22_t num2)
{
    int resultat;

    if ( E9M22_IS_NAN(num1) || E9M22_IS_NAN(num2) )
    {
        resultat = 0;
    }
    else 
    {
        if ( num1 == num2 )
        {
            resultat = 1;
        }
        else 
        {
            if ( E9M22_IS_ZERO(num1) && E9M22_IS_ZERO(num2) )
            {
                resultat = 1;
            }
            else
            {
                resultat = 0;
            }
        }
    }

    return resultat;
}


/* E9M22_are_ne(): indica si num1 ≠ num2, considerant els operands
                   num1 i num2 codificats en coma flotant E9M22.
                   Retorna un valor no-zero si num1 ≠ num2.
*/
int E9M22_are_ne_c_(E9M22_t num1, E9M22_t num2)
{
    int resultat;

    if ( E9M22_IS_NAN(num1) || E9M22_IS_NAN(num2) )
    {
        resultat = 1;
    }
    else 
    {
        if ( E9M22_IS_ZERO(num1) && E9M22_IS_ZERO(num2) )
        {
            resultat = 0;
        }
        else 
        {
            resultat = ( num1 != num2 );
        }
    }

    return resultat;
}


/* E9M22_are_unordered():
            indica si num1 i num2 no són "ordenables", 
            perquè num1 o num2 són NaN (en coma flotant E9M22).
            Retorna un valor no-zero si num1 i num2
            no es poden ordenar.
*/
int E9M22_are_unordered_c_(E9M22_t num1, E9M22_t num2)
{
    int resultat;

    if ( E9M22_IS_NAN(num1) || E9M22_IS_NAN(num2) )
    {
        resultat = 1;
    }
    else 
    {
        resultat = 0;
    }

    return resultat;
}


/* E9M22_is_gt(): indica si num1 > num2, considerant els operands
                  num1 i num2 codificats en coma flotant E9M22.
                  Retorna un valor no-zero si num1 > num2.
*/
int E9M22_is_gt_c_(E9M22_t num1, E9M22_t num2)
{
    int resultat;

    if ( E9M22_IS_NAN(num1) || E9M22_IS_NAN(num2) )
    {
        resultat = 0;
    }
    else 
    {
        if ( E9M22_IS_ZERO(num1) && E9M22_IS_ZERO(num2) )
        {
            resultat = 0;
        }
        else 
        {
            if ( (num1 & E9M22_MASK_SIGN) == (num2 & E9M22_MASK_SIGN))
                /* num1 i num 2 amb el mateix signe */
            {
                if ( E9M22_IS_NEGATIVE(num1) )
                {	/* amb negatius va al revés */
                    resultat = (num1 < num2);
                }
                else	/* amb positius va normal */
                {
                    resultat = (num1 > num2);
                }
            }
            else	/* num1 i num 2 amb signes diferents */
            {
                resultat = E9M22_IS_NEGATIVE(num2);
            }
        }
    }

    return resultat;
}


/* E9M22_is_ge(): indica si num1 ≥ num2, considerant els operands
                  num1 i num2 codificats en coma flotant E9M22.
                  Retorna un valor no-zero si num1 ≥ num2.
*/
int E9M22_is_ge_c_(E9M22_t num1, E9M22_t num2)
{
    int resultat;

    if ( E9M22_IS_NAN(num1) || E9M22_IS_NAN(num2) )
    {
        resultat = 0;
    }
    else 
    {
        if ( E9M22_IS_ZERO(num1) && E9M22_IS_ZERO(num2) )
        {
            resultat = 1;
        }
        else 
        {
            if ( (num1 & E9M22_MASK_SIGN) == (num2 & E9M22_MASK_SIGN))
                /* num1 i num 2 amb el mateix signe */
            {
                if ( E9M22_IS_NEGATIVE(num1) )
                {	/* amb negatius va al revés */
                    resultat = (num1 <= num2);
                }
                else	/* amb positius va normal */
                {
                    resultat = (num1 >= num2);
                }
            }
            else	/* num1 i num 2 amb signes diferents */
            {
                resultat = E9M22_IS_NEGATIVE(num2);
            }
        }
    }

    return resultat;
}


/* E9M22_is_lt(): indica si num1 < num2, considerant els operands
                  num1 i num2 codificats en coma flotant E9M22.
                  Retorna un valor no-zero si num1 < num2.
*/
int E9M22_is_lt_c_(E9M22_t num1, E9M22_t num2)
{
    int resultat;

    if ( E9M22_IS_NAN(num1) || E9M22_IS_NAN(num2) )
    {
        resultat = 0;
    }
    else 
    {
        if ( E9M22_IS_ZERO(num1) && E9M22_IS_ZERO(num2) )
        {
            resultat = 0;
        }
        else 
        {
            if ( (num1 & E9M22_MASK_SIGN) == (num2 & E9M22_MASK_SIGN))
                /* num1 i num 2 amb el mateix signe */
            {
                if ( E9M22_IS_NEGATIVE(num1) )
                {	/* amb negatius va al revés */
                    resultat = (num1 > num2);
                }
                else	/* amb positius va normal */
                {
                    resultat = (num1 < num2);
                }
            }
            else	/* num1 i num 2 amb signes diferents */
            {
                resultat = E9M22_IS_NEGATIVE(num1);
            }
        }
    }

    return resultat;
}


/* E9M22_is_le(): indica si num1 ≤ num2, considerant els operands
                  num1 i num2 codificats en coma flotant E9M22.
                  Retorna un valor no-zero si num1 ≤ num2.
*/
int E9M22_is_le_c_(E9M22_t num1, E9M22_t num2)
{
    int resultat;

    if ( E9M22_IS_NAN(num1) || E9M22_IS_NAN(num2) )
    {
        resultat = 0;
    }
    else 
    {
        if ( E9M22_IS_ZERO(num1) && E9M22_IS_ZERO(num2) )
        {
            resultat = 1;
        }
        else 
        {
            if ( (num1 & E9M22_MASK_SIGN) == (num2 & E9M22_MASK_SIGN))
                /* num1 i num 2 amb el mateix signe */
            {
                if ( E9M22_IS_NEGATIVE(num1) )
                {	/* amb negatius va al revés */
                    resultat = (num1 >= num2);
                }
                else	/* amb positius va normal */
                {
                    resultat = (num1 <= num2);
                }
            }
            else	/* num1 i num 2 amb signes diferents */
            {
                resultat = E9M22_IS_NEGATIVE(num1);
            }
        }
    }

    return resultat;
}



/**********************************************************/
/* Funcions auxiliars: NORMALITZACIÓ i ARRODONIMENT E9M22 */
/**********************************************************/

    /* Funció per normalitzar i arrodonir al més proper */
E9M22_t E9M22_normalize_and_round_c_(
            unsigned int signe, int exponent, unsigned int mantissa)
{
    E9M22_t resultat;
    unsigned leading_zeros, lz2, despl;

    leading_zeros = count_leading_zeros_c_(mantissa); 

    if (leading_zeros < 32)
    {
        exponent += E9M22_e - leading_zeros;
        if (leading_zeros < E9M22_e)
        {	// Cal desplaçar a la dreta
            despl = E9M22_e - leading_zeros;
            resultat = E9M22_round_c_(mantissa, despl);
            lz2 = count_leading_zeros_c_(resultat);
            if (lz2 < leading_zeros)
            {
                despl++;
                exponent++;
            }
            resultat >>= despl;
        }
        else if (leading_zeros > E9M22_e)
        {	// Cal desplaçar a l'esquerra
            despl = leading_zeros - E9M22_e;
            resultat = mantissa << despl;
        }
        else
        {	// No cal desplaçar
            // exponent = E9M22_m;
            resultat = mantissa;
        }
        
        // Exponent fora de rang?
        if (exponent > E9M22_Emax)
        {	// Overflow: ±∞
            resultat = signe | E9M22_MASK_EXP;
        }
        else if ( exponent < E9M22_Emin )
        {	// Underflow: Denormal o ±zero
            despl = E9M22_Emin - exponent;
            resultat = signe | (mantissa >> despl);
        }
        else
        {
            // Normalitzat, combinar components E9M22:
            resultat = signe | ((exponent + E9M22_bias) << E9M22_m) | (resultat & E9M22_MASK_FRAC);
        }
    }
    else
    {	// És el ±zero 
        resultat = signe;
    }

    return resultat;
}



/* Arrodoniment al més proper. Si es troba al mig, al més proper parell */
/* Retorna la mantissa arrodonida (+1) o l'original (trunc) sense desplaçar */
unsigned int E9M22_round_c_(unsigned int mantissa, unsigned int nbits_to_shift_right)
{
    unsigned int resultat, mask_guard, mask_round, mask_sticky;
    unsigned char guard, round, sticky; // LSB mantissa i els 2 següents bits a la seva dreta

    resultat = mantissa;

    if (nbits_to_shift_right >= 2)
    {
        mask_guard = 1 << nbits_to_shift_right;	// El bit de menys pes del valor normalitzat
        guard = ((mantissa & mask_guard) != 0) ? 1 : 0;
        mask_round = mask_guard >> 1;	// El bit a la dreta del de menys pes del valor normalitzat
        round = ((mantissa & mask_round) != 0) ? 1 : 0;
        mask_sticky = mask_round - 1;	// La resta de bits a la dreta del "round bit"
        sticky = ((mantissa & mask_sticky) != 0) ? 1 : 0;
        if ( round && ( guard || sticky) )	// cal arrodonir amunt?
        {
            resultat += mask_guard;
        }
    }
    else
        if (nbits_to_shift_right == 1)	// No hi ha sticky bit (es considera 0)
        {
            if ( (mantissa&3) == 3)	// cal arrodonir amunt?
            {
                resultat += 2;
            }
        }
        else // no cal desplaçar (ni arrodonir)
        {
        }

    return resultat;	
}


/****************************************************************/
/* Funcions AUXILIARS per treballar amb els bits de codificació */
/****************************************************************/

/* count_leading_zeros(): compta quants bits a 0 hi ha 
                          des del bit de més pes (esquerra).
*/
unsigned int count_leading_zeros_c_(unsigned int num)
{
    unsigned int resultat;

            /* ⚠️ En assemblador es pot fer molt més eficient */ 
    if ( num == 0)
        resultat = 32;
    else
    {
        resultat = 0;
        while ( (num & 0x80000000) == 0)
        {
            resultat++;
            num <<= 1;
        }
    }

    return resultat;
}

/* count_trailing_zeros(): compta quants bits a 0 hi ha 
                           des del bit de menys pes (dreta).
*/
unsigned int count_trailing_zeros_c_(unsigned int num)
{
    unsigned int resultat;

    if ( num == 0)
        resultat = 32;
    else
    {
        resultat = 0;
        while ( (num & 1) == 0)
        {
            resultat++;
            num >>= 1;
        }
    }

    return resultat;
}


