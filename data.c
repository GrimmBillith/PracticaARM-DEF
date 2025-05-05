/*-----------------------------------------------------------------------
|   "geotempData.c":
|		Dades per a ser processades per les rutines definides en
|	"avgmaxmintemp.c", a partir de taules que contenen valors de
|	temperatura per a diferents ciutats del món durant els 12 mesos
|	de l'any.
|	Font: https://en.wikipedia.org/wiki/List_of_cities_by_average_temperature
|------------------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(Març 2021-2024, Març 2025)
| -----------------------------------------------------------------------*/

#include "E9M22.h"				/* Tipus Coma Flotant E9M22 i operacions */
#include "geotemp.h"			/* cityinfo_t: tipus amb info. de ciutat */
#include "data.h"				/* defines número de ciutats */			

E9M22_t tempHNord_2020[NUMCITIESHNORD][12]; // To be set by converting from float
float f_tempHNord_2020[NUMCITIESHNORD][12] = { // float temperatures to be converted to E9M22 values
    { 13.4,  13.9,  15.7,  18.5, 21.2, 24.3, 25.9, 26.3, 25.1, 22.0,  18.7,  14.9},	// Alexandria
    {  2.2,   3.5,   5.8,   7.5, 11.5, 15.4, 18.8, 18.5, 14.9, 10.3,   5.7,   3.0},	// Andorra la Vella
    { -3.1,   0.3,   6.7,  14.8, 20.8, 24.9, 26.7, 25.5, 20.8, 13.7,   5.0,  -0.9},	// Beijing
    { -7.1,  -5.4,  -1.6,   4.6,  9.7, 13.7, 16.5, 15.8, 11.0,  5.2,  -2.4,  -6.8},	// Calgary
    {-24.8, -25.4, -22.1, -17.2, -7.8,  0.4,  4.9,  5.5,  1.6, -7.5, -17.5, -22.9},	// Dikson
    { 25.1,  25.7,  27.0,  28.7, 31.0, 34.2, 36.4, 36.0, 33.1, 29.3,  26.9,  25.4},	// Djibouti
    { 78.4,  79.0,  79.9,  81.7, 83.3, 84.6, 85.3, 85.5, 84.7, 83.3,  81.5,  79.2},	// George Town
    { 28.8,  33.5,  44.2,  54.8, 64.5, 73.5, 78.3, 77.0, 68.1, 56.3,  43.6,  31.4},	// Kansas City
    {  4.6,   5.1,   8.4,  14.2, 19.0, 23.0, 26.8, 28.2, 24.1, 17.8,  12.1,   7.0},	// Kyoto
    { 17.4,  18.1,  19.7,  22.1, 24.5, 27.1, 29.7, 30.2, 29.3, 26.2,  22.0,  18.6},	// La Paz
    { 41.3,  45.5,  54.0,  63.0, 71.7, 79.6, 82.7, 82.1, 75.2, 64.1,  53.2,  43.6},	// Memphis
    { 13.8,  16.5,  22.1,  28.7, 32.8, 34.0, 30.9, 29.7, 29.0, 26.1,  20.5,  15.3},	// New Delhi
    { 10.6,  10.6,  13.1,  17.1, 22.3, 26.9, 29.7, 29.4, 26.2, 22.3,  16.3,  12.0},	// Nicosia
    { 16.0,  18.2,  23.1,  27.9, 32.2, 36.4, 39.8, 38.4, 35.5, 29.2,  22.0,  17.8},	// Reggane
    { 77.6,  77.9,  78.9,  80.3, 81.9, 83.3, 83.4, 83.7, 83.5, 82.6,  80.6,  76.7},	// San Juan
    { 60.6,  63.2,  67.1,  71.7, 78.2, 82.0, 82.8, 83.0, 81.5, 76.0,  69.0,  63.0},	// Tampa
    {-21.6, -16.6,  -7.4,   2.0, 10.1, 15.7, 18.2, 16.0,  9.6,  0.5, -11.9, -19.0},	// Ulaanbaatar
    {-38.6, -33.8, -20.1,  -4.8,  7.5, 16.4, 19.5, 15.2,  6.1, -7.8, -27.0, -37.6}	// Yakutsk
  };


cityinfo_t info_HNord[NUMCITIESHNORD] = {
    {"Alexandria", 'C'},				// Egypt
    {"Andorra la Vella", 'C'},			// Andorra
    {"Beijing", 'C'},					// China
    {"Calgary", 'C'},					// Canada
    {"Dikson", 'C'},					// Russia
    {"Djibouti", 'C'},					// Djibouti
    {"George Town", 'F'},				// Cayman Islands
    {"Kansas City", 'F'},				// USA
    {"Kyoto", 'C'},						// Japan
    {"La Paz", 'C'},					// Mexico
    {"Memphis", 'F'},					// USA
    {"New Delhi", 'C'},					// India
    {"Nicosia", 'C'},					// Cyprus
    {"Reggane", 'C'},					// Argelia
    {"San Juan", 'F'},					// Puerto Rico
    {"Tampa", 'F'},						// USA
    {"Ulaanbaatar", 'C'},				// Mongolia
    {"Yakutsk", 'C'}					// Russia
};	


E9M22_t tempHSud_2020[NUMCITIESHSUD][12];	// To be set by converting from float
float f_tempHSud_2020[NUMCITIESHSUD][12] = { // float temperatures to be converted to E9M22 values
    {23.2, 23.3, 20.9, 17.6, 13.5, 12.1, 11.4, 12.4, 14.4, 16.7, 19.7, 21.3},	// Adelaide
    {15.0, 14.8, 11.9,  7.9,  4.9,  2.9,  2.1,  3.0,  5.1,  8.0, 10.8, 13.5},	// Bariloche
    {20.6, 20.1, 17.6, 13.4,  9.4,  6.7,  5.7,  7.0,  9.8, 12.8, 15.8, 18.8},	// Canberra
    {27.7, 27.6, 27.4, 27.4, 27.0, 26.8, 25.5, 25.1, 25.4, 26.0, 27.2, 27.4},	// Dili
    {20.8, 20.8, 20.0, 18.9, 16.7, 14.5, 14.0, 14.4, 16.2, 18.3, 19.7, 20.4},	// Fianarantsoa
    {25.9, 25.3, 24.1, 21.2, 17.6, 14.3, 14.1, 17.1, 21.5, 24.6, 25.7, 26.1},	// Ghanzi
    {23.3, 23.7, 23.1, 21.9, 20.1, 18.9, 18.0, 17.9, 18.3, 19.0, 20.4, 21.8},	// Hanga Roa
    {21.0, 20.7, 20.3, 18.8, 16.1, 13.7, 13.4, 15.5, 18.6, 20.8, 21.2, 20.9},	// Harare
    {27.1, 26.9, 27.1, 27.3, 27.2, 26.9, 26.4, 26.6, 26.9, 27.2, 27.3, 27.0},	// Jayapura
    {19.5, 19.0, 18.0, 15.3, 12.6,  9.6, 10.0, 12.5, 15.9, 17.1, 17.9, 19.0},	// Johannesburg
    {15.2, 14.8, 12.4,  9.2,  5.9,  2.6,  1.4,  3.6,  6.5,  8.8, 11.1, 13.2},	// Lake Tekapo
    {23.0, 22.5, 20.6, 17.2, 14.0, 11.1, 10.9, 11.7, 13.4, 16.0, 18.6, 21.3},	// Montevideo
    {27.4, 27.3, 27.1, 27.0, 26.9, 26.1, 25.7, 26.1, 26.5, 27.5, 27.6, 27.8},	// Port Moresby
    {10.8, 10.3,  8.5,  6.2,  3.7,  1.5,  1.4,  2.3,  4.2,  6.4,  8.4,  9.9},	// Punta Arenas
    {26.3, 26.6, 26.0, 24.4, 22.8, 21.8, 21.3, 21.8, 22.2, 22.9, 24.0, 25.3},	// Rio de Janeiro
    { 9.5,  8.9,  8.1,  6.1,  3.9,  2.2,  1.9,  2.2,  3.9,  5.3,  7.0,  8.1},	// Stanley
    {16.2, 15.7, 15.8, 15.5, 14.9, 13.8, 13.9, 14.9, 16.2, 16.9, 17.3, 16.7},	// Sucre
    { 9.6,  9.3,  7.7,  5.7,  3.2,  1.7,  1.3,  2.2,  4.1,  6.2,  7.8,  9.1},	// Ushuaia
    {16.9, 17.2, 15.8, 13.7, 11.7,  9.7,  8.9,  9.4, 10.8, 12.0, 13.5, 15.4}	// Wellington
};


cityinfo_t info_HSud[NUMCITIESHSUD] = {
    {"Adelaide", 'C'},					// Australia
    {"Bariloche", 'C'},					// Argentina
    {"Canberra", 'C'},					// Australia
    {"Dili", 'C'},						// East Timor
    {"Fianarantsoa", 'C'},				// Madagascar
    {"Ghanzi", 'C'},					// Botswana
    {"Hanga Roa", 'C'},					// Chile
    {"Harare", 'C'},					// Zimbabwe
    {"Jayapura", 'C'},					// Indonesia
    {"Johannesburg", 'C'},				// South Africa
    {"Lake Tekapo", 'C'},				// New Zealand
    {"Montevideo", 'C'},				// Uruguay
    {"Port Moresby", 'C'},				// Papua New Guinea
    {"Punta Arenas", 'C'},				// Chile
    {"Rio de Janeiro", 'C'},			// Brazil
    {"Stanley", 'C'},					// Falkland Islands
    {"Sucre", 'C'},						// Bolivia
    {"Ushuaia", 'C'},					// Argentina
    {"Wellington", 'C'}					// New Zealand
};

