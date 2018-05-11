/*
	---->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	A Shannon-Fano Encoding Implementation

	Filename:   sf.c
	Author:     Gerald Tamayo
	Date:       May 31, 2005
	
	NOTES:
	
	- Bit Assignments: Left/child_1 = 0; Right/child_2 = 1.
	
	- This library uses two (2) recursive functions:
		create_shannon_fano_tree() and sfcompress().

	---->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gtbitio.h"
#include "sf.h"

unsigned int sfcount = 0;
sffreq_type sffreq[ SF_MAX ];

listnode_t *list = NULL,   /* the list_head (current) pointer */
	*top = NULL;           /* the top of the Shannon-Fano tree */

/* this is the list of symbol addresses in the binary tree .*/
listp sflist[ SF_MAX ];	/* just an array of pointers. */

/* static allocation is faster. */
listnode_t sfnodes[ SF_MAX * 3 ];   /* a symbol will always have an extra node. */
unsigned int sfn = 0;

listnode_t *create_node( void )
{
	listnode_t *node;

	if ( sfn >= (SF_MAX * 3) ) return NULL;
	node = &sfnodes[ sfn++ ];	/* get a symbol node address. */

	node->freq = (unsigned long ) 0;
	node->next = NULL;

	node->parent = NULL;
	node->child_1 = NULL;
	node->child_2 = NULL;
	node->ch = -1;

	return node;	/* pass the symbol node address. */
}

/*
	inserts the node to list.

	Note that there are no extra tests after the while loop;
	using Igor Kolpakov's technique of fast insertion
	in a singly-linked list (Dr. Dobb's Journal/June/1999/p.128).
*/
void insert( listnode_t **list, listnode_t *node )
{
	listnode_t *curr;

	curr = *list;
	/* sorted from HIGHEST to LOWEST. */
	while( curr && node->freq < curr->freq ) {
		list = &curr->next;
		curr = curr->next;
	}
	*list = node;
	node->next = curr;
}

void init_sflist( void )
{
	unsigned int i;

	for ( i = 0; i < SF_MAX; i++ ) {
		sflist[ (unsigned char) i ] = NULL;
	}
}

void init_sffreq( void )
{
	int i;

	for ( i = 0; i < SF_MAX; i++ ){
		sffreq[i].f = (unsigned long) 0;
		sffreq[i].b = (unsigned char) i;
	}
}

/* creates the initial *SORTED* list of symbols. */
void create_symbol_list( void )
{
	unsigned int i = 0;
	listnode_t *node;

	sfcount = 0;
	for ( i = 0; i < SF_MAX; i++ ) {
		if ( sffreq[i].f > 0 ) {
			node = create_node();
			if ( node ) {
				node->freq = sffreq[i].f;
				node->ch = sffreq[i].b;
				insert( &list, node );
				sflist[ node->ch ] = node;
			}
			else {
				fprintf(stderr, "\nMemory allocation error!!!\n");
				exit(0);
			}
			sfcount++;	/* record the count of symbols. */
		}
	}
}

/*
	A RECURSIVE function that distributes a node's set/list
	of symbol nodes to its two children nodes.
*/
void create_shannon_fano_tree( listnode_t *node )
{
	listnode_t *d, *temp;  /* d = node to distribute. */

	/* create the two children nodes. */
	node->child_1 = create_node();
	node->child_1->parent = node;

	node->child_2 = create_node();
	node->child_2->parent = node;

	/*
		---- Distribute symbol nodes ----

		The list of symbol nodes starts at node's "next" pointer;

		Similarly, distribute these symbol nodes in the children
		nodes' "next" pointer according to their weights.
	*/

	d = node->next;   /* first node to distribute. */
	while ( d ) {
		temp = d->next;  /* remember next node to distribute. */
		if ( node->child_1->freq <= node->child_2->freq ) {
			insert( &(node->child_1->next), d );
			node->child_1->freq += d->freq;  /* add d's freq. */
		}
		else {
			insert( &(node->child_2->next), d );
			node->child_2->freq += d->freq;
		}
		d = temp;   /* next node to process. */
	}

	/*
		Do recursion now for the two lists.

		If there is only one node stored in child's list
		(i.e., "node->next"), then no need to process
		recursively; just connect the symbol node to this
		parent node.
	*/

	/* process child_1. */
	if ( node->child_1->next ) {
		if ( node->child_1->next->next ) {
			create_shannon_fano_tree( node->child_1 );
		}
		else {
    	node->child_1->next->parent = node;
			node->child_1 = node->child_1->next;
		}
	}
	/* process child_2. */
	if ( node->child_2->next ) {
		if ( node->child_2->next->next ) {
			create_shannon_fano_tree( node->child_2 );
		}
		else {
			node->child_2->next->parent = node;
			node->child_2 = node->child_2->next;
		}
	}
}

/*
Encodes a symbol by accepting its node. We use "recursion"
here just to be different than the Huffman code; the symbol
node's parent would suffice as an argument passed to the
recursive call inside the function.
*/
void sfcompress( listnode_t *node )
{
	if ( !node ) return;

	if ( node->parent ) {
		/* recurse immediately to the node's parent. */
		sfcompress( node->parent );

		/*
		Transmission of bits is done here for correct
		bit code output;
		Due to recursion, the first bit processed (from the
		symbol node) will be the last to be transmitted.
		*/
		if ( node->parent->child_1 == node ) {
			put_ZERO();
		}
		else if ( node->parent->child_2 == node ) {
			put_ONE();
		}
	}
}

int sfdecompress( listnode_t *node )
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
