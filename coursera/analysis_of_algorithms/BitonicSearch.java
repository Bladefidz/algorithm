/**
 * Compilation:  javac-algs4 BitonicSearch.java
 * Execution:    java-algs4 BitonicSearch file int
 *
 * Find an Integer in Bitonic Array of Distinc Integers.
 * Greedy algorithm gives 3*ln(N)
 * Optimal algorithm gives 2*ln(N)
 */
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class BitonicSearch
{
    /**
     * Find an integer in bitonic array.
     * Greedy Algorithm.
     * Running Time: 3*ln(N)
     * @param  int[] arr           Bitonic array
     * @param  int   key           Integer to find
     * @return       > -1 if found, -1 if not found
     */
    public static int find(int[] arr, int key)
    {
        int l = 0;
        int r = arr.length - 1;
        int m;
        int n = -1;
        int peak = 0;
        // Find index of maximum value
        while(l <= r)
        {
            m = (l + r) / 2;
            peak = m;

            if (arr[peak] > arr[m + 1])
            {
                if (arr[peak] > arr[m - 1])
                {
                    break;
                }
                else
                { // Switch to the right hill
                    r = m + 1;
                }
            }
            else
            { // Switch to the left hill
                l = m - 1;
            }
        }
        // Search
        r = peak;
        while(l <= r)
        { // Climb to left hill
            m = (l + r) / 2;
            if (arr[m] < key)
            {
                l = m + 1;
            }
            else if (arr[m] > key)
            {
                r = m - 1;
            }
            else
            {
                n = m;
                break;
            }
        }
        if (n == -1)
        { // Climb to right hill
            r = arr.length - 1;
            l = peak;
            while (r > l)
            {
                m = (l + r) / 2;
                if (arr[m] < key)
                {
                    r = m + 1;
                }
                else if (arr[m] > key)
                {
                    l = m - 1;
                }
                else
                {
                    n = m;
                    break;
                }
            }
        }
        return n;
    }

    public static void main(String[] args)
    {
        In in = new In(args[0]);
        int m = BitonicSearch.find(in.readAllInts(), Integer.parseInt(args[1]));
        StdOut.println(m);
    }
}
