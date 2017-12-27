import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Enhanced Merge Sort
 * Duplicate original array (a) into array aux. Then, switching between
 * them to do sort and merge.
 * Use more memory allocation in heap to do sort.
 */
public class FastMerge
{
    // is v < w?
    private static boolean less(Comparable v, Comparable w)
    {
        return v.compareTo(w) < 0;
    }

    // is a sorted?
    private static boolean isSorted(Comparable[] a)
    {
        for (int i = 1; i < a.length; i++)
            if (less(a[i], a[i - 1])) return false;
        return true;
    }

    // Merge sorted item from last merged (a) into new sorted (aux)
    private static void merge(Comparable[] a, Comparable[] aux, int lo, int mid, int hi)
    {
        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++)
        {
            if (i > mid) aux[k] = a[j++];
            else if (j > hi) aux[k] = a[i++];
            else if (less(a[j], a[i])) aux[k] = a[j++];
            else aux[k] = a[i++];
        }
    }

    // Sort last sorted (a) into new sorted (aux),
    // then merge new sorted (aux) back into a.
    private static void sort(Comparable[] a, Comparable[] aux, int lo, int hi)
    {
        if (hi <= lo) return;
        int mid = lo + (hi - lo) / 2;
        sort(aux, a, lo, mid);      // Switch to save heap
        sort(aux, a, mid + 1, hi);  // Switch to save heap
        merge(a, aux, lo, mid, hi);
    }

    // Sort Comparable array a
    public static void sort(Comparable[] a)
    {
        Comparable[] aux = new Comparable[a.length];
        for (int i = 0; i < a.length; i++)
        {
            aux[i] = a[i];
        }
        sort(aux, a, 0, a.length - 1);
    }

    // Merge half-sorted array a.
    // ~5*N/2+(N/2)*log(N/2)
    private static void mergeHalf(Comparable[] a, Comparable[] aux)
    {
        int mid = a.length / 2;
        Comparable tmp;
        int i = 0;
        int j = mid;
        int l = 0;

        // Sort half left
        for (int k = 0; k < mid; k++)
        {
            if (less(a[j], a[i]))
            {
                aux[k] = a[j++];
            }
            else
            {
                aux[k] = a[i++];
            }
        }

        // Subtitutution
        for (int k = 0; k < i; k++)
        {
            a[k] = aux[k];
        }
        for (int k = i; k < mid; k++)
        {
            aux[l++] = a[k];
            a[k] = aux[k];
        }
        l = a.length - j - 1;
        for (int k = j; k < a.length; k++)
        {
            aux[l++] = a[k];
        }

        // Sort half right
        Merge.sort(aux);
        for (int k = 0; k < mid; k++)
        {
            a[mid + k] = aux[k];
        }
    }

    // Sort half-sorted array a.
    // the subarray a[0] to a[n−1] is sorted and
    // the subarray a[n] to a[2∗n−1] is sorted.
    // The trick is just copy {0 - mid} items of array into aux 
    public static void sortHalf(Comparable[] a)
    {
        int mid = a.length / 2;
        Comparable[] aux = new Comparable[mid];
        for (int i = 0; i < mid; i++)
        {
            aux[i] = a[mid + i];
        }
        mergeHalf(a, aux);
    }

    // print array to standard output
    private static void show(Comparable[] a)
    {
        for (int i = 0; i < a.length; i++)
        {
            StdOut.println(a[i]);
        }
    }

    public static void main(String[] args)
    {
        // Read String
        String[] a = StdIn.readAllStrings();

        // FastMerge.sortHalf(a);
        FastMerge.sort(a);
        show(a);
    }
}
