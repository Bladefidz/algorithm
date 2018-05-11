import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.FlowEdge;
import edu.princeton.cs.algs4.FlowNetwork;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.Graph;
import edu.princeton.cs.algs4.BipartiteX;

public class TestBipartiteMatching
{
    private static final int UNMATCHED = -1;

    private final int V;                 // number of vertices in the graph
    private BipartiteX bipartition;      // the bipartition
    private int cardinality;             // cardinality of current matching
    private int[] mate;                  // mate[v] =  w if v-w is an edge in current matching
                                         //         = -1 if v is not in current matching
    private boolean[] inMinVertexCover;  // inMinVertexCover[v] = true iff v is in min vertex cover
    private boolean[] marked;            // marked[v] = true iff v is reachable via alternating path
    private int[] edgeTo;                // edgeTo[v] = w if v-w is last edge on path to w

    /**
     * Determines a maximum matching (and a minimum vertex cover)
     * in a bipartite graph.
     *
     * @param  G the bipartite graph
     * @throws IllegalArgumentException if {@code G} is not bipartite
     */
    public TestBipartiteMatching(Graph G) {
        bipartition = new BipartiteX(G);
        if (!bipartition.isBipartite()) {
            throw new IllegalArgumentException("graph is not bipartite");
        }

        this.V = G.V();

        // initialize empty matching
        mate = new int[V];
        for (int v = 0; v < V; v++)
            mate[v] = UNMATCHED;

        // alternating path algorithm
        while (hasAugmentingPath(G)) {

            // find one endpoint t in alternating path
            int t = -1;
            for (int v = 0; v < G.V(); v++) {
                if (!isMatched(v) && edgeTo[v] != -1) {
                    t = v;
                    break;
                }
            }

            // update the matching according to alternating path in edgeTo[] array
            for (int v = t; v != -1; v = edgeTo[edgeTo[v]]) {
                int w = edgeTo[v];
                mate[v] = w;
                mate[w] = v;
            }
            cardinality++;
        }

        // find min vertex cover from marked[] array
        inMinVertexCover = new boolean[V];
        for (int v = 0; v < V; v++) {
            if (bipartition.color(v) && !marked[v]) inMinVertexCover[v] = true;
            if (!bipartition.color(v) && marked[v]) inMinVertexCover[v] = true;
        }

        assert certifySolution(G);
    }


    /*
     * is there an augmenting path?
     *   - if so, upon termination adj[] contains the level graph;
     *   - if not, upon termination marked[] specifies those vertices reachable via an alternating
     *     path from one side of the bipartition
     *
     * an alternating path is a path whose edges belong alternately to the matching and not
     * to the matching
     *
     * an augmenting path is an alternating path that starts and ends at unmatched vertices
     *
     * this implementation finds a shortest augmenting path (fewest number of edges), though there
     * is no particular advantage to do so here
     */
    private boolean hasAugmentingPath(Graph G) {
        marked = new boolean[V];

        edgeTo = new int[V];
        for (int v = 0; v < V; v++)
            edgeTo[v] = -1;

        // breadth-first search (starting from all unmatched vertices on one side of bipartition)
        Queue<Integer> queue = new Queue<Integer>();
        for (int v = 0; v < V; v++) {
            if (bipartition.color(v) && !isMatched(v)) {
                queue.enqueue(v);
                marked[v] = true;
            }
        }

        // run BFS, stopping as soon as an alternating path is found
        while (!queue.isEmpty()) {
            int v = queue.dequeue();
            for (int w : G.adj(v)) {

                // either (1) forward edge not in matching or (2) backward edge in matching
                if (isResidualGraphEdge(v, w) && !marked[w]) {
                    edgeTo[w] = v;
                    marked[w] = true;
                    if (!isMatched(w)) return true;
                    queue.enqueue(w);
                }
            }
        }

        return false;
    }

    // is the edge v-w a forward edge not in the matching or a reverse edge in the matching?
    private boolean isResidualGraphEdge(int v, int w) {
        if ((mate[v] != w) &&  bipartition.color(v)) return true;
        if ((mate[v] == w) && !bipartition.color(v)) return true;
        return false;
    }

    /**
     * Returns the vertex to which the specified vertex is matched in
     * the maximum matching computed by the algorithm.
     *
     * @param  v the vertex
     * @return the vertex to which vertex {@code v} is matched in the
     *         maximum matching; {@code -1} if the vertex is not matched
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     *
     */
    public int mate(int v) {
        validate(v);
        return mate[v];
    }

    /**
     * Returns true if the specified vertex is matched in the maximum matching
     * computed by the algorithm.
     *
     * @param  v the vertex
     * @return {@code true} if vertex {@code v} is matched in maximum matching;
     *         {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     *
     */
    public boolean isMatched(int v) {
        validate(v);
        return mate[v] != UNMATCHED;
    }

    /**
     * Returns the number of edges in a maximum matching.
     *
     * @return the number of edges in a maximum matching
     */
    public int size() {
        return cardinality;
    }

    /**
     * Returns true if the graph contains a perfect matching.
     * That is, the number of edges in a maximum matching is equal to one half
     * of the number of vertices in the graph (so that every vertex is matched).
     *
     * @return {@code true} if the graph contains a perfect matching;
     *         {@code false} otherwise
     */
    public boolean isPerfect() {
        return cardinality * 2 == V;
    }

    /**
     * Returns true if the specified vertex is in the minimum vertex cover
     * computed by the algorithm.
     *
     * @param  v the vertex
     * @return {@code true} if vertex {@code v} is in the minimum vertex cover;
     *         {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public boolean inMinVertexCover(int v) {
        validate(v);
        return inMinVertexCover[v];
    }

    private void validate(int v) {
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }

    /**************************************************************************
     *
     *  The code below is solely for testing correctness of the data type.
     *
     **************************************************************************/

    // check that mate[] and inVertexCover[] define a max matching and min vertex cover, respectively
    private boolean certifySolution(Graph G) {

        // check that mate(v) = w iff mate(w) = v
        for (int v = 0; v < V; v++) {
            if (mate(v) == -1) continue;
            if (mate(mate(v)) != v) return false;
        }

        // check that size() is consistent with mate()
        int matchedVertices = 0;
        for (int v = 0; v < V; v++) {
            if (mate(v) != -1) matchedVertices++;
        }
        if (2*size() != matchedVertices) return false;

        // check that size() is consistent with minVertexCover()
        int sizeOfMinVertexCover = 0;
        for (int v = 0; v < V; v++)
            if (inMinVertexCover(v)) sizeOfMinVertexCover++;
        if (size() != sizeOfMinVertexCover) return false;

        // check that mate() uses each vertex at most once
        boolean[] isMatched = new boolean[V];
        for (int v = 0; v < V; v++) {
            int w = mate[v];
            if (w == -1) continue;
            if (v == w) return false;
            if (v >= w) continue;
            if (isMatched[v] || isMatched[w]) return false;
            isMatched[v] = true;
            isMatched[w] = true;
        }

        // check that mate() uses only edges that appear in the graph
        for (int v = 0; v < V; v++) {
            if (mate(v) == -1) continue;
            boolean isEdge = false;
            for (int w : G.adj(v)) {
                if (mate(v) == w) isEdge = true;
            }
            if (!isEdge) return false;
        }

        // check that inMinVertexCover() is a vertex cover
        for (int v = 0; v < V; v++)
            for (int w : G.adj(v))
                if (!inMinVertexCover(v) && !inMinVertexCover(w)) return false;

        return true;
    }

    /**
     * Unit tests the {@code HopcroftKarp} data type.
     * Takes three command-line arguments {@code V1}, {@code V2}, and {@code E};
     * creates a random bipartite graph with {@code V1} + {@code V2} vertices
     * and {@code E} edges; computes a maximum matching and minimum vertex cover;
     * and prints the results.
     *
     * @param args the command-line arguments
     */
    public static void main(String[] args) {
        In in = new In(args[0]);
        Graph G = new Graph(in);

        if (G.V() < 1000) StdOut.println(G);

        StdOut.println("Trace:");
        TestBipartiteMatching matching = new TestBipartiteMatching(G);
        StdOut.println();

        // print maximum matching
        StdOut.printf("Number of edges in max matching        = %d\n", matching.size());
        StdOut.printf("Number of vertices in min vertex cover = %d\n", matching.size());
        StdOut.printf("Graph has a perfect matching           = %b\n", matching.isPerfect());
        StdOut.println();

        if (G.V() >= 1000) return;

        StdOut.print("Max matching: ");
        for (int v = 0; v < G.V(); v++) {
            int w = matching.mate(v);
            if (matching.isMatched(v) && v < w)  // print each edge only once
                StdOut.print(v + "-" + w + " ");
        }
        StdOut.println();

        // print minimum vertex cover
        StdOut.print("Min vertex cover: ");
        for (int v = 0; v < G.V(); v++)
            if (matching.inMinVertexCover(v))
                StdOut.print(v + " ");
        StdOut.println();
    }

}