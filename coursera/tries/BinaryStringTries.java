import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * In data compression, a set of binary strings is if no string is a prefix of
 * another. For example, {01,10,0010,1111} is prefix free, but
 * {01,10,0010,10100} is not because 10 is a prefix of 10100. Design an
 * efficient algorithm to determine if a set of binary strings is prefix-free.
 * The running time of your algorithm should be proportional the number of
 * bits in all of the binary stings.
 *
 * Solution:
 * Build a binary tree.
 * Traversing all nodes until found a id correspond to key of string.
 * If found node with not null value and has not null link, then the binary
 * string is not prefix free.
 */
public class BinaryStringTries<Value>
{
	private Node root;
	private int N;
	private boolean isPrefixFree;

	private static class Node
	{
		private Object val;
		private Node[] next = new Node[2];  // Binary node
	}

	public BinaryStringTries()
	{
		N = 0;
		isPrefixFree = true;
	}

	private boolean isNodeNull(Node[] node)
	{
		for (int i = 0; i < 2; i++) {
			if (node[i] != null) return false;
		}
		return true;
	}

	public void put(String key, Value val)
	{
		if (key == null) throw new IllegalArgumentException("First argument to put() us null");
		if (val == null) delete(key);
		root = put(root, key, val, 0);
	}

	private Node put(Node x, String key, Value val, int d)
	{
		if (x == null) {
			x = new Node();
		}
		if (d == key.length()) {
			if (x.val == null) N++;
			if (isPrefixFree && !isNodeNull(x.next)) isPrefixFree = false;
			x.val = val;
			return x;
		}
		else {
			if (d > 0 && isPrefixFree && x.val != null) isPrefixFree = false;
		}
		int i = Character.getNumericValue(key.charAt(d));
		x.next[i] = put(x.next[i], key, val, d+1);
		return x;
	}

	private Node get(Node x, String key, int d)
	{
		if (x == null) return null;
		if (d == key.length()) return x;
		int i = Character.getNumericValue(key.charAt(d));
		return get(x.next[i], key, d+1);
	}

	public Value get(String key)
	{
		if (key == null) throw new IllegalArgumentException("Argument to get() is null.");
		Node x = get(root, key, 0);
		if (x == null) return null;
		return (Value) x.val;
	}

	public boolean contains(String key)
	{
		if (key == null) throw new IllegalArgumentException("Argument to contains() us null.");
		return get(key) != null;
	}

	public void delete(String key)
	{
		if (key == null) throw new IllegalArgumentException("Argument to delete() is null");
		root = delete(root, key, 0);
	}

	private Node delete(Node x, String key, int d)
	{
		if (x == null) return null;
		if (d == key.length()) {
			if (x.val != null) N--;
			x.val = null;
		}
		else {
			int i = Character.getNumericValue(key.charAt(d));
			x.next[i] = delete(x.next[i], key, d+1);
		}

		// Remove subtrie
		if (x.val != null) return x;
		for (int i = 0; i < 2; i++) {
			if (x.next[i] != null) return x;
		}
		return null;
	}

	public boolean isPrefixFree()
	{
		return isPrefixFree;
	}

	public static void main(String[] args)
	{
		BinaryStringTries<Integer> bst = new BinaryStringTries<>();
		In file = new In(args[0]);
		for (int i = 0; !file.isEmpty(); i++) {
			String key = file.readString();
			bst.put(key, i);
		}
		StdOut.print("Is prefix free? ");
		StdOut.print(bst.isPrefixFree());
		StdOut.println();
	}
}