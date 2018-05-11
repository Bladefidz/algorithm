/*
	Filename:    FGK.C
	Written by:  Gerald Tamayo
	Date:        2005
	
	Description:

	TRUE Algorithm FGK; maintains a 0-node; is fast.

	The code here produces a *correct* Huffman Tree according
	to the Sibling Property, see J.S. Vitter, "Design and Analysis
	of Dynamic Huffman Codes," Journal of the Association for Computing
	Machinery, Vol. 34, No.4, October 1987, pp. 825-845.

	Note, however, that my only source for Algorithm FGK is purely
	Vitter's discussion of it in his paper on dynamic Huffman codes.

	Vitter proves that his method produces smaller files,
	but the implementation might be slower. Algorithm FGK is
	faster and produces compressed files of roughly the same
	size as that of Vitter's algorithm.

	Read Vitter's paper (contains discussion of Algorithm FGK);
	it is available for *free* on the web.
*/

#include "adhuf.c"

huffnode_t *get_highest_equal_node( huffnode_t *node );
huffnode_t *get_highest_equal_LEAF_FGK( huffnode_t *node );
huffnode_t *get_highest_equal_nodeFGK( huffnode_t *node );
void update_treeFGK( int c );

/*
This is a common function for Algorithm FGK
which is actually a pseudo-FGK.

The code here simply looks on the "uncle" node. True
Algorithm FGK looks on the "uncles" and *all* "cousins."

(I include it here for reference.)
*/
huffnode_t *get_highest_equal_node( huffnode_t *node )
{
	huffnode_t *highest = NULL;

	if ( node->parent->child_1 == node &&
			node->freq == node->parent->child_2->freq ) {
		highest = node->parent->child_2;
	}

	/* Verify parent's sibling or "uncle." */
	if (
		node->parent->parent &&
		node->parent->parent->child_1 == node->parent &&
		node->parent->parent->child_2 	/* uncle */
		&& node->freq == node->parent->parent->child_2->freq
		){
		highest = node->parent->parent->child_2;
	}
	else if (
		node->parent->parent &&
		node->parent->parent->child_2 == node->parent &&
		node->parent->parent->child_1	/* uncle */
		&& node->freq == node->parent->parent->child_1->freq
		){
		highest = node->parent->parent->child_1;
	}

	return highest;
}

/*
This function is used when the symbol node is the
sibling of the 0-node.  Separating this function
speeds up the get_highest_equal_nodeFGK() function,
which can then have only one conditional test.
*/
huffnode_t *get_highest_equal_LEAF_FGK( huffnode_t *node )
{
	huffnode_t *highest = NULL;
	int i;

	/* start from the next numbered node. */
	for ( i = node->number + 1; i < ROOT_NODE_NUMBER; i++ ) {
		if (	/* Equal count? */
					node->freq == numbers[i]->freq ) {
			if (
					/* "leaf" nodes only! */
					numbers[i]->ch != INTERNAL_NODE
				)
				highest = numbers[i];
		}
		else break;
	}
	return highest;
}

/*
This is the heart of Algorithm FGK; it finds the
highest node of equal count with the current node.

This is a TRUE FGK implementation of getting the
highest node.
*/
huffnode_t *get_highest_equal_nodeFGK( huffnode_t *node )
{
	huffnode_t *highest = NULL;
	int i;

	/* start from the next numbered node. */
	for ( i = node->number + 1; i < ROOT_NODE_NUMBER; i++ ) {
		/* Equal count? */
		if (	node->freq == numbers[i]->freq )
			highest = numbers[i];
		else break;
	}

	return highest;
}

/*
This code for Update is meant to be readable and follows the
pseudo-code for Algorithm FGK as described by J.S. Vitter in
his paper on Algorithm V.
*/
void update_treeFGK( int c )
{
	huffnode_t *highest = NULL, *node = hufflist[c];

	if ( node == NULL ) {
		create_new_zero_node( c );
		node = hufflist[ c ];	/* right child just created. */
	}

	/* if a sibling of the 0-node. */
	if ( node->parent == zero_node->parent ) {
		highest = get_highest_equal_LEAF_FGK( node );
		if ( highest ){
			swap_nodes( node, highest );
		}

		/* increment the node's count. */
		node->freq++;

		/* traverse up the tree. */
		node = node->parent;
	}

	while( node != top ) {
		highest = get_highest_equal_nodeFGK( node );
		if ( highest ){
			swap_nodes( node, highest );
		}

		/* increment the node's count. */
		node->freq++;

		/* traverse up the tree. */
		node = node->parent;
	}
}
