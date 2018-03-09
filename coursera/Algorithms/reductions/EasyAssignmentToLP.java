/******************************************************************************
 *  Compilation:  javac EasyAssignmentToLP.java
 *  Execution:    java EasyAssignmentToLP n
 *  Dependencies: LinearProgramming.java
 *
 *  Solve an n-by-n assignment problem (maximum weighted bipartite matching)
 *  by reducing it to linear programming.
 *
 *  Warning: in practice, use Assignment.java which runs in n^3 log n time
 *  instead of this version.
 *
 *
 ******************************************************************************/

public class EasyAssignmentToLP {

    public static void main(String[] args) {

        int n = Integer.parseInt(args[0]);

        // cost vector
        double[] c = new double[n*n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                c[i*n+j] = StdRandom.uniform();    // cost of assigning i-j

        // RHS vector
        double[] b = new double[2*n];
        for (int i = 0; i < 2*n; i++)
            b[i] = 1.0;

        // constraint matrix
        double[][] A = new double[2*n][n*n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][i*n+j] = 1.0;
            }
        }
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                A[j+n][i*n+j] = 1.0;
            }
        }

        // solve linear program
        LinearProgramming lp = new LinearProgramming(A, b, c);

        // print weight of max weight perfect matching
        double weight = lp.value();
        StdOut.println(weight);

        // print max weight perfect matching
        double[] x = lp.primal();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (x[i*n+j] == 1.0) {
                    StdOut.println(i + "-" + j);
                }
            }
        }

    }
}
