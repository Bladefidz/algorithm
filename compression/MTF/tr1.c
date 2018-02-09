/*
	Filename:     tr1.c
	Description:  A transformation method for binary source (e.g., arithmetic) coding.
	Written by:   Gerald R. Tamayo, 2009/2010
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtbitio.c"

#define WBITS       15
#define WSIZE       (1<<WBITS)
#define WMASK       (WSIZE-1)

enum { ENCODE, DECODE };

unsigned char win_buf[ WSIZE ];

void copyright( void );
void compress( unsigned char w[] );
void decompress( void );

void usage( void )
{
	fprintf(stderr, "\n Usage: tr1 c|d infile outfile\n");
	copyright();
	exit(0);
}

int main( int argc, char *argv[] )
{
	int mode = ENCODE;
	
	if ( argc != 4 ) usage();
	init_buffer_sizes( (1<<15) );
	
	if ( !(strcmp(argv[argc=1], "c")) || !(strcmp(argv[argc=1], "C")) ){
		mode = ENCODE;
	}
	else if ( !(strcmp(argv[argc], "d")) || !(strcmp(argv[argc], "D")) ){
		mode = DECODE;
	}
	else {
		fprintf(stderr, "\n Error! Unknown: \"%s\"\n", argv[argc] );
		exit(0);
	}
	argc++;
	
	if ( (gIN=fopen( argv[argc], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		return 0;
	}
	if ( (pOUT=fopen( argv[argc+1], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		return 0;
	}
	init_put_buffer();
	
	if ( mode == ENCODE ){
		fprintf(stderr, "\n Encoding [ %s to %s ] ...", 
			argv[argc], argv[argc+1] );
		compress( win_buf );
	}
	else if ( mode == DECODE ) {
		init_get_buffer();
		fprintf(stderr, "\n Decoding...");
		decompress();
	}
	flush_put_buffer();
	
	fprintf(stderr, "done.\n  %s (%lu) -> %s (%lu).\n", 
		argv[argc], ftell(gIN), argv[argc+1], ftell(pOUT));	
	
	free_get_buffer();
	free_put_buffer();
	if ( gIN ) fclose( gIN );
	if ( pOUT ) fclose( pOUT );
	
	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n Written by: Gerald Tamayo, 2009\n");
}

void compress( unsigned char w[] )
{
	register int c, b, n, nread, i, prev = 0;
	
	while ( (nread=fread(w, 1, WSIZE, gIN)) ){
		n = 0;
		while ( n < nread ) {
			c = w[n++ & WMASK];
			for ( i = 7; i >= 0; --i ){
				b = (c >> i) & 1;
				if ( b == prev ) {
					c |= (1 << i);
				}
				else {
					c &= ~(1 << i);
					prev = 1 - prev;
				}
			}
			pfputc(c);
		}
	}
}

void decompress( void )
{
	register int b, c, i, prev = 0;
	
	while ( (c=gfgetc()) != EOF ){
		for ( i = 7; i >= 0; --i ){
			b = (c >> i) & 1;
			if ( b ) {
				if ( prev ) c |= (1 << i);
				else c &= ~(1 << i);
			}
			else {
				prev = 1 - prev;
				if ( prev ) c |= (1 << i);
				else c &= ~(1 << i);
			}
		}
		pfputc(c);
	}
}
