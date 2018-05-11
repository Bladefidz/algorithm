#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SF_H
	#define SF_H

#define SF_MAX 256

typedef struct listnode *listp;
typedef struct listnode {
	listp next;            /* for list processing convenience. */
	unsigned long freq;
	signed int ch;         /* signed for handling symbols <= - 1*/
	listp parent, child_1, child_2;
} listnode_t;

typedef struct {
	unsigned char b;
	unsigned long f;
} sffreq_type;

extern unsigned int sfcount;
extern sffreq_type sffreq[];

extern listnode_t *list, *top;
extern listp sflist[];
extern listnode_t sfnodes[];
extern unsigned int sfn;

/* ---- forward declarations ---- */
listnode_t *create_node( void );
void insert( listnode_t **list, listnode_t *node );
void init_sflist( void );
void init_sffreq( void );
void create_symbol_list( void );
void create_shannon_fano_tree( listnode_t *node );
void sfcompress ( listnode_t *node );
int  sfdecompress ( listnode_t *node );

#endif
