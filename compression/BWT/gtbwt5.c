/*
	Filename:       gtbwt5.c		(the encoder.)
	Description:    A Burrows-Wheeler Transform (BWT) Implementation
	Author:         Gerald Tamayo
	Date:           December 22, 2004

	The logic of the BWT reversal process might prompt you to sort the
	last column buffer using a *stable* sorting algorithm to keep the order
	of the indices. This means that what we really need to accomplish is to
	keep the indices in *correct* order.

	However, a stable sorting routine is not needed; the decoder just
	fills in the index table as it *should* appear when actually sorted.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BWT_SIZE (1<<15)

unsigned char *bwt_buf;
unsigned int index_table[ BWT_SIZE ];
unsigned int nread = 0, count = 0;

void bwt_encode ( FILE *in, FILE *out );
int bwt_comp( unsigned int *a, unsigned int *b );

void copyright ( void )
{
	fprintf(stderr, "\n Written by: Gerald Tamayo, 2004\n");
}

int main ( int argc, char *argv[] )
{
	FILE *in, *out;

	if ( argc != 3 ) {
		fprintf(stderr, "\n ----[ A Burrows-Wheeler Transform (BWT) Implementation ]----\n");
		fprintf(stderr, "\n Usage : gtbwt inputfile outputfile\n");
		copyright();
		return 0;
	}

	in = fopen( argv[1], "rb" );
	if ( !in ){
		fprintf(stderr, "\n Error opening input file!\n");
		return 0;
	}
	out = fopen( argv[2], "wb" );
	if ( !out ) {
		fprintf(stderr, "\n Error opening output file!\n");
		return 0;
	}

	bwt_buf = (unsigned char *) malloc( BWT_SIZE * sizeof(unsigned char) );
	if ( !bwt_buf ){
		fprintf(stderr, "\nAlloc error!");
		exit (0);
	}

	fprintf(stderr, "\n --[ A Burrows-Wheeler Transform (BWT) Implementation ]--\n");
	fprintf(stderr, "\n Input  filename: %s", argv[1] );
	fprintf(stderr, "\n Output filename: %s\n", argv[2] );

	fprintf(stderr, "\n BWT Encoding...");
	bwt_encode( in, out );
	fprintf(stderr, "done.\n" );
	
	free( bwt_buf );
	if ( in  ) fclose( in );
	if ( out ) fclose( out );

	return 0;
}

void bwt_encode ( FILE *in, FILE *out )
{
	unsigned int i = 0, last_index = BWT_SIZE;

	while( 1 ) {
		/* get bytes and save to buffer. */
		nread = fread( (unsigned char *) bwt_buf, 1, BWT_SIZE, in );

		/* write the size of the block read. */
		fwrite( &nread, sizeof( unsigned int ), 1, out );
                if ( nread == 0 ) break;

		/* initialize index table. */
		for ( i = 0; i < nread; i++ ) {
			index_table[i] = i;
		}

		/* sort the bwt buffer array; */
		qsort( index_table, nread, sizeof(int), 
			(int (*) (const void *, const void *)) bwt_comp );

		/* output Last column buffer. */
		last_index = nread;
		for ( i = 0; i < nread; i++ ){
			if ( index_table[i] == 0 ){
				putc( bwt_buf[ nread-1 ], out );
				last_index = i;
			}
			else putc( bwt_buf[ index_table[i] - 1 ], out );
		}
		fprintf(stderr, "\n-[ last index: %5d ]- ", last_index );

		/* output index of the original string in Last. */
		fwrite( &last_index, sizeof( unsigned int ), 1, out );
	}
}

int bwt_comp( unsigned int *a, unsigned int *b )
{
	register int a1 = *a, b1 = *b, tmp = a1;
	
	if ( bwt_buf[a1] != bwt_buf[b1] ) {
		if ( bwt_buf[a1] > bwt_buf[b1] ) return 1;
		else return -1;
	}
	else {
		do {
			if ( bwt_buf[++a1 == nread? a1 = 0 : a1] 
					!= bwt_buf[++b1 == nread? b1 = 0 : b1] ) {
				if ( bwt_buf[a1] > bwt_buf[b1] ) return 1;
				else return -1;
			}
		} while ( a1 != tmp );
	}
	return 0;
}
