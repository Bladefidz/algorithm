import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.Graph;
import edu.princeton.cs.algs4.SET;

/**
 * Boggle is a word game played on an 4-by-4 grid of tiles, where each tile
 * contains one letter in the alphabet. The goal is to find all words in the
 * dictionary that can be made by following a path of adjacent tiles (with no
 * tile repeated), where two tiles are adjacent if they are horizontal,
 * vertical, or diagonal neighbors.
 *
 * Solution:
 * 1. Choose 16 random characters.
 * 2. Build undirected graph of 4x4 board from 16 random chosen character.
 * 3. Build a 26-way Tries as dictionary of known words.
 * 4. Run DFS in undirected graph while search on Tries.
 */
public class Boggle
{
	private char[] chars;
	private Graph G;
	boolean[] marked;
	private Node root;
	private static final int R = 26;  // Lowercase ASCII
	private SET<String> matchedWords;
	private int wordCount;

	private static class Node
	{
		private Integer id;
		private Node[] next = new Node[R];
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

	private char toLower(int i)
	{
		return (char) (97+i);
	}

	private void addWord(String key)
	{
		if (key == null) {
			throw new IllegalArgumentException("calls addWord() with null argument");
		}
		root = addWord(root, key, 0);
	}

	private Node addWord(Node x, String key, int d)
	{
		if (x == null) x = new Node();
		if (d == key.length()) {
			if (x.id == null) {
				x.id = wordCount++;
			}
			return x;
		}
		int i = toInt26(key.charAt(d));
		if (i == -1)
			throw new IllegalArgumentException(
				"String contains non alphabet character");
		x.next[i] = addWord(x.next[i], key, d+1);
		return x;
	}

	private void processFile(In file)
	{
		while (!file.isEmpty()) {
			String key = file.readString();
			if (key.length() > 2)
				addWord(key);
		}
	}

	public int wordCount()
	{
		return wordCount;
	}

	public Boggle(In file)
	{
		generateRandomChars();
		G = generateGraph();
		processFile(file);
	}

	public Boggle(char[] charArray, In file)
	{
		if (charArray.length != 16)
			throw new IllegalArgumentException("First argument of constructor chars should has length exactly 16");
		chars = charArray;
		G = generateGraph();
		processFile(file);
	}

	private void generateRandomChars()
	{
		for (int i = 0; i < 16; i++)
			chars[i] = (char) (97+StdRandom.uniform(26));
	}

	private Graph generateGraph()
	{
		Graph G = new Graph(16);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				int edge = i*4+j;
				if (j < 3) G.addEdge(edge, edge+1);
				if (i < 3) {
					if (j > 0) G.addEdge(edge, edge+3);
					G.addEdge(edge, edge+4);
					if (j < 3) G.addEdge(edge, edge+5);
				}
			}
		}
		return G;
	}

	public void printBoardGraph()
	{
		StringBuilder s = new StringBuilder();
        s.append(G.V() + " vertices, " + G.E() + " edges " + '\n');
        for (int v = 0; v < G.V(); v++) {
            s.append(chars[v] + ": ");
            for (int w : G.adj(v)) {
                s.append(chars[w] + " ");
            }
            s.append('\n');
        }
		StdOut.println(s.toString());
	}

	public void solve()
	{
		matchedWords = new SET<>();
		if (root != null) {
			for (int v = 0; v < 16; v++) {
				marked = new boolean[16];
				Node node = root.next[toInt26(chars[v])];
				StringBuilder word = new StringBuilder();
				word.append(chars[v]);
				dfsOnTrie(v, v, node, word);
			}
		}
	}

	private void dfsOnTrie(int pre, int v, Node node, StringBuilder word)
	{
		if (node == null)
			return;
		if (node.id != null)
			matchedWords.add(word.toString());

		// StdOut.print(chars[v] + " - ");

		marked[v] = true;
		for (int w : G.adj(v)) {
			if (!marked[w]) {
				word.append(chars[w]);
				dfsOnTrie(pre, w, node.next[toInt26(chars[w])], word);
				word.deleteCharAt(word.length() - 1);
			}
		}
		marked[v] = false;
	}

	public void printBoard()
	{
		for (int i = 0; i < 4; i++) {
			int c = i*4;
			for (int j = 0; j < 4; j++) {
				StdOut.print(chars[c+j]);
				StdOut.print(" ");
			}
			StdOut.println();
		}
	}

	public Iterable<String> matchedWords()
	{
		return matchedWords;
	}

	public int matchedCount()
	{
		return matchedWords.size();
	}

	public static void main(String[] args)
	{
		Boggle boggle;
		In file;
		if (args.length > 1) {
			char[] chars = new char[16];
			In charFile = new In(args[0]);
			for (int i = 0; i < 16; i++) {
				chars[i] = charFile.readLine().charAt(0);
			}
			file = new In(args[1]);
			boggle = new Boggle(chars, file);
		} else {
			file = new In(args[0]);
			boggle = new Boggle(file);
		}

		boggle.printBoard();
		// boggle.printBoardGraph();

		StdOut.println("Dictionary length: " + boggle.wordCount());

		boggle.solve();
		StdOut.println("Matched words length: " + boggle.matchedCount());
		StdOut.println("Matched words in word list:");
		for (String str : boggle.matchedWords()) {
			StdOut.println(str);
		}
	}
}