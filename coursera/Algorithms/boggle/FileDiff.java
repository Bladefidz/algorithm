import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.SET;

public class FileDiff
{
	public static void main(String[] args) {
		In file1 = new In(args[0]);
		In file2 = new In(args[1]);

		SET<String> set1 = new SET<>();
		while (!file1.isEmpty()) {
			String key = file1.readString();
			key = key.toUpperCase();
			set1.add(key);
		}

		while (!file2.isEmpty()) {
			String key = file2.readString();
			key = key.toUpperCase();
			if (set1.contains(key)) {
				set1.delete(key);
			} else {
				set1.add(key);
			}
		}

		for (String s : set1) {
			StdOut.println(s);
		}
	}
}