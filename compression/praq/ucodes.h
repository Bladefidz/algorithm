/*
Filename:   ucodes.c (universal codes.)
Written by: Gerald Tamayo, 2009
*/
#include <stdio.h>
#include <stdlib.h>

#if !defined(_UCODES_)
	#define _UCODES_

/* Unary Codes. */
#define put_unary(n) put_golomb((n),0)
#define get_unary()  get_golomb(0)

/* Exponential Golomb coding */
#define put_xgolomb(n) put_vlcode((n), 0)
#define get_xgolomb()  get_vlcode(0)

/* Elias-Gamma coding.
Note: don't pass a zero (0) to the encoding function: only n > 0 */
#define put_elias_gamma(n) put_xgolomb((n)-1)
#define get_elias_gamma()  get_xgolomb()

/* Golomb Codes. */
void put_golomb( int n, int mfold );
int  get_golomb( int mfold );

/* The following variable-length encoding function can write 
Elias-Gamma codes and Exponential-Golomb codes according 
to the *len* parameter, which can be 0 to encode integer 0 as 
just 1 bit. */
void put_vlcode( int n, int len );
int  get_vlcode( int len );

#endif
