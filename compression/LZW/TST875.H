#include <stdio.h>
#include <stdlib.h>

/*
	BYTE-based Ternary Search Tree (TST) routines

	Written by: Gerald Tamayo, 2004
*/
#if !defined( TST875_H )
	#define TST875_H

typedef struct node_t *node_p;
typedef struct node_t {
	int b;                /* signed int to allow use of
                                   EOS = -1 as NULLdigit. */
	node_p left, center, right;
	node_p parent;        /* a la Huffman encoding. */
} node_t;

extern node_t root;
extern int lzw_code_cnt, lzw_curr_code;

/* the array of pointers to each string. */
extern node_p lzw_list[];

#define EOS -1
#define LZW_TST_MAX      4096      /* number of codes/strings */
#define TST_MAX_BUFSIZE  2048
#define TSTBUF_ARRAY_SIZE  32

extern unsigned int tst_BUFSIZE;
extern unsigned int tst_n;
extern node_p tst_buf, buf_array[];
extern unsigned int bn;

node_p create_tst_node( void );
node_p init_tst_buf( void );
void   destroy_tst_buffer( void );
int    tst_search( node_p p, int *s );
int    nr_tst_search ( node_p p, int *s );
node_p tst_insert( node_p parent, node_p p, int *s );
int    nr_tst_insert( node_p p, int *s );

#endif
