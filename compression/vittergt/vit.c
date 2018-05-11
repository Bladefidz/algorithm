/*
	Filename:  		VIT.C
	Written by:		Gerald Tamayo, 2005
	
	Description:

	Algorithm Vitter; maintains a 0-node; is fast.

	The code here produces a *correct* Huffman Tree according to
	the Sibling Property, see J.S. Vitter, "Design and Analysis
	of Dynamic Huffman Codes," Journal of the Association for Computing
	Machinery, Vol. 34, No.4, October 1987, pp. 825-845.

	Vitter proves that his method produces smaller files, but
	the implementation might be slower. Algorithm FGK is faster
	and produces compressed files of roughly the same size as
	that of Vitter's algorithm.

	Read Vitter's paper; it is available for *free* on the web.
*/

#include "adhuf.c"

void update_treeVITTER ( int c );
huffnode_t *get_block_leader_VITTER( huffnode_t *node );
void slide_node_VITTER( huffnode_t *node );
void SlideAndIncrement_VITTER( huffnode_t **node );
void slide_node( huffnode_t *node );
void SlideAndIncrement( huffnode_t **node );

/*
This function simply returns the leader of the block.
*/
huffnode_t *get_block_leader_VITTER( huffnode_t *node )
{
	huffnode_t *highest = NULL;
	int i;

	/* start from the next numbered node. */
	for ( i = node->number + 1; i < ROOT_NODE_NUMBER; i++ ) {
		if (	/* Equal count? */
					node->freq == numbers[i]->freq ) {
			if (
					/*
						must not be the node's parent,
						if the node is the sibling of
						the zero node.

						if not the sibling, then the node's
						count and its parent's count will
						never be equal.
					*/
					numbers[i] != node->parent
					)
					/* if a leaf, do not swap with an internal node. */
					if ( !(node->ch > INTERNAL_NODE
								 && numbers[i]->ch == INTERNAL_NODE) ){
						highest = numbers[i];
					}
					else break;
		}
		else break;
	}

	return highest;
}


/*
The following functions create codes that exactly match
Vitter's examples in his paper, the strings: "abc", "abcd",
"abcc" and "abacabdabaceabacabdfg".
*/

/*
This is the "equivalent" slide operation of Algorithm Vitter.
This function quickly does a *swap* to simulate the "slide" operation.

The "slide" operation of Vitter really moves the nodes in the list.
Since I already started with an array with my Algorithm FGK
implementation, I simply do a continuous swap of the nodes
to "simulate" the "floating tree" slide operation.

The main difference with FGK is that after you have incremented
the node's frequency (i.e., its weight w becomes w+1), and if the
node is an "internal" node, you still have to move the node into
the next block--the one with all nodes of weight (w+1).
*/
void slide_node_VITTER( huffnode_t *node )
{
	unsigned long freq;
	int i;

	/* this handles leaf or internal blocks. */
	if ( node->ch == INTERNAL_NODE ) freq = node->freq+1;	/* internal node? */
	else freq = node->freq;	/* leaf node. */

	/* start from the next numbered node. */
	for ( i = node->number + 1; i < ROOT_NODE_NUMBER; i++ ) {
		/* greater than (>) test satisfies only for an internal node. */
		if ( freq >= numbers[i]->freq ){
			if ( numbers[i] != node->parent ){
				/*
				This test exactly follows Vitter's illustration (of an internal node
				of weight 8 "sliding" into a block of leaves of weight 9) that the
				current (internal) node simply becomes the first one in the block
				of internal nodes of weight 9.  This indeed improves compression
				ratio than promoting the said internal node as the highest-numbered
				node (i.e., the leader) in the block of internal nodes of weight 9.
				*/
				if ( node->ch == INTERNAL_NODE
								&& numbers[i]->ch == INTERNAL_NODE
								&& freq == numbers[i]->freq )
					break;
				else swap_nodes( node, numbers[i] );	/* quick swap. */
			}
		}
		else break;
	}
}

/* Exactly follows Vitter's pseudo-code. */
void SlideAndIncrement_VITTER( huffnode_t **node )
{
	huffnode_t *temp;

	temp = (*node)->parent;
	slide_node_VITTER( *node );

	/* increment weight. */
	(*node)->freq++;

	if ( (*node)->ch > INTERNAL_NODE ) /* a leaf? */
		*node = (*node)->parent;
	else *node = temp;	/* former parent of node. */
}

/* ---- The New Update function of Algorithm Vitter ---- */

/*
The important difference of Algorithm V (as others would call it)
is that there are times that you do a "block slide" and at other
times you only have to swap two nodes in the list.

This implementation is meant to be *readable* and shows the effect
of Vitter's algorithm: it is better than Algorithm FGK.
*/
void update_treeVITTER ( int c )
{
	huffnode_t *leader = NULL,   /* the block leader. */

	*increment = NULL,            /* the leaf to increment. */
	*node = hufflist[ c ];        /* Vitter's "q" is "node" here. */

	if ( node == NULL ) {	/* have we just received a new symbol? */
		/*
		replace q by an internal 0-node with two leaf 0-node children
		such that the right child corresponds to the "new symbol."
		*/
		create_new_zero_node( c );

		/*
		The "internal 0-node just created" mentioned by Vitter is the
		"previous" 0-node (which "just became" an **internal** node because
		it is now the *parent* of the new 0-node and the new symbol!).

		Remember that the 0-node is always a "leaf."
		*/
		node = zero_node->parent;

		/*
		strictly Vitter's, "the right child of q" is the new symbol.
		*/
		increment = node->child_2;	/* or: increment = hufflist[c]; */

		/* ---- Variant No. 1 ---- */
		/* increment = node; */	/* the same internal node! */
	}
	else {
		/* interchange q in the tree with the leader of its block. */
		leader = get_block_leader_VITTER( node );
		if ( leader ){
			swap_nodes( node, leader );
		}

		/* if the node is the sibling of the 0-node. */
		if ( node->parent == zero_node->parent ){
			increment = node;
			node = node->parent;
		}
	}

	while( node != top ) {
		/* main loop; q must be the leader of its block. */
		SlideAndIncrement_VITTER( &node );
	}

	if ( increment != NULL ) {
		SlideAndIncrement_VITTER( &increment );
	}
}

/* ---- The following functions create better encodings ---- */
/* ---- (if coupled with Variant No. 1.) ---- */

void slide_node( huffnode_t *node )
{
	int i;

	/* start from the next numbered node. */
	for ( i = node->number + 1; i < ROOT_NODE_NUMBER; i++ ) {
		if ( node->freq == numbers[i]->freq ) {
			if ( numbers[i] != node->parent ){
				/* NOTE: if the node is an internal node, then during the
				re-slide operation, this completely promotes the node
				as the leader of its block. */
				swap_nodes( node, numbers[i] );	/* quick swap. */
			}
		}
		else break;
	}
}

/*
This "slide" operation completely promotes the node as the leader
of its block.
*/
void SlideAndIncrement( huffnode_t **node )
{
	huffnode_t *temp;

	/* the equivalent "slide" operation. */
	slide_node( *node );

	/* increment weight. */
	(*node)->freq++;

	/* still must be higher than the block of leaves. */
	if ( (*node)->ch == INTERNAL_NODE ){ /* internal node? */
		temp = (*node)->parent;
		slide_node( *node ); /* promote as leader of its block. */
	}

	if ( (*node)->ch > INTERNAL_NODE ) /* a leaf? */
		*node = (*node)->parent;
	else *node = temp;	/* former parent of node. */
}
