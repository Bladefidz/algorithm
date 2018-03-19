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
	public FringeAnalysis() { }

	public double brute(int N)
	{
		if (N < 0) throw new java.lang.IllegalArgumentException();;

		double prev = 0;
		double curr = 0;
		for (int i = 1; i <= N; i++) {
			curr = prev - (2*prev/i) + 2 - (4*prev/i);
			prev = curr;
		}
		return curr;
	}

	// Find An using recurrence relation
	public double eval(int N)
	{
		if (N < 6)
			return brute(N);
		return 2.0*(N+1.0)/7.0;
	}

	public double avg(int N)
	{
		double sum = 0;
		for (int i = 1; i <= N; i++)
			sum += eval(i);
		return sum/N;
	}

	public static void main(String[] args)
	{
		int N = Integer.parseInt(args[0]);
		FringeAnalysis F = new FringeAnalysis();
		for (int i = 0; i <= N; i++)
			StdOut.println(F.eval(i));
		StdOut.println("Average : " + F.avg(N));
		// Values.show(F, N);
	}
}