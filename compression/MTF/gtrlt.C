/*
	Filename:     gtrlt.c		(Single-file encoder/decoder.)
	Description:  A Run-Length Transform (RLT)
	Author:       Gerald Tamayo, 2004
	
	This is actually just a type of Run-Length Transformation 
	technique, replacing all runs of symbols to a "single-symbol
	run."
	
	Translates a run of different letters to a run of zero-bytes.
	For example, the run
	
		"XXXYYYZZZ"

	will be quickly translated by other run-length schemes as
	
		XX{1}, YY{1}, ZZ{1}

	while this RLT will encode it as X00, Y00, Z00.

	As you can see, there are now more "00" strings.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* this can be any byte from 0..255 */
#define RUN_CHAR_CODE 0

void copyright( void );
void rlt_encode ( FILE *in, FILE *out );
void rlt_decode ( FILE *in, FILE *out );

int main ( int argc, char *argv[] )
{
	FILE *in, *out;

	if ( argc < 3 || argc > 4 ){
		fprintf(stderr, "\n Usage : gtrlt inputfile outputfile [/d]\n");
		copyright();
		return 0;
	}

	in = fopen( argv[1], "rb" );
	if ( !in ){
		fprintf(stderr, "\n Error opening input file!\n");
		copyright();
		return 0;
	}

	out = fopen( argv[2], "wb" );
	if ( !out ){
		fprintf(stderr, "\n Error opening output file!\n");
		copyright();
		return 0;
	}

	fprintf(stderr, "\n --[ A Run-Length Transform (RLT) Implementation ]--\n");

	if ( argc == 4 ){
		if ( !strcmp(argv[3], "/d") || !strcmp(argv[3], "/D") ){
			fprintf(stderr, "\n RLT Decoding...");
			rlt_decode( in, out );
		}
		else {
			fprintf(stderr, "\n Error! Unknown: \"%s\"\n", argv[3]);
			copyright();
			return 0;
		}
	}
	else {
		fprintf(stderr, "\n RLT Encoding...");
		rlt_encode( in, out );
	}
	fprintf(stderr, "complete.\n");

	copyright();

	if ( in  ) fclose( in );
	if ( out ) fclose( out );

	return 0;
}

void rlt_encode ( FILE *in, FILE *out )
{
	int c, prev = 0;

	if ( (c=getc(in)) != EOF ) {
		putc( c, out );
		prev = c;
	}
	while ( (c=getc(in)) != EOF ) {
		if ( c == prev ){
			putc( RUN_CHAR_CODE, out );
		}
		else {
			if ( c == RUN_CHAR_CODE ) {
				putc( prev, out );
			}
			else {
				putc( c, out );
			}
			prev = c;
		}
	}
}

void rlt_decode ( FILE *in, FILE *out )
{
	int c, prev;

	if ( (c=getc(in)) != EOF ) {
		putc( c, out );
		prev = c;
	}

	while ( (c=getc(in)) != EOF ) {
		if ( c == RUN_CHAR_CODE ) {
			putc( prev, out );
		}
		else {
			if ( c == prev ) {
				putc( RUN_CHAR_CODE, out );
				prev = RUN_CHAR_CODE;
			}
			else {
				putc( c, out );
				prev = c;
			}
		}
	}
}

void copyright( void )
{
	fprintf(stderr, "\n Written by: Gerald Tamayo, 2004\n");
}
