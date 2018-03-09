import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdIn;

public class PeriodicRecurrence implements Sequence
{
	private double[] c;

	public PeriodicRecurrence()
	{}

	public void jingsawDown(int N)
	{
		c = new double[N+1];
		c[0] = 0;
		for (int i = 1; i <= N; i++) {
			c[i] = c[i-1] - Math.log(i);
		}
		// for (int i = 1; i <= N; i++) {
		// 	c[i] -= i * Math.log(i) / Math.log(2) + i;
		// }
	}

	public double eval(int i)
	{
		return c[i];
	}

	public static void main(String[] args)
	{
		int N = Integer.parseInt(args[0]);
		PeriodicRecurrence P = new PeriodicRecurrence();
		P.jingsawDown(N);
		Values.show(P, N);
	}
}