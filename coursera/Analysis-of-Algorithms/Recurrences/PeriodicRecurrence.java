import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdIn;
import java.lang.Math;

/**
 * Plot the periodic part of the solution to the recurrence:
 * 	$$a_N = 3a_{\lfloor N/3\rfloor} + N \quad{\rm for}\ N>3{\rm \ with\ }
 *  a_1=a_2=a_3=1$$
 * For \\(1 \leq N \leq 972\\)
 */
public class PeriodicRecurrence implements Sequence
{
	private double[] a;
	private double[] periods;

	// Compute the recurrence
	public PeriodicRecurrence(int N)
	{
		a = new double[N+1];
		a[0] = 0;
		a[1] = a[2] = a[3] = 1;
		double base = Math.log(3);
		double bn = 0;
		periods = new double[N+1];
		for (int i = 4; i <= N; i++) {
			double k = Math.floor(Math.log(N/4) / base);
			bn += Math.floor(N/Math.pow(3, k)) * Math.pow(3, k);
			a[i] = Math.pow(3, k+1) + bn;
			periods[i] = periodic(i);
		}
	}

	public double periodic(int N)
	{
		if (N < 1) return 0;
		return N * (Math.log(N) / Math.log(3));
	}

	public double eval(int i)
	{
		return a[i];
	}

	public double[] getRecurrenceSeries()
	{
		return a;
	}

	public double[] getPeriodicSeries()
	{
		return periods;
	}

	public static void main(String[] args)
	{
		int N = Integer.parseInt(args[0]);
		PeriodicRecurrence P = new PeriodicRecurrence(N);
		for (int i = 0; i <= N; i++) {
			StdOut.println("An       :" + P.eval(i));
			StdOut.println("Periodic :" + P.periodic(i));
		}
	}
}