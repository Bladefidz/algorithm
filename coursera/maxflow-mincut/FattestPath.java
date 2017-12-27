import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.DirectedEdge;
import edu.princeton.cs.algs4.IndexMaxPQ;
import edu.princeton.cs.algs4.EdgeWeightedDigraph;
import edu.princeton.cs.algs4.Stack;

/**
 * Fattest Path is a path such that no other path has a higher bottleneck
 * capacity. While bottleneck capacity of the path is the minimum weight of an
 * edge in the path.
 * Given an edge-weighted graph and two vertices s and t, this algorithm
 * find a fattest path in E Log E time.
 */
public class FattestPath
{
	private DirectedEdge[] edgeTo;
	private double[] distTo;
	private IndexMaxPQ<Double> pq;
	private boolean noFattestPath;
	private int goal;

	FattestPath(EdgeWeightedDigraph G, int s, int t)
	{
		this.edgeTo = new DirectedEdge[G.V()];
		this.distTo = new double[G.V()];
		this.pq = new IndexMaxPQ<>(G.V());
		this.noFattestPath = false;
		this.goal = t;

		for (int v = 0; v < G.V(); v++)
            this.distTo[v] = Double.NEGATIVE_INFINITY;
        this.distTo[s] = 0.0;

        // Find maximum path
        this.pq.insert(s, distTo[s]);
        boolean stop = false;
        while (!this.pq.isEmpty())
        {
            int v = this.pq.delMax();
            for (DirectedEdge e : G.adj(v))
            {
                relax(e);
                if (e.to() == this.goal) stop = true; break;
            }
            if (stop) break;
        }
	}

	private void relax(DirectedEdge e)
	{
		int v = e.from(), w = e.to();
		Double dist = this.distTo[v] + e.weight();
        if (dist > this.distTo[w])
        {
            this.distTo[w] = dist;
            this.edgeTo[w] = e;
            if (this.pq.contains(w)) this.pq.increaseKey(w, this.distTo[w]);
            else                	 this.pq.insert(w, this.distTo[w]);
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

    public Iterable<DirectedEdge> path()
    {
        if (this.noFattestPath) return null;
		Stack<DirectedEdge> path = new Stack<DirectedEdge>();
		for (DirectedEdge e = this.edgeTo[this.goal];
			e != null;
			e = this.edgeTo[e.from()])
		{
			path.push(e);
		}
		return path;
    }

	public static void main(String[] args)
	{
		In in = new In(args[0]);
        int s = Integer.parseInt(args[1]);
        int t = Integer.parseInt(args[2]);
        EdgeWeightedDigraph G = new EdgeWeightedDigraph(in);
        FattestPath fp = new FattestPath(G, s, t);
        if (fp.hasPathTo(t))
        {
        	StdOut.printf("%d to %d (%5.2f)  ", s, t, fp.distTo(t));
        	for (DirectedEdge e : fp.path())
        		StdOut.print(e + "   ");
        	StdOut.println();
        }
        else
        {
        	StdOut.printf("%d to %d           fattest path not found\n", s, t);
        }
	}
}