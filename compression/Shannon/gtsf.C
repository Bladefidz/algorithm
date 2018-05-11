/*
	---- A Static Shannon-Fano Encoding Implementation ----

	Filename:     GTSF.C (the encoding program.)
	Written by:   Gerald Tamayo, 2005
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utypes.h"
#include "gtbitio.c"
#include "sf.c"

typedef struct {
	char algorithm[4];
	ulong file_size;
} file_stamp;

/* create an input buffer for faster I/O */
#define IN_BUFSIZE 4096
char input_buf[ IN_BUFSIZE ];
uint nread = 0, in_i = 0;

void copyright( void );
void read_stats ( FILE *in, sffreq_type *sffreq );

int main( int argc, char *argv[] )
{
	ulong in_file_len, out_file_len;
	int c = 0;
	FILE *in;
	file_stamp fstamp;

	if ( argc != 3 ) {
		fprintf(stderr, "\n--( A Base-256 *Shannon-Fano* Implementation )--\n");
		fprintf(stderr, "\n Usage: gtsf in_file out_file");
		copyright();
		return 0;
	}

	if ( (in = fopen( argv[1], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		copyright();
		return 0;
	}
	if ( (pOUT = fopen( argv[2], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		copyright();
		return 0;
	}
	init_put_buffer();

	fprintf(stderr, "\n--( A Static *Shannon-Fano* Coding Implementation )--\n");
	fprintf(stderr, "\nName of input file : %s", argv[1] );

	/* display file length. */
	fseek( in, 0, SEEK_END );
	in_file_len = ftell( in );
	fprintf(stderr, "\nLength of input file     = %15lu bytes", in_file_len );

	/* ===== The Main Shannon-Fano Implementation ======= */

	/* initialize sffreq[] array for reading stats. */
	init_sffreq();

	/* get frequency counts of symbols. */
	fprintf(stderr, "\n\nAnalyzing file...");
	read_stats( in, sffreq );
	fprintf(stderr, "complete.");

	/* start compressing to output file. */
	fprintf(stderr, "\nShannon-Fano Compressing...");

	/* initialize the list of symbols. */
	init_sflist();
	create_symbol_list();

	/* prepare the root of the tree. */
	top = create_node();   /* allocate memory for top. */
	top->next = list;      /* point to the symbol list. */

	/* create the whole Shannon-Fano tree */
	create_shannon_fano_tree( top );

	/* encode FILE STAMP. */
	rewind( pOUT );
	strcpy( fstamp.algorithm, "TSF" );
	fstamp.file_size = in_file_len;
	fwrite( &fstamp, sizeof(file_stamp), 1, pOUT );

	/* then encode the count to the OUTPUT file */
	fwrite( &sfcount, sizeof(unsigned int), 1, pOUT );

	/* now, encode frequency table to the output file. */
	for ( c = 0; c < SF_MAX; c++ ) {
		if ( sffreq[c].f > 0 ) {
			fwrite( &sffreq[c], sizeof(sffreq_type), 1, pOUT );
		}
	}

	/* encode the symbols/file bytes. */
	rewind(in);
	while( 1 ) {
		/* load the input buffer. */
		nread = fread( input_buf, 1, IN_BUFSIZE, in );
		if ( nread == 0 ) break;
		in_i = 0;

		/* get bytes from the buffer and compress them. */
		while( in_i < nread ){
			c = (uchar) *(input_buf+in_i);
			++in_i;

			/* encode the byte c; just pass its node address. */
			sfcompress( sflist[c] );
		}
	}
	flush_put_buffer(); /* flush output buffer */
	fprintf(stderr, "complete.");

	/* get outfile's size and get compression ratio. */
	out_file_len = ftell( pOUT );

	fprintf(stderr, "\n\nName of output file: %s",  argv[2] );
	fprintf(stderr, "\nLength of input file     = %15lu bytes", in_file_len );
	fprintf(stderr, "\nLength of output file    = %15lu bytes", out_file_len );
	fprintf(stderr, "\nCompression ratio:         %15.2f %%\n",
		( ((float) in_file_len - (float) out_file_len) / (float) in_file_len
		) * (float) 100
	 );

	halt_prog:

	free_put_buffer();
	if ( in ) fclose( in );
	if ( pOUT ) fclose( pOUT );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2005\n");
}

void read_stats( FILE *in, sffreq_type *sffreq )
{
	int c;

	rewind(in);
	while ( (c=getc(in)) != EOF ) {
		sffreq[ c ].f++;
	}
}
