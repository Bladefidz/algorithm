import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Longest Palindromic Substring
 *
 * Given string S, find the longest substring that is palindrome
 * in expected linearithmic time
 *
 * Use given a parameter L
 * find all palindromic substrings of length exactly L in linear time
 * using a Karp-Rabin strategy.
 *
 * Hint (signing bonus): To do it in linear time in the worst case,
 * use Manacher's algorithm or suffix trees.
 */
public class LongestPalindrom
{
	public static void main(String[] args) {
		String txt = args[0];
		char[] revTxt = new char[txt.length()];
		int N = txt.length();

		for (int i = N-1; i >= 0; i--)
			revTxt[i] = txt[i];

		char[] polindrom = new char[txt.length()];
		int M = 0;
	}
}