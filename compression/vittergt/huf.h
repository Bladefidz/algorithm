/*
	Filename:   huf.h
	Author:     Gerald Tamayo
	Date:       June 22-24, 2004
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HUF_H
	#define HUF_H

#define H_MAX 256

typedef struct listnode *listp;
typedef struct listnode {
	int number;            /* used by Algorithm FGK and Vitter. */
	listp next;            /* for list processing convenience.  */
	unsigned long freq;
	signed int ch;         /* signed for handling symbols <= -1 */
	listp parent, child_1, child_2;
} listnode_t;

typedef struct{
	unsigned char b;
	unsigned long f;
} hfreq_type;

extern unsigned int hcount;
extern hfreq_type hfreq[];

extern listnode_t *list,    /* the list (current) pointer  */
	*top,                   /* the top of the Huffman tree */
	*list_head;             /* the list_head pointer       */

/* this is the list of symbol addresses in the binary/Huffman tree .*/
extern listp hufflist[];

extern listnode_t hnodes[];
extern unsigned int hn;

/*
the following are conveniently used
by the adaptive Huffman functions
*/
extern unsigned hmax_symbols;
extern int hmin;
extern unsigned int hsymbol_bit_size;

/* forward declarations */
listnode_t *create_node( void );
void insert ( listnode_t *node );
void init_hufflist( void );
void init_hfreq( void );
void create_symbol_list( void );
listnode_t *get_least_node ( void );
void create_hufftree( void );
void hcompress ( listnode_t *node );
int  hdecompress ( listnode_t *node );

/*
	the following are the functions needed
	to use the Huffman Code Table.
*/

#ifdef HCODE_TABLE

typedef struct code {
	unsigned int bit_cnt;
	/* anticipate an H_MAX-1 bit long symbol. */
	unsigned char bit[H_MAX/8];	/* 32 bytes. */
}	code_table_t;

extern code_table_t hcode_table[ ];

/* function declarations */
void get_hcode ( listnode_t *node );
void get_huffcodes ( void );
void huffcode_compress ( listnode_t *node );

#endif

#endif
