/*
	Filename:    ADHUF.C
	Written by:  Gerald Tamayo
	Date:        2005
	
	Description:

	Contains common definitions, objects and functions for
	Algorithm FGK and Algorithm Vitter; maintains a 0-node.
*/
#include "huf.h"

#define ZERO_NODE_SYMBOL  -2
#define INTERNAL_NODE     -1
#define ROOT_NODE_NUMBER  (H_MAX*2)

#define huffnode_t listnode_t

int hc;                        /* the "current" symbol. */
huffnode_t *zero_node = NULL;  /* the 0-node. */

/*
this array contains the node addresses
as indexed by their node numbers.
*/
huffnode_t *numbers[ H_MAX * 2 + 1 ];

/* the counter of numbers assigned so far. */
int aNUMBER = ROOT_NODE_NUMBER;

void create_new_zero_node( int c );
void swap_nodes( huffnode_t *a, huffnode_t *b );

/*
This function creates a new zero node from a current zero
node by creating two children nodes, one of which becomes
the new zero node.

The function accepts the new symbol and makes it the second
child (or "right" child) of the current zero node. It is the
first child (or "left" child) which becomes the new zero node.
*/
void create_new_zero_node( int c )
{
	/* create children and assign links. */
	zero_node->child_1 = create_node();
	zero_node->child_2 = create_node();

	zero_node->child_1->parent = zero_node;
	zero_node->child_2->parent = zero_node;

	/* now an internal node, reset. */
	zero_node->ch = INTERNAL_NODE;

	/* new symbol's node is child_2 or "right." */
	hufflist[c] = zero_node->child_2;
	hufflist[c]->ch = c;

	/* Number the nodes. */
	zero_node->number = aNUMBER--;
	hufflist[c]->number = aNUMBER--;

	/*
		Store them in the hash array indexed by
		the node numbers. Perfect hash again.
	*/
	numbers[ zero_node->number ] = zero_node;
	numbers[ hufflist[c]->number ] = hufflist[c];

	/* new zero node is child_1 or "left." */
	zero_node = zero_node->child_1;
	zero_node->ch = ZERO_NODE_SYMBOL;
}

/*
This function swaps two nodes by "re-pointing" the two
nodes' parent pointers, as well as the parent's children
pointers. Note that it is very careful when the two nodes
are siblings (same parent) and just re-arranges the parent's
children pointers.
*/
void swap_nodes( huffnode_t *a, huffnode_t *b )
{
	huffnode_t *parent, *temp = NULL;
	int i = 0;

	/* special swap if they have the same parent. */
	if ( a->parent == b->parent ) {
		parent = a->parent;
		if ( parent->child_1 == a ) {
			parent->child_1 = b;
			parent->child_2 = a;
		}
		else {
			parent->child_1 = a;
			parent->child_2 = b;
		}

		goto numbers_stuff;
	}

	if ( a->parent->child_1 == a ) {
		a->parent->child_1 = b;
	}
	else {
		a->parent->child_2 = b;
	}

	if ( b->parent->child_1 == b ) {
		b->parent->child_1 = a;
	}
	else {
		b->parent->child_2 = a;
	}

	parent = a->parent;
	a->parent = b->parent;
	b->parent = parent;

	numbers_stuff:

	/* Swap nodes in numbers[] array and their numbers. */
	temp = numbers[ a->number ];
	numbers[ a->number ] = numbers[ b->number ];
	numbers[ b->number ] = temp;

	i = a->number;
	a->number = b->number;
	b->number = i;
}
