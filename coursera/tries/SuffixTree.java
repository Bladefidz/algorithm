import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.TST;
import edu.princeton.cs.algs4.Queue;

public class SuffixTree
{
	public static void main(String[] args)
	{
		In file = new In(args[0]);
		TST<Integer> tst = new TST<>();
		for (int i = 0; !file.isEmpty(); i++) {
			String key = file.readString();
			if (key.length() > 2) {
				tst.put(key, i);
			}
		}
	}
}