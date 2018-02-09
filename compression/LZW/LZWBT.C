/*
	---- LZW string search and insert functions. ----

	Written by:      Gerald R. Tamayo, BSIE
	
	July  7, 2008  - improved the allocation function for better main() exit.
	               - Revised the binary-tree data structure for more-efficient memory use.
	Oct. 19, 2008  - 65536 lzw codes.
	April 14, 2010 - modified init_code_tables, lzw_search().
*/
#include <stdio.h>
#include <stdlib.h>
#include "lzwbt.h"

/* code_char[i] = character of code i */
unsigned char  *code_char;

/* code_prefix[i] = prefix of code i */
unsigned int   *code_prefix = NULL;

/* ---- the Binary-Tree data structure ---- */

/* bt_code[i] = first LZW code with prefix i */
unsigned int            *bt_code = NULL;  /* the "top" of the binary tree. */
unsigned int            *left = NULL;
unsigned int            *right = NULL;
unsigned int            prefix_string_code = 0, lzw_code_cnt = 0;

/* ---- LZW search and insert operations. ---- */

/* allocate memory to the code tables. */
int alloc_code_tables ( unsigned int size, int lzw_mode )
{
	code_char = (unsigned char *) calloc(size, sizeof(unsigned char));
	if ( !code_char ) {
		fprintf( stderr, "\n error alloc: code_chat.");
		return 0;
	}
	if ( lzw_mode == LZW_COMPRESS ){
		bt_code = (unsigned int *) calloc( size, sizeof(int) );
		if ( !bt_code ) {
			fprintf( stderr, "\n error alloc: bt_code.");
			return 0;
		}
		left = (unsigned int *) calloc( size, sizeof(int) );
		if ( !left ) {
			fprintf( stderr, "\n error alloc: left.");
			return 0;
		}
		right = (unsigned int *) calloc( size, sizeof(int) );
		if ( !right ) {
			fprintf( stderr, "\n error alloc: right.");
			return 0;
		}
	}
	if ( lzw_mode == LZW_DECOMPRESS ){
		code_prefix = (unsigned int *) calloc( size, sizeof(int) );
		if ( !code_prefix ) {
			fprintf( stderr, "\n error alloc: code_prefix.");
			return 0;
		}
	}
	
	return 1;
}

void init_code_tables ( unsigned int size, int lzw_mode )
{
	unsigned int i;
	
	prefix_string_code = 0, lzw_code_cnt = 0;
	
	if ( lzw_mode == LZW_COMPRESS ){
		for ( i = 0; i < size; i++ ) {
			bt_code[ i ] = LZW_NULL;
			left[ i ]  = LZW_NULL;
			right[ i ] = LZW_NULL;
		}
	}
	if ( lzw_mode == LZW_DECOMPRESS ){
		for ( i = 0; i < size; i++ ) {
			code_prefix[ i ] = LZW_NULL;
		}
	}
}

void free_code_tables( void )
{
	if ( bt_code ) free( bt_code );
	if ( left ) free( left );
	if ( right ) free( right );
	if ( code_prefix ) free( code_prefix );
}

/* Binary-tree search. */
int lzw_search( unsigned int prefix, unsigned char c )
{
	unsigned int code = bt_code[ prefix ];
	
	while( code != LZW_NULL ) {
		if ( c == code_char[ code ] ) {
			prefix_string_code = code;
			return 1;
		}
		else if ( c > code_char[ code ] )
			code = right[code];
		else code = left[code];
	}
	
	return 0;
}

/* Binary-tree insertion scheme. */
int lzw_comp_insert( unsigned int prefix, unsigned char c )
{
	int code = bt_code[prefix];

	if ( code != LZW_NULL ) {
		while ( 1 ) {
			if ( c < code_char[ code ] )	{
				if ( left[code] != LZW_NULL ) code = left[code];
				else {
					left[code] = lzw_code_cnt; /* insert code. */
					break;
				}
			}
			else {
				if ( right[code] != LZW_NULL ) code = right[code];
				else {
					right[code] = lzw_code_cnt; /* insert code. */
					break;
				}
			}
		}
	}
	else bt_code[ prefix ] = lzw_code_cnt;

	code_char[ lzw_code_cnt ] = c;

	return 1;
}

/* for the decompressor. */
int lzw_decomp_insert( unsigned int prefix, unsigned char c )
{
	code_prefix  [ lzw_code_cnt ] = prefix;
	code_char    [ lzw_code_cnt ] = c;
	return 1;
}
