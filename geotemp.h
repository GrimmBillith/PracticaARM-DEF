/*---------------------------------------------------------------------
|   Declaració de tipus per a informació del programa "geotemp.c".
| ---------------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(Març 2021, Març 2025)
| ---------------------------------------------------------------------*/

#ifndef GEOTEMP_H
#define GEOTEMP_H

  	/* declaració del tipus cityinfo_t */
typedef struct
{	
    char *name;		// nom de la ciutat
    char scale;		// escala utilitzada ('C': Celsius, 'F': Fahrenheit)
} cityinfo_t;


#endif 	/* GEOTEMP_H */
