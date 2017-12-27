import java.util.Comparator;
import java.util.LinkedList;
import java.lang.IllegalArgumentException;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Merge;

/**
 * Sorting-based method to find collinear points.
 * Let say a point p as origin.
 * For each point q, determine each slope that makes with p.
 * Sort the points according to the slopes they makes with p.
 * Check if any 3 (or more) adjacent points in the sorted order have equal
 * slopes with respect to p.
 * If so, these points, together with p are collinear.
 */
public class FastCollinearPoints
{
	private int segmentSize;
	private LinkedList<LineSegment> LineSegmentList;

	private void addNewSegment(Point[] p, int ip, Point[] pp, int from, int to)
	{
		Point min = p[ip];
		Point max = p[ip];
		Point[] tobePruned = new Point[to - from + 1];
		int j = 0;
		for (int i = from; i <= to; i++)
		{
			if (min.compareTo(pp[i]) == 1) min = pp[i];
			if (max.compareTo(pp[i]) == -1) max = pp[i];
			tobePruned[j++] = pp[i];
		}

		this.LineSegmentList.addLast(new LineSegment(min, max));
		this.segmentSize += 1;

		Merge.sort(tobePruned);
		j = 0;
		for (int i = 0; i < p.length; i++)
		{
			if (p[i] != null && p[i].compareTo(tobePruned[j]) == 0)
			{
				p[i] = null;
				j++;
				if (j == tobePruned.length)
				{
					break;
				}
			}
		}
	}

	private static boolean less(Comparator<Point> comparator, Point v, Point w)
    {
        return comparator.compare(v, w) < 0;
    }

	private static void merge(Point[] p, Point[] aux, Comparator<Point> comparator, int lo, int mid, int hi)
	{
		int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++)
        {
            if (i > mid) aux[k] = p[j++];
            else if (j > hi) aux[k] = p[i++];
            else if (less(comparator, p[j], p[i])) aux[k] = p[j++];
            else aux[k] = p[i++];
        }
	}

	private static void sort(Point[] p, Point[] aux, Comparator<Point> comparator, int lo, int hi)
	{
		if (hi > lo)
        {
            int mid = lo + (hi - lo) / 2;
            sort(aux, p, comparator, lo, mid);      // Switch to save heap
            sort(aux, p, comparator, mid + 1, hi);  // Switch to save heap
            merge(p, aux, comparator, lo, mid, hi);
        }
	}

	private static void sort(Point[] p, Comparator<Point> comparator)
	{
		Point[] aux = new Point[p.length];
		for (int i = 0; i < p.length; i++)
        {
            aux[i] = p[i];
        }
		sort(aux, p, comparator, 0, p.length-1);
	}

	// finds all line segments containing 4 points
	public FastCollinearPoints(Point[] p)
	{
		if (p != null)
		{
			// Check for null points values
			Point[] points = new Point[p.length];
			for (int i = 0; i < p.length; i++)
			{
				if (p[i] == null) throw new IllegalArgumentException(
					"Found null Points!");
				points[i] = p[i];
			}

			Comparator<Point> cmp;
			Point[] pp;
			LinkedList<Point> pl = new LinkedList<>();
			this.segmentSize = 0;
			this.LineSegmentList = new LinkedList<>();

			// Sort by y
			Merge.sort(points);

			// Check for duplicated value
			for (int i = 0; i < points.length - 1; i++)
			{
				if (points[i].compareTo(points[i + 1]) == 0)
				{
					throw new IllegalArgumentException(
						"Found duplicated points!");
				}
			}

			// points[i] is p or origin
			for (int i = 0; i < points.length - 3; i++)
			{
				if (points[i] != null)
				{
					// point[j] is q
					int k = 0;
					for (int j = 0; j < points.length; j++)
					{
						if (j != i && points[j] != null)
						{
							pl.addLast(points[j]);
							k++;
						}
					}

					// Find 3 (or more) points that have equal slope from p
					if (k > 2)
					{
						pp = new Point[k];
						for (int j = 0; j < k; j++)
						{
							pp[j] = pl.pop();
						}

						// Sort q by their slopes from p
						cmp = points[i].slopeOrder();
						sort(pp, cmp);

						int j = 1;
						k = 0;
						int l = 0;
						for (; j < pp.length; j++)
						{
							if (cmp.compare(pp[l], pp[j]) == 0)
							{
								k++;
							}
							else
							{
								if (k > 1)
								{
									addNewSegment(points, i, pp, l, j - 1);
								}
								l = j;
								k = 0;
							}
						}
						if (k > 1)
						{
							addNewSegment(points, i, pp, l, j - 1);
						}
					}
				}
			}
		}
		else
		{
			throw new IllegalArgumentException();
		}
	}

	// the number of line segments
	public int numberOfSegments()
	{
		return this.segmentSize;
	}

	// the line segments
	public LineSegment[] segments()
	{
		LineSegment[] ls = new LineSegment[this.segmentSize];
		for (int i = 0; i < this.segmentSize; i++)
		{
			ls[i] = this.LineSegmentList.pop();
		}
		return ls;
	}

	public static void main(String[] args)
	{
		// read the n points from a file
	    In in = new In(args[0]);
	    int n = in.readInt();
	    Point[] points = new Point[n];
	    for (int i = 0; i < n; i++)
	    {
	        int x = in.readInt();
	        int y = in.readInt();
	        points[i] = new Point(x, y);
	    }

	    // draw the points
	    StdDraw.enableDoubleBuffering();
	    StdDraw.setXscale(0, 32768);
	    StdDraw.setYscale(0, 32768);
	    for (Point p : points)
	    {
	        p.draw();
	    }
	    StdDraw.show();

	    // print and draw the line segments
	    FastCollinearPoints collinear = new FastCollinearPoints(points);
	    for (LineSegment segment : collinear.segments())
	    {
	        StdOut.println(segment);
	        segment.draw();
	    }
	    StdDraw.show();
	}
}
