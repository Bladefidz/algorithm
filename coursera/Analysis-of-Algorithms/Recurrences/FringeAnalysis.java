import edu.princeton.cs.algs4.StdOut;

/**
 * Fringe analysis of 2-3 trees
 *
 * $$A_N =  A_{N-1}-{2A_{N-1}\over N} + 2\Bigl(1-{2A_{N-1}\over N}\Bigr) \quad{\rm for}\ N>1{\rm \ with\ } A_1=1.$$
 *
 * This recurrence describes the following random process: A set of N elements
 * collect into "2-nodes" and "3-nodes." At each step each 2-node is likely to
 * turn into a 3-node with probability 2/N and each 3-node is likely to turn
 * into two 2-nodes with probability 3/N. What is the average number of
 * 2-nodes after N steps?
 */
public class FringeAnalysis implements Sequence
{
	private final double[] c;
	private final double avg;

	public FringeAnalysis(int N)
	{
		if (N < 1) throw new java.lang.IllegalArgumentException();;

		c = new double[N+1];
		c[0] = 0;
		c[1] = 1;
		double sum = 1;
		for (int i = 2; i < N; i++) {
			c[i] = c[i-1] - 2*c[i-1]/i + 2*i - 4*c[i-1]/i;
			sum += c[i];
		}
		avg = sum / N;
	}

	public double check(int N)
	{
		if (N < 1) return 1;

		double[] c1 = new double[N+1];
		return (N-6)*c1[N-1]/N+2;
	}

	public double eval(int i)
	{
		return c[i];
	}

	public static void main(String[] args)
	{
		int N = Integer.parseInt(args[0]);
		FringeAnalysis F = new FringeAnalysis(N);
		// for (int i = 0; i < N; i++)
		// 	StdOut.println(F.eval(i));
		StdOut.println("Average : " + F.avg);
		double c = (N-6)*F.c[N-1]/N+2;
		StdOut.println("check : " + c);
		Values.show(F, N);
	}
}