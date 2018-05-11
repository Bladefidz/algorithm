/*
	---- A Lempel-Ziv-Welch (LZW) Algorithm Implementation ----

	Filename:     LZWGT.C
	Description:  An LZW implementation using a Binary Search Tree.
	Author:       Gerald Tamayo, 2005/2009/2010
	
	Decompression program:	LZWGTD.C

	We reserve code 256 as the END-of-FILE code (EOF_LZW_CODE).

	*Variable-length* lzwcodes; code size starts at 9 bits, then
	adds more bits as necessary.

	Like LZSS, we just output a single prefix bit for a literal when 
	the file is inferred to be a binary file.
	
	Usage:
	
		lzwgt -n inputfile outputfile
	
	where n is bitsize of integer (i.e., 1<<n) added to CODE_MAX 
	to obtain N=CODE_MAX+(1<<n). After N codes are transmitted, 
	we reset the string table.
	
	Gerald R. Tamayo, 2005/2009
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utypes.h"
#include "gtbitio.c"
#include "lzwbt.c"

#define CODE_MAX_BITS     16
#define CODE_MAX          (1<<(CODE_MAX_BITS))

/* EOF_LZW_CODE = 256 */
#define START_LZW_CODE   ((EOF_LZW_CODE)+1)

#define output_code(a,b) put_nbits((a), (b))

typedef struct {
	char algorithm[3], 
	     binary_file;
	int N;
} file_stamp;

int bit_count =   9;  /* code size starts at 9 bits. */
int code_max  = 512;  /* start expanding the code size if we
                           already reached this value. */

void copyright( void );

void usage( void )
{
	fprintf(stderr, "\n Usage: lzwgt -n infile outfile\n");
	fprintf(stderr, "\n\n where n = bitsize of num codes added to CODE_MAX = %lu.", CODE_MAX);
	fprintf(stderr, "\n           string table resets after CODE_MAX+(1<<n) codes emitted.\n");
}

int main( int argc, char *argv[] )
{
	unsigned long in_file_len = 0, out_file_len = 0;
	float ratio = 0.0;
	int c = 0, i = 0, n, binary_file = 0, N = CODE_MAX;
	file_stamp fstamp;

	if ( argc != 4 ) {
		usage();
		copyright();
		exit (0);
	}
	
	if ( argv[1][0] == '-' && argv[1][1] != '\0' ) {
		n = atoi(&argv[1][1]);
		if ( n ) N += (1<<n);
	}
	else {
		usage();
		copyright();
		exit (0);
	}
		
	if ( (gIN = fopen( argv[2], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		return 0;
	}
	if ( (pOUT = fopen( argv[3], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		goto halt_prog;
	}
	init_buffer_sizes(8192);
	init_put_buffer();

	fprintf(stderr, "\n--[ A Lempel-Ziv-Welch (LZW) Implementation ]--");

	fprintf(stderr, "\n\nName of input file : %s", argv[2] );

	/* display file length. */
	fseek( gIN, 0, SEEK_END );
	in_file_len = ftell( gIN );
	fprintf(stderr, "\nLength of input file     = %15lu bytes", in_file_len );
	if ( in_file_len ==  0 ) {
		fprintf(stderr, "\nFile size = 0 bytes!" );
		goto halt_prog;
	}
	
	/* test for a binary file at the first 256-character block. */
	rewind(gIN);
	i = 0;
	while ( i++ < 256 ){ /* a la sixpack.c which uses 64 */
		if( (c=fgetc(gIN)) == EOF ) break;
		if ( c > 127 ) {
			binary_file = 1;
			break;
		}
	}
	
	/* Write the FILE STAMP. */
	rewind( pOUT );
	strcpy( fstamp.algorithm, "LZW" );
	fstamp.binary_file = binary_file;
	fstamp.N = N;
	fwrite( &fstamp, sizeof(file_stamp), 1, pOUT );
	
	/* start Compressing to output file. */
	fprintf(stderr, "\n\nCompressing...");
	
	/* make sure to rewind the input file */
	rewind(gIN);
	/* and initialize the input buffer. */
	init_get_buffer();
	
	/* allocate memory and initialize the code tables. */
	if ( !alloc_code_tables(CODE_MAX, LZW_COMPRESS) )
		goto halt_prog;
	init_code_tables(CODE_MAX, LZW_COMPRESS);
	
	/* start of codes to define. */
	lzw_code_cnt = START_LZW_CODE;
	
	/* get first character. */
	if ( (c=gfgetc()) != EOF ){
		prefix_string_code = c;	/* first prefix code. */
	}
	
	while( (c=gfgetc()) != EOF ) {
		if ( !lzw_search(prefix_string_code, c) ) {
			/* a different coding mode for a binary file for better compression. */
			if ( binary_file ){
				if( prefix_string_code < 256 ) {
					put_ONE();
					output_code ( (unsigned int) prefix_string_code, 8 );
				}
				else {
					put_ZERO();
					output_code ( (unsigned int) prefix_string_code, bit_count );
				}
			}
			else output_code ( (unsigned int) prefix_string_code, bit_count );
			
			/* ---- insert the string in the string table. ---- */
			if ( lzw_code_cnt < CODE_MAX ){
				lzw_comp_insert( prefix_string_code, c );
				if ( lzw_code_cnt == code_max ) {
					bit_count++;
					code_max <<= 1;
				}
			}
			
			/*  Instead of monitoring comp. ratio, we simply reset 
				the string table after N output codes. 
				No CLEAR_TABLE code is transmitted. */
			if ( lzw_code_cnt++ == N ) {
				init_code_tables(CODE_MAX, LZW_COMPRESS);
				lzw_code_cnt = START_LZW_CODE;
				bit_count =   9;
				code_max  = 512;
			}
			
			/* string = char */
			prefix_string_code = c;
		}
	}

	/* output last code. */
	if ( binary_file ){
		if( prefix_string_code < 256 ) {
			put_ONE();
			output_code ( (unsigned int) prefix_string_code, 8 );
		}
		else {
			put_ZERO();
			output_code ( (unsigned int) prefix_string_code, bit_count );
		}
	}
	else output_code ( (unsigned int) prefix_string_code, bit_count );

	/* output END-of-FILE code.*/
	if ( binary_file ) put_ZERO();
	output_code ( (unsigned int) EOF_LZW_CODE, bit_count );

	flush_put_buffer();
	fprintf(stderr, " complete.");

	/* get outfile's size and get compression ratio. */
	out_file_len = ftell( pOUT );

	fprintf(stderr, "\n\nName of output file: %s", argv[3] );
	fprintf(stderr, "\nLength of input file     = %15lu bytes", in_file_len );

	fprintf(stderr, "\nLength of output file    = %15lu bytes", out_file_len );
	ratio = (((float) in_file_len - (float) out_file_len)
						/ (float) in_file_len ) * (float) 100;

	fprintf(stderr, "\nCompression ratio:         %15.2f %%\n", ratio );

	halt_prog:

	free_put_buffer();
	free_get_buffer();
	free_code_tables();
	if ( gIN ) fclose( gIN );
	if ( pOUT ) fclose( pOUT );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n :: lzwgt file compressor, Gerald Tamayo, 2005\n");
}
