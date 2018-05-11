/*
	---->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	A Huffman Encoding Implementation

	Filename:   huf.c
	Author:     Gerald Tamayo
	Date:       June 22-24, 2004
	
	NOTES:
		the code table takes time to initialize, so it
		is not used here but included for reference.

		Also, the nodes are statically buffered;
		The sorted list allows the least-frequent nodes
		to be easily fetched when a node needs to be created
		and not deliberately searched in the hnode array.

		Bit Assignments:   Left/child_1 = 0; Right/child_2 = 1.

	---->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gtbitio.h"
#include "huf.h"

unsigned int hcount = 0;
hfreq_type hfreq[ H_MAX ];

listnode_t *list = NULL,   /* the list (current) pointer  */
	*top = NULL,           /* the top of the Huffman tree */
	*list_head = NULL;     /* the list_head pointer       */

/* this is the list of symbol addresses in the binary/Huffman tree .*/
listp hufflist[ H_MAX ];   /* just an array of pointers.  */

/*
Static allocation is faster;
extra two nodes used by Dynamic Huffman (Algorithm FGK & Vitter).
*/
listnode_t hnodes[ H_MAX * 2 + 1 ]; /* the buffer of Huffman nodes. */
unsigned int hn = 0;                /* this buffer's counter. */

/*
the following are conveniently used
by the adaptive Huffman functions
*/
unsigned hmax_symbols = H_MAX;
int hmin = -1;
/* default: log_2(H_MAX) = 8 */
unsigned int hsymbol_bit_size = 8;

listnode_t *create_node( void )
{
	listnode_t *node;

	if ( hn >= (H_MAX * 2 + 1) ) return NULL;
	node = &hnodes[ hn++ ];   /* get an address. */

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
void insert ( listnode_t *node )
{
	listnode_t *curr, *prev = NULL;

	curr = list;
	if ( list ) {
		while( curr && node->freq > curr->freq ) {
			prev = curr;
			curr = curr->next;
		}

		/* when node->freq is < and becomes the start of list */
		if ( prev == NULL && curr ) list = node;
		else prev->next = node;
		
		node->next = curr;
	}
	else {
		if ( list_head == NULL ) list_head = node;
		list = node;
	}
}

void init_hufflist( void )
{
	int i = 0;
	
	for ( ; i < hmax_symbols; ++i ) {
		hufflist[ i ] = NULL;
	}
}

void init_hfreq( void )
{
	int i = 0;
	
	for ( ; i < hmax_symbols; ++i ) {
		hfreq[i].f = (unsigned long) 0;
		hfreq[i].b = (unsigned char) i;
	}
}

void create_symbol_list( void )
{
	unsigned int i = 0;
	listnode_t *node;

	/* creates the initial *SORTED* list of symbols. */
	hcount = 0;
	for ( ; i < hmax_symbols; ++i ) {
		if ( hfreq[i].f > 0 ) {
			node = create_node();
			if ( node ) {
				node->freq = hfreq[i].f;
				node->ch = hfreq[i].b;
				insert(node);
				hufflist[ node->ch ] = node;
			}
			else {
				fprintf(stderr, "\nMemory allocation error!!!\n");
				exit(0);
			}
			hcount++;	/* record the count of symbols. */
		}
	}
}

listnode_t *get_least_node ( void )
{
	listnode_t *node;

	node = list;            /* get the least-freq node. */
	if ( list ){
		list = list->next;  /* move to next node.       */
	}
	return node;
}

void create_hufftree( void )
{
	listnode_t *node1, *node2, *parent;

	/* make sure all the node addresses are NULL. */
	init_hufflist();

	create_symbol_list();  /* create the initial list of symbols. */
	
	/*
	then create the Huffman tree,
	as long as there are nodes in the list.
	*/
	while( list ) {
		/* get the first least-freq node. */
		node1 = get_least_node();
		if ( list ) {
			/*
			a node is still available in the list.
			assign the second least-freq node.
			*/
			node2 = get_least_node();
			
			/* create the parent node. */
			parent = create_node();

			/* create the links. */
			node1->parent = parent;
			node2->parent = parent;
			parent->child_1 = node1;
			parent->child_2 = node2;

			/* add frequency counts. */
			parent->freq = node1->freq + node2->freq;

			/* two nodes ignored, insert the new parent node. */
			insert ( parent );
		}
		else {
			top = node1;
			if ( node1 == list_head ) {
				top = create_node();
				top->child_1 = node1;
				top->child_2 = node1;
				node1->parent = top;
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

	if ( !node ) return;

	while( node->parent ) {
		if ( node == node->parent->child_1 ) {
			bit[ bit_cnt ] = 0;	/* push. */
		}
		else if ( node == node->parent->child_2 ) {
			bit[ bit_cnt ] = 1;	/* push. */
		}
		bit_cnt++;	/* increment count. */
		node = node->parent;
	}

	/* now, output the bits in reverse for easy decompression. */
	while( bit_cnt-- ) {          /* decrement count and pop. */
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

#ifdef HCODE_TABLE

code_table_t hcode_table[ H_MAX ];

void get_hcode ( listnode_t *node )
{
	code_table_t code;
	signed int c;

	if ( !node ) return;

	/* traversal for this symbol to get its code is done just this once. */
	c = node->ch;  /* save node's symbol. */

	memset( code.bit, 0, (hmax_symbols >> 3) );
	code.bit_cnt = 0;

	/* use a non-recursive traversal of the huffman tree instead. */
	while( node->parent ) {
		if ( node == node->parent->child_2 ) {
			code.bit[code.bit_cnt >> 3] |= (1 << (code.bit_cnt & 7));
		}
		else if ( node == node->parent->child_1 ) {
			code.bit[code.bit_cnt >> 3] &= ~(1 << (code.bit_cnt & 7));
		}
		code.bit_cnt++;
		node = node->parent;
	}

	/* update the Huffman code table. */
	hcode_table[ c ] = code;
}

/* a one-time traversal of the tree to get the huffman codes. */
void get_huffcodes( void )
{
	int i = 0;

	for ( ; i < hmax_symbols; ++i ) {
		if ( hfreq[i].f > 0 ){
			get_hcode( hufflist[i] );
		}
	}
}

/* encodes a symbol; accepts a pointer to the symbol's node. */
void huffcode_compress ( listnode_t *node )
{
	code_table_t code;

	if ( !node ) return;

	/* get the huffman code. */
	code = hcode_table[ node->ch ];

	/* NOTE: no traversal of the entire tree! */

	/* now output the bits in reverse for easy decompression. */
	while( code.bit_cnt-- ) {
		if ( code.bit[code.bit_cnt >> 3] & (1 << (code.bit_cnt & 7)) ) {
			put_ONE();   /* output a ONE (1) bit. */
		}
		else {
			put_ZERO();    /* output a ZERO (0) bit. */
		}
	}
}

#endif
