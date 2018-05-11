/*
	Filename:    lzpn.c
	Written by:  Gerald R. Tamayo, (5/13-14/2010).(7/8/2010).(7/15/2010).(8/7/2010).(9/2/2017)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtbitio.c"
#include "ucodes.c"
#include "mtf.c"

/* Bitsize of the first N (i.e., 1<<BSIZE) high-ranking 
symbols, output codesize = 1+BSIZE */
#define BSIZE        3
#define EOF_CODE    16

#define NSEARCH     EOF_CODE

#define WBITS       20
#define WSIZE       (1<<WBITS)
#define WMASK       (WSIZE-1)

enum {
	/* modes */
	COMPRESS,
	DECOMPRESS,
};

typedef struct {
	char alg[4];
} file_stamp;

unsigned char win_buf[WSIZE];   /* the prediction buffer. */
unsigned char pattern[WSIZE];   /* the "look-ahead" buffer. */
int mcode = 0;

void copyright( void );
int found( unsigned char w[], int prev, unsigned char c, int size );
int search( unsigned char w[], int prev, unsigned char b );
void   compress( unsigned char w[], unsigned char p[] );
void decompress( unsigned char w[] );

void usage( void )
{
	fprintf(stderr, "\n Usage: lzpn c|d infile outfile\n"
		"\n Commands:\n  c = compress\n  d = decompress.\n"
	);
	copyright();
	exit(0);
}

int main( int argc, char *argv[] )
{
	float ratio = 0.0;
	int mode = -1;
	file_stamp fstamp;
	char *cmd = NULL;
	
	if ( argc != 4 ) usage();
	init_buffer_sizes( (1<<15) );
	
	cmd = argv[1];
	while ( cmd ){
		switch( *cmd ) {
			case 'c': if ( mode == -1 ) mode = COMPRESS; else usage(); cmd++; break;
			case 'd':
				if ( mode == -1 ) mode = DECOMPRESS; else usage(); cmd++; break;
			case 0: cmd = NULL; break;
			default: usage();
		}
	}
	
	if ( (gIN=fopen( argv[2], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		return 0;
	}
	if ( (pOUT=fopen( argv[3], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		return 0;
	}
	init_put_buffer();
	
	/* initialize prediction buffer to all zero (0) values. */
	memset( win_buf, 0, WSIZE );
	alloc_mtf(256);
	
	if ( mode == COMPRESS ){
		/* Write the FILE STAMP. */
		strcpy( fstamp.alg, "LZP" );
		fstamp.alg[3] = (char) mcode;
		fwrite( &fstamp, sizeof(file_stamp), 1, pOUT );
		
		fprintf(stderr, "\n Encoding [%s to %s] ...",
			argv[2], argv[3] );
		compress( win_buf, pattern );
	}
	else if ( mode == DECOMPRESS ){
		fread( &fstamp, sizeof(file_stamp), 1, gIN );
		mcode = fstamp.alg[3];
		init_get_buffer();
		fprintf(stderr, "\n Decoding...");
		decompress( win_buf );
		free_get_buffer();
	}
	flush_put_buffer();
	
	fprintf(stderr, "done.\n  %s (%lu) -> %s (%lu).\n", 
		argv[2], ftell(gIN), argv[3], ftell(pOUT));	
	if ( mode == COMPRESS ) {
		ratio = (((float) ftell(gIN) - (float) ftell(pOUT)) /
			(float) ftell(gIN) ) * (float) 100;
		fprintf(stderr, " Compression ratio: %3.2f %%\n", ratio );
	}
	
	free_put_buffer();
	free_mtf_table();
	if ( gIN ) fclose( gIN );
	if ( pOUT ) fclose( pOUT );
	
	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n Written by: Gerald R. Tamayo, 2010\n");
}

int found( unsigned char w[], int prev, unsigned char c, int size )
{
	register int b = 1;
	
	while ( b != size ){
		if ( w[prev & WMASK] == c ) return b;
		else prev = ((prev<<5)+w[prev]) & WMASK;
		b++;
	}
	return 0;
}

int search( unsigned char w[], int prev, unsigned char b )
{
	int c;
	
	while ( b-- ){
		c = w[prev & WMASK];
		prev = ((prev<<5)+c) & WMASK;
	}
	return c;
}

void compress( unsigned char w[], unsigned char p[] )
{
	register int c, b, n, prevc = 0, nread, prev = 0;  /* prev = context */
	
	while ( (nread=fread(p, 1, WSIZE, gIN)) ){
		n = 0;
		while ( n < nread ) {
			c = p[n++];
			b = found(w, prev, c, NSEARCH);
			put_unary(b);
			if ( b == 0 ) { /* not found */
				put_vlcode(mtf(c), BSIZE);
				prev = ((prev<<5)+c) & WMASK;
			}
			else if ( c != prevc ) 
				prev = ((prev<<5)+w[prev]) & WMASK;
			w[prev] = prevc = c;
		}
	}
	put_unary(EOF_CODE);
}

void decompress( unsigned char w[] )
{
	register int b, c, prevc = 0, prev = 0;  /* prev = context */
	
	do {
		b = get_unary();
		if ( b == 0 ){ /* not found */
			c = get_mtf_c( get_vlcode(BSIZE) );
			prev = ((prev<<5)+c) & WMASK;
		}
		else if ( b != EOF_CODE ){
			c = search(w, prev, b);
			if ( c != prevc )
				prev = ((prev<<5)+w[prev]) & WMASK;
		}
		else break;
		w[prev] = prevc = c;
		pfputc(c);
	} while( 1 );
}
