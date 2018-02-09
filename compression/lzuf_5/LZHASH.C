/*
	Filename:   LZHASH.C
	Author:     Gerald Tamayo
	Date:       May 17, 2008
	
	The code uses a hashing function to generate indices into
	a hash table of "doubly-linked" lists.
*/
#include <stdio.h>
#include <stdlib.h>
#include "lzhash.h"

/* this is the *hash table* of listheads. */
int *lzhash = NULL;

/*
	these arrays contain the "previous" and "next" pointers
	of the virtual nodes.
*/
int *lzprev = NULL;
int *lznext = NULL;

/*
	allocate memory to the hash table and linked-list tables.
*/
int alloc_lzhash( int size )
{
	int i;

	lzhash = (int *) malloc( sizeof(int) * size );
	if ( !lzhash ) {
		fprintf(stderr, "\nError alloc: hash table.");
		return(0);
	}

	lzprev = (int *) malloc( sizeof(int) * size );
	if ( !lzprev ) {
		fprintf(stderr, "\nError alloc: prev table.");
		return(0);
	}
	lznext = (int *) malloc( sizeof(int) * size );
	if ( !lznext ) {
		fprintf(stderr, "\nError alloc: next table.");
		return(0);
	}

	/* initialize the listheads to LZ_NULL (-1). */
	for ( i = 0; i < size; i++ ) {
		lzhash[i] = LZ_NULL;
	}

	return 1;
}

void free_lzhash( void )
{
	if ( lzhash ) free( lzhash );
	if ( lzprev ) free( lzprev );
	if ( lznext ) free( lznext );
}

/* ---- inserts a node (position) into the hash list ---- */
void insert_lznode( int h, int i )
{
	int k = lzhash[h];

	/* always insert at the beginning. */
	lzhash[h] = i;
	lzprev[i] = LZ_NULL;
	lznext[i] = k;
	if ( k != LZ_NULL ) lzprev[k] = i;
	/* that's it! */
}

/* ---- deletes an LZ node (position) ---- */
void delete_lznode( int h, int i )
{
	if ( lzhash[h] == i ) { /* the head of the list? */
		/* the next node becomes the head of the list */
		lzhash[h] = lznext[i];
		if ( lzhash[h] != LZ_NULL )  /* 4/25/2008 */
			lzprev[ lzhash[h] ] = LZ_NULL;
	}
	else {
		lznext[ lzprev[i] ] = lznext[i];
		/* only if there is a node following node i, shall we assign to it. */
		if ( lznext[i] != LZ_NULL ) lzprev[ lznext[i] ]= lzprev[i];
	}
}
