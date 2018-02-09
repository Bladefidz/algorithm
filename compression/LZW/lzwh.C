/*
	---- A Lempel-Ziv-Welch (LZW) Algorithm Implementation ----

	Filename: 		LZWH.C
	Description:	An LZW implementation.

	Decompression program:	LZWHD.C
	
	We reserve code 256 as the END-of-FILE code (EOF_LZW_CODE).

	Variable-length lzwcodes; code size starts at 9 bits, then
	adds more bits as necessary.
	
	Usage:
	
		lzwh -n inputfile outputfile

	where n is bitsize of integer (i.e., 1<<n) added to CODE_MAX 
	to obtain N=CODE_MAX+(1<<n). After N codes are transmitted, 
	we reset the string table.
	
	Gerald Tamayo, 2005/2009
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utypes.h"
#include "gtbitio.c"

#define CODE_MAX_BITS     16
#define CODE_MAX        (1<<CODE_MAX_BITS)

/* must be a prime number greater than CODE_MAX */
#if CODE_MAX_BITS == 12
	#define HASH_TABLE_SIZE   5021
#elif CODE_MAX_BITS == 13
	#define HASH_TABLE_SIZE   9859
#elif CODE_MAX_BITS == 14
	#define HASH_TABLE_SIZE  18041
#elif CODE_MAX_BITS == 15
	#define HASH_TABLE_SIZE  35023
#elif CODE_MAX_BITS == 16
	#define HASH_TABLE_SIZE  69001
#endif

#define HASH_SHIFT       (CODE_MAX_BITS-8)

#define LZW_NULL          -1
#define EOF_LZW_CODE     256
#define START_LZW_CODE   257

#define output_code(a,b) put_nbits((a), (b))

typedef struct {
	char algorithm[3], 
	     binary_file;
	int N;
} file_stamp;

int code[ HASH_TABLE_SIZE ];
int prefix[ HASH_TABLE_SIZE ];
unsigned char character[ HASH_TABLE_SIZE ];
int prefix_string_code = 0, lzw_code_cnt = 0;

int bit_count = 9;  /* code size starts at 9 bits. */
int code_max = 512; /* start expanding the code size if we
                         already reached this value. */
                         
void copyright( void );

/*
	initialize the codes with a value of LZW_NULL (-1),
	indicating that the hash table slot is open.
*/
void init_code_table( void )
{
	int i;

	for ( i = 0; i < HASH_TABLE_SIZE; i++ ) {
		code[ i ] = LZW_NULL;
	}
}

/*
	The insertion routine for the compressor, uses
	hashing to store the codes in the code tables.

	This function will always find an open slot, and
	when it does, it immediately exits the function.
*/
void insert_stringENC( int prefix_code, unsigned char c )
{
	int hindex;       /* the hashed index address. */
	int d;            /* the "displacement" to compute for the new index. */
	
	/* first probe is the hash function itself. */
	hindex = (c << HASH_SHIFT) ^ prefix_code;

	/* prepare for the second probe. */
	d = HASH_TABLE_SIZE - hindex;
	if ( hindex == 0 ) d = 1;

	do {
			/* available slot; store code here. */
			if ( code[ hindex ] == LZW_NULL ) {
				code[ hindex ] = lzw_code_cnt;
				prefix[ hindex ] = prefix_code;
				character[ hindex ] = c;
				return ;
			}
			/* otherwise, do a second probe. */
			if ( (hindex -= d ) < 0 )
				hindex += HASH_TABLE_SIZE;
	} while( 1 );
}

/*
	The decompression part does not actually need hashing,
	so just store the prefix codes and the append characters.
*/
void insert_stringDEC( int prefix_code, unsigned char c )
{
	prefix[ lzw_code_cnt ] = prefix_code;
	character[ lzw_code_cnt ] = c;
}

/*
	Search for the string composed of a prefix code
	and a character.
*/
int search_string( int prefix_code, unsigned char c )
{
	int hindex;       /* the hashed index address. */
	int d;            /* the "displacement" to compute for the new index. */
	
	hindex = (c << HASH_SHIFT) ^ prefix_code;

	if ( hindex == 0 ) d = 1;
	else d = HASH_TABLE_SIZE - hindex;
	
	do {
			/* empty slot; code pair not found. */
			if ( code[ hindex ] == LZW_NULL ) break;

			/* a code pair is stored here, so check it. */
			if (	prefix[ hindex ] == prefix_code
						&& character[ hindex ] == c ) { /* a match! */

				/* copy the code and return success. */
				prefix_string_code = code[ hindex ];
				return 1;
			}
			/* second probe; find another available slot. */
			if ( (hindex -= d) < 0 )
				hindex += HASH_TABLE_SIZE;
	} while( 1 );
	
	return 0;
}

void usage( void )
{
	fprintf(stderr, "\n Usage: lzwh -n infile outfile\n"
	              "\n\n where n = bitsize of num codes added to CODE_MAX = 65536."
	                "\n           string table resets after CODE_MAX+(1<<n) codes emitted.\n" );
}

int main( int argc, char *argv[] )
{
	unsigned long in_file_len = 0, out_file_len = 0;
	float ratio = 0.0;
	int c_eof = 0, i = 0, binary_file = 0, N = CODE_MAX;
	file_stamp fstamp;

	if ( argc != 4 ) {
		usage();
		copyright();
		exit (0);
	}
	
	if ( argv[1][0] == '-' && argv[1][1] != '\0' ) 
		i = (1<<(atoi(&argv[1][1])));
	else {
		usage();
		copyright();
		exit (0);
	}
	N += i;
	
	if ( (gIN = fopen( argv[2], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		return 0;
	}
	if ( (pOUT = fopen( argv[3], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		goto halt_prog;
	}
	init_put_buffer();
	
	fprintf(stderr, "\n--[ A Lempel-Ziv-Welch (LZW) Implementation ]--");
	fprintf(stderr, "\n\nName of input file : %s", argv[2] );

	/* display file length. */
	fseek( gIN, 0, SEEK_END );
	in_file_len = ftell( gIN );
	fprintf(stderr, "\nLength of input file     = %15lu bytes", in_file_len );
	
	/* test for a binary file at the first 256-character block. */
	rewind(gIN);
	i = 0;
	while( i++ < 256 ){ /* a la sixpack.c which uses 64 */
		if( (c_eof=fgetc(gIN)) == EOF ) break;
		if ( c_eof > 127 ) {
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
	fprintf(stderr, "\n\n Compressing...");

	/* make sure to rewind the input file */
	rewind(gIN);
	/* and initialize the input buffer. */
	init_get_buffer();

	/* initialize the LZW code table. */
	init_code_table();

	/* start of codes to define. */
	lzw_code_cnt = START_LZW_CODE;

	/* get first character. */
	if ( (c_eof=gfgetc()) != EOF ) {
		prefix_string_code = c_eof;	/* first prefix code. */
	}

	while( (c_eof=gfgetc()) != EOF ) {
		if ( !search_string(prefix_string_code, c_eof) ) {
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
				insert_stringENC( prefix_string_code, c_eof );
				if ( lzw_code_cnt == code_max ) {
					bit_count++;
					code_max <<= 1;
				}
			}
			
			/*  Instead of monitoring comp. ratio, we simply reset 
				the string table after N output codes. 
				No CLEAR_TABLE code is transmitted. */
			if ( lzw_code_cnt++ == N ) {
				init_code_table();
				lzw_code_cnt = START_LZW_CODE;
				bit_count =   9;
				code_max  = 512;
			}
			
			/* string = char */
			prefix_string_code = c_eof;
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

	fprintf(stderr, "\nCompression ratio:         %15.2f %%", ratio );
	copyright();

	halt_prog:

	free_put_buffer();
	free_get_buffer();
	if ( gIN ) fclose( gIN );
	if ( pOUT ) fclose( pOUT );

	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n :: Gerald Tamayo, 2005\n\n");
}
