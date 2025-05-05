/*---------------------------------------------------------------------
|   Declaració de variables globals definides en "data.c", 
|	amb informació per al programa "geotemp.c" .
| ---------------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(Març 2021-2024, Març 2025)
| ---------------------------------------------------------------------*/

#ifndef DATAGEOTEMP_H
#define DATAGEOTEMP_H

#include "E9M22.h"			// definició tipus E9M22_t
#include "geotemp.h"		// definitició tipus cityinfo_t

        /* declaracions estructures de dades externes amb les dades */
#define NUMCITIESHNORD	18
#define NUMCITIESHSUD	19

extern E9M22_t tempHNord_2020[NUMCITIESHNORD][12];
extern E9M22_t tempHSud_2020[NUMCITIESHSUD][12];

extern float f_tempHNord_2020[NUMCITIESHNORD][12];
extern float f_tempHSud_2020[NUMCITIESHSUD][12];

extern cityinfo_t info_HNord[NUMCITIESHNORD];
extern cityinfo_t info_HSud[NUMCITIESHSUD];

#endif 	/* DATAGEOTEMP_H */
