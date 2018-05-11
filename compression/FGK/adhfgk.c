/*
	Filename:    ADHFGK.C
	Written by:  Gerald Tamayo
	Date:        2005
	
	Description:

	True Algorithm FGK; maintains a 0-node; is fast.
*/
#include "gtbitio.h"
#include "fgk.c"

void adh_init_first_node( int c );
void adh_encode_symbol ( int c );
int  adh_decode_symbol ( void );

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

	aNUMBER = ROOT_NODE_NUMBER; /* reset. */

	/* update the Huffman tree. */
	update_treeFGK( c );	/* pass the symbol. */
}

void adh_encode_symbol ( int c )
{
	/* encode the byte c. */
	if ( hufflist[c] ){   /* valid address; previously seen. */
		hcompress( hufflist[c] );
	}
	else {  /* a new byte! */
		/* output the zero node's code. */
		hcompress( zero_node );

		/* what follows is the actual byte. */
		put_nbits( c, hsymbol_bit_size );
	}

	/* recompute the tree if necessary. */
	update_treeFGK( c );  /* pass the symbol. */
}

int adh_decode_symbol ( void )
{
	int c;

	c = hdecompress( top );

	if ( c == ZERO_NODE_SYMBOL ) {
		/* get raw byte. */
		c = get_nbits( hsymbol_bit_size );
	}

	/* update the Huffman tree. */
	update_treeFGK( c );  /* pass the symbol. */

	return c;
}
