import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.SET;

public class BoggleSolver
{
	private SET<String> matched;
	private BoggleBoard board;
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
	private boolean[][] marked;
	private int wordId;
	private int maxDepth;

	// Initializes the data structure using the given array of strings as the dictionary.
	// (You can assume each word in the dictionary contains only the uppercase letters A through Z.)
	public BoggleSolver(String[] dictionary)
	{
		wordId = 0;
		for (String word : dictionary) {
			if (word.length() > 2) {
				if (word.charAt(word.length()-1) != 'Q')
					addWord(word);
			}
		}
	}

	// Returns the set of all valid words in the given Boggle board, as an Iterable.
	public Iterable<String> getAllValidWords(BoggleBoard board)
	{
		matched = new SET<>();

		if (root == null) return matched;

		this.board = board;
		maxDepth = board.rows() * board.cols();

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

		return matched;
	}

	// Returns the score of the given word if it is in the dictionary, zero otherwise.
	// (You can assume the word contains only the uppercase letters A through Z.)
	public int scoreOf(String word)
	{
		if (!contains(word)) return 0;
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

	private Node get(Node x, String key, int d)
	{
		if (x == null) return null;
		if (d == key.length()) return x;
		char c = key.charAt(d);
		if (c == 'Q') {
			if (d < key.length()-1) {
				return get(x.next[toInt26(c)], key, d+2);
			}
			return null;
		}
		return get(x.next[toInt26(c)], key, d+1);
	}

	private boolean contains(String key)
	{
		if (key == null) throw new IllegalArgumentException("argument to contains() is null");
        Node x = get(root, key, 0);
        if (x == null) return false;
        return x.id != null;
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

	private void addWord(String key)
	{
		if (key == null)
			throw new IllegalArgumentException("calls addWord() with null argument");
		root = addWord(root, key, 0);
	}

	private Node addWord(Node x, String key, int d)
	{
		if (x == null) {
			x = new Node();
		}
		if (d >= key.length()) {
			if (x.id == null) {
				// StdOut.println(key);
				x.id = wordId++;
			}
			return x;
		}

		int cur = toInt26(key.charAt(d));
		if (cur == -1)
			throw new IllegalArgumentException(
				"String contains non alphabet character");
		if (cur == 16) {
			if (d < key.length()-1) {
				int u = toInt26(key.charAt(d+1));
				if (u == 20) {
					x.next[cur] = addWord(x.next[cur], key, d+2);
				}
			}
			return x;
		}

		// StdOut.print(key.charAt(d));
		x.next[cur] = addWord(x.next[cur], key, d+1);
		return x;
	}

	private void dfs(int r, int c, Node x, StringBuilder w)
	{
		if (x == null) return;
		if (x.id != null && w.length() > 2) {
			if (w.charAt(w.length() - 1) == 'Q') {
				w.append('U');
				matched.add(w.toString());
				w.deleteCharAt(w.length() - 1);
			} else {
				matched.add(w.toString());
			}
		}

		marked[r][c] = true;

		for (int i = 0; i < moveRowCol.length; i++) {
			int[] more = IfRowColMore[i];
			if (r > more[0] && c > more[1]) {
				int[] less = ifRowColLess[i];
				if (r < board.rows()+less[0] && c < board.cols()+less[1]) {
					int[] rw = moveRowCol[i];
					int nr = r+rw[0];
					int nc = c+rw[1];
					if (!marked[nr][nc]) {
						char curChar = board.getLetter(r, c);
						char nextChar = board.getLetter(nr, nc);
						if (curChar == 'Q')
							w.append('U');
						w.append(nextChar);
						dfs(nr, nc, x.next[toInt26(nextChar)], w);
						w.deleteCharAt(w.length() - 1);
						if (curChar == 'Q')
							w.deleteCharAt(w.length() - 1);
						// StdOut.println(w.toString());
					}
				}
			}
		}

		marked[r][c] = false;
	}

	public static void main(String[] args)
	{
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

		// // Dictionary checking
		// StdOut.println("Original words: " + dictionary.length);
		// StdOut.println("Valid words: " + solver.wordId);
		// int m = 0;
		// for (String word : dictionary) {
		// 	if (word.length() > 2) {
		// 		if (solver.contains(word)) m += 1;
		// 		// else StdOut.println(word);
		// 		// StdOut.println(word);
		// 	}
		// }
		// StdOut.print("Verified valid words: ");
		// StdOut.println(m);
	}
}