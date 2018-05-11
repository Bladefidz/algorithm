import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.SET;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.Queue;
import java.lang.IllegalArgumentException;

/**
 * Brute-force implementation.
 * A mutable data type that represents a set of points in the unit square.
 * Implement a red–black BST.
 *
 * Performance Requirements
 * Support insert() and contains() in time proportional to the logarithm of
 * the number of points in the set in the worst case
 * Support nearest() and range() in time proportional to the number of points
 * in the set.
 */
public class PointSET {
	private SET<Point2D> setPoints;

	// construct an empty set of points
	public PointSET()
	{
		this.setPoints = new SET<Point2D>();
	}

	// is the set empty?
	public boolean isEmpty()
	{
		return this.setPoints.isEmpty();
	}

	// number of points in the set
	public int size()
	{
		return this.setPoints.size();
	}

	// add the point to the set (if it is not already in the set)
	public void insert(Point2D p)
	{
		if (p != null)
		{
			this.setPoints.add(p);
		}
		else
		{
			throw new IllegalArgumentException();
		}
	}

	// does the set contain point p?
	public boolean contains(Point2D p)
	{
		if (p != null)
		{
			return this.setPoints.contains(p);
		}
		else
		{
			throw new IllegalArgumentException();
		}
	}

	// draw all points to standard draw
	public void draw()
	{
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);

		for (Point2D point2d : this.setPoints)
		{
			point2d.draw();
		}
	}

	// all points that are inside the rectangle (or on the boundary)
	public Iterable<Point2D> range(RectHV rect)
	{
		Queue<Point2D> inrect = new Queue<>();

		for (Point2D p : this.setPoints)
		{
			if (rect.contains(p))
			{
				inrect.enqueue(p);
			}
		}

		return inrect;
	}

	// a nearest neighbor in the set to point p; null if the set is empty
	public Point2D nearest(Point2D p)
	{
		if (p == null)
		{
			throw new IllegalArgumentException();
		}

		Point2D nearest = null;
		double nearestDist = 0.0;
		for (Point2D p1 : this.setPoints)
		{
			double currDist = p.distanceSquaredTo(p1);
			if (nearestDist == 0.0 || currDist < nearestDist)
			{
				nearest = p1;
				nearestDist = currDist;
			}
		}

		return nearest;
	}

	// unit testing of the methods (optional)
	public static void main(String[] args)
	{

	}
}