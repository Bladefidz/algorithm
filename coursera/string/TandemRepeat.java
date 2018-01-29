import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Tandem Repeat
 *
 * Let
 * 	B is base string
 * 	S is string
 * Tandem copy is repeated B in S
 *
 * For example:
 * 	 S = "abcabcababcaba"
 * 	 B = "abcab"
 * 	 tandem copy = 2
 *
 * Goal: Linear time algorithm
 */
public class TandemRepeat
{
	private String base;
	private int[][] dfa;

	public TandemRepeat(String base)
	{  // Construct DFA from pattern
		this.base = base;
		int M = base.length();
		int R = 256;
		dfa = new int[R][M];
		dfa[base.charAt(0)][0] = 1;
		for (int X = 0, j = 1; j < M; j++) {
			// Compute dfa[][]
			for (int c = 0; c < R; c++)
				dfa[c][j] = dfa[c][X];
			dfa[base.charAt(j)][j] = j+1;
			X = dfa[base.charAt(j)][X];
		}
	}

	public int count(String txt)
	{  // Simulate operation of DFA on txt
		int cnt = 0;
		int i, j, N = txt.length(), M = base.length();
		for (i = 0, j = 0; i<N; i++) {
			j = dfa[txt.charAt(i)][j];
			if (j == M) {
				cnt += 1;
				j = 0;
			}
		}
		return cnt;
	}

	/**
	 * Will print:
	 * % java TandemRepeat AACAA AABRAACADABRAACAADABRA
	 * text: AABRAACADABRAACAADABRA
	 * pattern:          AACAA
	 */
	public static void main(String[] args)
	{
		String b = args[0];
		String s = args[1];
		TandemRepeat TandemRepeat = new TandemRepeat(b);
		StdOut.println(TandemRepeat.count(s));
	}
}