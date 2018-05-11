import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Merge;

public class Plotter
{
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

	    // Sort every point by y
	    Merge.sort(points);
	   	for (int i = 0; i < n; i++)
	   	{
	   		StdOut.println(points[i].toString());
	   	}

	    // draw the points
	    StdDraw.setPenRadius(0.01);
	    StdDraw.setPenColor(StdDraw.BLACK);
	    StdDraw.enableDoubleBuffering();
	    StdDraw.setXscale(0, 32768);
	    StdDraw.setYscale(0, 32768);

	    for (Point p : points)
	    {
	        p.draw();
	    }
	    StdDraw.show();
	}
}