import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.UF;
import edu.princeton.cs.algs4.Edge;
import edu.princeton.cs.algs4.EdgeWeightedGraph;
import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.Queue;

/**
 * Feedback edge set is set of edges in subgraph which at least one edge in
 * every cyclic graph. If we removed an edge from feedback set, then our graph
 * became acyclic.
 * Minimum weighted feedback set is set of edges which consisted by lowest
 * weight edges.
 *
 * Find cyclic graphs using DFS
 * For every cyclic we found, calculate minimum weighted edge
 */
class MinimumWeightedFeedback
{
	private boolean[] marked;
	private double minWeight;

	MinimumWeightedFeedback(EdgeWeightedGraph G)
	{
		this.marked = new boolean[G.V()];
		this.minWeight = Double.POSITIVE_INFINITY;

		for (int v = 0; v < G.V(); v++)
		{
			if (!marked[v])
			{
				dfs(G, v, v);
			}
		}
	}

	private void dfs(EdgeWeightedGraph G, int v, int u)
	{
		this.marked[v] = true;
		for (Edge e: G.adj(v))
		{
			int w = e.other(v);
			if (!marked[w]) dfs(G, w, v);
			else if (w != u)
			{  // Found an edge that construct cyclic graph
				if (e.weight() < this.minWeight)
				{
					this.minWeight = e.weight();
				}
			}
		}
	}

	public double minWeight()
	{
		return this.minWeight;
	}

	public static void main(String[] args)
	{
        In in = new In(args[0]);
        EdgeWeightedGraph G = new EdgeWeightedGraph(in);
        MinimumWeightedFeedback mwf = new MinimumWeightedFeedback(G);
        StdOut.println("Minimum feedback edge: " + mwf.minWeight());
    }
}