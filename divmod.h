/*----------------------------------------------------------------
|   Prototype of a routine for computing integer division
| ----------------------------------------------------------------*/

#ifndef DIVMOD_H
#define DIVMOD_H

/* div_mod():	computes the natural division (num / den) and returns
                quotient and remainder by reference; it also returns
                an error code, 0 → everything OK, 1 → division by 0
*/
extern int div_mod(unsigned int num, unsigned int den,
                   unsigned int * quo, unsigned int * mod);

#endif /* DIVMOD_H */
