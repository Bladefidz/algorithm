import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.GraphGenerator;
import edu.princeton.cs.algs4.FlowEdge;
import edu.princeton.cs.algs4.FlowNetwork;
import edu.princeton.cs.algs4.IndexMaxPQ;
import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.Edge;
import edu.princeton.cs.algs4.EdgeWeightedDigraph;
import edu.princeton.cs.algs4.DirectedEdge;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.SET;

/**
 * Closure here means any subset of vertices S in a digraph which has no edges
 * pointing from S to a vertex outside S.
 * This algorithm find a closed subset of vertices of maximum total weight
 * in edge-weighted digraph.
 * The maximum subset of vertices are two subset S and T divided by minimum
 * cover of vertices.
 * I love to correlate this problem to open pit mining:
 * http://riot.ieor.berkeley.edu/Applications/OPM/OPMDetails.html
 */
public class MaxClosure
{
	private static final double FLOATING_POINT_EPSILON = 1E-11;
    private final int V;
    private boolean[] marked;
    private FlowEdge[] edgeTo;
    private FlowNetwork GF;
    private double value;  // Value of maximum closure capacity

	MaxClosure(EdgeWeightedDigraph G, int s, int t)
	{
		prepareGF(G, s, t);
		this.V = G.V();

		this.value = excess(t);
		while (hasAugmentingPath(s, t))
		{
			double bottle = Double.POSITIVE_INFINITY;
			for (int v = t; v != s; v = this.edgeTo[v].other(v))
				bottle = Math.min(bottle,
					this.edgeTo[v].residualCapacityTo(v));
			for (int v = t; v != s; v = this.edgeTo[v].other(v))
				this.edgeTo[v].addResidualFlowTo(v, bottle);
			value += bottle;
		}
	}

	private void prepareGF(EdgeWeightedDigraph G, int s, int t)
	{
		this.GF = new FlowNetwork(G.V());
		SET<Edge> edgeSet = new SET<Edge>();

		for (DirectedEdge e : G.adj(s))
		{
			int v = e.from();
			int w = e.to();
			Edge ed = new Edge(v, w, 0.0);
			edgeSet.add(ed);
			FlowEdge fe = new FlowEdge(v, w, e.weight());
		}

		for (int ev = 0; ev < G.V(); ev++)
		{
			for (DirectedEdge e: G.adj(ev))
			{
				int v = e.from();

				if (v == s || v == t) continue;

				int w = e.to();
				Edge ed = new Edge(v, w, 0.0);
				if (!edgeSet.contains(ed))
				{
					edgeSet.add(ed);
					FlowEdge fe;
					if (w == t) fe = new FlowEdge(v, w, e.weight());
					else fe = new FlowEdge(v, w, Double.POSITIVE_INFINITY);

					this.GF.addEdge(fe);
				}
			}
		}
	}

	public double value()
	{
		return this.value;
	}

	/* Check if given vertex belong to min cut vertices */
	public boolean inCut(int v)
	{
		return this.marked[v];
	}

	/* Exceed flow = inflow - outflow */
	private double excess(int v)
	{
		double excess = 0.0;
		for (FlowEdge e : this.GF.adj(v))
		{
			if (v == e.from()) excess -= e.flow();
			else			   excess += e.flow();
		}
		return excess;
	}

	private boolean hasAugmentingPath(int s, int t)
	{
		this.edgeTo = new FlowEdge[this.V];
        this.marked = new boolean[this.V];

        Queue<Integer> queue = new Queue<Integer>();
        queue.enqueue(s);
        this.marked[s] = true;
        while(!queue.isEmpty() && !this.marked[t])
        {
        	int v = queue.dequeue();
        	for (FlowEdge e : this.GF.adj(v))
        	{
        		int w = e.other(v);
        		if (e.residualCapacityTo(w) > 0)
        		{
        			if (!this.marked[w])
        			{
        				this.edgeTo[w] = e;
        				this.marked[w] = true;
        				queue.enqueue(w);
        			}
        		}
        	}
        }

        return this.marked[t];
	}

	public Iterable<FlowEdge> maxFlow()
	{
		Queue<FlowEdge> flows = new Queue<>();
		for (int v = 0; v < this.GF.V(); v++)
		{
            for (FlowEdge e : this.GF.adj(v))
            {
                if ((v == e.from()) && e.flow() > 0)
                    flows.enqueue(e);
            }
        }
        return flows;
	}

	public static void main(String[] args)
	{
        // create flow network with V vertices and E edges
        In in = new In(args[0]);
        int s = Integer.parseInt(args[1]);
        int t = Integer.parseInt(args[2]);

        EdgeWeightedDigraph G = new EdgeWeightedDigraph(in);
        StdOut.println(G);

        MaxClosure maxClosure = new MaxClosure(G, s, t);
        StdOut.println("Max flow from " + s + " to " + t);
        for (FlowEdge fe : maxClosure.maxFlow())
        	StdOut.println("   " + fe);

        StdOut.print("Min cut: ");
        for (int v = 0; v < G.V(); v++) {
            if (maxClosure.inCut(v)) StdOut.print(v + " ");
        }
        StdOut.println();

        StdOut.println("Max flow value = " +  maxClosure.value());
    }
}