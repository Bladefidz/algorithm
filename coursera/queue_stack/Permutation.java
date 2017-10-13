import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import java.lang.IllegalArgumentException;

public class Permutation {
    /**
     * Assume that 0 ≤ k ≤ n
     * where n is the number of string on standard input
     * @param String[] args StdIn arguments
     */
    public static void main(String[] args)
    {
        int k = Integer.parseInt(args[0]);
        RandomizedQueue<String> rq = new RandomizedQueue<>();
        while (!StdIn.isEmpty()) {
            rq.enqueue(StdIn.readString());
        }
        if (k > rq.size())
        {
            throw new IllegalArgumentException();
        }
        else
        {
            int i = 1;
            for (String s : rq)
            {
                if (i > k)
                {
                    break;
                }
                StdOut.println(s);
                i++;
            }
        }
    }
}
