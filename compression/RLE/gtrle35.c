/*	Filename:     GTRLE35.C
	
	Description:  A Run-Length Encoding (RLE) Implementation.
	
	How it works:

		If there is a run of bytes, two bytes are output
		and then the next number of runs are encoded.

		Example:   abcddddd
		Encoding:  abcdd3	{regard '3' as a value.}

	Written by:   Gerald Tamayo
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copyright (void);
void rle_encode (FILE *in, FILE *out);
void rle_decode (FILE *in, FILE *out);

int main (int argc, char *argv[])
{
	FILE *in, *out;
	
	if (argc < 3 || argc > 4){
		fprintf(stderr, "\n Run Length Encoding (RLE) implementation.\n" 
			"\n Usage : gtrle inputfile outputfile [/d]");
		copyright();
		return 0;
	}
	
	in = fopen(argv[1], "rb");
	if (!in) {
		fprintf(stderr, "\n Error opening input file!");
		copyright();
		return 0;
	}
	out = fopen(argv[2], "wb");
	if (!out) {
		fprintf(stderr, "\n Error opening output file!");
		copyright();
		return 0;
	}

	if (argc == 4){
		if (!strcmp(argv[3], "/d") || !strcmp(argv[3], "/D")){
			fprintf(stderr, "\n Run-Length decoding...");
			rle_decode(in, out);
		}
		else {
			fprintf(stderr, "\n Error! Unknown: \"%s\"\n", argv[3]);
			return 0;
		}
	}
	else {
		fprintf(stderr, "\n Run-Length encoding...");
		rle_encode(in, out);
	}
	fprintf(stderr, "complete.\n");
	
	if (in ) fclose(in);
	if (out) fclose(out);
	return 0;
}

void rle_encode (FILE *in, FILE *out)
{
	int c, prev, rle_cnt = 0;

	/* get first byte and assign it as the *previous* byte. */
	if ((c=getc(in)) != EOF) {
		putc((unsigned char) c, out);
		prev = c;
	}
	else return;

	while ((c=getc(in)) != EOF) {
		if (c != prev) {
			/* if there's a run, encode it. */
			if (rle_cnt) {
				putc((unsigned char) prev, out);
				putc((unsigned char) rle_cnt-1, out);
				rle_cnt = 0;
			}

			/* then encode the new byte. */
			putc((unsigned char) c, out);
			prev = c;
		}
		else {
			/* increment count; if count == 256, quickly encode it. */
			if ((++rle_cnt) == 256) {
				/* the first byte of the 256 bytes. */
				putc((unsigned char) prev, out);

				/* the next 255 bytes. (rle_cnt-1) == 255. */
				putc((unsigned char) 255, out);
				rle_cnt = 0;
			}
		}
	}

	/* if there's a run, encode it. */
	if (rle_cnt) {
		putc((unsigned char) prev, out);
		putc((unsigned char) rle_cnt-1, out);
	}
}

void rle_decode (FILE *in, FILE *out)
{
	int c, prev, rle_cnt;

	/* get first byte and assign it as the *previous* byte. */
	if ((c=getc(in)) != EOF) {
		prev = c;
		putc((unsigned char) c, out);
	}
	else return;

	while ((c=getc(in)) != EOF) {
		if (c == prev) {
			putc((unsigned char) prev, out);
			/*	output the next "run" of bytes, as
				stored in the rle_cnt variable.
			*/
			if ((rle_cnt = getc(in)) != EOF) {
				while(rle_cnt--) {
					putc((unsigned char) prev, out);
				}
			}
			else break;
		}
		else {
			putc((unsigned char) c, out);
			prev = c;
		}
	}
}

void copyright (void)
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo\n");
}
