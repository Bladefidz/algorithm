/*
---- A DYNAMIC Huffman (Vitter) Coding Implementation ----

Filename:     VITD.C	(Decoder)
Written by:   Gerald Tamayo, 2005

This is the decompressor for
	VITC.C, an *Adaptive* Huffman (Vitter) Algorithm implementation.

To compile:   tcc -w vitd.c gtbitio.c huf.c
              bcc32 -w vitd.c gtbitio.c huf.c
              g++ -O2 vitd.c gtbitio.c huf.c -s -o vitd
*/
#include <stdio.h>
#include <stdlib.h>
#include "gtbitio.h"
#include "vit.c"

typedef struct {
	char algorithm[4];
	unsigned long file_size;
} file_stamp;

void copyright( void );

int main( int argc, char *argv[] )
{
	unsigned long in_file_len = 0, out_file_len = 0;
	FILE *out;
	file_stamp fstamp;

	if ( argc != 3 ) {
		fprintf(stderr, "\n Usage: vitd infile outfile");
		copyright();
		return 0;
	}

	if ( (gIN = fopen( argv[1], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		copyright();
		return 0;
	}

	if ( (out = fopen( argv[2], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		copyright();
		return 0;
	}

	fprintf(stderr, "\n---[ A DYNAMIC Huffman (Vitter) Implementation ]---\n");
	fprintf(stderr, "\nName of input file : %s", argv[1] );
	fprintf(stderr, "\nName of output file: %s", argv[2] );

	/* get file length. */
	fseek( gIN, 0, SEEK_END );
	in_file_len = ftell( gIN );

	/* ===== The Main Huffman Implementation ======= */

	fprintf(stderr, "\n\nHuffman decompressing...");

	/* This is a DYNAMIC algorithm, so no need to read stats. */

	/* start the decoding process. */
	rewind( gIN );

	/* read first the file stamp/header. */
	fread( &fstamp, sizeof(file_stamp), 1, gIN );

	/* make sure all symbol node
			addresses are NULL. */
	init_hufflist();

	/* get FIRST symbol. */
	hc = fgetc( gIN );
	out_file_len = fstamp.file_size - 1;

	/*
	create first 0-node which quickly becomes
	the root of the Huffman tree.
	*/
	top = zero_node = create_node();

	/* output first symbol as a raw byte. */
	fputc( (unsigned char) hc, out );

	/* Update the Huffman tree. */
	update_treeVITTER( hc );	/* pass the symbol */

	/* now get the bit stream. */
	init_get_buffer();

	while ( out_file_len-- ) {
		/* get symbol. */
		hc = hdecompress( top );

		if ( hc == ZERO_NODE_SYMBOL ) {
			/* get raw byte. */
			hc = get_nbits( 8 );
		}

		/* output the decoded byte. */
		putc( (unsigned char) hc, out );

		/* Update the Hufman tree.*/
		update_treeVITTER( hc );	/* pass the symbol. */
	}
	fprintf(stderr, "done.");

	/* get outfile's size. */
	out_file_len = ftell( out );

	fprintf(stderr, "\n\nLength of input file     = %15lu bytes", in_file_len );
	fprintf(stderr, "\nLength of output file    = %15lu bytes\n", out_file_len );
	
	halt_prog:

	free_get_buffer();
	if ( gIN ) fclose( gIN );
	if ( out ) fclose( out );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2005\n");
}
