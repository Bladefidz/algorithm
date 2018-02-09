import edu.princeton.cs.algs4.SuffixArrayX;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdIn;

/**
 * Circular Suffix Array
 * Create suffix and sorted suffix array.
 *
 * Typical space		: Proportional to n + R
 * Construct Time		: Proportional to n log n
 * length() operation	: constant
 * index() operation	: constant
 */
public class CircularSuffixArray
{
	private int N;
	private int[] suffixIndex;

	// circular suffix array of s
	public CircularSuffixArray(String s)
	{
		if (s == null)
			throw new java.lang.IllegalArgumentException("Argument is null!");
		N = s.length();
		suffixIndex = new int[N];
		SuffixArrayX suffix = new SuffixArrayX(s);

		for (int i = 0; i < N; i++)
			suffixIndex[i] = suffix.index(i);
	}

	// length of s
	public int length()
	{
		return N;
	}

	// returns index of ith sorted suffix
	public int index(int i)
	{
		if (i > N)
			throw new java.lang.IllegalArgumentException("Index out range!");
		return suffixIndex[i];
	}

	// unit testing (required)
	public static void main(String[] args)
	{
		String s = StdIn.readAll().replaceAll("\n", " ").trim();
		CircularSuffixArray csa = new CircularSuffixArray(s);
		StdOut.println(csa.length());
		for (int i = 0; i < s.length(); i++)
			StdOut.println(csa.index(i));
	}
}
