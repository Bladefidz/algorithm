/*
	---- A DYNAMIC Huffman Coding Implementation (Algorithm V) ----

	Filename:   VITZ.C	(Encoder)
	Decoder:    VITZD.C
	Written by: Gerald Tamayo
	Date:       2008
	
	This is a quick rewrite of my previous Algorithm Vitter implementation
	to accommodate David A. Scott's suggestion that an "unseen" symbol must
	not be emitted as a raw byte (8 bits). For example, after 128 symbols
	are "seen" by the encoder, there are also only 128 unseen symbols left
	and thus they need only 7 bits for output.

	But if the unseen symbols are inserted into a binary tree, some symbols
	can be emitted even smaller than 7 bits, as this implementation
	demonstrates.

	Note that with this improvement, the code no longer adheres completely
	to Vitter's paper.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtbitio.c"
#include "hufmodel.c"
#include "adhvith.c"

#define IMPROVED_BYTE_OUTPUT

typedef struct {
	/* char algorithm[4]; */
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
	huff_model huff, h2;
	int c = EOF, i;
	file_stamp fstamp;

	if ( argc != 3 ) {
		fprintf(stderr, "\n-[ A Base-256 Adaptive Huffman (Vitter) Implementation ]-\n");
		fprintf(stderr, "\n Usage: vitz infile outfile");
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

	fprintf(stderr, "\n--[ A DYNAMIC Huffman (Vitter) Implementation ]--\n");

	fprintf(stderr, "\nName of input file : %s", argv[1] );
	fprintf(stderr, "\nName of output file: %s", argv[2] );

	/* get length of input file. */
	fseek( in, 0, SEEK_END );
	in_file_len = ftell( in );
	rewind( in );

	/* ===== The Main Huffman Implementation ======= */

	fprintf(stderr, "\n\nHuffman compressing...");

	/*	This is a DYNAMIC algorithm, so no need to read and store stats. */

	/* encode FILE STAMP. */
	rewind( pOUT );
	/* strcpy( fstamp.algorithm, "VTR" ); */
	fstamp.file_size = in_file_len;
	fwrite( &fstamp, sizeof( file_stamp ), 1, pOUT );

	/* ---- start huffman encoding the symbols/bytes. ---- */

	/* get first byte. */
	c = fgetc( in );
	if ( c == EOF ) goto halt_prog;

	/* output first byte. */
	fputc( (unsigned char) c, pOUT );

	/* ---- initialize the dynamic Huffman tree model ---- */
	init_huff_model( DYNAMIC_HUFFMAN, &huff, 256, 8 );
	adh_init_first_node ( &huff, c );

#ifdef IMPROVED_BYTE_OUTPUT
	/* create huffman tree for the set of 256 "unseen" symbols. */
	init_huff_model( DYNAMIC_HUFFMAN, &h2, 256, 8 );
	adh_init_first_node ( &h2, 0 );   /* 0 is the first symbol. */

	/* insert the rest of the bytes into the huffman tree. */
	for ( i = 1; i < 256; i++ ){
		update_treeVITTER( &h2, i );
	}

	/* the huffman tree is now "complete"; remove 0-node and 1st symbol. */
	remove_symbol( h2.zero_node );
	remove_symbol( h2.hufflist[c] );
#endif

	while( 1 ) {
		/* load the input buffer. */
		nread = fread( input_buf, 1, IN_BUFSIZE, in );
		if ( nread == 0 ) break;
		in_i = 0;

		/* get bytes from the buffer and compress them. */
		while( in_i < nread ){
			c = (unsigned char) *(input_buf+in_i);
			++in_i;

			/* encode the byte c. */
			if ( huff.hufflist[c] ){  /* VALID address; previously seen. */
				hcompress( huff.hufflist[c] );
			}
			else {  /* NULL address; a new byte! */
				/* output the zero node's code. */
				hcompress( huff.zero_node );

			#ifndef IMPROVED_BYTE_OUTPUT
				put_nbits( c, 8 );
			#else
				/* don't use 8 bits when you can use less.. */
				hcompress( h2.hufflist[c] );
				remove_symbol( h2.hufflist[c] );
			#endif
			}

			/* recompute the tree if necessary. */
			update_treeVITTER( &huff, c );	/* pass the symbol. */
		}
	}
	flush_put_buffer();
	fprintf( stderr, "done.");

	/* get outfile's size and get compression ratio. */
	out_file_len = ftell( pOUT );

	fprintf(stderr, "\n\nLength of input file     = %15lu bytes", in_file_len );
	fprintf(stderr, "\nLength of output file    = %15lu bytes", out_file_len );
	fprintf(stderr, "\nCompression ratio:         %15.2f %%\n",
		( ((float) in_file_len - (float) out_file_len) / (float) in_file_len	)
		* (float) 100 );
	
	halt_prog:

	free_put_buffer();
	free_huff_model( DYNAMIC_HUFFMAN, &huff );
	free_huff_model( DYNAMIC_HUFFMAN, &h2 );

	if ( in ) fclose( in );
	if ( pOUT ) fclose( pOUT );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2005\n");
}
