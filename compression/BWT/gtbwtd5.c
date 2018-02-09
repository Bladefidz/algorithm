/*
	Filename:       gtbwtd5.c		(the decoder to gtbwt5.c)
	Description:    A Burrows-Wheeler Transform (BWT) Implementation
	Author:         Gerald Tamayo
	Date:           December 22, 2004

	This code does not use a *stable* sorting method at all, but
	simply constructs the index table (while maintaining the
	stable sort idea) by filling in the index table with the
	correct indices.
*/

#include <stdio.h>
#include <stdlib.h>

#define BWT_SIZE (1<<15)

unsigned char *bwt_buf;                 /* holds the Last Column to process. */
unsigned int index_table[ BWT_SIZE ];
unsigned int freq[ 256 ];               /* the frequency table. */
unsigned int nread = 0;

/* function declarations. */
void copyright ( void );
void bwt_decode ( FILE *in, FILE *out );

int main ( int argc, char *argv[] )
{
	FILE *in, *out;

	if ( argc != 3 ){
		fprintf(stderr, "\n ----[ A Burrows-Wheeler Transform (BWT) Implementation ]----\n");
		fprintf(stderr, "\n Usage : gtbwtd5 inputfile outputfile\n");
		copyright();
		return 0;
	}

	in = fopen( argv[1], "rb" );
	if ( !in ){
		fprintf(stderr, "\n Error opening input file!\n");
		return 0;
	}
	out = fopen( argv[2], "wb" );
	if ( !out ){
		fprintf(stderr, "\n Error opening output file!\n");
		return 0;
	}

	/* dynamically allocate memory for the Last "column" buffer. */
	bwt_buf = (unsigned char *) malloc( BWT_SIZE * sizeof(unsigned char));
	if ( bwt_buf == NULL ) {
		fprintf(stderr, "\nAlloc error!");
		return (0);
	}

	fprintf(stderr, "\n --[ A Burrows-Wheeler Transform (BWT) Implementation ]--\n");
	fprintf(stderr, "\n Input  filename: %s", argv[1] );
	fprintf(stderr, "\n Output filename: %s\n", argv[2] );

	fprintf(stderr, "\n BWT Decoding...");
	bwt_decode( in, out );
	fprintf(stderr, "done.\n" );

	copyright();

	if ( bwt_buf ) free( bwt_buf );

	if ( in  ) fclose( in );
	if ( out ) fclose( out );

	return 0;
}

/*
The decoding function gets a "column" of bytes; records the frequency
of the individual bytes; and transforms the frequency table into
a *cumulative frequency* table.

The index table is then filled in with the correct values (i.e. the
indices of the bytes in the Last column), and outputs the corresponding
bytes in the First column.
*/
void bwt_decode ( FILE *in, FILE *out )
{
	unsigned int i, findex, last_index;
	unsigned int sum, temp;
	
	while ( 1 ) {
		/* get the size of the block. */
		fread ( &nread, sizeof(int), 1, in );
                if ( nread == 0 ) break;

		/* get character input (the last column). */
		nread = fread( bwt_buf, 1, nread, in );

		/* get the last index "pointer." */
		fread ( &last_index, sizeof(int), 1, in );

		/* initialize frequency array. */
		for ( i = 0; i < 256; i++ ) {
			freq[i] = 0;
		}
		
		/* ---- count frequency of characters. ---- */
		for ( i = 0; i < nread; i++ ) {
			freq[ bwt_buf[i] ]++;
		}

		/* ---- cumulative sum. ---- */
		sum = 0;
		/*
			after the index_table is filled in, the freq[] array 
			will contain the cumulative sum for all bytes.
		*/
		for ( i = 0; i < 256; i++ ) {
			if ( freq[i] ) {    /* only if existent in the last column! */
				temp = freq[i];  /* save. */
				freq[i] = sum;   /* the cumulative sum. */
				sum += temp;     /* add the current byte's frequency. */
			}
		}
		
		/* ---- finally, fill in the index_table. ---- */
		for ( i = 0; i < nread; i++ ) {
			index_table[ freq[bwt_buf[i]]++ ] = i;
		}
		
		/*
		NOTE: last_index is the row index of the original string
					in the *sorted* matrix (see gtbwt5.c).
		*/
		findex = index_table[last_index];
		fprintf(stderr, "\n--[ last index = %6d ]-- ", last_index );
		
		/* output the bytes; reverse BWT. */
		while ( nread-- ){
			putc( bwt_buf[ findex ], out );
			findex = index_table[ findex ];
		}
	}
}

void copyright ( void )
{
	fprintf(stderr, "\n Written by: Gerald Tamayo, 2004\n");
}
