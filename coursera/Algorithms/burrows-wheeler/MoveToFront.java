import edu.princeton.cs.algs4.BinaryStdOut;
import edu.princeton.cs.algs4.BinaryStdIn;

/**
 * Move Front Encoding
 *
 * n = number of input
 * R = number of alphabets
 *
 * Running time (encode & decode)
 * Worst case 	= Proportional to nR
 * Typical		= Proportional to n + R
 *
 * Memory usage (encode & decoding)
 * Worst case 	= Proportional to n + R
 */
public class MoveToFront
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

	public MoveToFront() {}

	// apply move-to-front encoding, reading from standard input and writing to standard output
	public static void encode()
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

		// BinaryStdOut.write(input.length);

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

	// apply move-to-front decoding, reading from standard input and writing to standard output
	public static void decode()
	{
		Node root = new Node((char) 0, null);
		Node x = root;
		for (int i = 1; i < R; i++) {
			Node y = new Node((char) i, null);
			x.next = y;
			x = x.next;
		}
		x = null;

		// int length = BinaryStdIn.readInt();
		while (!BinaryStdIn.isEmpty()) {
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

	// if args[0] is '-', apply move-to-front encoding
	// if args[0] is '+', apply move-to-front decoding
	public static void main(String[] args)
	{
		if      (args[0].equals("-")) encode();
		else if (args[0].equals("+")) decode();
	}
}