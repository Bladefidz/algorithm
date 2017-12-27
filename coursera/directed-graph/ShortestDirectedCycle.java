import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.MinPQ;
import java.util.Comparator;
import java.util.Iterator;

/**
 * Given a digraph G
 * Design an efficient algorithm to find a directed cycle with the minimum
 * number of edges (or report that the graph is acyclic).
 * The running time of your algorithm should be at most proportional to V(E+V)
 * and use space proportional to E+V, where V is the number of vertices and E
 * is the number of edges.
 */
class ShortestDirectedCycle
{
	private boolean[] marked;
	private int[] edgeTo;
	private int[] depth;
	private boolean[] onCheck;
	private Stack<Integer> shortestCycle;  // Null if no directed cycle found

	private class CycleHead
	{
		private int head;
		private int length;
		private int tail;

		public CycleHead(Integer head, Integer tail, int len)
		{
			this.head = head;
			this.tail = tail;
			this.length = len;
		}
	}

	private class CycleOrder implements Comparator<CycleHead>
    {
    	public int compare(CycleHead th, CycleHead that)
		{
			if (th.length > that.length) return 1;
			if (th.length < that.length) return -1;
			return 0;
		}
	}

	private Comparator<CycleHead> CycleOrder()
    {
        return new CycleOrder();
    }

	private MinPQ<CycleHead> cycleRoots;

	/**
	 * SHortest Directed Cycle constructor
	 * @param  G Digraph
	 */
	public ShortestDirectedCycle(Digraph G)
	{
		this.marked = new boolean[G.V()];
		this.onCheck = new boolean[G.V()];
		this.edgeTo = new int[G.V()];
		this.depth = new int[G.V()];
		this.cycleRoots = new MinPQ<CycleHead>(new CycleOrder());

		this.depth[0] = 0;
		for (int v = 0; v < G.V(); v++)
		{
			if (!this.marked[v])
			{
				dfs(G, v);
			}
		}

		if (!this.cycleRoots.isEmpty())
		{
			this.shortestCycle = new Stack<Integer>();
			int v = this.cycleRoots.min().head;
			int w = this.cycleRoots.min().tail;
			for (int x = v; x != w; x = this.edgeTo[x])
			{
				this.shortestCycle.push(x);
			}
			this.shortestCycle.push(w);
			this.shortestCycle.push(v);
		}
	}

	/**
	 * Find all possible cycle
	 * @param G Digraph
	 * @param v Vertex
	 */
	private void dfs(Digraph G, int v)
	{
		this.onCheck[v] = true;
		this.marked[v] = true;

		for (int w: G.adj(v))
		{
			if (!this.marked[w])
			{
				edgeTo[w] = v;
				depth[w] = depth[v] + 1;
				dfs(G, w);
			}
			else if (this.onCheck[w])
			{
				this.cycleRoots.insert(new CycleHead(
					v, w, depth[v] - depth[w]));
			}
		}

		this.onCheck[v] = false;
	}

	public boolean hasCycle()
	{
		return this.shortestCycle != null;
	}

	/**
	 * Return only shortest cycle
	 * @return [description]
	 */
	public Iterable<Integer> cycle()
	{
        return this.shortestCycle;
    }

	public static void main(String[] args)
	{
		In in = new In(args[0]);
		Digraph G = new Digraph(in);

		ShortestDirectedCycle sdc = new ShortestDirectedCycle(G);
		if (sdc.hasCycle())
		{
			StdOut.println("Shortest Directed Cycle");
			for (int v: sdc.cycle())
			{
				StdOut.print(v + " ");
			}
			StdOut.println();
		}
		else
		{
			StdOut.println("No directed cycle!");
		}
		StdOut.println();
	}
}