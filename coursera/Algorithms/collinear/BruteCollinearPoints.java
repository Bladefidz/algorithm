import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Merge;
import java.util.LinkedList;
import java.lang.IllegalArgumentException;

/**
 * Find Collinear Points Using Brute Force Algorithm
 * Technically speaking, the optimal way to solve combinatorial problem is
 * by iterating all possible combinations.
 * Time complexity is {n\choose 4}
 */
public class BruteCollinearPoints
{
	private int segmentSize;
	private LinkedList<LineSegment> LineSegmentList;
	private int collinearCount = 0;

	private void addNewSegment(Point[] p, int a, int b, double slope, double[] slopeMax)
	{
		if (this.collinearCount == 0)
		{
			this.LineSegmentList.addLast(new LineSegment(p[a], p[b]));
			this.segmentSize += 1;

			slopeMax[0] = slope;
			slopeMax[1] = b;
			this.collinearCount++;
		}
		else
		{
			boolean accept = true;
			for (int i = 0; i < this.collinearCount * 2; i += 2)
			{
				if (slope == slopeMax[i])
				{
					if (b == slopeMax[i + 1])
					{
						accept = false;
					}
					break;
				}
			}
			if (accept)
			{
				this.LineSegmentList.addLast(new LineSegment(p[a], p[b]));
				this.segmentSize += 1;

				slopeMax[this.collinearCount * 2] = slope;
				slopeMax[this.collinearCount * 2 + 1] = b;
				this.collinearCount++;
			}
		}
	}

	// finds all line segments containing 4 points
	public BruteCollinearPoints(Point[] p)
	{
		if (p != null)
		{
			// Check for null points values
			for (int i = 0; i < p.length; i++)
			{
				if (p[i] == null) throw new IllegalArgumentException(
					"Found null Points!");
			}

			// Prepare copy of points
			Point[] points = new Point[p.length];

			// Copy and check for duplicate points
			for (int i = 0; i < p.length - 1; i ++)
			{
				if (p[i].compareTo(p[i + 1]) != 0)
				{
					points[i] = p[i];
				}
				else
				{
					throw new IllegalArgumentException("Found duplicated points!");
				}
			}
			points[p.length - 1] = p[p.length - 1];

			// Initialize segment list
			this.segmentSize = 0;
			this.LineSegmentList = new LinkedList<>();

			// Sort points by y
			Merge.sort(points);

			// Brute force collinear
			Point[] pp = new Point[4];
			double[] slopeMax = new double[points.length - 2];
			for (int i = 0; i < points.length - 3; i ++)
			{
				for (int j = i + 1; j < points.length - 2; j++)
				{
					int collinearCount = 0;
					int highestPoint = j + 1;
					double currentSlope = points[i].slopeTo(points[j]);
					double nextSlope;

					for (int k = j + 1; k < points.length; k++)
					{
						nextSlope = points[i].slopeTo(points[k]);
						if (currentSlope == nextSlope)
						{
							collinearCount += 1;
							highestPoint = k;
						}
					}

					if (collinearCount > 1)
					{ // If detected collinear is 4 points or more
						addNewSegment(points, i, highestPoint, currentSlope, slopeMax);
						break;
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
	    BruteCollinearPoints collinear = new BruteCollinearPoints(points);
	    for (LineSegment segment : collinear.segments())
	    {
	        StdOut.println(segment);
	        segment.draw();
	    }
	    StdDraw.show();
	}
}
