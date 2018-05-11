import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;

class TestRect
{
	public static void main(String[] args)
	{
		// try
		// {
		// 	Thread.sleep(1000);
		// }
		// catch(InterruptedException ie)
		// {
		// 	Thread.currentThread().interrupt();
		// }

		Point2D a = new Point2D(0.206107, 0.095492);
		Point2D b = new Point2D(0.975528, 0.654508);
		Point2D c = new Point2D(0.024472, 0.345492);
		Point2D d = new Point2D(0.793893, 0.095492);
		Point2D e = new Point2D(0.793893, 0.904508);
		Point2D f = new Point2D(0.975528, 0.345492);
		Point2D g = new Point2D(0.206107, 0.904508);
		Point2D h = new Point2D(0.500000, 0.000000);
		Point2D i = new Point2D(0.024472, 0.654508);
		Point2D j = new Point2D(0.500000, 1.000000);

		StdDraw.setXscale(-0.1, 1.1);
		StdDraw.setYscale(-0.1, 1.1);
		StdDraw.rectangle(0.5, 0.5, 0.5, 0.5);

		// Draw I
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		i.draw();
		StdDraw.setPenColor(StdDraw.RED);
		StdDraw.setPenRadius();
		StdDraw.line(i.x(), c.y(), i.x(), 1.0);

		// Draw C
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		c.draw();
		StdDraw.setPenColor(StdDraw.BLUE);
		StdDraw.setPenRadius();
		StdDraw.line(0.0, c.y(), a.x(), c.y());

		// Draw A
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		a.draw();
		StdDraw.setPenColor(StdDraw.RED);
		StdDraw.setPenRadius();
		StdDraw.line(a.x(), 0.0, a.x(), 1.0);

		// Draw H
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		h.draw();
		StdDraw.setPenColor(StdDraw.BLUE);
		StdDraw.setPenRadius();
		StdDraw.line(a.x(), h.y(), d.x(), h.y());

		// Draw F
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		f.draw();
		StdDraw.setPenColor(StdDraw.BLUE);
		StdDraw.setPenRadius();
		StdDraw.line(d.x(), f.y(), 1.0, f.y());

		// Draw D
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		d.draw();
		StdDraw.setPenColor(StdDraw.RED);
		StdDraw.setPenRadius();
		StdDraw.line(d.x(), 0.0, d.x(), b.y());

		// Draw B
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		b.draw();
		StdDraw.setPenColor(StdDraw.BLUE);
		StdDraw.setPenRadius();
		StdDraw.line(a.x(), b.y(), 1.0, b.y());

		// Draw J
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		j.draw();
		StdDraw.setPenColor(StdDraw.RED);
		StdDraw.setPenRadius();
		StdDraw.line(j.x(), g.y(), j.x(), 1.0);

		// Draw G
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		g.draw();
		StdDraw.setPenColor(StdDraw.BLUE);
		StdDraw.setPenRadius();
		StdDraw.line(g.x(), g.y(), e.x(), g.y());

		// Draw E
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		e.draw();
		StdDraw.setPenColor(StdDraw.RED);
		StdDraw.setPenRadius();
		StdDraw.line(e.x(), b.y(), e.x(), 1.0);
	}
}