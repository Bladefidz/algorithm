/*
	---- A DYNAMIC Huffman Coding Implementation ----

	Filename:     FGKC.C	(Encoder)
	Decoder:      FGKD.C
	Written by:   Gerald Tamayo
	Date:         2005

	To compile:   tcc -w fgkc.c gtbitio.c huf.c
	              bcc32 -w fgkc.c gtbitio.c huf.c
	              g++ -O2 fgkc.c gtbitio.c huf.c -s -o fgkc
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gtbitio.h"
#include "fgk.c"

typedef struct {
	char algorithm[4];
	unsigned long file_size;
} file_stamp;

/* create an input buffer for faster I/O */
#define IN_BUFSIZE 16384
unsigned char input_buf[ IN_BUFSIZE ];
unsigned int nread = 0, in_i = 0;

void copyright( void );

int main( int argc, char *argv[] )
{
	unsigned long in_file_len, out_file_len;
	FILE *in;
	file_stamp fstamp;

	if ( argc != 3 ) {
		fprintf(stderr, "\n-[ A Base-256 Adaptive Huffman (FGK) Implementation ]-\n");
		fprintf(stderr, "\n Usage: fgkc infile outfile");

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

	fprintf(stderr, "\n---- A DYNAMIC Huffman (FGK) Implementation ----\n");

	fprintf(stderr, "\nName of input file : %s", argv[1] );
	fprintf(stderr, "\nName of output file: %s", argv[2] );

	/* get file length. */
	fseek( in, 0, SEEK_END );
	in_file_len = ftell( in );

	/* ===== The Main Huffman Implementation ======= */

	fprintf(stderr, "\n\nHuffman compressing...");

	/*	This is a DYNAMIC algorithm,
			so no need to read and store stats. */

	/* encode FILE STAMP. */
	rewind( pOUT );
	strcpy( fstamp.algorithm, "FGK" );
	fstamp.file_size = in_file_len;
	fwrite( &fstamp, sizeof(file_stamp), 1, pOUT );

	/* start huffman encoding the symbols/file bytes. */
	rewind( in );

	/* make sure all symbol node
			addresses are NULL. */
	init_hufflist();

	/* get first byte. */
	hc = fgetc( in );
	if ( hc == EOF ) goto halt_prog;

	/*
	create first 0-node which quickly becomes
	the root of the tree.
	*/
	top = zero_node = create_node();

	/* output first byte. */
	fputc( (unsigned char) hc, pOUT );

	/* recompute the tree if necessary. */
	update_treeFGK( hc );  /* pass the symbol. */

	while( 1 ) {
		/* load the input buffer. */
		nread = fread( input_buf, 1, IN_BUFSIZE, in );
		if ( nread == 0 ) break;
		in_i = 0;

		/* get bytes from the buffer and compress them. */
		while( in_i < nread ){
			hc = (unsigned char) *(input_buf+in_i);
			++in_i;

			/* encode the byte hc. */
			if ( hufflist[ hc ] ){     /* VALID address; previously seen. */
				/* output the symbol's code. */
				hcompress( hufflist[ hc ] );
			}
			else {	/* NULL address, a new byte! */
				/* output the zero node's code. */
				hcompress( zero_node );

				/* send the actual byte. */
				put_nbits( hc, 8 );
			}

			/* recompute the tree if necessary. */
			update_treeFGK( hc );	/* pass the symbol. */
		}
	}
	flush_put_buffer();
	fprintf(stderr, "done.");

	/* get outfile's size and get compression ratio. */
	out_file_len = ftell( pOUT );

	fprintf(stderr, "\n\nLength of input file     = %15lu bytes", in_file_len );
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
