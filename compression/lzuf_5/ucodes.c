/* 
Filename:    ucodes.c (universal codes.)
Written by:  Gerald Tamayo, 2009
*/
#include <stdio.h>
#include <stdlib.h>
#include "gtbitio.h"
#include "ucodes.h"

/* Golomb Codes.

We divide integer n by (1<<mfold), write the result as a 
unary code, and then output the remainder as a binary number, 
the bitlength of which is exactly the length of the unary_code-1.

In the implementation below, mfold is an exponent of two:
mfold = {0, 1, 2, ...} and (1<<mfold) is thus a power of two. 
Each 1 bit of the unary code signifies a (1<<mfold) *part* of 
integer n. In *exponential* Golomb coding, each 1 bit signifies 
succeeding powers of 2. (We allow a length/mfold of 0 to encode 
n as a plain unary code.)
*/
void put_golomb( int n, int mfold )
{
	int i = n >> mfold;
	
	while ( i-- ) {
		put_ONE();
	}
	put_ZERO();
	if ( mfold ) 
		put_nbits( n%(1<<mfold), mfold );
}

int get_golomb( int mfold )
{
	int n = 0;
	
	while ( get_bit() ) n++;
	n <<= mfold;
	if ( mfold )
		n += get_nbits(mfold);
	
	return n;
}

/* The following variable-length encoding function can write 
Elias-Gamma codes and Exponential-Golomb codes according 
to the *len* parameter, which can be 0 to encode integer 0
as just 1 bit. */
void put_vlcode( int n, int len )
{
	while ( n >= (1<<len) ){
		put_ONE();
		n -= (1<<len++);
	}
	put_ZERO();
	if ( len ) put_nbits( n, len );
}

int get_vlcode( int len )
{
	int n = 0;
	
	while ( get_bit() ){
		n += (1<<len++);
	}
	if ( len ) n += get_nbits(len);
	return n;
}
