import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class RabinKarp
{
	private String pat;		// Only needed for Las Vegas
	private long patHash;
	private int M;
	private long Q;
	private int R = 256;
	private long RM;

	public RabinKarp(String pat)
	{
		this.pat = pat;
		this.M = pat.length();
		Q = longRandomPrime();
		RM = 1;
		for (int i = 1; i <= M; i++)
			RM = (R * RM) % Q;
		patHash = hash(pat, M);
	}

	// Monte carlo
	// public boolean check(int i)
	// {
	// 	return true;
	// }

	// Las Vegas
	public boolean check(String txt, int i)
	{
		for (int j = 0; j < M; j++)
			if (pat.charAt(j) != txt.charAt(i+j))
				return false;
		return true;
	}

	private long hash(String key, int M)
	{
		long h = 0;
		for (int j = 0; j < M; j++)
			h = (R * h + key.charAt(j)) % Q;
		return h;
	}

	private int search(String txt)
	{
		int N = txt.length();
		long txtHash = hash(txt, M);
		if (patHash == txtHash) return 0;
		for (int i = M; i < N; i++) {
			txtHash = (txtHash + Q - RM*txt.charAt(i-M) % Q) % Q;
			txtHash = (txtHash*R + txt.charAt(i)) % Q;

			int offset = i - M + 1;
			if (patHash == txtHash && check(txt, offset))
				return offset;  // Match
		}
		return N;  // No match found
	}
}