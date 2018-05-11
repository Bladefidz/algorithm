import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.SET;

/**
 * Given an array of n objects with integer keys between 0 and R−1,
 * design a linear-time algorithm to rearrange them in ascending order.
 * Use extra space at most proportional to R
 *
 * Hint:
 * Compute the count[] array, which tells you where the keys need to go.
 * Scan over the input array. Take the first key, find the bin in which it
 * belong, and swap it into place (updating the corresponding count[] entry).
 * Repeat with the second key, but be careful to skip over keys already known
 * to be where they belong.
 */
public class AmericanFlagSort
{
	private static final int BITS_PER_BYTE = 8;
	private static final int BITS_PER_INT = 32;
	private static final int R = 256;
	private static final int CUTOFF = 15;

	public AmericanFlagSort(int[] a)
	{
		int n = a.length;
		int[] aux = new int[n];
		sort(a, 0, n-1, 0, aux);
	}

	private static void sort(int[] a, int lo, int hi, int d, int[] aux)
	{
		if (hi <= lo + CUTOFF)
		{
			insertion(a, lo, hi, d);
			return;
		}

		// Compute frequency count
		int[] count = new int[R+1];
		int mask = R-1;
		int shift = BITS_PER_INT - BITS_PER_BYTE*d - BITS_PER_BYTE;
		for (int i = lo; i <= hi; i++)
		{
			int c = (a[i] >> shift) & mask;
			count[c+1]++;
		}

		// Transform frequency count into indices
		for (int r = 0; r < R; r++)
			count[r+1] += count[r];

		// Distribute
		for (int i = lo; i <= hi; i++)
		{
			int c = (a[i] >> shift) & mask;
			aux[count[c]++] = a[i];
		}

		// Copy back
		for (int i = lo; i <= hi; i++)
			a[i] = aux[i - lo];

		// No more bits
		if (d == 4) return;

		// Recursively sort for each bytes
		if (count[0] > 0)
			sort(a, lo, lo + count[0] - 1, d+1, aux);
		for (int r = 0; r < R; r++)
			if (count[r+1] > count[r])
				sort(a, lo + count[r], lo + count[r+1]-1, d+1, aux);
	}

	private static void insertion(int[] a, int lo, int hi, int d)
	{
		for (int i = lo; i <= hi; i++)
			for (int j = i; j > lo && a[j] < a[j-1]; j--)
				exch(a, j, j-1);
	}

	private static void exch(int[] a, int i, int j)
	{
		int temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}

	public static void main(String[] args)
	{
		int N = 20;
		int R = 256;
		int[] a = new int[N];
		SET<Integer> nums = new SET<>();
		for (int i = N-1; i >= 0; i--)
		{
			int r = StdRandom.uniform(100);
			while (nums.contains(r)) r = StdRandom.uniform(100);
			nums.add(r);
			a[i] = r;
		}

		StdOut.println("Before:");
		for (int i = 0; i < N; i++)
			StdOut.print(a[i] + " ");
		StdOut.println();

		AmericanFlagSort flag = new AmericanFlagSort(a);

		StdOut.println("After:");
		for (int i = 0; i < N; i++)
			StdOut.print(a[i] + " ");
		StdOut.println();
	}
}