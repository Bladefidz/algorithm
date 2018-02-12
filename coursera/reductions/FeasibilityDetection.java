import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

/**
 * In the case we does not know appropriate basis, for example if there is
 * one or more constrains that lead negative value.
 * LP duality tells us that: For each maximization problem, there is a dual
 * form which is minimization problem.
 * Implement two-phase Simplex where at phase 1, we try to find basis feasible
 * solution (bfs).
 */
class FeasibilityDetection
{
	private static final double EPSILON = 1.0E-8;
	private double[][] a;	// Tableaux
	private int m;			// Number of constrains
	private int n;			// Number of original values
	private int[] basis;	// basis variables

	FeasibilityDetection(double[][] A, double[] b, double[] c)
	{
		// initialization
		m = b.length;
		n = c.length;
		a = new double[m+2][n+m+m+1];
		// Populate decision variables (x) into tableaux
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				a[i][j] = A[i][j];
			}
		}
		// Populate matrix identity (I) in the tableaux
		for (int i = 0; i < m; i++) {
			a[i][n+i] = 1.0;
		}
		// Populate RHS of constrains (b)
		for (int i = 0; i < m; i++) {
			a[i][n+m+m] = b[i];
		}
		// Populate objective functions (c)
		for (int j = 0; j < n; j++) {
			a[m][j] = c[j];
		}

		// If negative RHS, multiply by -1
		for (int i = 0; i < m; i++) {
			if (b[i] < 0) {
				for (int j = 0; j <= n+m+m; j++) {
					a[i][j] = -a[i][j];
				}
			}
		}

		// Artificial variables (xa) from initial basis
		// Populate matrix identity (I) in artificial variable column
		for (int i = 0; i < m; i++) {
			a[i][n+m+i] = 1.0;
		}
		// Populate dual row (d) below I xa
		for (int i = 0; i < m; i++) {
			a[m+1][n+m+i] = -1.0;
		}
		// Pivot by implementing Gauss-Jordan elimination
		for (int i = 0; i < m; i++) {
			pivot(i, n+m+i);
		}

		basis = new int[m];
		for (int i = 0; i < m; i++) {
			basis[i] = n + m + i;
		}

		phase1();
	}

	private void phase1()
	{
		while (true) {
			int q = bland1();			// Entering column q
			if (q == -1) break;  		// optimal

			int p = minRatioRule(q);	// Leaving row q
			if (p == -1) throw new ArithmeticException("Linear program is unbounded");

			pivot(p, q); // Pivot

			basis[p] = q; // update basis
		}
		if (a[m+1][n+m+m] > EPSILON) throw new ArithmeticException("Infeasible LP!");
	}

	// Lowest index of a non-basic column with a positive cost
	// using artificial objective function
	private int bland1()
	{
		for (int j = 0; j < n+m; j++) {
			if (a[m+1][j] > EPSILON) return j;
		}
		return -1;  // optimal
	}

	private int minRatioRule(int q)
	{
		int p = -1;
		for (int i = 0; i < m; i++) {
			if 		(a[i][q] <= 0) continue;
			else if (p == -1)	   p = i;
			else if ((a[i][n+m+m] / a[i][q]) < (a[p][n+m+m] / a[p][q])) p = i;
		}
		return p;
	}

	// Pivot on entry (p, q) using Gauss-Jordan elimination
	private void pivot(int p, int q)
	{
		// For everything row p and column q
		for (int i = 0; i < m+1; i++) {
			for (int j = 0; j <= n+m+m; j++) {
				if (i != p && j != q)
					a[i][j] -= a[p][j] * a[i][q] / a[p][q];
			}
		}

		// Zero out column q
		for (int i = 0; i <= m+1; i++) {
			if (i != p) a[i][q] = 0.0;
		}

		// Scale row p
		for (int j = 0; j <= n+m+m; j++) {
			if (j != q) a[p][j] /= a[p][q];
		}
		a[p][q] = 1.0;
	}

	// return dual solution vector
	public double[] dual() {
		double[] y = new double[m];
		for (int i = 0; i < m; i++)
			y[i] = -a[m][n+i];
		return y;
	}

	// is the solution dual feasible?
	public boolean isDualFeasible(double[][] A, double[] c) {
		double[] y = dual();

		// check that y >= 0
		for (int i = 0; i < y.length; i++) {
			if (y[i] < 0.0) {
				StdOut.println("y[" + i + "] = " + y[i] + " is negative");
				return false;
			}
		}

		// check that yA >= c
		for (int j = 0; j < n; j++) {
			double sum = 0.0;
			for (int i = 0; i < m; i++) {
				sum += A[i][j] * y[i];
			}
			if (sum < c[j] - EPSILON) {
				// StdOut.println("not dual feasible");
				// StdOut.println("c[" + j + "] = " + c[j] + ", sum = " + sum);
				return false;
			}
		}
		return true;
	}

	public double[] bfs()
	{
		double[] bfs = new double[m];
		for (int r = 0; r < m; r++) {
			bfs[r] = a[r][n+m+m];
		}
		return bfs;
	}

	public static void test(double[][] A, double[] b, double[] c) {
		FeasibilityDetection fd = new FeasibilityDetection(A, b, c);

		StdOut.println("Is dual feasible? " + fd.isDualFeasible(A, c));

		double[] bfs = fd.bfs();
		StdOut.print("(");
		for (int r = 0; r < b.length; r++) {
			StdOut.print(bfs[r]);
			if (r < b.length-1)
				StdOut.print(", ");
		}
		StdOut.println(")");
	}

	// b >= 0
	public static void test0()
	{
		double[] c = {  13.0,  23.0 };
		double[] b = { 480.0, 160.0, 1190.0 };
		double[][] A = {
			{  5.0, 15.0 },
			{  4.0,  4.0 },
			{ 35.0, 20.0 },
		};
		test(A, b, c);
	}

	// b <= 0 and b >= 0
	public static void testDual()
	{
		double[] b = {  -13.0,  -23.0 };
		double[] c = { -480.0, -160.0, -1190.0 };
		double[][] A = {
			{  -5.0, -4.0, -35.0 },
			{ -15.0, -4.0, -20.0 }
		};
		test(A, b, c);
	}

	public static void main(String[] args)
	{
		StdOut.println("Test 0");
		test0();

		StdOut.println();
		StdOut.println("Test dual");
		testDual();
	}
}