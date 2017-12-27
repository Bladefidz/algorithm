import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Queue;
import java.util.Iterator;
import java.lang.IllegalArgumentException;
import java.lang.Math;

public class Board
{
	private final int[][] board;
    private long hash = 0;
	private int blankI;
	private int blankJ;
	private int dimension;
    private int manhattan = -1;

    private void rollHash()
    {
        int sq = this.dimension * this.dimension;
        for (int i = 0; i < this.dimension; i++)
        {
            for (int j = 0; j < this.dimension; j++)
            {
                this.hash = this.board[i][j] + (sq * this.hash);
            }
        }
    }

	// construct a board from an n-by-n array of blocks
	// (where blocks[i][j] = block in row i, column j)
    public Board(int[][] blocks)
    {
    	if (blocks == null)
    	{
    		throw new IllegalArgumentException();
    	}

    	this.dimension = blocks.length;
		this.board = new int[this.dimension][this.dimension];

		for (int i = 0; i < this.dimension; i++)
		{
			for (int j = 0; j < this.dimension; j++)
			{
				this.board[i][j] = blocks[i][j];
				if (blocks[i][j] == 0)
				{
					this.blankI = i;
					this.blankJ = j;
				}
			}
		}

        rollHash();
    }

    // board dimension n
    public int dimension()
    {
    	return this.dimension;
    }

    // number of blocks out of place
    public int hamming()
    {
    	int dist = 0;
    	for (int i = 0; i < this.dimension; i++)
    	{
    		for (int j = 0; j < this.dimension; j++)
    		{
    			if (this.board[i][j] != 0 &&
    				this.board[i][j] != (this.dimension * i) + j + 1
    			) {
    				dist++;
    			}
    		}
    	}
    	return dist;
    }

    // Get correct I-position of tile a
    private int getI(int a)
    {
    	return (int) Math.floor((a - 1) / this.dimension);
    }

    // Get correct J-position of tile a
    private int getJ(int a)
    {
    	return (a - 1) % this.dimension;
    }

    // sum of Manhattan distances between blocks and goal
    public int manhattan()
    {
        if (this.manhattan < 0)
        {
            this.manhattan = 0;
            for (int i = 0; i < this.dimension; i++)
            {
                for (int j = 0; j < this.dimension; j++)
                {
                    if (this.board[i][j] != 0)
                    {
                        int correct = (this.dimension * i) + j + 1;
                        if (this.board[i][j] != correct)
                        {
                            this.manhattan += Math.abs(getI(this.board[i][j]) - i);
                            this.manhattan += Math.abs(getJ(this.board[i][j]) - j);
                        }
                    }
                }
            }
        }
    	return this.manhattan;
    }

    // is this board the goal board?
    public boolean isGoal()
    {
    	if (hamming() == 0)
    	{
    		return true;
    	}
    	else
    	{
    		return false;
    	}
    }

    // a board that is obtained by exchanging any pair of blocks
    // the idea is the fact that a board is solvable if has odd inversion,
    // if we swap any tile (except zero tile) left or right, then the
    // inversion will changed from odd to even or otherwise.
    // One of origin board or its twin must be solvable and the other not.
    public Board twin()
    {
    	int[][] b = new int[this.dimension][this.dimension];
    	int k = 0;

    	if (this.board[k][0] == 0 || this.board[k][1] == 0)
    	{  // If 0,0 or 0,1 contains 0, then change swap index to next column
    		k = 1;
    	}

    	for (int i = 0; i < this.dimension; i++)
    	{
    		for (int j = 0; j < this.dimension; j++)
    		{
    			if (i == k && j == 0)
    			{
    				b[i][j] = this.board[k][1];
    			}
    			else if (i == k && j == 1)
    			{
    				b[i][j] = this.board[k][0];
    			}
    			else
    			{
    				b[i][j] = this.board[i][j];
    			}
    		}
    	}

    	return new Board(b);
    }

    // does this board equal y?
    public boolean equals(Object y)
    {
        if (y == this) return true;
    	if (y == null) return false;
    	if (y.getClass() != this.getClass()) return false;

        if (y instanceof Board)
        {
            Board yb = (Board) y;

            if (this.dimension != yb.dimension()) return false;
            if (this.hash != yb.hash) return false;
        }

    	return true;
    }

    // Return a neighbor of blank tile given by pair of index i and j
    private Board getNeighbor(int x, int y)
    {
    	int [][] neighbor = new int[this.dimension][this.dimension];

    	for (int i = 0; i < this.dimension; i++)
    	{
    		for (int j = 0; j < this.dimension; j++)
    		{
    			if (i == this.blankI && j == this.blankJ)
    			{
    				neighbor[i][j] = this.board[x][y];
    			}
    			else if (i == x && j == y)
    			{
    				neighbor[i][j] = this.board[this.blankI][this.blankJ];
    			}
    			else
    			{
    				neighbor[i][j] = this.board[i][j];
    			}
    		}
    	}

    	return new Board(neighbor);
    }

    // all neighboring boards
    public Iterable<Board> neighbors()
    {
    	// Move clockwise
    	Queue<Board> neighbors = new Queue<>();

    	if (this.blankI != this.dimension - 1)
    	{  // Move up
    		neighbors.enqueue(getNeighbor(this.blankI + 1, this.blankJ));
    	}

    	if (this.blankJ != this.dimension - 1)
    	{  // Move left
    		neighbors.enqueue(getNeighbor(this.blankI, this.blankJ + 1));
    	}

    	if (this.blankI != 0)
    	{  // Move down
    		neighbors.enqueue(getNeighbor(this.blankI - 1, this.blankJ));
    	}

    	if (this.blankJ != 0)
    	{  // Move right
    		neighbors.enqueue(getNeighbor(this.blankI, this.blankJ - 1));
    	}

    	return neighbors;
    }

    // string representation of this board
    // (in the output format specified below)
    public String toString()
    {
    	StringBuilder s = new StringBuilder();
	    s.append(this.dimension + "\n");
	    for (int i = 0; i < this.dimension; i++)
	    {
	        for (int j = 0; j < this.dimension; j++)
	        {
	            s.append(String.format("%2d ", this.board[i][j]));
	        }
	        s.append("\n");
	    }
	    return s.toString();
    }

    // unit tests (not graded)
    public static void main(String[] args)
    {
    	// create initial board from file
	    In in = new In(args[0]);
	    int n = in.readInt();
	    int[][] blocks = new int[n][n];
	    for (int i = 0; i < n; i++)
	        for (int j = 0; j < n; j++)
	            blocks[i][j] = in.readInt();
	    Board initial = new Board(blocks);
    }
}