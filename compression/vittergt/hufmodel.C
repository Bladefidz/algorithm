/*
	---->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	A Huffman Encoding Implementation

	Filename:  hufmodel.c
	Author:    Gerald Tamayo, 1/21/2008
	
	NOTE:

		Bit Assignments:   Left/child_1 = 0; Right/child_2 = 1.

	---->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gtbitio.h"
#include "hufmodel.h"

void init_huff_model( int type, huff_model *h, int max_sym, int bit_size )
{
	h->hcount = 0;
	h->top = h->list = h->list_head = NULL;
	h->hmax_symbols = max_sym;
	h->hsymbol_bit_size = bit_size;
	h->hn = 0;

	h->hfreq = (hfreq_type *) malloc( sizeof(hfreq_type) * max_sym );
	if ( !h->hfreq ) {
		fprintf( stderr, "Alloc error! Huff freq table." );
		exit(0);
	}

	h->hufflist = (listp *) malloc( sizeof(listp) * max_sym );
	if ( !h->hufflist ) {
		fprintf( stderr, "Alloc error! Huffman nodes address table." );
		exit(0);
	}

	h->hnodes = (listnode_t *) malloc( sizeof(listnode_t) * (max_sym * 2 + 1) );
	if ( !h->hnodes ) {
		fprintf( stderr, "Alloc error! Huffman nodes address table." );
		exit(0);
	}

	if ( type == DYNAMIC_HUFFMAN ){
		h->numbers = (listp *) malloc( sizeof(listp) * (max_sym * 2 + 1) );
		if ( !h->hnodes ) {
			fprintf( stderr, "Alloc error! Huffman nodes numbers table." );
			exit(0);
		}
		h->zero_node = NULL;
		h->aNUMBER = ROOT_NODE_NUMBER;
	}
}

void free_huff_model( int type, huff_model *h )
{
	if ( h->hfreq )    free( h->hfreq );
	if ( h->hufflist ) free( h->hufflist );
	if ( h->hnodes )    free( h->hnodes );
	if ( type == DYNAMIC_HUFFMAN ){
		if ( h->numbers ) free( h->numbers );
	}
}

listnode_t *create_node( huff_model *h )
{
	listnode_t *node;

	if ( h->hn >= (h->hmax_symbols * 2 + 1) ) return NULL;
	node = &h->hnodes[ h->hn++ ];   /* get an address. */

	node->number = 0;
	node->freq = (unsigned long ) 0;
	node->next = NULL;

	node->parent = NULL;
	node->child_1 = NULL;
	node->child_2 = NULL;
	node->ch = -1;

	return node;            /* pass the address. */
}

/* inserts the node to list */
void insert ( huff_model *h, listnode_t *node )
{
	listnode_t *curr, *prev = NULL;

	curr = h->list;
	if ( h->list ) {
		while( curr && node->freq > curr->freq ) {
			prev = curr;
			curr = curr->next;
		}

		/* when node->freq is < and becomes the start of list */
		if ( prev == NULL && curr ) {
			h->list = node;
		}
		else {
			prev->next = node;
		}

		node->next = curr;
	}
	else {
		if ( h->list_head == NULL ) h->list_head = node;
		h->list = node;
	}
}

void init_hufflist( huff_model *h )
{
	unsigned int i;

	for ( i = 0; i < h->hmax_symbols; i++ ) {
		h->hufflist[ i ] = NULL;
	}
}

void init_hfreq( huff_model *h )
{
	int i;

	for ( i = 0; i < h->hmax_symbols; i++ ){
		h->hfreq[i].f = (unsigned long) 0;
		h->hfreq[i].b = (unsigned char) i;
	}
}

void create_symbol_list( huff_model *h )
{
	unsigned int i = 0;
	listnode_t *node;

	/* creates the initial *SORTED* list of symbols. */
	h->hcount = 0;
	for ( i = 0; i < h->hmax_symbols; i++ ) {
		if ( h->hfreq[i].f > 0 ) {
			node = create_node( h );
			if ( node ) {
				node->freq = h->hfreq[i].f;
				node->ch = h->hfreq[i].b;
				insert( h, node );
				h->hufflist[ node->ch ] = node;
			}
			else {
				fprintf(stderr, "\nMemory allocation error!!!\n");
				exit ( 0 );
			}
			h->hcount++;	/* record the count of symbols. */
		}
	}
}

listnode_t *get_least_node ( huff_model *h )
{
	listnode_t *node;

	node = h->list;	/* get the least-freq node */
	if ( h->list ){	/* if a node is still available */
		h->list = h->list->next;	/* just move to the next node. */
	}
	return node;
}

void create_hufftree( huff_model *h )
{
	listnode_t *node1, *node2, *parent;

	/* make sure all the node addresses are NULL. */
	init_hufflist( h );

	create_symbol_list( h );	/* create the initial list of symbols. */

	/*
	then create the Huffman tree,
	as long as there are nodes in the list.
	*/
	while( h->list ) {
		/* get the first least-freq node. */
		node1 = get_least_node( h );
		if ( h->list ) {
			/* a node is still available in the list. */
			/* assign the second least-freq node. */
			node2 = get_least_node( h );

			/* create the parent node. */
			parent = create_node( h );

			/* create the links. */
			node1->parent = parent;
			node2->parent = parent;
			parent->child_1 = node1;
			parent->child_2 = node2;

			/* add frequency counts. */
			parent->freq = node1->freq + node2->freq;

			/* two nodes ignored, insert the new parent node. */
			insert ( h, parent );
		}
		else {
			h->top = node1;
			if ( node1 == h->list_head ) {
				h->top = create_node( h );
				h->top->child_1 = node1;
				h->top->child_2 = node1;
				node1->parent = h->top;
			}
			break;
		}
	}
}

/* this is the one used by the compression program. */
void hcompress ( listnode_t *node )
{
	int bit_cnt = 0;
	char bit [ H_MAX ];	/* a stack. */

	while( node ) {
		if ( node->parent ) {
			if ( node == node->parent->child_1 ) {
				bit[ bit_cnt ] = 0;	/* push. */
			}
			else if ( node == node->parent->child_2 ) {
				bit[ bit_cnt ] = 1;	/* push. */
			}
			bit_cnt++;	/* increment count. */
			node = node->parent;
		}
		else break;
	}
	/* now, output the bits in reverse for easy decompression. */
	while( bit_cnt-- ) {     /* decrement count and pop. */
		if ( bit[ bit_cnt ] ) {
			put_ONE();            /* output a ONE (1) bit. */
		}
		else {
			put_ZERO();           /* output a ZERO (0) bit. */
		}
	}
}

int hdecompress( listnode_t *node )
{
	/*
	for faster processing, only one test would suffice;
	and make it child_1 because it is the first child pointer
	we assign to when there is only one symbol.
	*/
	while( node->child_1 && node->child_2 ) {
		if ( get_bit() ) {
			node = node->child_2;
		}
		else {
			node = node->child_1;
		}
	}
	return node->ch;
}
