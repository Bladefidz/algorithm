/*
	Filename:   hufmodel.h
	Author:     Gerald Tamayo, 1/21/2008
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HUF_H
	#define HUF_H

#define H_MAX 256
#define STATIC_HUFFMAN   1
#define DYNAMIC_HUFFMAN  0

#define ZERO_NODE_SYMBOL  -2
#define INTERNAL_NODE     -1
#define ROOT_NODE_NUMBER  (H_MAX*2)

typedef struct listnode *listp;
typedef struct listnode {
	int number;     /* used by Algorithms FGK & Vitter. */
	signed int ch;  /* signed for handling symbols <= -1 */
	unsigned long freq;
	listp next;     /* for list processing convenience. */
	listp parent, child_1, child_2;
} listnode_t;

typedef struct {
	unsigned char b;
	unsigned long f;
} hfreq_type;

/* one struct so that the dynamic Huffman can have two or more models. */
typedef struct {
	unsigned int hcount,
					 hn, aNUMBER,
					 hmax_symbols,
					 hsymbol_bit_size;
	hfreq_type *hfreq;
	listnode_t *list,          /* the list (current) pointer  */
				  *top,           /* the top of the Huffman tree */
				  *list_head,     /* the list_head pointer       */
				  *hnodes,
				  *zero_node;     /* used by Algorithms FGK & Vitter. */
	listp      *hufflist,
				  *numbers;       /* used by Algorithms FGK & Vitter. */
} huff_model;

/* --- forward declarations. ---- */
void init_huff_model( int type, huff_model *h, int max_sym, int bit_size );
void free_huff_model( int type, huff_model *h );
listnode_t *create_node( huff_model *h );
void insert ( huff_model *h, listnode_t *node );
void init_hufflist( huff_model *h );
void init_hfreq( huff_model *h );
void create_symbol_list( huff_model *h );
listnode_t *get_least_node ( huff_model *h );
void create_hufftree( huff_model *h );
void hcompress ( listnode_t *node );
int  hdecompress ( listnode_t *node );

#endif
