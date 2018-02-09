#include <stdio.h>
#include <stdlib.h>

/*
	---- LZW string search and insert functions. ----

	Written by:  Gerald Tamayo
*/
#define LZW_COMPRESS       1
#define LZW_DECOMPRESS     0
#define EOF_LZW_CODE     256
#define LZW_NULL         EOF_LZW_CODE

extern unsigned char    *code_char;
extern unsigned int     *code_prefix;

extern unsigned int     *bt_code;
extern unsigned int     *left;
extern unsigned int     *right;
extern unsigned int     prefix_string_code, lzw_code_cnt;

/* ---- LZW search and insert functions. ---- */
int alloc_code_tables ( unsigned int size, int lzw_mode );
void init_code_tables ( unsigned int size, int lzw_mode  );
void free_code_tables( void );

/* Binary-tree search. */
int lzw_search( unsigned int prefix, unsigned char c );

/* Binary-tree insertion scheme. */
int lzw_comp_insert( unsigned int prefix, unsigned char c );
int lzw_decomp_insert( unsigned int prefix, unsigned char c );
