/*----------------------------------------------------------------
 *  Author:        Hafidz Jazuli Luthfi
 *  Written:       2017/04/28
 *  Last updated:  2017/10/06
 *
 *  Compilation:   javac-algs4 PercolationStats.java
 *  Execution:     java-algs4 PercolationStats
 *
 *  Implementing monte carlo algorithm to compute percolation threshold.
 *----------------------------------------------------------------*/

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;

public class PercolationStats {
    private int trials;
    private double percThreshold[];

    /**
     * Perform trials independent experiments on an n-by-n grid
     */
    public PercolationStats(int n, int trials)
    {
        if (n > 0 && trials > 0) {
            this.trials = trials;
            this.percThreshold = new double[trials];

            int row; int col;
            for (int i = 0; i < trials; i++) {
                Percolation percObj = new Percolation(n);
                while (!percObj.percolates()) {
                    row = StdRandom.uniform(1, n + 1);
                    col = StdRandom.uniform(1, n + 1);
                    percObj.open(row, col);
                }
                this.percThreshold[i] = (double) percObj.numberOfOpenSites() / (n * n);
            }
        } else {
            throw new java.lang.IllegalArgumentException("Value of n and trials must be greater than 0!");
        }
    }

    /**
     * Sample mean of percolation threshold
     */
    public double mean()
    {
        return StdStats.mean(percThreshold);
    }

    /**
     * Sample standard deviation of percolation threshold
     */
    public double stddev()
    {
        double sd = StdStats.stddev(percThreshold);
        if (sd < 1) {
            return sd;
        } else {
            return Double.NaN;
        }
    }

    /**
     * Low endpoint of 95% confidence interval
     */
    public double confidenceLo()
    {
        return mean() - (1.96 * stddev() / Math.sqrt(this.trials));
    }

    /**
     * High endpoint of 95% confidence interval
     */
    public double confidenceHi()
    {
        return mean() + (1.96 * stddev() / Math.sqrt(this.trials));
    }

    /**
     * Test client (described below)
     */
    public static void main(String[] args)
    {
        PercolationStats ps = new PercolationStats(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
        StdOut.println("mean                    = " + ps.mean());
        StdOut.println("stddev                  = " + ps.stddev());
        StdOut.println("95% confidence interval = [" + ps.confidenceLo() + ", " + ps.confidenceHi() + "]");
    }
}
