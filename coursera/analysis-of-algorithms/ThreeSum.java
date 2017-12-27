/**
 * Find Sum of Any Three Number That Equal to Zero.
 * Use improved algorithm that has N^2 order of growth
 * that better than brute force algorithm which has N^3 order of growth.
 */
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class ThreeSum
{
    public static int count(int[] a)
    {
        int N = a.length;
        int cnt = 0;

        // Sort array
        int k;
        int j;
        for (int i = 1; i < N; i++)
        {
            k = a[i];
            for (j = i - 1; j >= 0; j--)
            {
                if (k < a[j])
                {
                    a[j + 1] = a[j];
                }
                else
                {
                    break;
                }
            }
            a[j + 1] = k;
        }

        int l;
        int m;
        int n;
        for (int i = 0; i < N - 1; i++)
        {
            for (j = i + 1; j < N; j++)
            {
                if (a[i] < a[j])
                {
                    k = a[i] + a[j];
                    if (k != 0)
                    {
                        k *= -1;
                        if ((k > 0 && k <= a[N - 1]) || (k < 0 && k >= a[0]))
                        {
                            l = 0;
                            m = N - 1;
                            while(l <= m)
                            {
                                n = (l + m) / 2;
                                if (a[n] < k)
                                {
                                    l = n + 1;
                                }
                                else if (a[n] > k)
                                {
                                    m = n - 1;
                                }
                                else
                                {
                                    cnt++;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Sum of this three number are zero?

        return cnt;
    }

    public static void main(String[] args)
    {
        int[] a = In.readInts(args[0]);
        StdOut.println(count(a));
    }
}
