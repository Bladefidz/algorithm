import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.UF;
import edu.princeton.cs.algs4.Edge;
import edu.princeton.cs.algs4.EdgeWeightedGraph;
import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.Queue;

/**
 * Given an weighted-directed graph.
 * The bottleneck capacity is weight of largest edge in spanning tree.
 * A minimum bottleneck spanning tree is
 * a spanning tree of minimum bottleneck capacity.
 *
 * Computing MST using prim's algorithm always choose shortest edge to
 * connecting two vertices.
 * Checking for crossing edge optimality, always found the shortest crossing
 * edge which connected set of vertices.
 */
public class BottleneckMST
{
	private IndexMinPQ<Double> pq;
	private Edge[] edgeTo;
	private double bottleneckCapacity;
	private double minWeightMst;
	private double minWeightCrossingCut;

	BottleneckMST(EdgeWeightedGraph G)
	{
		this.pq = new IndexMinPQ<Double>(G.V());
		this.bottleneckCapacity = 0.0;
		this.minWeightMst = Double.POSITIVE_INFINITY;
		this.minWeightCrossingCut = Double.POSITIVE_INFINITY;

		edgeTo = new Edge[G.V()];
		double[] weightTo = new double[G.V()];
		boolean[] marked = new boolean[G.V()];

		for (int v = 0; v < G.V(); v++)
			weightTo[v] = Double.POSITIVE_INFINITY;

		// Prim algorithm
		for (int s = 0; s < G.V(); s++)
		{
			if (!marked[s])
			{
				weightTo[s] = 0.0;
				this.pq.insert(s, weightTo[s]);
				while (!this.pq.isEmpty())
				{
					int v = this.pq.delMin();
					marked[v] = true;
					for (Edge e : G.adj(v))
					{
						int w = e.other(v);
						if (marked[w]) continue;
						if (e.weight() > this.bottleneckCapacity)
						{
							this.bottleneckCapacity = e.weight();
						}
						if (e.weight() < weightTo[w])
						{   // Always choose an edge with lowest weight
							weightTo[w] = e.weight();
							edgeTo[w] = e;
							if (e.weight() < this.minWeightMst)
							{
								this.minWeightMst = e.weight();
							}
							if (this.pq.contains(w))
							{
								this.pq.decreaseKey(w, weightTo[w]);
							} else {
								this.pq.insert(w, weightTo[w]);
							}
						}
					}
				}
			}
		}

		// Find minimum weight in overall to test minimum weight of cut edge
		for (Edge e : this.edges())
		{
			UF uf = new UF(G.V());
			for (Edge f : this.edges())
			{
				int x = f.either();
				int y = f.other(x);
				if (f != e)
				{
					uf.union(x, y);
				}
			}

			for (Edge f : G.edges())
			{
				int x = f.either();
				int y = f.other(x);
				if (!uf.connected(x, y))
				{
					if (f.weight() < e.weight())
					{
						if (f.weight() < this.minWeightCrossingCut)
						{
							this.minWeightCrossingCut = f.weight();
						}
					}
				}
			}
		}
	}

	public Iterable<Edge> edges() {
        Queue<Edge> mst = new Queue<Edge>();
        for (int v = 0; v < this.edgeTo.length; v++) {
            Edge e = this.edgeTo[v];
            if (e != null) {
                mst.enqueue(e);
            }
        }
        return mst;
    }

	public double bottleneckCapacity()
	{
		return this.bottleneckCapacity;
	}

	public double minWeightMst()
	{
		return this.minWeightMst;
	}

	/**
	 * Proof that every MST is minimum bottleneck spanning tree
	 * if only if minimum weight in mst equal to minimum weight in crossing
	 * cut edges.
	 */
	public double minWeightCrossingCut()
	{
		if (this.minWeightCrossingCut == Double.POSITIVE_INFINITY)
		{
			this.minWeightCrossingCut = this.minWeightMst;
		}
		return this.minWeightCrossingCut;
	}

	/* Testing */
	public static void main(String[] args)
	{
        In in = new In(args[0]);
        EdgeWeightedGraph G = new EdgeWeightedGraph(in);
        BottleneckMST bmst = new BottleneckMST(G);
        StdOut.println("Bottleneck capacity: " + bmst.bottleneckCapacity());
        StdOut.println("Minimum weight in mst: " + bmst.minWeightMst());
        StdOut.println("Minimum weight in crossing cut: " + bmst.minWeightCrossingCut());
    }
}