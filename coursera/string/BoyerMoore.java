import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

class BoyerMoore
{
	private int[] right;
	private String pat;

	BoyerMoore(String pat)
	{  // Computer skip table
		this.pat = pat;
		int M = pat.length();
		int R = 256;
		right = new int[R];
		for (int c=0; c<R; c++)
			right[c] = -1;
		for (int j=0; j<M; j++)
			right[pat.charAt(j)] = j;
	}

	public int search(String txt)
	{  // Search for pattern in txt
		int N = txt.length();
		int M = pat.length();
		int skip;
		for (int i=0; i<=N-M; i+=skip) {
			skip = 0;
			for (int j=M-1; j>=0; j--) {
				if (pat.charAt(j) != txt.charAt(i+j)) {
					skip = j-right[txt.charAt(i+j)];
					if (skip<1) skip = 1;
					break;
				}
			}
			if (skip == 0) return i;  // Found
		}
		return N;					  // Not found
	}

	/**
	 * Will print:
	 * % java BoyerMoore AACAA AABRAACADABRAACAADABRA
	 * text: AABRAACADABRAACAADABRA
	 * pattern:          AACAA
	 */
	public static void main(String[] args)
	{
		String pat = args[0];
		String txt = args[1];
		BoyerMoore boyerMoore = new BoyerMoore(pat);
		StdOut.println("text:    " + txt);
		int offset = boyerMoore.search(txt);
		StdOut.print("pattern: ");
		for (int i = 0; i < offset; i++)
			StdOut.print(" ");
		StdOut.println(pat);
	}
}