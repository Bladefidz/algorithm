/**
 * Design an algorithm to implement move-to-front encoding that
 * maintain alphabet of symbols in a list.
 * A symbol is encoded as the number of symbols that precede it in the list.
 * After encoding a symbol, move it to the front of the list.
 * Should takes logarithmic time in the worst case.
 */
import edu.princeton.cs.algs4.RedBlackBST;
import edu.princeton.cs.algs4.StdOut;

public class MTF
{
	// alphabet size of extended ASCII
	private static final int R = 256;

	private static class Node
	{
		private final char ch;
		private Node next;

		Node(char ch, Node next)
		{
			this.ch = ch;
			this.next = next;
		}
	}

	private MTF() {}

	public static void compress()
	{
		Node root = new Node((char) 0, null);
		Node x = root;
		for (int i = 1; i < R; i++) {
			Node y = new Node((char) i, null);
			x.next = y;
			x = x.next;
		}
		x = null;

		String s = BinaryStdIn.readString();
		char[] input = s.toCharArray();

		BinaryStdOut.write(input.length);
		for (int i = 0; i < input.length; i++) {
			int index = 0;

			// Find index
			boolean found = false;
			x = root;
			Node y = root;
			while (!found) {
				if (y.ch == input[i]) {
					found = true;
					// StdOut.println(index + " " + y.ch);
					BinaryStdOut.write((char) index);
					break;
				}
				Node z = y.next;
				x = y;
				y = z;
				index++;
			}

			// Move to front
			if (x.ch != y.ch) {
				x.next = y.next;
				Node z = root;
				root = y;
				y.next = z;
			}
		}
		// close output stream
		BinaryStdOut.close();
	}

	public static void expand()
	{
		Node root = new Node((char) 0, null);
		Node x = root;
		for (int i = 1; i < R; i++) {
			Node y = new Node((char) i, null);
			x.next = y;
			x = x.next;
		}
		x = null;

		int length = BinaryStdIn.readInt();
		for (int i = 0; i < length; i++) {
			int indexComp = BinaryStdIn.readInt(8);
			int index = 0;

			// Find index
			boolean found = false;
			x = root;
			Node y = root;
			while (!found) {
				if (index == indexComp) {
					found = true;
					BinaryStdOut.write(y.ch);
					break;
				}
				Node z = y.next;
				x = y;
				y = z;
				index++;
			}

			// Move to front
			if (x.ch != y.ch) {
				x.next = y.next;
				Node z = root;
				root = y;
				y.next = z;
			}
		}
		BinaryStdOut.close();
	}

	public static void main(String[] args) {
		if      (args[0].equals("-")) compress();
		else if (args[0].equals("+")) expand();
	}
}