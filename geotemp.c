/*-----------------------------------------------------------------------
|   "geotemp.c":
|		programa per calcular els valors mig, màxim i mínim d'una taula
|	de temperatures, expressades en graus °C o °F i en format E9M22, 
|	on cada fila correspon a una ciutat i cada columna a un mes.
|	Les taules amb la informació estan definides en un altre mòdul
|	(variables globals externes).
|------------------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(Abril 2021, Març 2022-2024, Març 2025)
| -----------------------------------------------------------------------*/

#include "E9M22.h"				/* Tipus Coma Flotant E9M22 i operacions */
#include "divmod.h"				/* rutina div_mod(), en "LibFonCompus.a" */
#include "CelsiusFahrenheit.h"	/* rutines de conversió °C→°F i °F→°C */
#include "avgmaxmintemp.h"		/* mmres: tipus de retorn de màxim i mínim */
#include "geotemp.h"			/* cityinfo_t: tipus amb info. de ciutat */
#include "data.h"				/* definició de dades externes */


/* normalitzar_temperatures(): funció per a convertir les temperatures
            expressades en graus Fahrenheit a graus Celsius, 
            segons el vector d'informació de les ciutats registrades.
    Paràmetres:
        vinfo[]		→ vector amb una entrada per ciutat, indicant el nom
                      i l'escala utilitzada per expressar les temperatures,
        ttemp[][12]	→ taula amb una fila per ciutat i una columna per mes;
                      se suposa que les temperatures de cada fila 
                      estaran expressades en l'escala indicada en vinfo[];
        num_cities	→ número de ciutats, tant al vector tinfo[] 
                      com a la taula ttemp[];
    Resultats:
        ttemp[][12]	→ les temperatures convertides a graus Celsius es
                      es guardaran sobre la mateixa taula d'entrada 
                      (pas per referencia).
*/
void normalitzar_temperatures(cityinfo_t vinfo[], E9M22_t ttemp[][12], unsigned short num_cities)
{
    unsigned short i, j;
    
    for (i = 0; i < num_cities; i++)	// recorregut de totes les ciutats
        if (vinfo[i].scale == 'F')		// si l'escala utilitzada és Fahrenheit
            for (j = 0; j < 12; j++)	// recorregut de tots els mesos
                ttemp[i][j] = Fahrenheit2Celsius(ttemp[i][j]);
}


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


int main(void)
{
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"	// Evitar warning avgres
    E9M22_t	avgres[4];
    maxmin_t maxminres[4];

		// dades float → E9M22
    ini_temperatures(f_tempHNord_2020, tempHNord_2020, NUMCITIESHNORD);
    ini_temperatures(f_tempHSud_2020,  tempHSud_2020,  NUMCITIESHSUD);

		// conversió °F → °C
    normalitzar_temperatures(info_HNord, tempHNord_2020, NUMCITIESHNORD);
    normalitzar_temperatures(info_HSud,  tempHSud_2020,  NUMCITIESHSUD);
    
            // càlculs sobre George Town (Cayman Island)
    avgres[0] = avgmaxmin_city(tempHNord_2020, NUMCITIESHNORD, 6, &maxminres[0]);
            // càlculs sobre el mes d'agost (hemisferi nord)
    avgres[1] = avgmaxmin_month(tempHNord_2020, NUMCITIESHNORD, 7, &maxminres[1]);
            // càlculs sobre Wellington (New Zealand)
    avgres[2] = avgmaxmin_city(tempHSud_2020, NUMCITIESHSUD, 18, &maxminres[2]);
            // càlculs sobre el mes de desembre (hemisferi sud)
    avgres[3] = avgmaxmin_month(tempHSud_2020, NUMCITIESHSUD, 11, &maxminres[3]);


/* TESTING POINT: check the results
    (gdb) p /x avgres		-> {0x40ef8e39, 0x40e4f543, 0x40a75554, 0x40cc6bca}
    (gdb) p /x maxminres[0]	-> {0x40e71c71, 0x40f6e38f, 0x414e6666, 0x41558000, 0x0, 0x7}
    (gdb) p /x maxminres[1]	-> {0x40580000, 0x410ccccd, 0x4113cccd, 0x41651eb8, 0x4, 0xd}
    (gdb) p info_HNord[4].name	-> "Dikson"
    (gdb) p info_HNord[13].name	-> "Reggane"
    (gdb) p /x maxminres[2]	-> {0x40873333, 0x40c4cccd, 0x41200a3e, 0x413deb85, 0x6, 0x1}
    (gdb) p /x maxminres[3]	-> {0x4080cccd, 0x40ef3333, 0x411d28f6, 0x41520a3e, 0xf, 0xc}
    (gdb) p info_HSud[15].name	-> "Stanley"
    (gdb) p info_HSud[12].name	-> "Port Moresby"	
*/

/* BREAKPOINT */
    return(0);
}
