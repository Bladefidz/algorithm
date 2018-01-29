import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.SET;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

class OrderWordFile
{
	public static void main(String[] args)
	{
		String fileName = args[0];
		In file = new In(args[0]);
		if (!file.isEmpty()) {
			SET<String> words = new SET<>();
			while (!file.isEmpty()) {
				String key = file.readString();
				if (key.length() > 2)
					words.add(key);
			}

			try {
				BufferedWriter writer = new BufferedWriter(
					new FileWriter(fileName + "-ordered"));
				for (String word : words)
					writer.write(word+'\n');
				writer.close();
			}
			catch (IOException e) {
				StdOut.println(e);
			}
		}
	}
}