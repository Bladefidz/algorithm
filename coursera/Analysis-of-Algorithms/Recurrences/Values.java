import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdDraw;

public class Values
{
	public static void  print(Sequence f, int maxN)
	{
		for (int N = 0; N < maxN; N++)
		StdOut.printf("%10.6f %10.6f\n", f.eval(N), N*f.eval(N));
	}

	public static void show(Sequence f, int N)
	{
		double max = 0;
		for (int i = 0; i < N; i++) {
			if (f.eval(i) > max) max = f.eval(i);
		}
		for (int i = 0; i < N; i++) {
			double x = 1.0 * i / N;
			double y = 1.0 * f.eval(i) / max;
			// StdOut.println("x: " + x + " y:" + y);
			StdDraw.filledCircle(x, y, .004);
		}
		StdDraw.show();
	}
}