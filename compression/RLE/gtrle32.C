/*	Filename:     GTRLE32.C
	
	Description:  A *text-only* encoder.
	
	              Encodes the run of bytes in the next 128 values of a byte
	              (text-file byte values: 0..127).  If a byte has a run, we 
	              turn on its 8th bit, or bit-7, to signify to the decoder 
	              that what follows is a run length count.
	Written by:   Gerald Tamayo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copyright(void);
void rle_encode (FILE *in, FILE *out, char *ofn);
void rle_decode (FILE *in, FILE *out);
void msg_exit(FILE *out, char *s);

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
		return 0;
	}
	out = fopen(argv[2], "wb");
	if (!out) {
		fprintf(stderr, "\n Error opening output file!");
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
		rle_encode(in, out, argv[2]);
	}
	fprintf(stderr, "complete.\n");
	
	if (in ) fclose(in);
	if (out) fclose(out);
	return 0;
}

void rle_encode (FILE *in, FILE *out, char *ofn)
{
	int curr, rle_cnt = 0, prev = 0;

	if ((curr=getc(in)) != EOF) {
		if (curr > 127) msg_exit(in, ofn);
		prev = curr;
	}
	else return;
	while ((curr=getc(in)) != EOF) {
		if (curr > 127) msg_exit(out, ofn);
		if (curr == prev) {
			/* turn on prev's bit 128; write prev. */
			putc((unsigned char) (prev | 128), out);
			
			/* if rle count == 255, break. */
			for (rle_cnt = 1; rle_cnt < 255;) {
				if (((curr=getc(in)) != EOF)) {
					if (curr > 127) msg_exit(out, ofn);
					if (curr == prev) ++rle_cnt;
					else break;
				}
				else break;
			}

			/* write rle count. */
			putc((unsigned char) rle_cnt, out);
			if (rle_cnt == 255) curr = getc(in);
		}
		else putc((unsigned char) prev, out);
		prev = curr;
	}

	/* flush prev. */
	if (prev != EOF) putc((unsigned char) prev, out);
}

void rle_decode (FILE *in, FILE *out)
{
	int curr, rle_cnt = 0;

	while ((curr=getc(in)) != EOF) {
		/* write curr if no runs. */
		if (curr < 128) putc(curr, out);
		else {               /* there's a run! */
			curr -= 128;		/* set off bit 128. */

			/* output the character. */
			putc((unsigned char) curr, out);

			/* output the run of bytes. */
			if ((rle_cnt=getc(in)) != EOF) {
				for(; rle_cnt > 0; rle_cnt--){
					putc((unsigned char) curr, out);
				}
			}
		}
	}
}

void msg_exit(FILE *out, char *s)
{
	fprintf(stderr, "\n\n Error: This program compresses only *text* files.\n");
	fclose(out);
	remove(s);
	exit(0);
}

void copyright(void)
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo\n");
}
