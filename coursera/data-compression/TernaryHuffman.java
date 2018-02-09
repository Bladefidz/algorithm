import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.TST;
import edu.princeton.cs.algs4.StdOut;

/**
 * Generalize the Huffman algorithm to codewords over the ternary
 * alphabet (0, 1, and 2) instead of the binary alphabet.
 * Given a byte stream, find a prefix-free ternary code that uses as
 * few trits (0s, 1s, and 2s) as possible.
 *
 * Prove of optimality?
 * Lower than binary trie, but used extra a bit for each codeword to encode.
 */
public class TernaryHuffman
{
	private static final int R = 256;

	public TernaryHuffman() { }

	private static class Node implements Comparable<Node>
	{
		private final char ch;
		private final int freq;
		private final Node left, mid, right;

		Node(char ch, int freq, Node left, Node mid, Node right)
		{
			this.ch = ch;
			this.freq = freq;
			this.left = left;
			this.mid = mid;
			this.right = right;
		}

		private boolean isLeaf() {
			assert ((left == null) && (mid == null) && (right == null)) ||
				   ((left != null) && (mid == null) && (right != null));
			return (left == null) && (mid == null) && (right == null);
		}

		public int compareTo(Node that) {
			return this.freq - that.freq;
		}
	}

	public static void compress() {
		// read the input
		String s = BinaryStdIn.readString();
		char[] input = s.toCharArray();

		// tabulate frequency counts
		int[] freq = new int[R];
		for (int i = 0; i < input.length; i++)
			freq[input[i]]++;

		// build Huffman trie
		Node root = buildTrie(freq);

		// build code table
		String[] st = new String[R];
		buildCode(st, root, "");

		// print trie for decoder
		writeTrie(root);

		// print number of bytes in original uncompressed message
		BinaryStdOut.write(input.length);

		// use Huffman code to encode input
		for (int i = 0; i < input.length; i++) {
			String code = st[input[i]];
			for (int j = 0; j < code.length(); j++) {
				if (code.charAt(j) == '0') {
					BinaryStdOut.write(false);
					BinaryStdOut.write(false);
				}
				else if (code.charAt(j) == '1') {
					BinaryStdOut.write(false);
					BinaryStdOut.write(true);
				}
				else if (code.charAt(j) == '2') {
					BinaryStdOut.write(true);
					BinaryStdOut.write(false);
				}
				else throw new IllegalStateException("Illegal state");
			}
		}

		// close output stream
		BinaryStdOut.close();
	}

	private static Node buildTrie(int[] freq) {
		MinPQ<Node> pq = new MinPQ<Node>();
		for (char i = 0; i < R; i++)
			if (freq[i] > 0)
				pq.insert(new Node(i, freq[i], null, null, null));

		// merge three smallest trees
		while (pq.size() > 2) {
			Node left  = pq.delMin();
			Node mid = pq.delMin();
			Node right = pq.delMin();
			Node parent = new Node(
				'\0',
				left.freq + mid.freq + right.freq,
				left, mid, right);
			pq.insert(parent);
		}

		// Merge last one or if size < 3
		if (pq.size() > 1) {
			Node left  = pq.delMin();
			Node mid = pq.delMin();
			Node right = new Node('\0', 0, null, null, null);
			Node parent = new Node(
				'\0',
				left.freq + mid.freq,
				left, mid, right);
			pq.insert(parent);
		}
		return pq.delMin();
	}

	private static void writeTrie(Node x)
	{
		if (x.isLeaf()) {
			BinaryStdOut.write(true);
			BinaryStdOut.write(x.ch, 8);
			return;
		}
		BinaryStdOut.write(false);
		writeTrie(x.left);
		writeTrie(x.mid);
		writeTrie(x.right);
	}

	private static void buildCode(String[] st, Node x, String s)
	{
		if (!x.isLeaf()) {
			buildCode(st, x.left, s + '0');
			buildCode(st, x.mid, s + '1');
			buildCode(st, x.right, s + '2');
		} else {
			st[x.ch] = s;
		}
	}

	public static void expand()
	{
		Node root = readTrie();
		int length = BinaryStdIn.readInt();

		for (int i = 0; i < length; i++) {
			Node x = root;
			while (!x.isLeaf()) {
				int bit = BinaryStdIn.readInt(2);
				if 		(bit > 1) 	x = x.right;
				else if (bit == 1) 	x = x.mid;
				else 				x = x.left;
			}
			BinaryStdOut.write(x.ch, 8);
		}
		BinaryStdOut.close();
	}

	private static Node readTrie()
	{
		boolean isLeaf = BinaryStdIn.readBoolean();
		if (isLeaf) {
			return new Node(BinaryStdIn.readChar(), -1, null, null, null);
		} else {
			return new Node('\0', -1, readTrie(), readTrie(), readTrie());
		}
	}

	public static void codeword() {
		// read the input
		String s = BinaryStdIn.readString();
		char[] input = s.toCharArray();

		// tabulate frequency counts
		int[] freq = new int[R];
		for (int i = 0; i < input.length; i++)
			freq[input[i]]++;

		// build Huffman trie
		Node root = buildTrie(freq);

		// show code table
		showCode(root, "");
	}

	private static void showCode(Node x, String s)
	{
		if (!x.isLeaf()) {
			showCode(x.left, s + '0');
			showCode(x.mid, s + '1');
			showCode(x.right, s + '2');
		} else {
			if (x.ch == ' ')
				StdOut.println("SP   " + s);
			else if (x.ch == '\n')
				StdOut.println("LF   " + s);
			else if (x.ch == '\0')
				StdOut.println("EOF  " + s);
			else
				StdOut.println(x.ch + "    " + s);
		}
	}

	public static void size()
	{
		// read the input
		String s = BinaryStdIn.readString();
		char[] input = s.toCharArray();

		// tabulate frequency counts
		int[] freq = new int[R];
		for (int i = 0; i < input.length; i++)
			freq[input[i]]++;

		// build Huffman trie
		Node root = buildTrie(freq);

		// build code table
		String[] st = new String[R];
		buildCode(st, root, "");

		// print number of bytes in original uncompressed message
		BinaryStdOut.write(input.length);
	}

	public static void main(String[] args)
	{
		if (args[0].equals("-")) compress();
		else if (args[0].equals("+")) expand();
		else if (args[0].equals("--")) codeword();
		else if (args[0].equals("$")) size();
		else throw new IllegalArgumentException("Illegal command line!");
	}
}