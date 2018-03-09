import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class TestArrayCopy
{
	public static void main(String[] args)
	{
		int[][] test = new int[5][5];
		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				test[x][y] = x*y;
			}
		}
		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				StdOut.print(test[x][y] + " ");
			}
			StdOut.println();
		}

		StdOut.println("------");
		int[][] copy = new int[4][5];
		for (int x = 0; x < 4; x++) {
			System.arraycopy(test[x], 0, copy[x], 0, 2);
			System.arraycopy(test[x], 3, copy[x], 2, 2);
		}
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 5; y++) {
				StdOut.print(copy[x][y] + " ");
			}
			StdOut.println();
		}
	}
}