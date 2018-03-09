import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Solving recurrences for quick sequence:
 * NC_N = (N + 1)C_{N-1} + 2N
 *
 * Sum of this recurrence can be checked using Harmonic Series
 */
public class QuickSeq implements Sequence
{
	private final double[] c;

	public QuickSeq(int N)
	{
		c = new double[N + 1];
		c[0] = 0;
		for (int i = 1; i <= N; i++) {
			c[i] = (i+1)*c[i-1]/i+2;
		}
	}

	public double eval(int N)
	{
		return c[N];
	}

	public static void main(String[] args)
	{
		int N = Integer.parseInt(args[0]);
		QuickSeq Q = new QuickSeq(N);
		// for (int i = 0; i < N; i++)
		// 	StdOut.println(Q.eval(i));
		Values.show(Q, N);
	}
}