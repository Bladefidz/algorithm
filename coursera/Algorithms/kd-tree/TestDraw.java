import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;

class TestDraw
{
	public static void main(String[] args)
	{
		// initialize the data structures from file
        String filename = args[0];
        In in = new In(filename);
        KdTree kdtree = new KdTree();
        while (!in.isEmpty()) {
            double x = in.readDouble();
            double y = in.readDouble();
            Point2D p = new Point2D(x, y);
            kdtree.insert(p);
        }

        // draw the points
        StdDraw.clear();
        StdDraw.setXscale(-0.1, 1.1);
		StdDraw.setYscale(-0.1, 1.1);
        kdtree.draw();
        StdDraw.show();
	}
}