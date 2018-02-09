This archive contains *basic* implementations of the following algorithms:

/*
(1)  Run-Length Encoding (RLE) (gtrle32.c and gtrle35.c);
(2)  Shannon-Fano Coding (gtsf.c and gtsfd.c);
(3)  Static Huffman Coding (gthc.c and gthd.c);
(4)  Dynamic Huffman coding-Algorithm FGK (fgkc.c and fgkd.c);
(5)  Dynamic Huffman coding II-Algorithm Vitter (vitc.c and vitd.c);
(6)  Run-Length Transform (RLT, a zero-order MTF) (gtrlt.c);
(7)  Move-To-Front Coding (MTF) (gtmtf.c);
(8)  Burrows-Wheeler Transform (BWT) (gtbwt5.c and gtbwtd5.c);
(9)  Lempel-Ziv algorithm (LZ77) (
	gtlz772.c gtlz772d.c  [basic LZ77/LZSS implementation];
	gtlz77hc.c gtlz77hd.c [basic LZ77/LZSS + Huffman];
	lzh.c and lzhd.c      [2-byte hash];
	lzhh.c and lzhhx.c    [2-byte hash plus Huffman] );
(10) Lempel-Ziv-Welch (LZW) 
	lzwh.c and lzwhd.c    [using LZC hashing]
	lzwgt.c and lzwgtd.c  [using a Binary Search Tree];

*/

(11)  PPP
	(praq, praq1, praq2, praq3) [PPP/LZP + MTF + variable-length codes].


Notes:

For personal, academic, and research purposes only. Freely distributable.

-- Gerald R. Tamayo, BSIE(Mapua I.T.)
   Philippines
