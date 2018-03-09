import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.DirectedEdge;
import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.EdgeWeightedDigraph;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.DijkstraSP;

/**
 * Execution: java MonotonicSP <file> <source> <0 or 1>
 * Example	:
 * 		Increasing order: java MonotonicSP <file> <source> 1
 * 		Decreasing order: java MonotonicSP <file> <source> 0
 *
 * A path called as Monotonic if only if sequence of edge has weight that
 * either strictly increasing or strictly decreasing.
 * Design an E*log*V algorithm to find Monotonic Shortest Path in edge weighted
 * digraph.
 *
 * Use Dijkstra algorithm with modified edge relaxation.
 */
class MonotonicSP
{
	private DirectedEdge[] edgeTo;
	private double[] distTo;
	private IndexMinPQ<Double> pq;

	MonotonicSP(EdgeWeightedDigraph G, int s, int increasing)
	{
		this.edgeTo = new DirectedEdge[G.V()];
		this.distTo = new double[G.V()];
		this.pq = new IndexMinPQ<Double>(G.V());

		for (int v = 0; v < G.V(); v++)
			this.distTo[v] = Double.POSITIVE_INFINITY;
		this.distTo[s] = 0.0;
		this.pq.insert(s, distTo[s]);
		while (!this.pq.isEmpty())
			relax(G, this.pq.delMin(), increasing);
	}

	private void relax(EdgeWeightedDigraph G, int v, int increasing)
	{
		for (DirectedEdge e : G.adj(v))
		{
			if (this.distTo[v] + e.weight() < this.distTo[w])
			{
				boolean monotonic = false;
				if (distTo[v] == 0)
				{
					monotonic = true;
				}
				else
				{
					if (increasing > 0)
					{
						if (e.weight() >= this.edgeTo[v].weight())
						{
							monotonic = true;
						}
					}
					else
					{
						if (e.weight() <= this.edgeTo[v].weight())
						{
							monotonic = true;
						}
					}
				}

				if (monotonic)
				{
					int w = e.to();
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
	}

	public double distTo(int v)
	{
		return this.distTo[v];
	}

	public boolean hasPathTo(int v)
	{
		return this.distTo[v] < Double.POSITIVE_INFINITY;
	}

	public Iterable<DirectedEdge> monotonicTo(int v)
	{
		if (!hasPathTo(v)) return null;
		Stack<DirectedEdge> path = new Stack<DirectedEdge>();
		for (DirectedEdge e = this.edgeTo[v]; e != null; e = this.edgeTo[e.from()])
		{
			path.push(e);
		}
		return path;
	}

	public static void main(String[] args)
	{
        In in = new In(args[0]);
        int s = Integer.parseInt(args[1]);
        int increasing = Integer.parseInt(args[2]);
        EdgeWeightedDigraph G = new EdgeWeightedDigraph(in);

        MonotonicSP sp = new MonotonicSP(G, s, increasing);
        // DijkstraSP sp = new DijkstraSP(G, s);

        for (int v = 0; v < G.V(); v++) {
            if (sp.hasPathTo(v)) {
                StdOut.printf("%d to %d (%5.2f)  ", s, v, sp.distTo(v));
                for (DirectedEdge e : sp.monotonicTo(v)) {
                    StdOut.print(e + "   ");
                }
                StdOut.println();
            }
            else {
                StdOut.printf("%d to %d           no path\n", s, v);
            }
        }
    }
}