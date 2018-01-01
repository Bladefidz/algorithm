import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Graph;
import edu.princeton.cs.algs4.Edge;
import edu.princeton.cs.algs4.BipartiteX;
import edu.princeton.cs.algs4.Queue;

/**
 * This case will used bipartite algorithm extensively.
 * Suppose that we have n women and n men, so that is a balanced bipartite.
 * The relationship between them is k women exactly know k men and otherwise.
 * The case is find best possible match were each man and women know each
 * other.
 */
public class PerfectMatchingKBipartite
{
	private BipartiteX bipartition;
	private int cardinality;
	private static final int UNMATCHED = -1;
	private final int V;
    private int[] mate;
    private boolean[] marked;
    private int[] edgeTo;

	PerfectMatchingKBipartite(Graph G)
	{
		// Create bipartition graph. Basically, the BipartiteX class coloring each vertex on the each edge by white and black using BFS.
		this.bipartition = new BipartiteX(G);
		if (!bipartition.isBipartite()) {
            throw new IllegalArgumentException("graph is not bipartite");
        }

		this.V = G.V();
		this.mate = new int[V];
        for (int v = 0; v < V; v++)
            this.mate[v] = UNMATCHED;

        while (hasAugmentingPath(G))
        {
        	// Find the last endpoint of last path found
        	int t = -1;
        	for (int v = 0; v < G.V(); v++)
        	{
        		if (!this.isMatched(v) && this.edgeTo[v] != -1)
        		{
        			t = v;
        			break;
        		}
        	}

        	// Insert and update the matching based on last path found
        	for (int v = t; v != -1; v = this.edgeTo[this.edgeTo[v]])
        	{
        		int w = this.edgeTo[v];
        		this.mate[v] = w;
        		this.mate[w] = v;
        	}

        	this.cardinality++;
        }
	}

	private boolean hasAugmentingPath(Graph G)
	{
		this.marked = new boolean[G.V()];
		this.edgeTo = new int[G.V()];
		for (int v = 0; v < this.V; v++)
			this.edgeTo[v] = -1;

		// Do BFS starting from all unmatched vertices on one side of bipartition
		Queue<Integer> queue = new Queue<>();
		for (int v = 0; v < this.V; v++)
		{
			if (this.bipartition.color(v) && !this.isMatched(v))
			{
				queue.enqueue(v);
				this.marked[v] = true;
			}
		}

		// Run BFS to find an augmented path and stop as soon as we found another path
		while (!queue.isEmpty())
		{
			int v = queue.dequeue();
			for (int w : G.adj(v))
			{
				if (this.isResidualGraphEdge(v, w) && !this.marked[w])
				{
					this.edgeTo[w] = v;
					this.marked[w] = true;
					if (!this.isMatched(w)) return true;  // stop here
					queue.enqueue(w);
				}
			}
		}

		return false;  // No any augmented path found
	}

	private boolean isResidualGraphEdge(int v, int w)
	{
		if ((this.mate[v] != w) && this.bipartition.color(v)) return true;
		if ((this.mate[v] == w) && !this.bipartition.color(v)) return true;
		return false;
	}

	public boolean isMatched(int v)
	{
        return this.mate[v] != UNMATCHED;
    }

    public boolean isPerfect()
    {
        return this.cardinality * 2 == this.V;
    }

    public Iterable<Edge> matches()
    {
    	Queue<Edge> matches = new Queue<>();
    	if (isPerfect())
    	{
    		for (int v = 0; v < this.V; v++)
        	{
        		int w = this.mate[v];
        		if (this.isMatched(v) && v < w)
        		{  // Add only any forward edges from s to t
        			Edge e = new Edge(v, w, 1);
	    			matches.enqueue(e);
        		}
	    	}
    	}
    	return matches;
    }

	public static void main(String[] args)
	{
		In in = new In(args[0]);
        Graph G = new Graph(in);

		PerfectMatchingKBipartite matching = new PerfectMatchingKBipartite(G);
		StdOut.print("Max matching: ");
		for (Edge e: matching.matches())
		{
			int v = e.either();
			int w = e.other(v);
			StdOut.print(v + "-" + w + " ");
		}
		StdOut.println();
	}
}