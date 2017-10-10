/*----------------------------------------------------------------
 *  Author:        Hafidz Jazuli Luthfi
 *  Written:       2017/04/28
 *  Last updated:  2017/10/06
 *
 *  A simple program aimed to find and analyze percolation. Used as client of PercolationVisualizer.
 *----------------------------------------------------------------*/
import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private WeightedQuickUnionUF grid;
    private int size;
    private boolean[] sites;
    private int opened;
    private int[] btmRowOpens;
    private int btmRowOpenCnt;
    private boolean havePercolated = false;

    // Create n-by-n grid, with all sites blocked.
    public Percolation(int n)
    {
        if (n > 0) {
            size = n;
            grid = new WeightedQuickUnionUF(n * n + 2); // 0 for vistual top node, last index for virtual bottom node
            btmRowOpens = new int[n];  // Track bottom row
            btmRowOpenCnt = 0;
            sites = new boolean[n * n + 2];
            for (int i = 0; i < (n * n + 2); i++) sites[i] = false;
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

    private boolean isRowColValid(int row, int col)
    {
        int limit = this.size + 1;
        if (row >= 1 && col >= 1) {
            if (limit == 2 || (row < limit && col < limit)) {
                return true;
            } else {
                throw new java.lang.IllegalArgumentException("Value of row or coloumn must be greater than 0 and less than "+ limit +" !");
            }
        } else {
            throw new java.lang.IllegalArgumentException("Value of row or coloumn must be greater than 0 and less than "+ limit +" !");
        }
    }

    // Open site (row, col) if it is not open already.
    public void open(int row, int col)
    {
        int id;
        if (isRowColValid(row, col)) {
            id = xyTo1D(row, col);
            if (!isOpen(row, col)) {
                sites[id] = true;
                opened += 1;

                if (row <= 1) { // connected to virtual top node
                    grid.union(id, 0);
                    if (isOpen(row + 1, col)) {
                        grid.union(id, xyTo1D(row + 1, col));
                    }
                    if (this.size == 1) { // If matrix consisted only 1 row
                        this.btmRowOpens[this.btmRowOpenCnt] = id;
                        this.btmRowOpenCnt += 1;
                    }
                } else {
                    if (isOpen(row - 1, col)) {
                        grid.union(id, xyTo1D(row - 1, col));
                    }
                    if (row < this.size) {
                        if (isOpen(row + 1, col)) {
                            grid.union(id, xyTo1D(row + 1, col));
                        }
                    } else {
                        this.btmRowOpens[this.btmRowOpenCnt] = id;
                        this.btmRowOpenCnt += 1;
                    }
                }
                if (col > 1) {
                    if (isOpen(row, col - 1)) {
                        grid.union(id, xyTo1D(row, col - 1));
                    }
                    if (col < size) {
                        if (isOpen(row, col + 1)) {
                            grid.union(id, xyTo1D(row, col + 1));
                        }
                    }
                } else {
                    if (isOpen(row, col + 1)) {
                        grid.union(id, xyTo1D(row, col + 1));
                    }
                }
            }
        }
    }

    // Is site (row, col) open?
    public boolean isOpen(int row, int col)
    {
        isRowColValid(row, col);
        return sites[xyTo1D(row, col)];
    }

    // is site (row, col) full?
    public boolean isFull(int row, int col)
    {
        isRowColValid(row, col);
        return grid.connected(xyTo1D(row, col), 0);
    }

    // get number of open sites.
    public int numberOfOpenSites()
    {
        return opened;
    }

    // Does the system percolate?
    public boolean percolates()
    {
        if (!this.havePercolated) {
            for (int i = 0; i < this.btmRowOpenCnt; i++) {
                if (grid.connected(0, this.btmRowOpens[i])) {
                    this.havePercolated = true;
                    break;
                }
            }
        }
        return this.havePercolated;
    }
}
