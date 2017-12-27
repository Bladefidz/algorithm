/******************************************************************************
 *  Compilation:  javac Point.java
 *  Execution:    java Point
 *  Dependencies: none
 *
 *  An immutable data type for points in the plane.
 *  For use on Coursera, Algorithms Part I programming assignment.
 *
 ******************************************************************************/

import java.util.Comparator;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class Point implements Comparable<Point>
{
    private final int x;     // x-coordinate of this point
    private final int y;     // y-coordinate of this point

    /**
     * Initializes a new point.
     *
     * @param  x the <em>x</em>-coordinate of the point
     * @param  y the <em>y</em>-coordinate of the point
     */
    public Point(int x, int y)
    {
        /* DO NOT MODIFY */
        this.x = x;
        this.y = y;
    }

    /**
     * Draws this point to standard draw.
     */
    public void draw()
    {
        /* DO NOT MODIFY */
        StdDraw.point(x, y);
    }

    /**
     * Draws the line segment between this point and the specified point
     * to standard draw.
     *
     * @param that the other point
     */
    public void drawTo(Point that)
    {
        /* DO NOT MODIFY */
        StdDraw.line(this.x, this.y, that.x, that.y);
    }

    /**
     * Compares two points by y-coordinate, breaking ties by x-coordinate.
     * Formally, the invoking point (x0, y0) is less than the argument point
     * (x1, y1) if and only if either y0 < y1 or if y0 = y1 and x0 < x1.
     *
     * @param  that the other point
     * @return the value <tt>0</tt> if this point is equal to the argument
     *         point (x0 = x1 and y0 = y1);
     *         a negative integer if this point is less than the argument
     *         point; and a positive integer if this point is greater than the
     *         argument point
     */
    public int compareTo(Point that)
    {
        if (this.y > that.y) return 1;
        if (this.y < that.y) return -1;
        if (this.x > that.x) return 1;
        if (this.x < that.x) return -1;
        return 0;
    }

    /**
     * Returns the slope between this point and the specified point.
     * Formally, if the two points are (x0, y0) and (x1, y1), then the slope
     * is (y1 - y0) / (x1 - x0). For completeness, the slope is defined to be
     * +0.0 if the line segment connecting the two points is horizontal;
     * Double.POSITIVE_INFINITY if the line segment is vertical;
     * and Double.NEGATIVE_INFINITY if (x0, y0) and (x1, y1) are equal.
     *
     * @param  that the other point
     * @return the slope between this point and the specified point
     */
    public double slopeTo(Point that)
    {
        if (compareTo(that) == 0) return Double.NEGATIVE_INFINITY;

        double dx = that.x - this.x;
        double dy = that.y - this.y;
        if (dx != 0)
        {
            if (dy != 0)
            {
                return dy/dx;
            }
            else
            {
                return +0.0;
            }
        }
        else
        {
            return Double.POSITIVE_INFINITY;
        }
    }

    /**
     * Comparator class that compare two points correspond to this point.
     */
    private class SlopeOrder implements Comparator<Point>
    {
        /**
         * Compare (x1, y1) and (x2, y2) relative to their slopes to (x0, y0)
         * @param  p1 First coordinate (x1, y1)
         * @param  p2 Second coordinate (x2, y2)
         * @return    -1, 0 or 1
         */
        public int compare(Point p1, Point p2)
        {
            double f1 = slopeTo(p1);
            double f2 = slopeTo(p2);
            if (f1 < f2)
            {
                return -1;
            } else if (f1 > f2)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    /**
     * Compares two points by the slope they make with this point.
     * The slope is defined as in the slopeTo() method.
     *
     * @return the Comparator that defines this ordering on points
     */
    public Comparator<Point> slopeOrder()
    {
        return new SlopeOrder();
    }


    /**
     * Returns a string representation of this point.
     * This method is provide for debugging;
     * your program should not rely on the format of the string representation.
     *
     * @return a string representation of this point
     */
    public String toString()
    {
        /* DO NOT MODIFY */
        return "(" + x + ", " + y + ")";
    }

    /**
     * Unit tests the Point data type.
     */
    public static void main(String[] args)
    {
        In in = new In(args[0]);
        int n = in.readInt();
        int i = in.readInt();
        int j = in.readInt();

        // Test slope of (x1-x0)/(y1-y0)
        while (!in.isEmpty())
        {
            Point p = new Point(i, j);
            i = in.readInt();
            j = in.readInt();
            StdOut.println(p.slopeTo(new Point(i, j)));
        }
    }
}
