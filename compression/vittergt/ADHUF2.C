/*
	Filename:    ADHUF2.C
	Written by:  Gerald Tamayo
	Date:        2008
	
	Description:

	Contains common definitions, objects and functions for
	Algorithm FGK and Algorithm Vitter; maintains a 0-node.
*/
#include "hufmodel.h"

#define huffnode_t listnode_t

void create_new_zero_node( huff_model *h, int c );
void swap_nodes( huff_model *h, huffnode_t *a, huffnode_t *b );

/*
This function creates a new zero node from a current zero
node by creating two children nodes, one of which becomes
the new zero node.

The function accepts the new symbol and makes it the second
child (or "right" child) of the current zero node. It is the
first child (or "left" child) which becomes the new zero node.
*/
void create_new_zero_node( huff_model *h, int c )
{
	/* create children and assign links. */
	h->zero_node->child_1 = create_node(h);
	h->zero_node->child_2 = create_node(h);

	h->zero_node->child_1->parent = h->zero_node;
	h->zero_node->child_2->parent = h->zero_node;

	/* now an internal node, reset. */
	h->zero_node->ch = INTERNAL_NODE;

	/* new symbol's node is child_2 or "right." */
	h->hufflist[c] = h->zero_node->child_2;
	h->hufflist[c]->ch = c;

	/* Number the nodes. */
	h->zero_node->number = h->aNUMBER--;
	h->hufflist[c]->number = h->aNUMBER--;

	/*
		Store them in the hash array indexed by
		the node numbers. Perfect hash again.
	*/
	h->numbers[ h->zero_node->number ] = h->zero_node;
	h->numbers[ h->hufflist[c]->number ] = h->hufflist[c];

	/* new zero node is child_1 or "left." */
	h->zero_node = h->zero_node->child_1;
	h->zero_node->ch = ZERO_NODE_SYMBOL;
}

/*
This function swaps two nodes by "re-pointing" the two
nodes' parent pointers, as well as the parent's children
pointers. Note that it is very careful when the two nodes
are siblings (same parent) and just re-arranges the parent's
children pointers.
*/
void swap_nodes( huff_model *h, huffnode_t *a, huffnode_t *b )
{
	huffnode_t *parent, *temp;
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
	temp = h->numbers[ a->number ];
	h->numbers[ a->number ] = h->numbers[ b->number ];
	h->numbers[ b->number ] = temp;

	i = a->number;
	a->number = b->number;
	b->number = i;
}
