import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.DirectedEdge;
import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.EdgeWeightedDigraph;
import edu.princeton.cs.algs4.DijkstraSP;
import edu.princeton.cs.algs4.Stack;

/**
 * Find a shortest path from s to t in edge weighted digraph where there is a
 * edge which could be ignored or by assuming that its weight equal to zero.
 * Solution should be in E*log*V running time.
 *
 * Let S  = {W{v}} 	is set of shortest paths from s to every vertex w.
 * let T' = {W{v}}	is set of shortest paths from w to t.
 */
class OneSkipableSP
{
	private DirectedEdge[] edgeTo;
	private double[] distTo;
	private double[] shortestDistTo;
	private IndexMinPQ<Double> pq;

	OneSkipableSP(EdgeWeightedDigraph G, int s, int t)
	{
		this.edgeTo = new DirectedEdge[G.V()];
		this.distTo = new double[G.V()];
		this.shortestDistTo = new double[G.V()];
		this.pq = new IndexMinPQ<Double>(G.V());

		for (int v = 0; v < G.V(); v++)
		{
			this.distTo[v] = Double.POSITIVE_INFINITY;
			this.shortestDistTo[v] = Double.POSITIVE_INFINITY;
		}
		this.distTo[s] = 0.0;
		this.pq.insert(s, distTo[s]);
		while (!this.pq.isEmpty())
			relax(G, this.pq.delMin());

		for (DirectedEdge e : G.edges())
		{
			
		}
	}

	private void relax(EdgeWeightedDigraph G, int v)
	{
		for (DirectedEdge e : G.adj(v))
		{
			int w = e.to();
			if (shortestDistTo[w])
			if (this.distTo[v] + e.weight() < this.distTo[w])
			{
				this.distTo[w] = this.distTo[v] + e.weight();
				this.edgeTo[w] = e;
				if (this.pq.contains(w))
				{
					this.pq.decreaseKey(w, this.distTo[w]);
				}
				else
				{
					this.pq.insert(w, this.distTo[w]);
				}
			}
		}
	}

	public static void main(String[] args)
	{
        In in = new In(args[0]);
        int s = Integer.parseInt(args[1]);
        int t = Integer.parseInt(args[2]);
        EdgeWeightedDigraph G = new EdgeWeightedDigraph(in);

        OneSkipableSP sp = new OneSkipableSP(G, s, t);

        if (sp.hasPathTo(t)) {
            StdOut.printf("%d to %d (%5.2f)  ", s, v, sp.distTo(v));
            for (DirectedEdge e : sp.path(v)) {
                StdOut.print(e + "   ");
            }
            StdOut.println();
        }
        else {
            StdOut.printf("%d to %d           no path\n", s, v);
        }
    }
}