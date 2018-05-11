/*
	Filename:    ADHVIT.C
	Written by:  Gerald Tamayo, 2008
	
	Description:

	Algorithm Vitter; maintains a 0-node; is fast.
*/
#include "gtbitio.h"
#include "vith.c"

void adh_init_first_node( huff_model *h, int c );
void adh_encode_symbol ( huff_model *h, int c );
int adh_decode_symbol ( huff_model *h  );

void adh_init_first_node( huff_model *h, int c )
{
	/* make sure all the symbol node addresses are NULL. */
	init_hufflist( h );
	
	/*
	create first 0-node and quickly becomes
	the root of the tree.
	*/
	h->top = h->zero_node = create_node( h );

	h->aNUMBER = ROOT_NODE_NUMBER; /* reset. */

	/* update the Huffman tree. */
	update_treeVITTER( h, c );	/* pass the symbol. */
}

void adh_encode_symbol ( huff_model *h, int c )
{
	/* encode the byte c. */
	if ( h->hufflist[c] ){   /* valid address; previously seen. */
		hcompress( h->hufflist[c] );
	}
	else {	/* a new byte! */
		/* output the zero node's code. */
		hcompress( h->zero_node );

		/* what follows is the actual byte. */
		put_nbits( c, h->hsymbol_bit_size );
	}

	/* recompute the tree if necessary. */
	update_treeVITTER( h, c );  /* pass the symbol. */
}

int adh_decode_symbol ( huff_model *h )
{
	int c;

	c = hdecompress( h->top );

	if ( c == ZERO_NODE_SYMBOL ) {
		/* get raw byte. */
		c = get_nbits( h->hsymbol_bit_size );
	}

	/* update the Huffman tree. */
	update_treeVITTER( h, c );	/* pass the symbol. */

	return c;
}
