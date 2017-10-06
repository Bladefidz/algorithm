/*----------------------------------------------------------------
 *  Author:        Hafidz Jazuli Luthfi
 *  Written:       2017/04/28
 *  Last updated:  2017/10/06
 *
 *  A simple program aimed to find and analyze percolation. Used as client of PercolationVisualizer.
 *----------------------------------------------------------------*/
import edu.princeton.cs.algs4.WeightedQuickUnionUF;
import edu.princeton.cs.algs4.StdOut;

public class Percolation {
    private WeightedQuickUnionUF grid;
    private int size;
    private int[] sites;
    private int opened;

    //Create n-by-n grid, with all sites blocked.
    public Percolation(int n)
    {
        if (n > 0) {
            size = n;
            grid = new WeightedQuickUnionUF(n * n + 2); // 0 for vistual top node, last index for virtual bottom node
            sites = new int[n * n + 2];
            for (int i = 0; i < (n * n + 2); i++) sites[i] = 0;
            opened = 0;
        } else {
            throw new java.lang.IllegalArgumentException("Value of n must be greater than 0!");
        }
    }

    // Get index of array of coordinate(row, col)
    private int xyTo1D(int row, int col)
    {
        return (size * row) - (size - col);
    }

    // Open site (row, col) if it is not open already.
    public void open(int row, int col)
    {
        int id;
        if (row <= size && col <= size) {
            id = xyTo1D(row, col);
            if (!isOpen(row, col)) {
                sites[id] = 1;
                opened += 1;
                if (row == 1) { // connected to virtual top node
                    grid.union(id, 0);
                }
                if (row == size) { // Coneected to virtual bottom node
                    grid.union(id, size * size + 1);
                }
                if (row > 1 && isOpen(row - 1, col)) {
                    grid.union(id, xyTo1D(row - 1, col));
                }
                if (row < size && isOpen(row + 1, col)) {
                    grid.union(id, xyTo1D(row + 1, col));
                }
                if (col > 1 && isOpen(row, col - 1)) {
                    grid.union(id, xyTo1D(row, col - 1));
                }
                if (col < size && isOpen(row, col + 1)) {
                    grid.union(id, xyTo1D(row, col + 1));
                }
            }
        } else {
            throw new java.lang.IndexOutOfBoundsException("Row or column index out of bound!");
        }
    }

    // Is site (row, col) open?
    public boolean isOpen(int row, int col)
    {
        if (row <= size && col <= size) {
            if (sites[xyTo1D(row, col)] == 1) {
                return true;
            } else {
                return false;
            }
        } else {
            throw new java.lang.IndexOutOfBoundsException("Row or column index out of bound!");
        }
    }

    // is site (row, col) full?
    public boolean isFull(int row, int col)
    {
        if (row <= size && col <= size) {
            return grid.connected(xyTo1D(row, col), 0);
        } else {
            throw new java.lang.IndexOutOfBoundsException("Row or column index out of bound!");
        }
    }

    // get number of open sites.
    public int numberOfOpenSites()
    {
        return opened;
    }

    // Does the system percolate?
    public boolean percolates()
    {
        return grid.connected(0, size * size + 1);
    }
}
