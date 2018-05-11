/*
	Filename:     praq1.c
	Description:  PPP/LZP with variable-length encoding of MTF (SR) codes.
	Written by:   Gerald R. Tamayo, (July 2009, 1/12/2010, 4/14/2010, 9/2/2017)
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

#define EOF_PPP    254
#define EOF_VLC    256

#define WBITS       20
#define WSIZE       (1<<WBITS)
#define WMASK       (WSIZE-1)

enum {
	/* modes */
	COMPRESS,
	DECOMPRESS,
};

enum {
	/* codes */
	mPPP = 1,
	mVLCODE
};

typedef struct {
	char alg[4];
} file_stamp;

unsigned char win_buf[ WSIZE ];   /* the prediction buffer. */
unsigned char pattern[ WSIZE ];   /* the "look-ahead" buffer. */
int mcode = 0;

void copyright( void );
void   compress( unsigned char w[], unsigned char p[] );
void decompress( unsigned char w[] );

void usage( void )
{
	fprintf(stderr, "\n Usage: praq1 c[1|2]|d infile outfile\n"
		"\n Commands:\n  c1 = PPP (raw byte output) \n  c2 = MTF coding\n  d  = decoding.\n"
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
	
	cmd = argv[argc=1];
	while ( cmd ){
		switch( *cmd ) {
			case 'c': if ( mode == -1 ) mode = COMPRESS; else usage(); cmd++; break;
			case 'd':
				if ( mode == -1 ) mode = DECOMPRESS; else usage();
				if ( *(cmd+1) != 0 ) usage(); cmd++; break;
			case '1':
				if ( mode == -1 || mode == DECOMPRESS || mcode ) usage();
				mcode = mPPP; cmd++; break;
			case '2':
				if ( mode == -1 || mode == DECOMPRESS || mcode ) usage();
				mcode = mVLCODE; cmd++; break;
			case 0: cmd = NULL; if ( mcode == 0 ) mcode = mPPP; break;
			default : usage();
		}
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
	
	/* initialize prediction buffer to all zero (0) values. */
	memset( win_buf, 0, WSIZE );
	alloc_mtf(256);
	
	if ( mode == COMPRESS ){
		/* Write the FILE STAMP. */
		strcpy( fstamp.alg, "LZP" );
		fstamp.alg[3] = (char) mcode;
		fwrite( &fstamp, sizeof(file_stamp), 1, pOUT );
		
		fprintf(stderr, "\n Encoding [ %s to %s ] ...", 
			argv[argc], argv[argc+1] );
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
		argv[argc], ftell(gIN), argv[argc+1], ftell(pOUT));	
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

void compress( unsigned char w[], unsigned char p[] )
{
	register int c, b=0, n, nread, prev = 0;  /* prev = context */
	
	while ( (nread=fread(p, 1, WSIZE, gIN)) ){
		n = 0;
		while ( n < nread ) {
			if ( w[prev] == (c=p[n&WMASK]) ){
				b++;
				/* The fact that the symbol is correctly predicted means that 
				it might have a higher frequency count than the other symbols 
				or that it is currently highly-ranked according to recency 
				(e.g., local-block runs). Thus, it is likely to occur again but 
				may not be predicted according to the current context, so move 
				it to front to maintain higher rank (i.e., for the next 1<<BSIZE
				input symbols) or shorter code of BSIZE+1 number of bits. */
				if ( mcode == mVLCODE ) {
					/* one type of frequency-based MTF coding */
					mtf(c);   /* c is moved to front here.. */
					/* but if 2nd position symbol is higher in frequency, 
					let c be moved to the second position */
					if ( head->next->f > ++head->f )
						mtf(head->next->c);
				}
			}
			else {
				if ( b ) {
					put_ONE();
					put_vlcode(--b, 0);
				}
				else put_ZERO();
				b = 0;
				if ( mcode == mPPP ) {
					put_nbits(c, 8);
					if ( c == EOF_PPP ) put_ZERO();
				}
				else {
					put_vlcode( mtf(c), BSIZE );
					if ( head->next->f > ++head->f )
						mtf(head->next->c);
				}
				w[prev] = c;
			}
			n++;
			prev = ((prev<<5)+c) & WMASK;
		}
	}
	/* flag EOF */
	if ( b ) {
		put_ONE();
		put_vlcode(--b, 0);
	}
	else put_ZERO();
	if ( mcode == mPPP ) {
		put_nbits(EOF_PPP, 8);
		put_ONE();
	}
	else put_vlcode(EOF_VLC, BSIZE);
}

void decompress( unsigned char w[] )
{
	register int b, c, prev = 0;  /* prev = context */
	
	do {
		if ( get_bit() ){
			b = get_vlcode(0)+1;
			do {
				pfputc( c=w[prev] );
				if ( mcode == mVLCODE ) {
					mtf(c);
					if ( head->next->f > ++head->f )
						mtf(head->next->c);
				}
				prev = ((prev<<5)+c) & WMASK;
			} while ( --b );
		}
		if ( mcode == mPPP ) {
			c = get_nbits(8);
			if ( c == EOF_PPP && get_bit() ) return;
		}
		else {
			if( (c=get_vlcode(BSIZE)) == EOF_VLC ) return;
			c = get_mtf_c(c);
			if ( head->next->f > ++head->f )
				mtf(head->next->c);
		}
		pfputc( c );
		w[prev] = c;
		prev = ((prev<<5)+c) & WMASK;
	} while( 1 );
}
