/*
	---- A Static Huffman Encoding Implementation ----
	
	Filename:    gthc.c  (decoder: gthd.c)
	Written by:  Gerald Tamayo
	Date:        June 22-24, 2004
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utypes.h"
#include "gtbitio.h"
#include "huf.h"

typedef struct {
	char algorithm[4];
	ulong file_size;
} file_stamp;

/* create an input buffer for faster I/O */
#define IN_BUFSIZE 4096
char input_buf[ IN_BUFSIZE ];
uint nread = 0, in_i = 0;

void copyright( void );
void read_stats ( FILE *in, hfreq_type *hfreq );

int main( int argc, char *argv[] )
{
	ulong in_file_len, out_file_len;
	int c = 0;
	FILE *in;
	file_stamp fstamp;

	if ( argc != 3 ) {
		fprintf(stderr, "\n --( A Base-256 Huffman Implementation )--\n");
		fprintf(stderr, "\n Usage: gthc in_file out_file");
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

	fprintf(stderr, "\n---- A Static Huffman Implementation ----\n");
	fprintf(stderr, "\nName of input file : %s", argv[1] );

	/* display file length. */
	fseek( in, 0, SEEK_END );
	in_file_len = ftell( in );
	fprintf(stderr, "\nLength of input file     = %15lu bytes", in_file_len );

	/* ===== The Main Huffman Implementation ======= */

	/* initialize hfreq[] array for reading stats. */
	init_hfreq();

	/* get frequency counts of symbols. */
	fprintf(stderr, "\n\nAnalyzing file...");
	read_stats( in, hfreq );
	fprintf(stderr, "complete.");

	/* start compressing to output file. */
	fprintf(stderr, "\nHuffman compressing...");

	/* create the whole huffman tree */
	create_hufftree();

	/* encode FILE STAMP. */
	rewind( pOUT );
	strcpy( fstamp.algorithm, "HUF" );
	fstamp.file_size = in_file_len;
	fwrite( &fstamp, sizeof(file_stamp), 1, pOUT );

	/* then encode the count to the OUTPUT file */
	fwrite( &hcount, sizeof(unsigned int), 1, pOUT );

	/* now encode frequency table to the output file. */
	for ( c = 0; c < H_MAX; c++ ) {
		if ( hfreq[c].f > 0 ) {
			fwrite( &hfreq[c], sizeof(hfreq_type), 1, pOUT );
		}
	}

	/* huffman encode the symbols/file bytes. */
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
			hcompress( hufflist[c] );
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
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2004\n");
}

void read_stats( FILE *in, hfreq_type *hfreq )
{
	int c;

	rewind(in);
	while ( (c=getc(in)) != EOF ) {
		hfreq[ c ].f++;
	}
}
