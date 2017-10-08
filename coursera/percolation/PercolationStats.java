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

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;

public class PercolationStats {
    private static final double CONFIDENCE_95 = 1.96;
    private int trials;
    private double mean;
    private double stddev;

    /**
     * Perform trials independent experiments on an n-by-n grid
     */
    public PercolationStats(int n, int trials)
    {
        if (n > 0 && trials > 0) {
            int row;
            int col;
            double[] percThreshold = new double[trials];
            this.trials = trials;
            for (int i = 0; i < trials; i++) {
                Percolation percObj = new Percolation(n);
                while (!percObj.percolates()) {
                    row = StdRandom.uniform(1, n + 1);
                    col = StdRandom.uniform(1, n + 1);
                    percObj.open(row, col);
                }
                percThreshold[i] = (double) percObj.numberOfOpenSites() / (n * n);
            }
            this.mean = StdStats.mean(percThreshold);
            this.stddev = StdStats.stddev(percThreshold);
        } else {
            throw new java.lang.IllegalArgumentException("Value of n and trials must be greater than 0!");
        }
    }

    /**
     * Sample mean of percolation threshold
     */
    public double mean()
    {
        return this.mean;
    }

    /**
     * Sample standard deviation of percolation threshold
     */
    public double stddev()
    {
        double sd = this.stddev;
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
        return this.mean - (CONFIDENCE_95 * this.stddev / Math.sqrt(this.trials));
    }

    /**
     * High endpoint of 95% confidence interval
     */
    public double confidenceHi()
    {
        return this.mean + (CONFIDENCE_95 * this.stddev / Math.sqrt(this.trials));
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
