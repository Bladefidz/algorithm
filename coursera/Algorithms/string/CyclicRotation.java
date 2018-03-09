import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Cyclic rotation of a string
 *
 * T is text
 * S is substring
 * S is cyclic rotation of T if:
 * 	S.length() == T.length()
 * 	S consist of a suffix of T
 * 		followed by a prefix of T
 *
 * For example:
 * "winterbreak" is a cyclic rotation of "breakwinter" (and vice versa)
 *
 * Goal: Linear time algorithm to find suffix and prefix
 */
public class CyclicRotation
{
	private String s;
	private int[][] dfa;

	public CyclicRotation(String s)
	{  // Construct DFA from pattern
		this.s = s;
		int M = s.length();
		int R = 256;
		dfa = new int[R][M];
		dfa[s.charAt(0)][0] = 1;
		for (int X = 0, j = 1; j < M; j++) {
			// Compute dfa[][]
			for (int c = 0; c < R; c++)
				dfa[c][j] = dfa[c][X];
			dfa[s.charAt(j)][j] = j+1;
			X = dfa[s.charAt(j)][X];
		}
	}

	public boolean cyclicTo(String t)
	{  // Simulate operation of DFA on txt
		int i, j, N = t.length(), M = s.length();

		if (N != M) return false;

		// Compute suffix last position
		for (i = 0, j = 0; i<N; i++) {
			j = dfa[t.charAt(i)][j];
		}
		// StdOut.println(N-j);

		// Compute prefix
		int k = N-j;
		for (i = 0; i<k; i++) {
			// StdOut.println(t.charAt(i));
			j = dfa[t.charAt(i)][j];
		}
		// StdOut.println(j);
		if (j == M)	return true;  	// Found
		else		return false; 		// Not found
	}

	public static void main(String[] args)
	{
		String s = args[0];
		String t = args[1];
		CyclicRotation cr = new CyclicRotation(s);
		StdOut.println(cr.cyclicTo(t));
	}
}