/*----------------------------------------------------------------
|   "avgmaxmintemp.c": rutines de càlcul de valors mitjans, màxims
|	i mínims d'una taula de temperatures, expressades en graus
|	Celsius o Fahrenheit, en format de coma flotant E9M22.
|	Implementació cridant rutines aritmètiques libE9M22.
| ----------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(Febrer-Març 2025)
| ----------------------------------------------------------------*/

#include "E9M22.h"				/* Tipus Coma Flotant E9M22 i operacions */
#include "CelsiusFahrenheit.h"	/* rutines de conversió C->F i F->C */
#include "avgmaxmintemp.h"		/* mmres: tipus de retorn de màxim i mínim */

#define E9M22_12	0x40A00000	// Valor 12.0 codificat en coma flotant E9M22

/* avgmaxmin_city(): calcula la temperatura mitjana, màxima i mínima 
|				d'una ciutat d'una taula de temperatures, 
|				amb una fila per ciutat i una columna per mes, 
|				expressades en graus Celsius en format E9M22.
|	Paràmetres:
|		ttemp[][12]	→	taula de temperatures, amb 12 columnes i nrows files
|		nrows		→	número de files de la taula
|		id_city		→	índex de la fila (ciutat) a processar
|		*mmres		→	adreça de l'estructura maxmin_t que retornarà 
|						els resultats de temperatures màximes i mínimes
|	Resultat:	temperatura mitjana, expressada en graus Celsius, en format E9M22.
*/		
E9M22_t avgmaxmin_city(E9M22_t ttemp[][12], unsigned short nrows, unsigned short id_city, maxmin_t *mmres)
{
    E9M22_t avg, max, min;
    unsigned short idmin = 0, idmax = 0;	// índexs de temp. mínima i màx.
    unsigned short i;
    E9M22_t tvar;						// variable temporal de temperatura
  
    avg = ttemp[id_city][0];		// inicialitza valors amb primera columna
    max = avg;						// (mes de gener)
    min = avg;
    for (i = 1; i < 12; i++)		// per a la resta de mesos
    {
        tvar = ttemp[id_city][i];			// obtenir temperatura del següent mes
        avg = E9M22_add(avg, tvar);

        if ( E9M22_is_gt (tvar, max) )
        {	
            max = tvar;						// actualitzar valors resultat
            idmax = i;
        }
        if ( E9M22_is_lt (tvar, min) )
        {
            min = tvar;
            idmin = i;
        }
    }

        //	avg /= 12;	ajustar valor mitjà
    avg = E9M22_div(avg, E9M22_12);	

    mmres->tmin_C = min;			// transferir mínim-màxim en Celsius
    mmres->tmax_C = max;
                                    // transferir mínim-màxim en Fahrenheit
    mmres->tmin_F = Celsius2Fahrenheit(min);
    mmres->tmax_F = Celsius2Fahrenheit(max);
                                    // transferir índexos mínim i màxim
    mmres->id_min = idmin;
    mmres->id_max = idmax;

    return(avg);
}



/* avgmaxmin_month(): calcula la temperatura mitjana, màxima i mínima d'un mes
|				d'una taula de temperatures, amb una fila per ciutat i una
|				columna per mes, expressades en graus Celsius en format E9M22.
|	Paràmetres:
|		ttemp[][12]	→	taula de temperatures, amb 12 columnes i nrows files
|		nrows		→	número de files de la taula (mínim 1 fila)
|		id_month	→	índex de la columna (mes) a processar
|		*mmres		→	adreça de l'estructura maxmin_t que retornarà 
|						els resultats de temperatures màximes i mínimes
|	Resultat:	temperatura mitjana, expressada en graus Celsius, en format E9M22.
*/		
E9M22_t avgmaxmin_month(E9M22_t ttemp[][12], unsigned short nrows, unsigned short id_month, maxmin_t *mmres)
{
    E9M22_t avg, max, min;
    unsigned short idmin = 0, idmax = 0;	// índexos de temp. mínima i màx.
    unsigned short i;
    E9M22_t tvar;					// variable temporal de temperatura
 
    avg = ttemp[0][id_month];		// inicialitza valors amb primera fila
    max = avg;						// (primera ciutat de la taula)
    min = avg;
    i = 1;							// posicionar índex a la segona fila
    while (i < nrows)				// per a la resta de ciutats
    {
        tvar = ttemp[i][id_month];		// obtenir temperatura següent ciutat
        //avg += tvar;
        avg = E9M22_add(avg, tvar);

        if ( E9M22_is_gt (tvar, max) )
        {	
            max = tvar;						// actualitzar valors resultat
            idmax = i;
        }
        if ( E9M22_is_lt (tvar, min) )
        {
            min = tvar;
            idmin = i;
        }

        i++;
    }
        // calcular valor mitjà sobre avg
    avg = E9M22_div(avg, int_to_E9M22(nrows) );	

    mmres->tmin_C = min;			// transferir mínim-màxim en Celsius
    mmres->tmax_C = max;
                                    // transferir mínim-màxim en Fahrenheit
    mmres->tmin_F = Celsius2Fahrenheit(min);
    mmres->tmax_F = Celsius2Fahrenheit(max);
                                    // transferir índexos mínim i màxim
    mmres->id_min = idmin;
    mmres->id_max = idmax;

    return(avg);
}
