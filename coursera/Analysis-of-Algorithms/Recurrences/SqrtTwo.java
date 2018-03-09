import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdIn;

/**
 * Using Newton method to compute series of value that converge to
 * square root of two.
 *
 * Recurrence: C_N = 1/2*(C_{N-1}+2/(C_{N-1}))
 */
public class SqrtTwo implements Sequence
{
	private final double[] c;

	public SqrtTwo(int N)
	{
		c = new double[N+1];
		c[0] = 1;
		for (int i = 1; i <= N; i++) {
			c[i] = (c[i-1] + 2/c[i-1])/2;
		}
	}

	public double eval(int i)
	{
		return c[i];
	}

	public static void main(String[] args)
	{
		int N = Integer.parseInt(args[0]);
		SqrtTwo test = new SqrtTwo(N);
		for (int i = 0; i < N; i++) {
			StdOut.println(test.eval(i));
		}
	}
}