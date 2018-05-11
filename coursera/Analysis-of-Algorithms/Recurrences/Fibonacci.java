import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Computing Fibonacci:
 * Recursive: O(N^2)
 * Dynamic programming or recurrence: O(N logN)
 */
public class Fibonacci implements Sequence
{
	private double[] F;

	public Fibonacci() {}

	public double recursive(int N)
	{
		if (N == 0) return 0;
		if (N == 1) return 1;
		return recursive(N-1) + recursive(N-2);
	}

	public void recurrence(int N)
	{
		F = new double[N+1];
		F[0] = 0; F[1] = 1;
		for (int i = 2; i <= N; i++) {
			F[i] = F[i-1] + F[i-2];
		}
	}

	public double eval(int i)
	{
		return F[i];
	}

	public static void main(String[] args)
	{
		int N = Integer.parseInt(args[0]);
		Fibonacci F = new Fibonacci();
		F.recurrence(N);
		for (int i = 0; i < N; i++)
			StdOut.println(F.eval(i));
	}
}