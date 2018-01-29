import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.SET;

public class BoggleSolver
{
	private BoggleBoard board;
	private SET<String> matched;
	private Node root;
	private static final int R = 26;  // Uppercase / Lowercase ASCII
	private int[][] IfRowColMore = {
		{ 0, 0}, { 0, -1}, { 0, -1},
		{-1, 0}, 		   {-1, -1},
		{-1, 0}, {-1, -1}, {-1, -1}
	};
	private int[][] ifRowColLess = {
		{ 0, 0}, { 0, 0}, { 0, -1},
		{ 0, 0}, 		  { 0, -1},
		{-1, 0}, {-1, 0}, {-1, -1}
	};
	private int[][] moveRowCol = {
		{-1, -1}, {-1, 0}, {-1, 1},
		{ 0, -1},          { 0, 1},
		{ 1, -1}, { 1, 0}, { 1, 1}
	};
	boolean[][] marked;
	private int wordId;

	// Initializes the data structure using the given array of strings as the dictionary.
	// (You can assume each word in the dictionary contains only the uppercase letters A through Z.)
	public BoggleSolver(String[] dictionary)
	{
		wordId = 0;
		matched = new SET<>();
		for (int i=0; i<dictionary.length; i++) {
			if (dictionary[i].length() > 2) {
				addWord(dictionary[i]);
				// StdOut.println();
			}
		}
	}

	// Returns the set of all valid words in the given Boggle board, as an Iterable.
	public Iterable<String> getAllValidWords(BoggleBoard board)
	{
		if (matched.isEmpty()) {
			this.board = board;

			for (int r = 0; r < board.rows(); r++) {
				for (int c = 0; c < board.cols(); c++) {
					marked = new boolean[board.rows()][board.cols()];
					char letter = board.getLetter(r, c);
					Node node = root.next[toInt26(letter)];
					StringBuilder word = new StringBuilder();
					word.append(letter);
					dfs(r, c, node, word);
					// StdOut.println();
				}
			}
			marked = null;

			if (matched.isEmpty()) {
				matched.add("");
			}
		}
		return matched;
	}

	// Returns the score of the given word if it is in the dictionary, zero otherwise.
	// (You can assume the word contains only the uppercase letters A through Z.)
	public int scoreOf(String word)
	{
		if (word.length() < 3) return 0;
		if (word.length() < 5) return 1;
		if (word.length() < 6) return 2;
		if (word.length() < 7) return 3;
		if (word.length() < 8) return 5;
		return 11;
	}

	private static class Node
	{
		private Integer id;
		private Node[] next;

		Node() {
			next = new Node[R];
		}
	}

	private int toInt26(char c)
	{
		int i = (int) c;
		if (i < 91 && i > 64)
			return i-65;
		if (i < 123 && i > 96)
			return i-97;
		throw new IllegalArgumentException("char " + c + "is not 26 alphabet char");
	}

	public void addWord(String key)
	{
		if (key == null)
			throw new IllegalArgumentException("calls addWord() with null argument");
		root = addWord(root, key, 0);

		// if (root == null) root = new Node();
		// Node x = root;
		// Node y = x;
		// int d = 0;
		// for (d = 0; d < key.length(); d++) {
		// 	int cur = toInt26(key.charAt(d));

		// 	if (cur == -1)
		// 		throw new IllegalArgumentException(
		// 			"String contains non alphabet character");
		// 	if (cur==20 && d>0) {
		// 		int prev = toInt26(key.charAt(d-1));
		// 		if (prev==16) {
		// 			if (d<key.length()-1) {
		// 				cur = toInt26(key.charAt(d+1));
		// 				// StdOut.print(key.charAt(d+1));
		// 				Node next = x.next[cur];
		// 				if (next == null) next = new Node();
		// 				x = next;
		// 				d+=1;
		// 			}
		// 			continue;
		// 		}
		// 	}

		// 	// StdOut.print(key.charAt(d));
		// 	Node next = x.next[cur];
		// 	if (next == null) next = new Node();
		// 	x = next;
		// }
		// if (d >= key.length()) {
		// 	if (x.id == null) {
		// 		x.id = wordId++;
		// 	}
		// }
		// root = y;
		// checkRoot(key);
	}

	private Node addWord(Node x, String key, int d)
	{
		if (x == null) x = new Node();
		if (d >= key.length()) {
			if (x.id == null) {
				// StdOut.println(d);
				x.id = wordId++;
			}
			return x;
		}

		int cur = toInt26(key.charAt(d));
		if (cur == -1)
			throw new IllegalArgumentException(
				"String contains non alphabet character");
		if (cur==20 && d>0) {
			int prev = toInt26(key.charAt(d-1));
			if (prev==16) {
				if (d<key.length()-1) {
					cur = toInt26(key.charAt(d+1));
					x.next[cur] = addWord(x.next[cur], key, d+2);
				}
				return x;
			}
		}

		// StdOut.print(key.charAt(d));
		x.next[cur] = addWord(x.next[cur], key, d+1);
		return x;
	}

	private void dfs(int r, int c, Node x, StringBuilder w)
	{
		if (x == null) return;
		if (x.id != null) matched.add(w.toString());

		marked[r][c] = true;

		for (int i=0; i<moveRowCol.length; i++) {
			int[] more = IfRowColMore[i];
			if (r>more[0] && c>more[1]) {
				int[] less = ifRowColLess[i];
				if (r<board.rows()+less[0] && c<board.cols()+less[1]) {
					int[] rw = moveRowCol[i];
					int nr = r+rw[0];
					int nc = c+rw[1];
					if (!marked[nr][nc]) {
						// StdOut.println(
						// 	r + " " + c + " = " + nr + " " + nc);
						char ch = board.getLetter(nr, nc);
						w.append(ch);
						dfs(nr, nc, x.next[toInt26(ch)], w);
						w.deleteCharAt(w.length() - 1);
					}
				}
			}
		}

		marked[r][c] = false;
	}

	private void checkRoot(String key)
	{
		if (root == null) StdOut.print("Root is empty.");
		Node x = root;
		int d = 0;
		while(x!=null) {
			Node next = x.next[toInt26(key.charAt(d))];
			if (next != null) {
				StdOut.print(key.charAt(d));
			}
			x = next;
			d++;
		}
	}

	public static void main(String[] args) {
		In in = new In(args[0]);
		String[] dictionary = in.readAllStrings();
		BoggleSolver solver = new BoggleSolver(dictionary);
		BoggleBoard board = new BoggleBoard(args[1]);
		int score = 0;
		// int cnt = 0;
		for (String word : solver.getAllValidWords(board)) {
		    StdOut.println(word);
		    score += solver.scoreOf(word);
		    // cnt++;
		}
		StdOut.println("Score = " + score);
		// StdOut.println("Matched = " + cnt);
	}
}