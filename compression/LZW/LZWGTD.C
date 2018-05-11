/*
	---- A Lempel-Ziv-Welch (LZW) Algorithm Implementation ----

	Filename:  LZWGTD.C	(the decoder to LZWGT.C)
	Author:    Gerald R. Tamayo, 2005/2008/2010
*/
#include <stdio.h>
#include <stdlib.h>
#include "utypes.h"
#include "gtbitio.c"
#include "lzwbt.c"

#define CODE_MAX_BITS     16
#define CODE_MAX        (1<<CODE_MAX_BITS)

#define EOF_LZW_CODE     256
#define START_LZW_CODE   257

#define get_code() get_nbits( bit_count )

typedef struct {
	char algorithm[3],
	     binary_file;
	int N;
} file_stamp;

int bit_count = 9;  /* code size starts at 9 bits. */
int code_max = 512; /* start expanding the code size if we
                         already reached this value. */
                        
int stack_cnt = 0;
unsigned char stack_buffer[ CODE_MAX ], *stack=NULL;

void get_string( int lzwcode );
void copyright( void );

int main( int argc, char *argv[] )
{
	FILE *out;
	file_stamp fstamp;
	int old_lzw_code = 0, new_lzw_code = 0, binary_file, lzwcode;
	int cnt = 0, N;

	if ( argc != 3 ) {
		fprintf(stderr, "\n Usage: lzwgtd infile outfile");
		copyright();
		return 0;
	}
	if ( (gIN = fopen( argv[1], "rb")) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		return 0;
	}
	if ( (out = fopen( argv[2], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		return 0;
	}
	
	fprintf(stderr, "\nName of input file : %s", argv[1] );
	
	/* start deCompressing to output file. */
	fprintf(stderr, "\n Decompressing...");

	/* read the file header, */
	fread( &fstamp, sizeof( file_stamp ), 1, gIN );

	/* and initialize the input buffer. */
	init_get_buffer();
	
	binary_file = fstamp.binary_file;
	N = fstamp.N;
	
	/* allocate and initialize the code tables. */
	if ( !alloc_code_tables(CODE_MAX, LZW_DECOMPRESS) ) goto halt_prog;
	init_code_tables(CODE_MAX, LZW_DECOMPRESS);
	
	/* set the starting code to define. */
	lzw_code_cnt = START_LZW_CODE;
	
	/* get first code. */
	switch ( binary_file ){
		case 0: old_lzw_code = get_nbits( bit_count ); break;
		case 1:
			if ( get_bit() == 1 ) old_lzw_code = get_nbits( 8 );
			else old_lzw_code = get_nbits( bit_count );
			break;
	}

	/* first code is a character; output it. */
	putc( (unsigned char) old_lzw_code, out );
	
	while( 1 ) {
		switch ( binary_file ){
			case 0: new_lzw_code = get_nbits( bit_count ); break;
			case 1:
				if ( get_bit() == 1 ) new_lzw_code = get_nbits( 8 );
				else new_lzw_code = get_nbits( bit_count );
				break;
		}
		
		if ( new_lzw_code == EOF_LZW_CODE ) break;
		else if ( new_lzw_code >= lzw_code_cnt ) lzwcode = old_lzw_code;
		else lzwcode = new_lzw_code;
		
		/* GET STRING/PATTERN. */
		stack = stack_buffer;
		while ( lzwcode > EOF_LZW_CODE ) {
			*stack++ = code_char[ lzwcode ];
			/* when loop exits, lzwcode must be a character. */
			lzwcode = code_prefix[ lzwcode ];
		}
		
		/* K = get first character of string. */
		*stack = lzwcode;
		
		/* OUTPUT STRING/PATTERN. */
		while( stack >= stack_buffer ) {
			putc( *stack--, out );
		}
		/* if undefined code. */
		if ( new_lzw_code >= lzw_code_cnt ) {
			putc( lzwcode, out );
		}

		/* add PREV_CODE+K to the string table. */
		if ( lzw_code_cnt < CODE_MAX ) {
			lzw_decomp_insert( old_lzw_code, (unsigned char) lzwcode );
			if ( bit_count < CODE_MAX_BITS ){
				if ( lzw_code_cnt == (code_max-1) ) {
					bit_count++;
					code_max <<= 1;
				}
			}
		}
		
		/* PREV_CODE = CURR_CODE */
		old_lzw_code = new_lzw_code;
		
		/* reset table. */
		if ( ++lzw_code_cnt == N ) {
			lzw_code_cnt = START_LZW_CODE;
			bit_count =   9;
			code_max  = 512;
			
			/* get first code. */
			switch ( binary_file ){
				case 0: old_lzw_code = get_nbits( bit_count ); break;
				case 1:
					if ( get_bit() == 1 ) old_lzw_code = get_nbits( 8 );
					else old_lzw_code = get_nbits( bit_count );
					break;
			}
		
			/* first code is a character; output it. */
			putc( (unsigned char) old_lzw_code, out );
		}
	}
	fprintf(stderr, "done.");
	fprintf(stderr, "\nName of output file: %s\n", argv[2] );

	halt_prog:

	free_get_buffer();
	if ( gIN ) fclose( gIN );
	if ( out ) fclose( out );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n:: Gerald R. Tamayo, 2005\n");
}
