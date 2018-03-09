import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Edge;
import edu.princeton.cs.algs4.EdgeWeightedGraph;
import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.Queue;

/**
 * Determine whatever an edge e found in some MST of edge-weighted graph G.
 * I used Prim algorithm which is using lazy approach to maintain edges in
 * priority queue. Since there are possibility that we found more than one
 * suitable cutting edges, then what do if we can not found an edge in MST,
 * we need to check priority queue if there are another suitable cutting edge.
 *
 * Find given edge in linear time.
 * Construct a full MST need E log E, and find another shortest crossing cut
 * need at most 2.
 */
public class FindEdgeInMST
{
	private Edge[] edgeTo;
	private double[] distTo;
	private boolean[] marked;
	private IndexMinPQ<Double> pq;

	/**
	 * Implementing Prim algorithm to construct MST.
	 */
	FindEdgeInMST(EdgeWeightedGraph G)
	{
		this.edgeTo = new Edge[G.V()];
		this.distTo = new double[G.V()];
		this.marked = new boolean[G.V()];
		this.pq = new IndexMinPQ<Double>(G.V());

		for (int v = 0; v < G.V(); v++)
		{
			this.distTo[v] = Double.POSITIVE_INFINITY;
		}
		for (int v = 0; v < G.V(); v++)
		{
			if (!this.marked[v]) this.prim(G, v);
		}
	}

	/* Run prim's algorithm starting from vertex s */
	private void prim(EdgeWeightedGraph G, int s)
	{
		this.distTo[s] = 0.0;
		this.pq.insert(s, this.distTo[s]);
		while (!this.pq.isEmpty())
		{
			int v = this.pq.delMin();
			this.scan(G, v);
		}
	}

	/* Scan vertex v */
	private void scan(EdgeWeightedGraph G, int v)
	{
		this.marked[v] = true;
		for (Edge e : G.adj(v))
		{
			// Avoid self loop
			int w = e.other(v);
			if (this.marked[w]) continue;  // Ignore already visited

			if (e.weight() < this.distTo[w])
			{
				this.distTo[w] = e.weight();
				this.edgeTo[w] = e;
				if (this.pq.contains(w))
				{
					this.pq.decreaseKey(w, this.distTo[w]);
				} else {
					this.pq.insert(w, this.distTo[w]);
				}
			}
		}
	}

	public Iterable<Edge> edges()
	{
		Queue<Edge> mst = new Queue<Edge>();
        for (int v = 0; v < edgeTo.length; v++)
        {
            Edge e = edgeTo[v];
            if (e != null) {
                mst.enqueue(e);
            }
        }
        return mst;
	}

	public boolean find(int[] f)
	{
		boolean found = false;
		for (int s = 0; s < edgeTo.length; s++)
		{
            Edge e = edgeTo[s];
            if (e != null) {
                int v = e.either();
                int w = e.other(v);
                if (v == f[0] && w == f[1]) found = true;
                if (v == f[1] && w == f[0]) found = true;
                if (found) break;
            }
        }
		return found;
	}

	public static void main(String[] args)
	{
		In in = new In(args[0]);
        EdgeWeightedGraph G = new EdgeWeightedGraph(in);
        FindEdgeInMST feimst = new FindEdgeInMST(G);

        for (Edge e : feimst.edges())
        {
            StdOut.println(e);
        }

        StdOut.println("\nEnter an edge to find: ");
        while(!StdIn.isEmpty())
        {
        	String l = StdIn.readLine();
        	String[] eStr = l.split("-");
        	int[] edge = new int[2];
        	edge[0] = Integer.parseInt(eStr[0]);
        	edge[1] = Integer.parseInt(eStr[1]);
        	if (feimst.find(edge))
        	{
    			StdOut.println("True");
    		} else {
    			StdOut.println("False");
    		}
    		StdOut.println("Enter an edge to find: ");
        }
	}
}