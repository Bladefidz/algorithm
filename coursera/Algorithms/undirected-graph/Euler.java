import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Graph;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.Queue;

/**
 * Find Euler cycle
 * An Euler cycle in a graph is a cycle (not necessarily simple)
 * that uses every edge in the graph exactly one.
 *
 * Fact: connected graph has an Euler cycle if and only if every vertex has
 * even degree.
 *
 * Use depth-first search and piece together the cycles you discover
 */
class EulerCycle
{
	private Queue<Integer> path;
	private Stack<Integer> cycle;
	private int[] edgeTo;

	public EulerCycle(Graph G)
	{
		this.path = new Queue<>();
		this.cycle = new Stack<>();
		this.edgeTo = new int[G.V()];

		dfs(G, 0, new boolean[G.V()]);
	}

	/**
	 * Find path using dfs
	 * @param G Graph
	 * @param v vertex
	 */
	private void dfs(Graph G, int v, boolean[] marked)
	{
		marked[v] = true;
		for (int w: G.adj(v))
		{
			if (!marked[w])
			{
				this.edgeTo[w] = v;
				dfs(G, w, marked);
			}
		}
	}

	public Iterable<Integer> path()
	{
        return this.path;
    }

	public Iterable<Integer> cycle()
	{
        return this.cycle;
    }

	/**
	 * Testing client
	 * @param args [description]
	 */
	public static void main(String[] args)
	{
		In in = new In(args[0]);
        Graph G = new Graph(in);
        EulerCycle euler = new EulerCycle(G);

        for (int v: euler.path())
        {
        	StdOut.println(v);
        }
    }
}