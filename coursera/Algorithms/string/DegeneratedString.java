import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Alternate implementation of brute-force substring search (explicit backup)
 */
public class DegeneratedString
{
	public static int search(String pat, String txt)
	{
		int j, M = pat.length();
		int i, N = txt.length();
		for (i = 0, j = 0; i<N && j<M; i++) {
			StdOut.println(i + " " + j);
			if (txt.charAt(i) == pat.charAt(j)) j++;
			else { i -= j; j = 0; }
		}
		StdOut.println("--" + j);
		if (j == M) return i - M;
		else		return N;
	}

	public static void main(String[] args)
	{
		StdOut.println(DegeneratedString.search("AAAAB", "AAAAAAAAAB"));
	}
}