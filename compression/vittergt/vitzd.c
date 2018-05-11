/*
---- A DYNAMIC Huffman (Vitter) Coding Implementation ----

Filename:     VITZD.C	(Decoder)
Written by:   Gerald Tamayo, 2008

This is the decompressor for VITZ.C,
	an *Adaptive* Huffman (Vitter) Algorithm implementation.
*/
#include <stdio.h>
#include <stdlib.h>
#include "gtbitio.c"
#include "hufmodel.c"
#include "adhvith.c"

#define IMPROVED_BYTE_OUTPUT

typedef struct {
	/* char LEGACY[4]; */
	unsigned long file_size;
} file_stamp;

void copyright( void );

int main( int argc, char *argv[] )
{
	unsigned long in_file_len = 0, out_file_len = 0;
	FILE *out;
	huff_model h, h2;
	int hc, i;
	file_stamp fstamp;

	if ( argc != 3 ) {
		fprintf(stderr, "\n Usage: vitzd infile outfile");
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

	/* get FIRST symbol. */
	hc = fgetc( gIN );
	out_file_len = fstamp.file_size - 1;

	/* output first symbol as a raw byte. */
	fputc( (unsigned char) hc, out );

	/* ---- initialize the dynamic Huffman tree model ---- */
	init_huff_model( DYNAMIC_HUFFMAN, &h, 256, 8 );
	adh_init_first_node ( &h, hc );

#ifdef IMPROVED_BYTE_OUTPUT
	/* create huffman tree for the set of 256 "unseen" symbols. */
	init_huff_model( DYNAMIC_HUFFMAN, &h2, 256, 8 );
	adh_init_first_node ( &h2, 0 );  /* 0 is first symbol. */
	/* insert the rest of the bytes into the huffman tree. */
	for ( i = 1; i < 256; i++ ){
		update_treeVITTER( &h2, i );
	}
	/* Huffman tree is now "complete"; remove 0-node and 1st symbol. */
	remove_symbol( h2.zero_node );
	remove_symbol( h2.hufflist[hc] );
#endif

	/* now get the bit stream. */
	init_get_buffer();

	while ( out_file_len-- ) {
		/* get symbol. */
		hc = hdecompress( h.top );

		if ( hc == ZERO_NODE_SYMBOL ) {
			/* get symbol. */
		#ifndef IMPROVED_BYTE_OUTPUT
			hc = get_nbits(8);
		#else
			hc = hdecompress( h2.top );
			remove_symbol( h2.hufflist[hc] );
		#endif
		}

		/* output the decoded byte. */
		putc( (unsigned char) hc, out );

		/* Update the Hufman tree. */
		update_treeVITTER( &h, hc );	/* pass the symbol. */
	}
	fprintf(stderr, "done.");

	/* get outfile's size. */
	out_file_len = ftell( out );

	fprintf(stderr, "\n\nLength of input file     = %15lu bytes", in_file_len );
	fprintf(stderr, "\nLength of output file    = %15lu bytes\n", out_file_len );

	halt_prog:

	free_get_buffer();
	free_huff_model( DYNAMIC_HUFFMAN, &h );
	free_huff_model( DYNAMIC_HUFFMAN, &h2 );

	if ( gIN ) fclose( gIN );
	if ( out ) fclose( out );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2008\n");
}
