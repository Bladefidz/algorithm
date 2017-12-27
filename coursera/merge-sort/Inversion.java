import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * An inversion in an array a[] is a pair of entries a[i] and a[j] such that i<j but a[i]>a[j]. Given an array, design a linearithmic algorithm to count the number of inversions.
 */
public class Inversion
{
    private static boolean less(Comparable v, Comparable w)
    {
        return v.compareTo(w) < 0;
    }

    private static long mergeCount(Comparable[] a, Comparable[] b, int lo, int mid, int hi)
    {
        int i = lo, j = mid + 1;
        long r = 0;
        for (int k = lo; k <= hi; k++)
        {
            if (i > mid)
            {
                b[k] = a[j++];
            }
            else if (j > hi)
            {
                b[k] = a[i++];
            }
            else if (less(a[j], a[i]))
            {
                b[k] = a[j++];
                r += (mid - i) + 1;
            }
            else
            {
                b[k] = a[i++];
            }
        }
        return r;
    }

    private static long count(Comparable[] a, Comparable[] b, Comparable[] aux, int lo, int hi)
    {
        long r = 0;
        if (hi > lo)
        {
            int mid = lo + (hi - lo) / 2;
            r += count(a, b, aux, lo, mid);       // Switch to save heap
            r += count(a, b, aux, mid + 1, hi);  // Switch to save heap
            r += mergeCount(b, aux, lo, mid, hi);
        }
        StdOut.println(r);
        return r;
    }

    public static long count(Comparable[] a)
    {
        Comparable[] b = new Comparable[a.length];
        Comparable[] aux = new Comparable[a.length];
        for (int i = 0; i < a.length; i++)
        {
            b[i] = a[i];
        }
        return count(a, b, aux, 0, a.length - 1);
    }

    public static void main(String[] args)
    {
        int[] nums = StdIn.readAllInts();
        Integer[] a = new Integer[nums.length];
        for (int i = 0; i < nums.length; i++)
        {
            a[i] = nums[i];
        }
        StdOut.println(Inversion.count(a));
    }
}
