/*
	Filename:    ADHVITT.C
	Written by:  Gerald Tamayo, 2005
	
	Description:

	Algorithm VITTER; maintains a 0-node; is moderately fast.
*/
#include "gtbitio.h"
#include "vit.c"

void adh_init_first_node( int c );
void adh_encode_symbol ( int c );
int adh_decode_symbol ( void );

void adh_encode_symbol ( int c )
{
	/* encode the byte c. */
	if ( hufflist[c] ){   /* valid address; previously seen. */
		hcompress( hufflist[ c ] );
	}
	else {	/* a new byte! */
		/* output the zero node's code. */
		hcompress( zero_node );

		/* 	what follows is the actual byte. */
		put_nbits( c, hsymbol_bit_size );
	}

	/* recompute the tree if necessary. */
	update_treeVITTER( c );
}

int adh_decode_symbol ( void )
{
	int c;

	c = hdecompress( top );

	if ( c == ZERO_NODE_SYMBOL ) {
		/* get raw byte. */
		c = get_nbits( hsymbol_bit_size );
	}

	/*
	update the Huffman tree.
	*/
	update_treeVITTER( c );

	return c;
}

void adh_init_first_node( int c )
{
	/* make sure all the symbol node
			addresses are NULL. */
	init_hufflist();
	
	/*
	create first 0-node and quickly becomes
	the root of the tree.
	*/
	top = zero_node = create_node();

	aNUMBER = ROOT_NODE_NUMBER;	/* reset. */

	/* update the Huffman tree. */
	update_treeVITTER( c );
}
