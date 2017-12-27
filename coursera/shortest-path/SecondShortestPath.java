import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.DirectedEdge;
import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.EdgeWeightedDigraph;
import edu.princeton.cs.algs4.Stack;

/**
 * Finding possible second shortest path in E*log*V time.
 *
 * let v is goal vertex.
 * Let u is a vertex which has in-degree more than zero and lead next to the
 * goal.
 * Let t is vertex connected to t in shortest path.
 * Find any path to v which not included t.
 */
class SecondShortestPath
{
	private DirectedEdge[] edgeTo;
	private double[] distTo;
	private IndexMinPQ<Double> pq;
	private int pathToGoalCnt;
	private int goal;
	private boolean foundSecondPath;

	SecondShortestPath(EdgeWeightedDigraph G, int s, int t)
	{
		this.edgeTo = new DirectedEdge[G.V()];
		this.distTo = new double[G.V()];
		this.pq = new IndexMinPQ<Double>(G.V());
		this.pathToGoalCnt = 0;
		this.goal = t;

		for (int v = 0; v < G.V(); v++)
			this.distTo[v] = Double.POSITIVE_INFINITY;

		this.distTo[s] = 0.0;
		this.pq.insert(s, distTo[s]);
		while (!this.pq.isEmpty())
		{
			for (DirectedEdge e : G.adj(this.pq.delMin()))
			{
				relax(G, e);
				if (e.to() == this.goal) break;
			}
		}

		int pivot = t;
		while (pivot != s)
		{
			if (G.indegree(pivot) > 1)
				break;
			DirectedEdge e = this.edgeTo[pivot];
			pivot = e.from();
		}

		this.foundSecondPath = false;
		if (pivot != s)
		{
			this.foundSecondPath = true;
			int forbid = this.edgeTo[pivot].from();
			this.pq = new IndexMinPQ<Double>(G.V());

			for (int v = 0; v < G.V(); v++)
				this.distTo[v] = Double.POSITIVE_INFINITY;

			this.distTo[s] = 0.0;
			this.pq.insert(s, distTo[s]);
			while (!this.pq.isEmpty())
			{
				for (DirectedEdge e : G.adj(this.pq.delMin()))
				{
					if (e.to() != forbid)
						relax(G, e);
					if (e.to() == this.goal) break;
				}
			}
		}
	}

	private void relax(EdgeWeightedDigraph G, DirectedEdge e)
	{
		int v = e.from();
		int w = e.to();
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
		if (!this.foundSecondPath) return null;
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
        SecondShortestPath ssp = new SecondShortestPath(G, s, t);
        if (ssp.hasPathTo(t))
        {
        	StdOut.printf("%d to %d (%5.2f)  ", s, t, ssp.distTo(t));
        	for (DirectedEdge e : ssp.path())
        		StdOut.print(e + "   ");
        	StdOut.println();
        }
        else
        {
        	StdOut.printf("%d to %d           no second path\n", s, t);
        }
    }
}