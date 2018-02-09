import java.lang.IllegalArgumentException;
import java.lang.Math;
import java.util.NoSuchElementException;
import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;
// import edu.princeton.cs.algs4.MinPQ;
// import edu.princeton.cs.algs4.StdOut;

/**
 * Running time (Worst case)
 * transform()						: Proportional to n + R
 * inverseTransform()				: Proportional to n + R
 *
 * Memory usage
 * transform() + inverTransform() 	: Proportional to n + R
 */
public class BurrowsWheeler
{
	// alphabet size of extended ASCII
	private static final int R = 256;

	private static class NodeChar
	{
		private final char val;
		private NodeChar next = null;

		NodeChar(char val)
		{
			this.val = val;
		}
	}

	private static class NodeInt
	{
		private final int val;
		private NodeInt next = null;

		NodeInt(int val)
		{
			this.val = val;
		}
	}

	// private static class NodeCharFreq implements Comparable<NodeCharFreq>
	// {
	// 	private final char ch;
	// 	private final int freq;

	// 	NodeCharFreq(char ch, int freq)
	// 	{
	// 		this.ch = ch;
	// 		this.freq = freq;
	// 	}

	// 	public int compareTo(NodeCharFreq that) {
	// 		return this.freq - that.freq;
	// 	}
	// }

	private static class InvertedIndex
	{
		private Branch[] indexes;
		private NodeInt[] refRight;
		private NodeInt[] refLeft;
		private final int min;
		private final int max;

		private class Branch
		{
			private NodeInt left;
			private NodeInt right;

			Branch() {}
		}

		InvertedIndex(int min, int max)
		{
			this.min = min;
			this.max = max;
			int n = max - min + 1;
			indexes = new Branch[n];
			refRight = new NodeInt[n];
			refLeft = new NodeInt[n];
		}

		private void enqueueRight(int i, int val)
		{
			if (i < min || i > max)
				throw new IllegalArgumentException("Index out range!");
			int key = i - min;
			NodeInt newNode = new NodeInt(val);
			if (indexes[key] == null) {
				indexes[key] = new Branch();
				indexes[key].right = newNode;
				refRight[key] = indexes[key].right;
			} else if (indexes[key].right == null) {
				indexes[key].right = newNode;
				refRight[key] = indexes[key].right;
			} else {
				NodeInt next = new NodeInt(val);
				refRight[key].next = next;
				refRight[key] = next;
			}
		}

		private void enqueueLeft(int i, int val)
		{
			if (i < min || i > max)
				throw new IllegalArgumentException("Index out range!");
			int key = i - min;
			NodeInt newNode = new NodeInt(val);
			if (indexes[key] == null) {
				indexes[key] = new Branch();
				indexes[key].left = newNode;
				refLeft[key] = indexes[key].left;
			} else if (indexes[key].left == null) {
				indexes[key].left = newNode;
				refLeft[key] = indexes[key].left;
			} else {
				refLeft[key].next = newNode;
				refLeft[key] = newNode;
			}
		}

		private int dequeueRight(int i)
		{
			if (i < min || i > max)
				throw new IllegalArgumentException("Index out range!");
			int key = i - min;

			if (indexes[key] == null)
				throw new NoSuchElementException("Queue underflow!");
			if (indexes[key].right == null)
				return -1;

			int val = indexes[key].right.val;
			NodeInt old = indexes[key].right;
			indexes[key].right = indexes[key].right.next;
			old = null;

			return val;
		}

		private int dequeueLeft(int i)
		{
			if (i < min || i > max)
				throw new IllegalArgumentException("Index out range!");
			int key = i - min;

			if (indexes[key] == null)
				throw new NoSuchElementException("Queue underflow!");
			if (indexes[key].left == null)
				return -1;

			int val = indexes[key].left.val;
			NodeInt old = indexes[key].left;
			indexes[key].left = indexes[key].left.next;
			old = null;

			return val;
		}
	}

	public BurrowsWheeler() {}

	// apply Burrows-Wheeler transform, reading from standard input and writing to standard output
	public static void transform()
	{
		String s = BinaryStdIn.readString();
		char[] input = s.toCharArray();
		CircularSuffixArray csa = new CircularSuffixArray(s);

		// Construct t[] contains every last character in sorted suffix
		char[] t = new char[s.length()];
		int l = csa.length();
		for (int i = 0; i < l; i++) {
			int index = csa.index(i);
			if (index == 0)
				// Write index of first original string in sorted suffix
				BinaryStdOut.write(i);
			if (index == 0)
				t[i] = input[l-1];
			else
				t[i] = input[index-1];
		}

		// Write t[] to BinaryStdOut
		for (int i = 0; i < s.length(); i++)
			BinaryStdOut.write(t[i]);

		BinaryStdOut.close();
	}

	// apply Burrows-Wheeler inverse transform, reading from standard input and writing to standard output
	public static void inverseTransform()
	{
		// Read index of first original string in sorted suffix
		int first = BinaryStdIn.readInt();

		// Count character frequencies and
		// reconstruct list of last character of sorted suffix
		int[] count = new int[R+1];

		char c = BinaryStdIn.readChar();
		count[c+1]++;

		NodeChar root = new NodeChar(c);
		NodeChar x = root;
		int N = 1;
		int min = (int) c;
		int max = min;

		while (!BinaryStdIn.isEmpty()) {
			c = BinaryStdIn.readChar();

			count[c+1]++;
			N++;

			NodeChar y = new NodeChar(c);
			x.next = y;
			x = x.next;

			int m = (int) c;
			if (m > max) max = m;
			if (m < min) min = m;
		}

		x = null;
		x = root;
		InvertedIndex invIndex = new InvertedIndex(min, max);
		for (int i = 0; i < N; i++) {
			invIndex.enqueueRight(x.val, i);
			NodeChar y = x.next;
			x = y;
		}
		for (int r = 0; r < R; r++) {
			count[r+1] += count[r];
		}

		x = null;
		x = root;
		char[] sorted = new char[N];
		for (int i = 0; i < N; i++) {
			int j = count[x.val]++;
			sorted[j] = x.val;
			invIndex.enqueueLeft(x.val, j);
			NodeChar y = x.next;
			// x = null;
			x = y;
		}

		x = null;
		x = root;
		int[] next = new int[N];
		int toScan = -1;
		while (x != null) {
			int r = invIndex.dequeueRight(x.val);
			int l = invIndex.dequeueLeft(x.val);
			while(r != -1 || l != -1) {
				if (r == first) toScan = l;
				next[l] = r;
				r = invIndex.dequeueRight(x.val);
				l = invIndex.dequeueLeft(x.val);
			}
			NodeChar y = x.next;
			x = null;
			x = y;
		}

		if (toScan != -1) {
			for (int i = 0; i < N; i++) {
				BinaryStdOut.write(sorted[next[toScan]]);
				toScan = next[toScan];
			}
		}

		BinaryStdOut.close();
		// // Testing inverted index
		// StdOut.println("min: " + min + " max: " + max);
		// StdOut.println("Right Branch");
		// for (int i = min; i <= max; i++) {
		// 	int f = 0;
		// 	try {
		// 		f = invIndex.dequeueRight(i);
		// 	} catch (NoSuchElementException e) {
		// 		continue;
		// 	}
		// 	StdOut.print((char) i + " : " + f);
		// 	while(true) {
		// 		try {
		// 			f = invIndex.dequeueRight(i);
		// 			StdOut.print(" " + f);
		// 		} catch (NoSuchElementException e) {
		// 			break;
		// 		}
		// 	}
		// 	StdOut.println();
		// }
		// StdOut.println("Left Branch");
		// for (int i = min; i <= max; i++) {
		// 	int f = 0;
		// 	try {
		// 		f = invIndex.dequeueLeft(i);
		// 	} catch (NoSuchElementException e) {
		// 		continue;
		// 	}
		// 	StdOut.print((char) i + " : " + f);
		// 	while(true) {
		// 		try {
		// 			f = invIndex.dequeueLeft(i);
		// 			StdOut.print(" " + f);
		// 		} catch (NoSuchElementException e) {
		// 			break;
		// 		}
		// 	}
		// 	StdOut.println();
		// }
	}

	// if args[0] is '-', apply Burrows-Wheeler transform
	// if args[0] is '+', apply Burrows-Wheeler inverse transform
	public static void main(String[] args)
	{
		if 		  (args[0].equals("-")) {
			transform();
		} else if (args[0].equals("+")) {
			inverseTransform();
		} else throw new IllegalArgumentException("Illegal command line argument");
	}
}