#include "huf.h"

void show_bitcode( listnode_t *node )
{
	int bit_cnt = 0;
	char bit [ H_MAX ];	/* a stack. */

	printf("\n%c : ", node->ch );
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
	while( bit_cnt-- ) {
		if ( bit[ bit_cnt ] ) {
			printf("1");
		}
		else {
			printf("0");
		}
	}
}

/*
static  Huffman:	show_bitcodes( NULL );
dynamic Huffman:	show_bitcodes( zero_node );
*/
void show_bitcodes( listnode_t *node )
{
	int c;

	for ( c = 0; c < 256; c++ ){
		if ( hufflist[c] ) show_bitcode( hufflist[c] );
	}
	if ( node ) show_bitcode( node );
}
