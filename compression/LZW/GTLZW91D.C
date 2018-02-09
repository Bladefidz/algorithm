/*
	---- An LZW Compression Implementation ----

	Filename:  GTLZW91D.C
	Author:    Gerald Tamayo, 2004
*/
#include <stdio.h>
#include <stdlib.h>
#include "utypes.h"
#include "gtbitio.c"
#include "tst875.c"

/*
define this when you want to see an undefined
code as it is encountered by the decoder.
*/
/* #define PRINT_STATS */

#define CODE_MAX        4096
#define CODE_MAX_BITS     12

#define EOF_LZW_CODE     256
#define START_LZW_CODE   257

typedef struct {
	char algorithm[4];
	ulong file_size;
} file_stamp;

/* a "backwards" stack buffer specifically-designed for the TST string. */
int stack_buffer[ CODE_MAX ];
int stack_cnt = 0;

void get_string ( int code );
void copyright( void );

int main( int argc, char *argv[] )
{
	FILE *out;
	file_stamp fstamp;
	int old_lzw_code = 0, new_lzw_code = 0, lzw_character = 0;
	int cnt = 0;

	if ( argc != 3 ) {
		fprintf(stderr, "\n Usage: gtlzw91d infile outfile");
		copyright();
		return 0;
	}
	if ( (gIN = fopen( argv[1], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		return 0;
	}
	if ( (out = fopen( argv[2], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		return 0;
	}

	fprintf(stderr, "\n----[  A Lempel-Ziv-Welch (LZW) Implementation  ]----");

	fprintf(stderr, "\n\nName of input file : %s", argv[1] );

	/* start Compressing to output file. */
	fprintf(stderr, "\n Decompressing...");

	/* make sure to rewind the input file, */
	rewind( gIN );

	/* read the file header, */
	fread( &fstamp, sizeof( file_stamp ), 1, gIN );

	/* and initialize the input buffer. */
	init_get_buffer();

	if ( !init_tst_buf() ) {
		fprintf(stderr, "\nMemory Alloc Error!");
		goto halt_prog;
	}

	/* get first code. */
	old_lzw_code = get_nbits( CODE_MAX_BITS );
	fputc( (unsigned char) old_lzw_code, out );
	lzw_character = old_lzw_code;

	lzw_code_cnt = START_LZW_CODE;
	stack_buffer[ CODE_MAX -1 ] = EOS;

	while( 1 ) {
		/* get lzwcode. */
		new_lzw_code = get_nbits( CODE_MAX_BITS );

		if ( new_lzw_code == EOF_LZW_CODE ) {
			#ifdef PRINT_STATS
			fprintf(stderr, "\n");
			#endif
			break;
		}

		if ( new_lzw_code >= lzw_code_cnt ) {
			#ifdef PRINT_STATS
			fprintf(stderr, "\nUndefined code! -> %4d.", new_lzw_code );
			#endif
			get_string( old_lzw_code );
		}
		else {
			get_string( new_lzw_code );
		}

		/* OUTPUT PATTERN. */
		cnt = stack_cnt;
		while( cnt != CODE_MAX-2 ) {
			fputc( stack_buffer[ cnt++ ], out );
		}
		/* if undefined code. */
		if ( new_lzw_code >= lzw_code_cnt ) {
			fputc( lzw_character, out );
		}

		/* K = first character in string. */
		lzw_character = (int) stack_buffer[ stack_cnt ];

		/* add PREV_CODE+K to the string table. */
		get_string( old_lzw_code );
		stack_buffer[ CODE_MAX-2 ] = lzw_character;
		if ( lzw_code_cnt < CODE_MAX ) {
			nr_tst_insert( &root, &stack_buffer[ stack_cnt ] );
			lzw_code_cnt++;
		}

		/* PREV_CODE = CURR_CODE */
		old_lzw_code = new_lzw_code;
	}
	fprintf(stderr, "done.");

	fprintf(stderr, "\nName of output file: %s\n", argv[2] );

	copyright();

	halt_prog:

	destroy_tst_buffer();
	free_get_buffer();
	if ( gIN ) fclose( gIN );
	if ( out ) fclose( out );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2004\n");
}

void get_string ( int code )
{
	node_p node;

	/*
	The CODE_MAX-2 array position is reserved for the "appended char" K.
	With the --stack_cnt decrement operations below, we always start with
	position CODE_MAX-3 of the array.
	*/
	stack_cnt = CODE_MAX-2;

	if ( code < 256 ) {
		stack_buffer[ --stack_cnt ] = code;
		return;
	}

	node = lzw_list[ code ];
	while( node ) {
		if ( node->parent && node->parent->center == node ){
			stack_buffer[ --stack_cnt ] = (int) node->parent->b;
		}
		node = node->parent;
	}
}
