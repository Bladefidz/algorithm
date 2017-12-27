import edu.princeton.cs.algs4.StdOut;
import java.lang.Math;

/**
 * Java autoboxing and equals().
 * Consider two double values a and b
 * and their corresponding Double values x and y.
 * Find values such that (a==b) is true but x.equals(y) is false.
 * Find values such that (a==b) is false but x.equals(y) is true.
 */
class AutoboxingEqual
{
	public static void main(String[] args)
	{
		double a = -0.0;
		double b = +0.0;
		Double a1 = (Double) a;
		Double b1 = (Double) b;

		boolean eq1 = (a == b);
		boolean eq2 = a1.equals(b1);

		StdOut.println(a+" == "+b+"="+ eq1);
		StdOut.println(a1+" equals "+b1+"="+ eq2);

		a1 = Double.NaN;
		b1 = Double.NaN;
		a = Double.NaN;
		b = Double.NaN;

		eq1 = (a == b);
		eq2 = a1.equals(b1);

		StdOut.println(a+" == "+b+"="+ eq1);
		StdOut.println(a1+" equals "+b1+"="+ eq2);
	}
}