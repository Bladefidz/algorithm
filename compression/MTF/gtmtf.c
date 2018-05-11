/*
	Filename:     gtmtf.c   (Single-file encoder/decoder.)
	Description:  A Move-to-Front (MTF) Implementation
	Author:       Gerald Tamayo, 2004/2009
	
	
	MTF translates a run of different letters to a run of zero-bytes.

	For example, the run

		"XXXYYYZZZ"

	will be translated by other run-length schemes as
	
		XX{1}, YY{1}, ZZ{1}

	while this MTF will just encode it as X00, Y00, Z00.

	As you can see, there are now more "00" strings, making the data 
	source more conducive for better compression.

	For a file heavy with runs of "distinct" characters, the MTF output 
	file will then have many "runs of zeros," instead of individual runs 
	of different bytes. LZW, for example, will only assign a single code 
	for these runs of zeros instead of assigning more codes for runs of 
	the different bytes, allowing LZW to define more strings.
	
	More runs of "different bytes" are usually seen in the output of the 
	Burrows-Wheeler-Transform (BWT) transform, so using our simple MTF
	encoder to the output of BWT will create a BWT file filled with many
	runs of zeros. In fact, MTF was suggested by Burrows and Wheeler for
	their invented transform.
	
	MTF also creates just one "code string" for similar-context patterns 
	like "XYXYXY" and "MNMNMN", outputting just the integer values 
	{1, 1, 1, 1} for the last pairs of "XY" strings.
	
	This MTF version uses a doubly-linked list for a faster move-to-front 
	operation (instead of actually shifting the characters in a symbol table).
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtf.c"

void copyright(void);

/* the (slow) table version. */
void mtf_encode ( FILE *in, FILE *out );
void mtf_decode ( FILE *in, FILE *out );

/* these functions use mtf(c) and get_mtf_c() from mtf.c */
void mtf_list_encode ( FILE *in, FILE *out );
void mtf_list_decode ( FILE *in, FILE *out );

int main (int argc, char *argv[])
{
	FILE *in, *out;

	if (argc != 4){
		fprintf(stderr, "\n Usage : gtmtf c|d inputfile outputfile\n");
		copyright();
		return 0;
	}
	in = fopen(argv[2], "rb");
	if (!in){
		fprintf(stderr, "\n Error opening input file!\n");
		copyright();
		return 0;
	}
	out = fopen(argv[3], "wb");
	if (!out){
		fprintf(stderr, "\n Error opening output file!\n");
		copyright();
		return 0;
	}
	if (!strcmp(argv[1], "d") || !strcmp(argv[1], "D")){
		fprintf(stderr, "\n MTF decoding...");
		mtf_list_decode(in, out);
	}
	else if (!strcmp(argv[1], "c") || !strcmp(argv[1], "C")){
		fprintf(stderr, "\n --[ A Move-to-Front (MTF) Implementation ]--\n");
		fprintf(stderr, "\n MTF coding...");
		mtf_list_encode(in, out);
	}
	else {
		fprintf(stderr, "\n Error! Unknown: \"%s\"\n", argv[1]);
		copyright();
		return 0;
	}
	fprintf(stderr, "done.\n");
	copyright();
	
	if (in)  fclose(in);
	if (out) fclose(out);
	return 0;
}

void mtf_encode ( FILE *in, FILE *out )
{
	int curr, i;
	int table[ 256 ];

	/* initialize the table. */
	for ( i = 0; i < 256; i++ ){
		table[i] = i;
	}

	while ( (curr = fgetc(in) ) != EOF ) {
		/* find curr in the table. */
		for ( i = 0; i < 256; i++ ) {
			if ( table[i] == curr ) {
				/* and output its index in the table. */
				fputc( i, out );
				break;
			}
		}
		
		/* shift (to the right) the bytes to its left. */
		while ( i-- ) {
			table[ i+1 ] = table[ i ];
		}

		/* move-to-front.*/
		table[ 0 ] = (unsigned char) curr;
	}
}

void mtf_decode ( FILE *in, FILE *out )
{
	int curr, i;
	int table[ 256 ];

	/* initialize the table. */
	for ( i = 0; i < 256; i++ ){
		table[ i ] = i;
	}

	while ( (i = fgetc(in) ) != EOF ) {
		/*	i is a position in the table.
			output the byte in that position.
		*/
		fputc( table[ i ], out );
		curr = table[ i ]; /* save it in curr. */

		/* shift (to the right) the bytes to its left. */
		while( i-- ) {
			table[ i+1 ] = table[ i ];
		}

		/* move-to-front.*/
		table[ 0 ] = (unsigned char) curr;
	}
}

/*  MTF list encode() and decode() file routines. */
void mtf_list_encode (FILE *in, FILE *out)
{
	int c;
	
	alloc_mtf(256);
	while ((c=getc(in)) != EOF) {
		putc(mtf(c), out);
	}
	free_mtf_table();
}

void mtf_list_decode (FILE *in, FILE *out)
{
	int curr, i;
	
	alloc_mtf(256);
	while ((i=getc(in)) != EOF) {
		putc(get_mtf_c(i), out);
	}
	free_mtf_table();
}

void copyright(void)
{
	fprintf(stderr, "\n Written by: Gerald Tamayo, 2004/2009\n");
}
