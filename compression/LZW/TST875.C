#include <stdio.h>
#include <stdlib.h>
#include "tst875.h"

/*
	BYTE-based Ternary Search Tree (TST) routines

	---- for Lempel-Ziv-Welch (LZW) implementation. ----

	- A good discussion of TSTs can be found in Jon Bentley and Robert
		Sedgewick's April 1998 DDJ article: "Ternary Search Trees".
		(See also Robert Sedgewick's "Algorithms in C," Third Edition)

		Their example implementation (in the article) is for C strings;
		this one is completely *byte-based* and offers a non-recursive
		insert function. Note that the strings are actually signed
		integers to use EOS (end-of-string = -1) as the NULLdigit.

	Written by: Gerald Tamayo, 2004

	- modified Oct. 15, 2008.
*/

/* Allocate for root statically. */
node_t root = { -1, NULL, NULL, NULL, NULL };

int lzw_code_cnt = 0, lzw_curr_code = 0;

/*
	The array of pointers to the last node of every
	string. This last node is the NULLdigit EOS.
*/
node_p lzw_list [ LZW_TST_MAX ];

unsigned int tst_BUFSIZE = TST_MAX_BUFSIZE;
unsigned int tst_n = 0;
node_p tst_buf = NULL, buf_array[ TSTBUF_ARRAY_SIZE ];
unsigned int bn = 0;

/*
	This function creates a node by "getting" a node address
	from the current dynamically-allocated buffer of nodes.
*/
node_p create_tst_node( void )
{
	node_p node;

	if ( !tst_buf ) return NULL;
	node = &tst_buf[ tst_n++ ];
	if ( tst_n == tst_BUFSIZE ) {
		tst_buf = NULL;
		tst_n = 0;
		tst_BUFSIZE = TST_MAX_BUFSIZE;
		while( !buf_array[ bn ] ) {
			buf_array[bn] = (node_p) malloc( sizeof(node_t) * tst_BUFSIZE );
			if ( !buf_array[bn] ) tst_BUFSIZE--;
			else {
				tst_buf = buf_array[ bn++ ];
				break;
			}
			if ( tst_BUFSIZE == 0 ) break;
		}
	}
	return node;
}

/*
	This routine initializes the current buffer of nodes.
	When all the nodes are used up, a new blocksize is
	allocated	and its address is stored in the next item
	in the buffer array. I use an array of buffer pointers
	so that we can deallocate the whole array later.
*/
node_p init_tst_buf( void )
{
	tst_buf = NULL;
	tst_n = 0;
	tst_BUFSIZE = TST_MAX_BUFSIZE;
	while( !buf_array[ bn ] ) {
		buf_array[ bn ] = (node_p) malloc( sizeof(node_t) * tst_BUFSIZE );
		if ( !buf_array[ bn ] ) tst_BUFSIZE--;
		else {
			tst_buf = buf_array[ bn++ ];
			break;
		}
		if ( tst_BUFSIZE == 0 ) return NULL;
	}
	return tst_buf;
}

/*
	The buffer array is convenient in completely
	deallocating the dynamically allocated nodes.
*/
void destroy_tst_buffer( void )
{
	int i;
	for ( i = 0; i < bn; i++ ) {
		if ( buf_array[i] ) free( buf_array[i] );
	}
}

/*
	A familiar *recursive* TST search function.
*/
int tst_search( node_p p, int *s )
{
	if ( !p ) return 0;

	if ( *s == p->b ) {
		if ( *s == EOS ) {
			/* the code must have been stored
				here as a node_p. cast it to int. */
			lzw_curr_code = (int) p->center;
			return 1;
		}
		else {
			return tst_search( p->center, ++s );
		}
	}
	else if ( *s < p->b ) {
		return tst_search( p->left, s );
	}
	else {
		return tst_search( p->right, s );
	}
}

/*
	A *NON-recursize* TST search routine; is faster,
	and is very important for LZW.
*/
int nr_tst_search ( node_p p, int *s )
{
	while( p ) {
		if ( *s == p->b ) {
			if ( *s == EOS ){
				/* the code must have been stored
					here as a node_p. cast it to int. */
				lzw_curr_code = (int) p->center;
				return 1;
			}
			p = p->center;
			++s;
		}
		else if ( *s < p->b ) {
			p = p->left;
		}
		else {
			p = p->right;
		}
	}
	return 0;
}

/*
	A usual recursive TST insert routine. Because the root
	is statically-allocated, you dont have to assign the
	first node as root.
*/
node_p tst_insert( node_p parent, node_p p, int *s )
{
	if ( p == NULL ) {
		p = (node_p) create_tst_node();
		if ( !p ) {
			fprintf(stderr, "TST memory allocation error!");
			return p;
		}
		p->b = *s;
		p->left = p->center = p->right = NULL;
		p->parent = parent;
	}

	if ( *s == p->b ) {
		if ( *s == EOS ){
			/* store this NULL_digit node address a la Huffman. */
			lzw_list[ lzw_code_cnt ] = p;

			/* store the code value in the "center" pointer. */
			p->center = (node_p) lzw_code_cnt;
			return p;
		}
		else {
			p->center = tst_insert( p, p->center, ++s );
		}
	}
	else if ( *s < p->b ){
		p->left = tst_insert( p, p->left, s );
	}
	else {
		p->right = tst_insert( p, p->right, s );
	}
	return p;
}

/*
	A *NON-recursive* TST insert function is a little faster.
*/
int nr_tst_insert( node_p p, int *s )
{
	node_p parent = NULL, *pt = NULL;

	for ( ;; ){
		if ( p == NULL ) {
			p = (node_p) create_tst_node();
			if ( !p ) {
				fprintf(stderr, "TST memory allocation error!");
				return 0;
			}
			p->b = *s;
			p->left = p->center = p->right = NULL;

			/*
				Another of the many advantages of a statically-allocated
				root: you don't have to test for pt as NULL or assign the
				first allocated node as root (root = p).
			*/
			p->parent = parent;
			*pt = p;
		}

		if ( *s == p->b ) {
			if ( *s == EOS ){
				/* store this NULL_digit node address a la Huffman. */
				lzw_list[ lzw_code_cnt ] = p;

				/* store the code value in the "center" pointer. */
				p->center = (node_p) lzw_code_cnt;
				return 1;
			}
			else {
				pt = &p->center;
				++s;
			}
		}
		else if ( *s < p->b ){
			pt = &p->left;
		}
		else {
			pt = &p->right;
		}

		/* do assignments here. */
		parent = p;
		p = *pt;
	}
}
