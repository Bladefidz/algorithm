/*
	Filename:	mtf.c
	Written by:	Gerald R. Tamayo
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtf.h"

int tSIZE = 0;
mtf_list_t *p = NULL, *head = NULL, *tail = NULL, *table = NULL;

int alloc_mtf( int tsize )
{
	if ( tsize <= 0 ) tsize = 256;
	
	table=(mtf_list_t *) calloc(sizeof(mtf_list_t), tSIZE=tsize);
	if ( !table ) {
		fprintf(stderr, "error alloc!");
		return 0;
	}
	init_mtf();
	return 1;
}

void init_mtf(void)
{
	int i;
	
	/* initialize the list. */
	for ( i = tSIZE-1; i >= 0; i-- ) {
		table[i].c = i;
		table[i].f = i+tSIZE+1;
		table[i].next = &table[i-1];
		table[i].prev = &table[i+1];
	}
	table[tSIZE-1].prev = NULL;
	table[0].next = NULL;
	head = &table[tSIZE-1];
	tail = &table[0];
}

void free_mtf_table( void )
{
	if ( table ) free( table );
}

int mtf( int c )
{
	int i = 0;
	
	/* find c. */
	p = head;
	while( p->c != c ) {
		i++;
		p = p->next;
	}
	/* move-to-front. */
	if ( p->prev ) {
		if ( p->next ) {
			p->prev->next = p->next;
			p->next->prev = p->prev;
		}
		else {
			p->prev->next = NULL;
			tail = p->prev;
		}
		p->prev = NULL;
		p->next = head;
		head->prev = p;
		head = p;
	} /* front, don't MTF! */
	
	return i;
}

int get_mtf_c( int i )
{
	/* find c. */
	p = head;
	while( i-- ) {
		p = p->next;
	}
	/* move-to-front. */
	if ( p->prev ) {
		if ( p->next ) {
			p->prev->next = p->next;
			p->next->prev = p->prev;
		}
		else {
			p->prev->next = NULL;
			tail = p->prev;
		}
		p->prev = NULL;
		p->next = head;
		head->prev = p;
		head = p;
	}
	return p->c;
}

int mtb( int c )
{
	int i = tSIZE-1;
	
	/* find c. */
	p = tail;
	while( p->c != c ) {
		i--;
		p = p->prev;
	}
	/* move-to-back. */
	if ( p->next ) {
		if ( p->prev ) {
			p->next->prev = p->prev;
			p->prev->next = p->next;
		}
		else {
			p->next->prev = NULL;
			head = p->next;
		}
		p->next = NULL;
		p->prev = tail;
		tail->next = p;
		tail = p;
	} /* back, don't MTF! */
	
	return i;
}
