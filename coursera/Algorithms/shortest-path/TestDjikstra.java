import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.DirectedEdge;
import edu.princeton.cs.algs4.EdgeWeightedDigraph;
import edu.princeton.cs.algs4.DijkstraSP;

class TestDjikstra
{
	public static void main(String[] args)
	{
        In in = new In(args[0]);
        int s = Integer.parseInt(args[1]);
        int t = Integer.parseInt(args[2]);
        EdgeWeightedDigraph G = new EdgeWeightedDigraph(in);

        DijkstraSP sp = new DijkstraSP(G, s);

        if (sp.hasPathTo(t)) {
            StdOut.printf("%d to %d (%5.2f)  ", s, t, sp.distTo(t));
            for (DirectedEdge e : sp.pathTo(t)) {
                StdOut.print(e + "   ");
            }
            StdOut.println();
        }
        else {
            StdOut.printf("%d to %d           no path\n", s, t);
        }
    }
}