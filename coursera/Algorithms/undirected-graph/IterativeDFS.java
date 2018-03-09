import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Graph;
import edu.princeton.cs.algs4.Stack;

/**
 * Implement depth-first search in an undirected graph without using recursion
 */
public class IterativeDFS
{
	private boolean[] marked;
	private int[] edgeTo;
	private int s;

	public IterativeDFS(Graph G, int s)
	{
		this.marked = new boolean[G.V()];
		validateVertex(s);
		dfs(G, s);
	}

	private void dfs(Graph G, int v)
	{
		Stack<Integer> sv = new Stack<Integer>();
		sv.push(v);
		while (!sv.isEmpty())
		{
			v = sv.pop();
			if (!this.marked[v])
            {
				// StdOut.println(v);  // Testing
				this.marked[v] = true;

				// Need to reserve adjacency order
				Stack<Integer> reverse = new Stack<Integer>();
				for (int w : G.adj(v))
				{
	            	if (!this.marked[w])
	            	{
	                	reverse.push(w);
	            	}
		        }

		        for (int w : reverse)
	            {
	                sv.push(w);
	            }
		    }
		}
	}

	private void validateVertex(int v)
	{
		int V = this.marked.length;
		if (v < 0 || v >= V)
			throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
	}

	public static void main(String[] args)
	{
		In in = new In(args[0]);
        Graph G = new Graph(in);
        int s = Integer.parseInt(args[1]);
        IterativeDFS idfs = new IterativeDFS(G, s);
        // for (int v = 0; v < G.V(); v++) {
        //     if (idfs.marked(v))
        //         StdOut.print(v + " ");
        // }
	}
}