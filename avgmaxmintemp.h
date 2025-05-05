/*---------------------------------------------------------------------
|   Declaració de prototipus de rutines de càlcul de valors mitjos,
|	màxims i mínims d'una taula de temperatures, expressades en graus
|	Celsius, en format Coma Flotant E9M22.
| ---------------------------------------------------------------------
|	pere.millan@urv.cat
|	santiago.romani@urv.cat
|	(Març 2021-2024, Març 2025)
| ---------------------------------------------------------------------*/

#ifndef AVGMAXMINTEMP_H
#define AVGMAXMINTEMP_H

#include "E9M22.h"			// E9M22_t: tipus Coma Flotant E9M22


  	/* declaració del tipus maxmin_t, per retornar resultats */
typedef struct
{	
    E9M22_t tmin_C;			// temperatura mínima, en graus Celsius
    E9M22_t tmax_C;			// temperatura màxima, en graus Celsius
    E9M22_t tmin_F;			// temperatura mínima, en graus Fahrenheit
    E9M22_t tmax_F;			// temperatura màxima, en graus Fahrenheit
    unsigned short id_min;	// índex temperatura mínima
    unsigned short id_max; 	// índex temperatura màxima
} maxmin_t;


/* avgmaxmin_city(): calcula la temperatura mitjana, màxima i mínima 
|				d'una ciutat d'una taula de temperatures, 
|				amb una fila per ciutat i una columna per mes, 
|				expressades en graus Celsius en format E9M22.
|	Paràmetres:
|		ttemp[][12]	→	taula de temperatures, amb 12 columnes i nrows files
|		nrows		→	número de files de la taula
|		id_city		→	índex de la fila (ciutat) a processar
|		*mmres		→	adreça de l'estructura maxmin_t que retornarà els
|						resultats de temperatures màximes i mínimes
|	Resultat:	temperatura mitjana, expressada en graus Celsius, en format E9M22.
*/		
E9M22_t avgmaxmin_city(E9M22_t ttemp[][12], unsigned short nrows, unsigned short id_city, maxmin_t *mmres);


/* avgmaxmin_month(): calcula la temperatura mitjana, màxima i mínima 
|				d'un mes d'una taula de temperatures, 
|				amb una fila per ciutat i una columna per mes, 
|				expressades en graus Celsius en format E9M22.
|	Paràmetres:
|		ttemp[][12]	→	taula de temperatures, amb 12 columnes i nrows files
|		nrows		→	número de files de la taula
|		id_month	→	índex de la columna (mes) a processar
|		*mmres		→	adreça de l'estructura maxmin_t que retornarà els
|						resultats de temperatures màximes i mínimes
|	Resultat:	temperatura mitjana, expressada en graus Celsius, en format E9M22.
*/		
E9M22_t avgmaxmin_month(E9M22_t ttemp[][12], unsigned short nrows, unsigned short id_month, maxmin_t *mmres);


#endif 	/* AVGMAXMINTEMP_H */
