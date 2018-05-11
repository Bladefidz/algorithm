/*
	Filename:   lzufd2.c (Oct. 22, 2008).(4/11/2010)
	Encoder:    lzuf2.c
	
	Decompression in LZ77/LZSS is faster since you just have to extract
	the bytes from the window buffer using the pos and len variables.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utypes.h"
#include "gtbitio.c"
#include "mtf.c"
#include "ucodes.c"

#define LTCB              17
#ifdef LTCB
	#define NUM_POS_BITS   LTCB
#else
	#define NUM_POS_BITS   15
#endif

#define WIN_BUFSIZE    (1<<(NUM_POS_BITS))
#define WIN_MASK       ((WIN_BUFSIZE)-1)

#define PAT_BUFSIZE    (WIN_BUFSIZE>>1)
#define MIN_LEN            4

typedef struct {
	char algorithm[4];
	ulong file_size;
} file_stamp;

typedef struct {
	uint pos, len;
} dpos_t;

dpos_t dpos;
uchar win_buf[ WIN_BUFSIZE ];  /* the "sliding window" buffer. */
uchar pattern[ PAT_BUFSIZE ];  /* the "look-ahead" buffer (LAB). */
int win_cnt = 0, len_CODE = 0;

void copyright( void );

int main( int argc, char *argv[] )
{
	unsigned long fsize = 0;
	unsigned int i, k, large_text, len;
	file_stamp fstamp;

	if ( argc != 3 ) {
		fprintf(stderr, "\n Usage: lzufd2 infile outfile");
		copyright();
		return 0;
	}
	init_buffer_sizes( (1 << 15) );
	
	if ( (gIN = fopen(argv[1], "rb")) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		return 0;
	}
	fread( &fstamp, sizeof(file_stamp), 1, gIN );
	init_get_buffer();

	if ( (pOUT = fopen(argv[2], "wb")) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		goto halt_prog;
	}
	init_put_buffer();
	
	fprintf(stderr, "\n Name of input  file : %s", argv[1] );
	fprintf(stderr, "\n Name of output file : %s", argv[2] );

	fprintf(stderr, "\n\n  Decompressing...");

	/* initialize sliding-window. */
	memset( win_buf, 0, WIN_BUFSIZE );
	
	alloc_mtf(256);
	
	if ( fstamp.algorithm[3] == 1 ) large_text = 1;
	else large_text = 0;
	
	fsize = fstamp.file_size;
	while( fsize ) {
		if ( get_bit() == 1 ){
			/* get length. */
			if ( !large_text ) len_CODE = get_vlcode(1);
			else {
				for ( len_CODE = 0; get_bit(); len_CODE++ ) ;
				#define MFOLD    2
				len_CODE <<= MFOLD;
				len_CODE += get_nbits(MFOLD);
			}
			
			/* get position. */
			dpos.pos = get_nbits( NUM_POS_BITS );
			dpos.len = len_CODE + (MIN_LEN+1);  /* actual length. */
			
			/* if its a match, then "slide" the window buffer. */
			if ( dpos.pos < win_cnt ) {
				i = dpos.len;
				while ( i-- ) {
					pattern[i] = win_buf[ (win_cnt+i) & WIN_MASK ] 
						= win_buf[ (dpos.pos+i) & WIN_MASK ];
				}
				/* output string. */
				i = 0;
				do {
					pfputc(pattern[i++]);
				} while ( i < dpos.len );
			}
			else {
				/* update window and output string. */
				for ( i = 0; i < dpos.len; i++ ) {
					pfputc( win_buf[ (win_cnt+i) & WIN_MASK ] 
						= win_buf[ (dpos.pos+i) & WIN_MASK ] );
				}
			}
			fsize -= dpos.len;
			win_cnt = (win_cnt + dpos.len) & WIN_MASK;
		}
		else {
			switch( get_bit() ){
			case 0:

			/* get VL-coded byte and output it. */
			k = get_mtf_c(get_vlcode(3));
			pfputc( win_buf[ (win_cnt) & WIN_MASK ] = k );
			if ( (++win_cnt) == WIN_BUFSIZE ) win_cnt = 0;
			--fsize;

			break;

			case 1:

			/* get position. */
			dpos.pos = get_nbits( NUM_POS_BITS );
			dpos.len = MIN_LEN;
			
			/* if its a match, then "slide" the window buffer. */
			if ( dpos.pos < win_cnt ) {
				i = dpos.len;
				while ( i-- ) {
					pattern[i] = win_buf[ (win_cnt+i) & WIN_MASK ] 
						= win_buf[ (dpos.pos+i) & WIN_MASK ];
				}
				/* output string. */
				i = 0;
				do {
					pfputc(pattern[i++]);
				} while ( i < dpos.len );
			}
			else {
				/* update window and output string. */
				for ( i = 0; i < dpos.len; i++ ) {
					pfputc( win_buf[ (win_cnt+i) & WIN_MASK ] 
						= win_buf[ (dpos.pos+i) & WIN_MASK ] );
				}
			}
			fsize -= dpos.len;
			win_cnt = (win_cnt + dpos.len) & WIN_MASK;
		
			break;
			}	/* end switch */
		}
	}
	fprintf(stderr, "complete.");
	flush_put_buffer();
	copyright();

	halt_prog:
	
	free_get_buffer();
	free_put_buffer();
	free_mtf_table();
	if ( gIN ) fclose( gIN );
	if ( pOUT ) fclose( pOUT );
	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2008\n");
}
