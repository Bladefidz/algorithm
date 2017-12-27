import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import java.lang.IllegalArgumentException;

/**
 * Measuring the semantic relatedness of two nouns.
 * Semantic relatedness refers to the degree to which two concepts are related.
 *
 * Semantic relatedness of two wordnet nouns A and B:
 * 	distance(A, B) = min(distance(AncestralPath(A.vi, B.vi)))
 * 	where v is synset
 *
 * Outcast detection:
 *  Which noun is the least related to the others?
 *  let:
 *  	nouns = {Ai}
 *  	Ai.dist = sum(dist(Ai, Aj))
 *  outcast = At = max(Ai.dist)
 */
public class Outcast
{
	private final WordNet wordnet;

	// constructor takes a WordNet object
	public Outcast(WordNet wordnet)
	{
		this.wordnet = wordnet;
	}

	// given an array of WordNet nouns, return an outcast
	public String outcast(String[] nouns)
	{
		// return a noun At for which dt is maximum
		int maxDist = 0;
		String outcastStr = "";
		for (int i = 0; i < nouns.length; i++) {
			if (this.wordnet.isNoun(nouns[i])) {
				int dist = 0;

				for (int j = 0; j < nouns.length; j++) {
					dist += this.wordnet.distance(nouns[i], nouns[j]);
				}

				if (dist > maxDist) {
					maxDist = dist;
					outcastStr = nouns[i];
				}

				StdOut.println("Distance " + nouns[i] + " = " + dist);
			} else {
				throw new IllegalArgumentException(
					nouns[i] + " is not in the wordnet!");
			}
		}
		return outcastStr;
	}

	// see test client below
	public static void main(String[] args)
	{
		WordNet wordnet = new WordNet(args[0], args[1]);
	    Outcast outcast = new Outcast(wordnet);
	    for (int t = 2; t < args.length; t++) {
	        In in = new In(args[t]);
	        String[] nouns = in.readAllStrings();
	        StdOut.println(args[t] + ": " + outcast.outcast(nouns));
	    }
	}
}