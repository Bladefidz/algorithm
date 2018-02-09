#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#if !defined( GTBITIO_H )
	#define GTBITIO_H

/* for get_nbits() and put_nbits().

INT_BIT is the number of bits in an 
integer: sizeof(int) * 8. (16,32,64...)

you can "get" and "put" at most INT_BIT
number of bits.
*/
#if !defined( INT_BIT )
	#if INT_MAX == 0x7fff
		#define INT_BIT 16
	#elif INT_MAX == 0x7fffffff
		#define INT_BIT 32
	#else
		#define INT_BIT (8*sizeof(int))
	#endif
#endif

#define pset_bit() *pbuf |= (1<<p_cnt)

/* ---- writes a ONE (1) bit. ---- */
#define put_ONE() { pset_bit(); advance_buf(); }

/* ---- writes a ZERO (0) bit. ---- */
#define put_ZERO() advance_buf()

/* just increment the pbuf buffer for faster processing. */
#define advance_buf()		\
{                          \
	if ( (++p_cnt) == 8 ) { \
		p_cnt = 0; \
		if ( (++pbuf_count) == pBUFSIZE ){ \
			pbuf = pbuf_start; \
			fwrite( pbuf, pBUFSIZE, 1, pOUT ); \
			memset( pbuf, 0, pBUFSIZE ); \
			pbuf_count = 0; \
			nbytes_out += pBUFSIZE; \
		} \
		else pbuf++; \
	} \
}

extern FILE *gIN, *pOUT;
extern unsigned int pBUFSIZE, gBUFSIZE;
extern unsigned char *pbuf, *pbuf_start, p_cnt;
extern unsigned char *gbuf, *gbuf_start, *gbuf_end, g_cnt;
extern unsigned int pbuf_count, nfread;
extern unsigned int bit_read, nbits_read;
extern unsigned long nbytes_out;

void init_buffer_sizes( unsigned int size );
void init_put_buffer( void );
void init_get_buffer( void );
void free_put_buffer( void );
void free_get_buffer( void );
void flush_put_buffer( void );
int  get_bit( void );
int  gfgetc( void );
void pfputc( int c );
unsigned int get_nbits( int size );
void put_nbits( unsigned int k, int size );
int get_symbol( int size );
unsigned long get_nbytes_out( void );

#endif
