import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.ST;
import edu.princeton.cs.algs4.SET;
import edu.princeton.cs.algs4.Queue;
import java.lang.IllegalArgumentException;
import java.util.NoSuchElementException;
import java.util.Comparator;
import java.util.Iterator;

/**
 * Built a wordnet representation from digraph.
 * Each vertex v is an integer that represents a synset, and each directed
 * edge v→w represents that w is a hypernym of v.
 * The wordnet digraph is a rooted DAG: it is acyclic and has one vertex
 * —the root— that is an ancestor of every other vertex. However, it is not
 * necessarily a tree because a synset can have more than one hypernym.
 */
public class WordNet
{
   private final ST<String, Queue<Integer>> invertedIndex;
   private final ST<Integer, String> index;
   private final SAP sap;

   private int countRoot(Digraph D) {
      int rootCnt = 0;
      for (int i = 0; i < D.V(); i++) {
         if (D.outdegree(i) == 0 && D.indegree(i) > 0) {
            rootCnt++;
         }
      }
      return rootCnt;
   }

   // constructor takes the name of the two input files
   public WordNet(String synsets, String hypernyms)
   {
      if (synsets == null)
      {
         throw new IllegalArgumentException("synsets is null!");
      }

      if (hypernyms == null)
      {
         throw new IllegalArgumentException("hypernyms is null!");
      }

      // Built invertedIndex and index of nouns and words
      this.invertedIndex = new ST<>();
      this.index = new ST<>();
      In in = new In(synsets);
      try
      {
         while (!in.isEmpty())
         {
            String line = in.readLine();
            String[] syn = line.split(",");
            Integer synId = Integer.parseInt(syn[0]);

            // Put to index symbol table
            this.index.put(synId, syn[1]);

            // Put to invertedIndex symbol table
            String[] word = syn[1].split(" ");
            for (int i = 0; i < word.length; i++)
            {
               if (!this.invertedIndex.contains(word[i]))
               {
                  Queue<Integer> synIdQ = new Queue<Integer>();
                  synIdQ.enqueue(synId);
                  this.invertedIndex.put(word[i], synIdQ);
               }
               else
               {
                  this.invertedIndex.get(word[i]).enqueue(synId);
               }
            }
         }
      }
      catch(NoSuchElementException e)
      {
         throw new IllegalArgumentException(e);
      }

      // Built a wordnet digraph
      boolean foundRootVertex = false;
      Digraph digraph = new Digraph(this.index.size());
      in = new In(hypernyms);
      try
      {
         while (!in.isEmpty())
         {
            String line = in.readLine();
            String[] hyps = line.split(",");

            // if (hyps.length == 1)
            // {
            //    foundRootVertex = true;
            // }

            Integer ancestor = Integer.parseInt(hyps[0]);
            int i = 1;
            for (; i < hyps.length; i++)
            {
               Integer child = Integer.parseInt(hyps[i]);
               digraph.addEdge(ancestor, child);
            }
         }
      }
      catch(NoSuchElementException e)
      {
         throw new IllegalArgumentException(e);
      }

      int rc = countRoot(digraph);
      if (rc == 0) throw new IllegalArgumentException("Not roted DAG!");
      if (rc > 1) throw new IllegalArgumentException("Have more than one root!");

      this.sap = new SAP(digraph);
   }

   // returns all WordNet nouns
   public Iterable<String> nouns()
   {
      return this.invertedIndex.keys();
   }

   // is the word a WordNet noun?
   public boolean isNoun(String word)
   {
      return this.invertedIndex.get(word) != null;
   }

   // distance between nounA and nounB (defined in SAP)
   public int distance(String nounA, String nounB)
   {
      if (nounA == null) throw new IllegalArgumentException(
         "Word " + nounA + " is not in digraph!");
      if (nounB == null) throw new IllegalArgumentException(
         "Word " + nounB + " is not in digraph!");

      return this.sap.length(
         this.invertedIndex.get(nounA), this.invertedIndex.get(nounB)
      );
   }

   // a synset (second field of synsets.txt) that is the common ancestor of nounA and nounB
   // in a shortest ancestral path (defined in SAP)
   public String sap(String nounA, String nounB)
   {
      if (nounA == null) throw new IllegalArgumentException(
         "Word " + nounA + " is not in digraph!");
      if (nounB == null) throw new IllegalArgumentException(
         "Word " + nounB + " is not in digraph!");

      return this.index.get(
         this.sap.ancestor(
            this.invertedIndex.get(nounA), this.invertedIndex.get(nounB)
         )
      );
   }

   // do unit testing of this class
   public static void main(String[] args)
   {
      WordNet wordnet = new WordNet(args[0], args[1]);

      // // Test for word lookup
      // while (!StdIn.isEmpty()) {
      //    String line = StdIn.readLine();
      //    for (int i : wordnet.invertedIndex.get(line)) {
      //       StdOut.println(i + " " + wordnet.index.get(i));
      //    }
      // }

      // Test SAP between two nouns
      while (!StdIn.isEmpty()) {
         String line = StdIn.readLine();
         String[] nouns = line.split(" ");
         StdOut.println("distance = " + wordnet.distance(nouns[0], nouns[1]));
         StdOut.println("parent = " + wordnet.sap(nouns[0], nouns[1]));
      }

      // // Show all available nouns
      // StdOut.println("Nouns in the wordnet:");
      // for (String noun : wordnet.nouns()) {
      //    StdOut.println(noun + " ");
      //    for (int id : wordnet.invertedIndex.get(noun)) {
      //       StdOut.print(id + " ");
      //    }
      //    StdOut.println();
      // }
   }
}