import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.StdRandom;

/**
 * Given an array a of n 64-bit integers and a target value T,
 * determine whether there are two distinct integers i and j such that
 * ai+aj=T. Your algorithm should run in linear time in the worst case
 *
 * Solution:
 * Radix sort the array in linear time. Scan a pointer i from left to right
 * and a pointer j from right to left: consider a[i] + a[j]. If it is bigger
 * than T, advance the j pointer; if it is smaller than T, advance the i
 * pointer; if it is equal to T, we have found the desired indices.
 * Note that the array of integers can be radix sorted in linear time and
 * constant extra space using the advanced radix sorting algorithm of
 * Franceschini, Muthukrishnan, and Patrascu.
 */
public class TwoSum
{
	private static final int BITS_PER_BYTE = 8;
	private Stack<long[]> matchedPairs;

	public TwoSum(long[] a, long T)
	{
		this.matchedPairs = new Stack<>();
		radix8Bit(a);

		StdOut.println("Sorted:");
		for (int i = 0; i < a.length; i++)
			StdOut.print(a[i] + " ");
		StdOut.println();

		// Check for possible pairs
		int j = a.length-1;
		for (int i = 0; i < a.length; i++)
		{
			if (i >= j) break;

			long ii = a[i];
			long jj = a[j];
			long sum = ii + jj;
			if (sum == T)
			{
				long[] pair = new long[2];
				pair[0] = a[i]; pair[1] = a[j];
				this.matchedPairs.push(pair);
				while(a[i] == ii && i < a.length) ++i;
				while(a[j] == jj && j > 0) --j;
			}
			else if (sum < T)
			{
				while(a[i] == ii && i < a.length) ++i;
			}
			else if (sum > T)
			{
				while(a[j] == jj && j > 0) --j;
			}
		}
	}

	/**
	 * Radix sort an array of 64 bits by partitioning into 8 bits segmentations
	 * @param a Array of 64 bits number
	 */
	private void radix8Bit(long[] a)
	{
		final int BITS = 64;				// Each integer is 64 bits
		final int R = 1 << BITS_PER_BYTE;	// Each bytes is between 0 and 255
		final int MASK = R - 1;				// 0xFF
		final int w = BITS / BITS_PER_BYTE;	// Each integer is 8 bytes

		int n = a.length;
		long[] aux = new long[n];

		for (int d = 0; d < w; d++)
		{
			// Compute frequency count
			int[] count = new int[R+1];
			for (int i = 0; i < n; i++)
			{
				// Bitwise AND for each 8 byte block
				int c = (int) (a[i] >> BITS_PER_BYTE*d) & MASK;
				count[c+1]++;
			}

			// Compute cumulates
			for (int r = 0; r < R; r++)
				count[r+1] += count[r];

			// For most significant byte, 0x80=0xFF comes before 0x00-0x7F
			if (d == w-1)
			{
				int shift1 = count[R] - count[R/2];
				int shift2 = count[R/2];
				for (int r = 0; r < R/2; r++)
					count[r] += shift1;
				for (int r = R/2; r < R; r++)
					count[r] -= shift2;
			}

			// Move data
			for (int i = 0; i < n; i++)
			{
				int c = (int) (a[i] >> BITS_PER_BYTE*d) & MASK;
				aux[count[c]++] = a[i];
			}

			// Copy back
			for (int i = 0; i < n; i++)
				a[i] = aux[i];
		}
	}

	public Iterable<long[]> matchedPairs()
	{
		return this.matchedPairs;
	}

	public static void main(String[] args)
	{
		// Generate array of 64 bit integers
		int N = 20;
		long[] a = new long[N];
		for (int i = 0; i < N; i++)
			a[i] = StdRandom.uniform(N/2);
		long T = StdRandom.uniform(N);

		StdOut.println("Generated random 64-bit integers:");
		for (int i = 0; i < N; i++)
			StdOut.print(a[i] + " ");
		StdOut.println();

		TwoSum sum = new TwoSum(a, T);
		StdOut.println("Result for T = " + T);
		for (long[] p : sum.matchedPairs())
			StdOut.println(p[0] + " + " + p[1] + " = " + T);
	}
}