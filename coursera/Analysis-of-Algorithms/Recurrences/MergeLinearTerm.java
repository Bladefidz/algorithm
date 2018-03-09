import edu.princeton.cs.algs4.StdOut;
import java.lang.Math;

/**
 * Number of compares for sort (for N > 1 with C_1 = 0):
 * 		C_N = C_{\lfloor N/2\rfloor}+C_{\lceil N/2\rceil}+N
 */
public class MergeLinearTerm implements Sequence
{
	private final double[] c;

	public MergeLinearTerm(int N)
	{
		c = new double[N+1];
		c[0] = 0;
		for (int i = 1; i <= N; i++) {
			c[i] = i + c[i/2] + c[i - (i/2)];
		}
		for (int i = 1; i <= N; i++) {
			c[i] -= i * Math.log(i) / Math.log(2) + i;
		}
	}

	public double eval(int i)
	{
		return c[i];
	}

	public static void main(String[] args)
	{
		int N = Integer.parseInt(args[0]);
		MergeLinearTerm M = new MergeLinearTerm(N);
		for (int i = 0; i < N; i++)
			StdOut.println(M.eval(i));
		Values.show(M, N);
	}
}