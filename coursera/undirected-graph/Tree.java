import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Graph;
import edu.princeton.cs.algs4.Stack;
import java.lang.IllegalArgumentException;

/**
 * Tree is an acyclic connected graph.
 *
 * To compute the diameter, pick a vertex s; run BFS from s; then run BFS
 * again from the vertex that is furthest from s.
 * Center is vertices on the longest path..
 */
public class Tree
{
	private int diameter;
	private int center;
	private int[] trunk;
	private boolean isTree;

	public Tree(Graph G)
	{
		this.isTree = true;

		if (Tree.hasValidVertex(G))
		{
			this.diameter = 0;
			this.center = 0;
			this.trunk = new int[G.V()];

			countDiameter(G);
			findCenter(G);
		}
		else
		{
			throw new IllegalArgumentException("Graph is not acyclic!");
		}
	}

	public static boolean hasValidVertex(Graph G)
	{
		if (G.E() == G.V() - 1)
		{
			return true;
		}
		return false;
	}

	public boolean isTree()
	{
		return this.isTree;
	}

	/**
	 * Count diameter of tree
	 * @param G Graph
	 */
	private void countDiameter(Graph G)
	{
		if (G.V() > 1)
		{ // At least we have root and one child
			this.trunk[this.diameter] = 0;
			this.diameter++;
		}
		for (int v = 0; v < G.V(); v++)
		{
	        if (G.degree(v) > 1)
	        {
	        	// StdOut.println(v);  // Testing
	        	this.trunk[this.diameter] = v;
	        	this.diameter++;

	        	if (this.isTree && v == w)
	        	{ // Graph contains cycle
	        		this.isTree = false;
	        	}
	        }
		}
	}

	/**
	 * Diameter of the tree is longest simple path in the graph.
	 * @return Distance of longest simple path
	 */
	public int diameter()
	{
		return this.diameter;
	}

	/**
	 * Find a vertex in the center of the tree
	 * @param G Graph
	 */
	private void findCenter(Graph G)
	{
		this.center = this.trunk[this.diameter / 2];
	}

	/**
	 * Vertex which in center of the tree.
	 * So, its maximum distance from any other vertex is minimized
	 * @return Vertex id
	 */
	public int center()
	{
		return this.center;
	}

	/**
	 * TestingS client
	 * @param args [description]
	 */
	public static void main(String[] args)
	{
		In in = new In(args[0]);
        Graph G = new Graph(in);
        Tree tree = new Tree(G);

        StdOut.println("Is a tree: " + tree.isTree());
        StdOut.println("Diameter = " + tree.diameter());
        StdOut.println("Center = " + tree.center());
    }
}