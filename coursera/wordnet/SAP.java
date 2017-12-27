import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.Queue;
import java.lang.IllegalArgumentException;
import java.lang.IndexOutOfBoundsException;
import java.util.NoSuchElementException;
import java.util.Comparator;
import java.util.Iterator;

/**
 * Representation of Shortest Ancestral Path.
 * An ancestral path between two vertices v and w in a digraph is
 * a directed path from v to a common ancestor x, together with a directed
 * path from w to the same ancestor x.
 * A shortest ancestral path is an ancestral path of minimum total length.
 */
public class SAP
{
   private final Digraph digraph;

   // constructor takes a digraph (not necessarily a DAG)
   public SAP(Digraph G)
   {
      if (G == null)
      {
         throw new IllegalArgumentException("Digraph is null!");
      }

      this.digraph = new Digraph(G);
   }

   // Is vertex valid?
   private boolean isVertexValid(int v) {
      if (v < 0 || v >= this.digraph.V()) throw new IllegalArgumentException("Not valid vertex!");
      return true;
   }

   private boolean isVertexHasEdge(int v) {
      if (this.digraph.outdegree(v) == 0 && this.digraph.indegree(v) == 0) {
         return false;
      }
      return true;
   }

   // Are vertices null?
   private boolean isVerticesEmpty(Iterable<Integer> v) {
      if (v == null) throw new IllegalArgumentException("Vertices empty!");
      return false;
   }

   // Return [sap length, common parent]
   // Just avoid to be greedy, since this class accept cyclic digraph.
   // So, just walk until queue is empty and return cheapest path.
   private int[] bfs(Queue<Integer> q, boolean[] marked, int[] distTo) {
      // Length and parent. Both are -1 if no such path
      int[] sap = new int[2];
      sap[0] = -1;
      sap[1] = -1;

      int minLength = Integer.MAX_VALUE;
      while(!q.isEmpty()) {
         int v = q.dequeue();
         for (int w : this.digraph.adj(v)) {
            if (!marked[w]) {
               distTo[w] = distTo[v] + 1;
               marked[w] = true;
               q.enqueue(w);
            } else {
               int l = distTo[w] + distTo[v] + 1;
               if (l < minLength) {
                  sap[0] = l;
                  sap[1] = w;
                  minLength = l;
               }
            }
         }
      }

      return sap;
   }

   private class NodeStack {
      class Node {
         int val;
         Node next;
         Node(int v, Node n) {
            this.val = v;
            this.next = n;
         }
      }

      Node nodes;
      int size;

      NodeStack() {
         this.nodes = null;
         this.size = 0;
      }

      boolean isEmpty() {
         return this.size == 0;
      }

      void push(int v) {
         if (this.nodes == null) {
            this.nodes = new Node(v, null);
         } else {
            Node tmp = this.nodes;
            this.nodes = new Node(v, tmp);
         }
         this.size++;
      }

      int pop() {
         if (this.nodes == null) throw new IndexOutOfBoundsException(
            "List is empty!");
         Node tmp = this.nodes;
         int v = this.nodes.val;
         this.nodes = this.nodes.next;
         this.size--;
         tmp = null;
         return v;
      }
   }

   // Find ancestral path of two nodes
   private int[] ancestralPath(int v, int w) {
      Queue<Integer> qv = new Queue<>();
      qv.enqueue(v);
      Queue<Integer> qw = new Queue<>();
      qw.enqueue(w);
      return ancestralPath(qv, qw);
   }

   // Find ancestral path of more than two nodes
   // Assume that always given by acyclic digraph
   private int[] ancestralPath(Iterable<Integer> v, Iterable<Integer> w)
   {
      Queue<Integer> q = new Queue<>();
      boolean[] vmarked = new boolean[this.digraph.V()];
      int[] vdistTo = new int[this.digraph.V()];

      for (int x : v) {
         if (isVertexValid(x) && !vmarked[x]) {
            q.enqueue(x);
            vmarked[x] = true;
            vdistTo[x] = 0;
         }
      }

      int[] sapV = new int[2];
      sapV = bfs(q, vmarked, vdistTo);

      boolean[] wmarked = new boolean[this.digraph.V()];
      int[] wdistTo = new int[this.digraph.V()];
      for (int x : w) {
         if (isVertexValid(x) && !wmarked[x]) {
            q.enqueue(x);
            wmarked[x] = true;
            wdistTo[x] = 0;
         }
      }

      int[] sapW = new int[2];
      sapW = bfs(q, wmarked, wdistTo);

      int[] sapX = new int[2];
      sapX[0] = -1;
      sapX[1] = -1;
      for (int x = 0; x < this.digraph.V(); x++) {
         if (vmarked[x] && wmarked[x]) {
            int ndist = vdistTo[x] + wdistTo[x];
            if (sapX[0] == -1 || ndist < sapX[0]) {
               sapX[0] = ndist;
               sapX[1] = x;
            }
         }
      }
      return sapX;
   }

   // length of shortest ancestral path between v and w; -1 if no such path
   public int length(int v, int w)
   {
      if (!isVertexValid(v)) return -1;
      if (!isVertexValid(w)) return -1;

      if (v == w) return 0;

      if (!isVertexHasEdge(v)) return -1;
      if (!isVertexHasEdge(w)) return -1;

      return ancestralPath(v, w)[0];
   }

   // a common ancestor of v and w that participates in a shortest ancestral path; -1 if no such path
   public int ancestor(int v, int w)
   {
      if(!isVertexValid(v)) return -1;
      if(!isVertexValid(w)) return -1;

      if (v == w) return v;

      if (!isVertexHasEdge(v)) return -1;
      if (!isVertexHasEdge(w)) return -1;

      return ancestralPath(v, w)[1];
   }

   // length of shortest ancestral path between any vertex in v and any vertex in w; -1 if no such path
   public int length(Iterable<Integer> v, Iterable<Integer> w)
   {
      if(isVerticesEmpty(v)) return -1;
      if(isVerticesEmpty(w)) return -1;

      return ancestralPath(v, w)[0];
   }

   // a common ancestor that participates in shortest ancestral path; -1 if no such path
   public int ancestor(Iterable<Integer> v, Iterable<Integer> w)
   {
      if(isVerticesEmpty(v)) return -1;
      if(isVerticesEmpty(w)) return -1;

      return ancestralPath(v, w)[1];
   }

   // do unit testing of this class
   public static void main(String[] args)
   {
      In in = new In(args[0]);
      Digraph G = new Digraph(in);
      SAP sap = new SAP(G);

      // // Check digraph
      // StdOut.println(G);

      // Test for digraph
      while (!StdIn.isEmpty()) {
         int v = StdIn.readInt();
         int w = StdIn.readInt();
         int length   = sap.length(v, w);
         // StdOut.printf("length = %d\n", length);
         int ancestor = sap.ancestor(v, w);
         StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);
      }
   }
}