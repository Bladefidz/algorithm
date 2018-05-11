import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class Point2D implements Comparable<Point2D>
{
    private final double x;
    private final double y;

    public Point2D(double x, double y)
    {
        this.x = x;
        this.y = y;
    }

    public double getX()
    {
        return this.x;
    }

    public double getY()
    {
        return this.y;
    }

    public int compareTo(Point2D that)
    {
        if (this.x < that.x) return -1;
        if (this.x > that.x) return 1;
        if (this.y < that.y) return -1;
        if (this.y > this.y) return 1;
        return 0;
    }

    public static void intersect(Comparable[] a, Comparable[] b, Point2D[] c)
    {
        int n = a.length;
        int m = b.length;
        int p = n;
        if (m < n)
        {
            p = m;
        }
        for (int i = 0; i < p; i++)
        {
            if (a[i].compareTo(b[i]) != 0)
            {
                c[2 * i] = (Point2D) a[i];
                c[2 * i + 1] = (Point2D) b[i];
            }
        }
        if (m > n)
        {
            for (int i = 0; i < (m - n); i++)
            {
                c[i + p] = (Point2D) b[i + p];
            }
        }
        else if (m < n)
        {
            for (int i = 0; i < (n - m); i++)
            {
                c[i + p] = (Point2D) a[i + p];
            }
        }
    }

    public static void main(String[] args)
    {
        Point2D[] a = new Point2D[5];
        Point2D[] b = new Point2D[5];
        Point2D[] c = new Point2D[a.length + b.length];
        StdOut.println("ITERSECTION OF 2D POINTS");
        StdOut.println("Before:");
        for (int i = 0; i < 5; i++)
        {
            a[i] = new Point2D(i, i * 2);
            StdOut.print("a[" + i + "] = (" + a[i].getX() + ',' + a[i].getY() + ')');
            b[i] = new Point2D(i + 1, (i + 1) * 3);
            StdOut.print(" ");
            StdOut.print("b[" + i + "] = (" + b[i].getX() + ',' + b[i].getY() + ')');
            StdOut.print('\n');
        }
        StdOut.println("After:");
        Point2D.intersect(a, b, c);
        for (int i = 0; i < 10; i++)
        {
            StdOut.println("c[" + i + "] = (" + c[i].getX() + ',' + c[i].getY() + ')');
        }
    }
}
