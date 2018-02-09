/*
	Filename:    lzpn.c
	Written by:  Gerald R. Tamayo, (5/13-14/2010).(7/8/2010).(7/15/2010).(7/17-18/2010).(8/7/2010).(9/2/2017)
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

#define EOF_BYTE   254
#define B_SIZE       2

#define NSEARCH     (1<<B_SIZE)

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

char *msg[] = {
	"Usage: lzpn c|d infile outfile",
	"Commands:\n  c = compress\n  d = decompress.",
	"Gerald R. Tamayo, 2010",
	"\0"
	};
	
void print_msg( char *s );
int found( unsigned char w[], int prev, unsigned char c, int size );
int search( unsigned char w[], int prev, unsigned char b );
void   compress( unsigned char w[], unsigned char p[] );
void decompress( unsigned char w[] );

void usage( char *msg[] )
{
	char **s = msg;
	
	while ( **s ){
		print_msg(*s++);
	}
	exit(0);
}

int main( int argc, char *argv[] )
{
	float ratio = 0.0;
	int mode = -1;
	file_stamp fstamp;
	char *cmd = NULL;
	
	if ( argc != 4 ) usage(msg);
	init_buffer_sizes( (1<<15) );
	
	cmd = argv[1];
	while ( cmd ){
		switch( *cmd ) {
			case 'c': if ( mode == -1 ) mode = COMPRESS; else usage(msg); cmd++; break;
			case 'd':
				if ( mode == -1 ) mode = DECOMPRESS; else usage(msg); cmd++; break;
			case 0: cmd = NULL; break;
			default: usage(msg);
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

void print_msg( char *s )
{
	fprintf(stderr, "\n %s\n", s);
}

int found( unsigned char w[], int prev, unsigned char c, int size )
{
	register int b = 1;
	
	while ( size-- > 1 ){
		if ( w[prev & WMASK] == c ) return b;
		/* extend the search beyond the current hash. */
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
	int c, b, n, nread, prev = 0;  /* prev = context */
	
	while ( (nread=fread(p, 1, WSIZE, gIN)) ){
		n = 0;
		while ( n < nread ) {
			c = p[n++];
			b = found(w, prev, c, NSEARCH);
			put_nbits(b, B_SIZE);
			if ( b == 0 ) { /* not found */
				put_vlcode(mtf(c), BSIZE);
				if ( c == EOF_BYTE ) put_ZERO();
			}
			else mtf(c);
			prev = ((prev<<5)+w[prev]) & WMASK;
			w[prev] = c;
		}
	}
	/* flag EOF */
	put_nbits(0, B_SIZE);
	put_vlcode(mtf(EOF_BYTE), BSIZE);
	put_ONE();
}

void decompress( unsigned char w[] )
{
	int b, c, prev = 0;  /* prev = context */
	
	do {
		b = get_nbits(B_SIZE);
		if ( b == 0 ){
			c = get_mtf_c( get_vlcode(BSIZE) );
			if ( c == EOF_BYTE && get_bit() ) break;
		}
		else mtf( c=search(w, prev, b) );
		prev = ((prev<<5)+w[prev]) & WMASK;
		w[prev] = c;
		pfputc(c);
	} while( 1 );
}
