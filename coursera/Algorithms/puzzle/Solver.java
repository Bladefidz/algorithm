import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.Stack;
import java.util.Comparator;
import java.util.Iterator;
import java.lang.IllegalArgumentException;
import java.lang.Math;

public class Solver
{
	private Node goal = null;

	// Node representation
	private class Node
	{
		private Node predecessor = null;
		private Board board;
		private int moveCount;

		public Node(Board newConf, int mc)
		{
			this.board = newConf;
			this.moveCount = mc;
		}
	}

	private class NodeOrder implements Comparator<Node>
    {
    	public int compare(Node th, Node that)
		{
			int dist1 = th.board.manhattan() + th.moveCount;
			int dist2 = that.board.manhattan() + that.moveCount;

			if (dist1 > dist2) return 1;
			if (dist1 < dist2) return -1;
			return 0;
		}
	}

	private Comparator<Node> NodeOrder()
    {
        return new NodeOrder();
    }

	// find a solution to the initial board (using the A* algorithm)
	public Solver(Board initial)
	{
		if (initial == null)
		{
			throw new IllegalArgumentException();
		}

		// Number of move in worst case scenario
		int maxMove = initial.dimension() + initial.dimension();
		maxMove *= initial.dimension() * initial.dimension();

		// Initial search tree
		MinPQ<Node> originTree = new MinPQ<Node>(maxMove, new NodeOrder());
		Node currentOriginNode = new Node(initial, 0);

		// Twin search tree
		MinPQ<Node> twinTree = new MinPQ<Node>(maxMove, new NodeOrder());
		Node currentTwinNode = new Node(initial.twin(), 0);

		// int i = 0;

		while(
			!currentOriginNode.board.isGoal() &&
			!currentTwinNode.board.isGoal()
		) {
			// StdOut.println("====");
			// StdOut.println(currentOriginNode.board.toString());
			// StdOut.println("====");
			for (Board neighbor : currentOriginNode.board.neighbors())
			{
				if (currentOriginNode.predecessor == null ||
					!neighbor.equals(currentOriginNode.predecessor.board)
				) {
					Node newNode = new Node(
						neighbor,
						currentOriginNode.moveCount + 1);
					newNode.predecessor = currentOriginNode;
					originTree.insert(newNode);
					// StdOut.println(neighbor.toString());
					// StdOut.println("Manhattan: "+neighbor.manhattan());
				}
			}

			Node lastOriginNode = currentOriginNode;
			currentOriginNode = originTree.delMin();

			for (Board neighbor : currentTwinNode.board.neighbors())
			{
				if (currentTwinNode.predecessor == null ||
					!neighbor.equals(currentTwinNode.predecessor.board)
				) {
					Node newNode = new Node(
						neighbor,
						currentTwinNode.moveCount + 1);
					newNode.predecessor = currentTwinNode;
					twinTree.insert(newNode);
				}
			}

			Node lastTwinNode = currentOriginNode;
			currentTwinNode = twinTree.delMin();
			currentTwinNode.predecessor = lastTwinNode;

			// i++;
			// if (i > 1)
			// {
			// 	break;
			// }
		}

		if (currentOriginNode.board.isGoal())
		{  // Remember, exactly one of origin or twin will be solvable.
			this.goal = currentOriginNode;
		}
	}

	// is the initial board solvable?
	public boolean isSolvable()
	{
		if (this.goal == null) return false;
		return true;
	}

	// min number of moves to solve initial board; -1 if unsolvable
	public int moves()
	{
		if (this.goal == null) return -1;
		return this.goal.moveCount;
	}

	// sequence of boards in a shortest solution; null if unsolvable
	public Iterable<Board> solution()
	{
		if (this.goal == null) return null;

		Node p = this.goal;
		Stack<Board> path = new Stack<>();
		while(p != null)
		{
			path.push(p.board);
			p = p.predecessor;
		}

		return path;
	}

	// solve a slider puzzle (given below)
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

	    // solve the puzzle
	    Solver solver = new Solver(initial);

	    // print solution to standard output
	    if (!solver.isSolvable())
	        StdOut.println("No solution possible");
	    else {
	        StdOut.println("Minimum number of moves = " + solver.moves());
	        for (Board board : solver.solution())
	            StdOut.println(board);
	    }
	}
}
