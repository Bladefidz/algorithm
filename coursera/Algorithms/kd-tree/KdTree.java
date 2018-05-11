import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.SET;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.Queue;
import java.lang.IllegalArgumentException;

/**
 * kd-tree implementation.
 * For 2 dimensional case, we called this as 2d-tree.
 */
public class KdTree
{
	private final int VERTICAL = 1;
	private final int HORIZONTAL = 2;

	/**
	 * A node is either a VERTICAL or HORIZONTAL and each node
	 * has sum of AREA of RECTANGLE from its two child.
	 *
	 * A VERTICAL NODE contains two HORIZONTAL CHILD defined by X value:
	 * 		left_child.x < parent.x
	 * 		right_child.x > parent.x
	 * A HORIZONTAL NODE contains two VERTICAL CHILD defined by y value:
	 * 		left_child.y < parent.y
	 * 		right_child.y > parent.y
	 *
	 * Node.rect = left_child.rect + right_child.rect
	 */
	private static class Node
	{
		// the point
		private Point2D point;

		// the axis-aligned rectangle corresponding to this node
		private RectHV rect;

		// the left/bottom subtree
		private Node leftBelow;

		// the right/top subtree
		private Node rightAbove;

		Node(Point2D p, RectHV r)
		{
			this.point = p;

			if (r == null)
			{ // Draw a full rectangle.
				r = new RectHV(0, 0, 1, 1);
			}

			this.rect = r;
			this.leftBelow = null;
			this.rightAbove = null;
		}
	}

	// Root node of kd-tree
	private Node root;

	// Size of nodes in kd-tree
	private int size;

 	// construct an empty set of points
	public KdTree()
	{
		this.root = null;
		this.size = 0;
	}

	// is the set empty?
	public boolean isEmpty()
	{
		return this.size == 0;
	}

	// number of points in the set
	public int size()
	{
		return this.size;
	}

	// Put node into kd-tree as vertical node that divide parent node.
	// Divide parent area by X.
	// See slide 25 at handout of Geometry Search.
	private Node put(Point2D p, Node n, RectHV r, int split)
	{
		if (n == null)
		{ // return point as new node.
			return new Node(p, r);
		}

		if (n.point.equals(p))
		{ // Reject point.
			return n;
		}

		if (split == VERTICAL)
		{
			int cmp = Point2D.X_ORDER.compare(n.point, p);
			if (cmp > 0)
			{
				if (n.leftBelow == null)
				{
					// min.x to parent.x
					r = new RectHV(r.xmin(), r.ymin(), n.point.x(), r.ymax());
				}
				else
				{
					r = n.leftBelow.rect;
				}
				n.leftBelow = put(p, n.leftBelow, r, HORIZONTAL);
			}
			else
			{
				if (n.rightAbove == null)
				{
					// min.x to parent.x
					r = new RectHV(n.point.x(), r.ymin(), r.xmax(), r.ymax());
				}
				else
				{
					r = n.rightAbove.rect;
				}
				n.rightAbove = put(p, n.rightAbove, r, HORIZONTAL);
			}
		}
		else if (split == HORIZONTAL)
		{
			int cmp = Point2D.Y_ORDER.compare(n.point, p);
			if (cmp > 0)
			{
				if (n.leftBelow == null)
				{
					// min.y to parent.y
					r = new RectHV(r.xmin(), r.ymin(), r.xmax(), n.point.y());
				}
				else
				{
					r = n.leftBelow.rect;
				}
				n.leftBelow = put(p, n.leftBelow, r, VERTICAL);
			}
			else
			{
				if (n.rightAbove == null)
				{
					// parent.y to max.y
					r = new RectHV(r.xmin(), n.point.y(), r.xmax(), r.ymax());
				}
				else
				{
					r = n.rightAbove.rect;
				}
				n.rightAbove = put(p, n.rightAbove, r, VERTICAL);
			}
		}

		// Return tree.
		return n;
	}

	// add the point to the set (if it is not already in the set)
	// implement red-black BST and interval search
	public void insert(Point2D p)
	{
		if (p != null)
		{
			if (isEmpty())
			{
				this.root = new Node(p, null);;
			}
			else
			{
				this.root = put(p, this.root, root.rect, VERTICAL);
			}

			this.size++;
		}
		else
		{
			throw new IllegalArgumentException();
		}
	}

	// Is any n-nodes contain point p?
	private boolean contains(Node n, Point2D p, int split)
	{
		if (n == null)
		{ // Reach the deepest of tree.
			return false;
		}

		if (n.point.equals(p))
		{ // Found a node contains point p
			return true;
		}

		if (split == VERTICAL)
		{
			int cmp = Point2D.X_ORDER.compare(n.point, p);
			if (cmp > 0)
			{
				return contains(n.leftBelow, p, HORIZONTAL);
			}
			else
			{
				return contains(n.rightAbove, p, HORIZONTAL);
			}
		}
		else if (split == HORIZONTAL)
		{
			int cmp = Point2D.Y_ORDER.compare(n.point, p);
			if (cmp > 0)
			{
				return contains(n.leftBelow, p, VERTICAL);
			}
			else
			{
				return contains(n.rightAbove, p, VERTICAL);
			}
		}

		return false;
	}

	// does the kd-tree contain point p?
	public boolean contains(Point2D p)
	{
		if (p != null)
		{
			return contains(root, p, VERTICAL);
		}
		else
		{
			throw new IllegalArgumentException();
		}
	}

	// Draw each nodes in the tree from most bottom to the root recursively.
	// Draw VERTICAL NODE as red line.
	// Draw HORIZONTAL NODE as blue line.
	private void drawLines(Node n, int split)
	{
		if (n.leftBelow != null)
		{ // Walk to left child
			if (split == VERTICAL)
			{
				drawLines(n.leftBelow, HORIZONTAL);
			}
			else if (split == HORIZONTAL)
			{
				drawLines(n.leftBelow, VERTICAL);
			}
		}

		if (n.rightAbove != null)
		{ // Walk to right child
			if (split == VERTICAL)
			{
				drawLines(n.rightAbove, HORIZONTAL);
			}
			else if (split == HORIZONTAL)
			{
				drawLines(n.rightAbove, VERTICAL);
			}
		}

		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);
		n.point.draw();

		if (split == VERTICAL)
		{
			StdDraw.setPenColor(StdDraw.RED);
			StdDraw.setPenRadius();
			StdDraw.line(n.point.x(), n.rect.ymin(), n.point.x(), n.rect.ymax());
		}
		else if (split == HORIZONTAL)
		{
			StdDraw.setPenColor(StdDraw.BLUE);
			StdDraw.setPenRadius();
			StdDraw.line(n.rect.xmin(), n.point.y(), n.rect.xmax(), n.point.y());
		}
	}

	// draw all points to standard draw
	public void draw()
	{
		// StdDraw.setXscale(-0.1, 1.1);
		// StdDraw.setYscale(-0.1, 1.1);

		// Draw a full rectangle
		StdDraw.setPenRadius();
		StdDraw.line(0.0, 0.0, 1.0, 0.0);
		StdDraw.line(1.0, 0.0, 1.0, 1.0);
		StdDraw.line(1.0, 1.0, 0.0, 1.0);
		StdDraw.line(0.0, 1.0, 0.0, 0.0);

		// Draw tree
		if (!isEmpty())
			drawLines(this.root, VERTICAL);
	}

	/**
	 * To find all points contained in a given query rectangle,
	 * start at the root and recursively search for points in both subtrees
	 * using the following pruning rule:
	 * 	- If the query rectangle does not intersect the rectangle corresponding
	 * 	to a node, there is no need to explore that node (or its subtrees).
	 *  - A subtree is searched only if it might contain a point contained in
	 *  the query rectangle.
	 */
	private void enqueuInRange(Node n, Queue<Point2D> q, RectHV r)
	{
		if (n != null)
		{ // While we are not reaching any leafs in tree
			if (r.contains(n.point))
			{
				q.enqueue(n.point);
			}
			if (n.leftBelow != null)
			{
				if (r.intersects(n.leftBelow.rect))
				{
					enqueuInRange(n.leftBelow, q, r);
				}
			}
			if (n.rightAbove != null)
			{
				if (r.intersects(n.rightAbove.rect))
				{
					enqueuInRange(n.rightAbove, q, r);
				}
			}
		}
	}

	// all points that are inside the rectangle (or on the boundary)
	public Iterable<Point2D> range(RectHV rect)
	{
		Queue<Point2D> inrange = new Queue<>();
		enqueuInRange(this.root, inrange, rect);
		return inrange;
	}

	/**
	 * To find a closest point to a given query point,
	 * start at the root and recursively search in both subtrees
	 * using the following pruning rule:
	 * 	- if the closest point discovered so far is closer than
	 * 	the distance between the query point and the rectangle
	 * 	corresponding to a node, there is no need to explore that node
	 * 	(or its subtrees).
	 * @param  p     Query point
	 * @param  q     Nearest point to query so far
	 * @param  n     Node
	 * @param  split Split line of current node
	 * @return       q
	 */
	private Point2D nearestTo(Point2D p, Point2D q, Node n, int split)
	{
		if (n != null)
		{
			if (p.distanceSquaredTo(n.point) < p.distanceSquaredTo(q))
			{ // Update q
				q = n.point;
			}

			boolean left = true;

			// Check the rectangle corresponding to a node
			if (split == VERTICAL)
			{
				if (n.point.x() >= p.x())
					left = false;
				split = HORIZONTAL;
			}
			else if (split == HORIZONTAL)
			{
				if (n.point.y() >= p.y())
					left = false;
				split = VERTICAL;
			}

			// Check for left child or right child
			if (left)
			{
				// Get nearest point from right sibling
				q = nearestTo(p, q, n.rightAbove, split);

				if (n.leftBelow != null)
				{
					// Compare to distance to area of next left child
					if (n.leftBelow.rect.distanceSquaredTo(p)
						<= q.distanceSquaredTo(p))
					{
						// Explore next left node
						q = nearestTo(p, q, n.leftBelow, split);
					}
				}
			}
			else
			{
				// Get nearest point from left sibling
				q = nearestTo(p, q, n.leftBelow, split);

				if (n.rightAbove != null)
				{
					// Compare to distance to area of next right child
					if (n.rightAbove.rect.distanceSquaredTo(p)
						<= q.distanceSquaredTo(p))
					{
						// Explore next left node
						q = nearestTo(p, q, n.rightAbove, split);
					}
				}
			}
		}

		return q;
	}

	// a nearest neighbor in the kd-tree to point p;
	// null if the kd-tree is empty
	public Point2D nearest(Point2D p)
	{
		if (p == null)
		{
			throw new IllegalArgumentException();
		}

		if (!isEmpty())
		{
			return nearestTo(p, this.root.point, this.root, VERTICAL);
		}

		return null;
	}

	// unit testing of the methods (optional)
	public static void main(String[] args)
	{

	}
}
