/*
	---- An LZW Compression Implementation ----

	Filename:     GTLZW91.C
	Description:  An LZW implementation using Ternary Search Trees (TSTs).
	Author:       Gerald Tamayo
	Date:         November, 2004

	Decompression program:	gtlzw91d.c

	We reserve code 256 as the END-of-FILE code (EOF_LZW_CODE)
	passed to the decoder.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utypes.h"
#include "gtbitio.c"
#include "tst875.c"

/*
define this if you want to see the codes
as they are defined and transmitted as output.
*/
/* #define PRINT_STATS */

#define CODE_MAX          4096
#define CODE_MAX_BITS       12

#define EOF_LZW_CODE       256
#define START_LZW_CODE     257

typedef struct {
	char algorithm[4];
	ulong file_size;
} file_stamp;

#define PAT_BUFSIZE 2000

int pattern[ PAT_BUFSIZE ], *pat;
int pat_cnt = 0;

void copyright( void );

int main( int argc, char *argv[] )
{
	unsigned long in_file_len = 0, out_file_len = 0;
	float ratio = 0.0;
	int c;
	file_stamp fstamp;
	node_p curr;

	#ifdef PRINT_STATS
	int prev_code = 0;
	#endif

	if ( argc != 3 ) {
		fprintf(stderr, "\n Usage: gtlzw91 infile outfile");
		copyright();
		return 0;
	}

	if ( (gIN = fopen( argv[1], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		return 0;
	}

	if ( (pOUT = fopen( argv[2], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		return 0;
	}
	init_put_buffer();

	fprintf(stderr, "\n----[  A Lempel-Ziv-Welch (LZW) Implementation  ]----");

	fprintf(stderr, "\n\nName of input file : %s", argv[1] );

	/* display file length. */
	fseek( gIN, 0, SEEK_END );
	in_file_len = ftell( gIN );
	fprintf(stderr, "\nLength of input file     = %15lu bytes", in_file_len );
	if ( in_file_len ==  0 ) {
		fprintf(stderr, "\nFile size = 0 bytes!");
		goto halt_prog;
	}

	/* Write the FILE STAMP. */
	rewind( pOUT );
	strcpy( fstamp.algorithm, "LZW" );
	fstamp.file_size = in_file_len;
	fwrite( &fstamp, sizeof(file_stamp), 1, pOUT );

	/* start Compressing to output file. */
	fprintf(stderr, "\n\nCompressing...");

	/* make sure to rewind the input file */
	rewind(gIN);

	/* and initialize the input buffer. */
	init_get_buffer();

	/* initial allocation to the TST buffer of nodes. */
	if ( !init_tst_buf() ){
		fprintf(stderr, "\nMemory allocation error!");
		goto halt_prog;
	}

	pat_cnt = 0; /* get first character. */
	if ( (c = gfgetc()) != EOF ) {
		pat = pattern;
		pattern[ pat_cnt ] = c;
		lzw_curr_code = c;
		pat_cnt++;
	}

	/* start of codes to define. */
	lzw_code_cnt = START_LZW_CODE;

	#ifdef PRINT_STATS
	prev_code = START_LZW_CODE;
	#endif

	curr = &root; /* at start, point to root. */
	while( (c = gfgetc()) != EOF ) {
		pattern[ pat_cnt ] = c;
		pattern[ pat_cnt+1 ] = EOS;

		if ( lzw_curr_code >= START_LZW_CODE ) {
			/*
			We have a "run" of search hits; search or insert
			starting at the end of the prefix string.
			*/
			curr = lzw_list[ lzw_curr_code ];
			pat = &pattern[ pat_cnt ];
		}

		if ( !nr_tst_search(curr, pat) ) {
			/* output lzwcode. */
			put_nbits( lzw_curr_code, CODE_MAX_BITS );

			#ifdef PRINT_STATS
			if ( lzw_code_cnt == CODE_MAX ) fprintf(stderr, "\n");
			if ( lzw_curr_code >= 256 ) fprintf(stderr, "Out >> %5d", lzw_curr_code );
			else fprintf(stderr, "Out >> %5c", lzw_curr_code );
			if ( lzw_curr_code == prev_code ) fprintf(stderr, " ?");
			#endif

			/* insert the string in the string table (TST). */
			if ( lzw_code_cnt < CODE_MAX ){
				#ifdef PRINT_STATS
				prev_code = lzw_code_cnt;
				#endif

				if ( nr_tst_insert( curr, pat ) ) {
					#ifdef PRINT_STATS
					fprintf(stderr, "\nDefined %d. ", lzw_code_cnt );
					#endif
					lzw_code_cnt++;
				}
			}

			/* string = char */
			pat_cnt = 0;
			pattern[ pat_cnt ] = c;
			lzw_curr_code = c;

			/* reset. */
			curr = &root;
			pat = pattern;
		}

		pat_cnt++;
	}

	/* output last lzwcode. */
	put_nbits( lzw_curr_code, CODE_MAX_BITS );
	#ifdef PRINT_STATS
	if ( lzw_code_cnt == CODE_MAX ) fprintf(stderr, "\n");
	if ( lzw_curr_code >= 256 ) fprintf(stderr, "Out >> %5d", lzw_curr_code );
	else fprintf(stderr, "Out >> %5c", lzw_curr_code );
	#endif

	/* send end-of-file code. */
	put_nbits( EOF_LZW_CODE, CODE_MAX_BITS );
	#ifdef PRINT_STATS
	fprintf(stderr, "\nOutput EOF_LZW_CODE: %5d\n", EOF_LZW_CODE );
	#endif

	flush_put_buffer();
	fprintf(stderr, "complete.");

	/* get outfile's size and get compression ratio. */
	out_file_len = ftell( pOUT );

	fprintf(stderr, "\n\nName of output file: %s", argv[2] );
	fprintf(stderr, "\nLength of input file     = %15lu bytes", in_file_len );
	fprintf(stderr, "\nLength of output file    = %15lu bytes", out_file_len );

	ratio = (((float) in_file_len - (float) out_file_len)
						/ (float) in_file_len ) * (float) 100;
	fprintf(stderr, "\nCompression ratio:         %15.2f %%\n", ratio );
	
	halt_prog:

	/* ---- free the buffer of nodes. ---- */
	destroy_tst_buffer();

	free_put_buffer();
	free_get_buffer();
	if ( gIN ) fclose( gIN );
	if ( pOUT ) fclose( pOUT );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2004\n");
}
