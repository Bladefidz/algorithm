import java.awt.Color;
import java.lang.IllegalArgumentException;
import java.lang.Math;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Picture;
import edu.princeton.cs.algs4.IndexMinPQ;

public class SeamCarver
{
	private int[][] colors;
	private double[][] energies;
	private int width;
	private int height;

	// create a seam carver object based on the given picture
	public SeamCarver(Picture picture)
	{
		if (picture == null) throw new IllegalArgumentException();
		this.width = picture.width();
		this.height = picture.height();
		this.colors = new int[picture.width()][picture.height()];
		for (int y = 0; y < picture.height(); y++)
			for (int x = 0; x < picture.width(); x++)
				this.colors[x][y] = picture.get(x, y).getRGB();
		this.energies = new double[picture.width()][picture.height()];
		for (int y = 0; y < picture.height(); y++)
			for (int x = 0; x < picture.width(); x++)
				this.energies[x][y] = energy(x, y);
	}

	// current picture
	public Picture picture()
	{
		Picture pict = new Picture(this.width, this.height);
		for (int y = 0; y < this.height; y++)
			for (int x = 0; x < this.width; x++)
				pict.set(x, y, new Color(this.colors[x][y]));
		return pict;
	}

	// width of current picture
	public int width()
	{
		return this.width;
	}

	// height of current picture
	public int height()
	{
		return this.height;
	}

	// energy of pixel at column x and row y
	public double energy(int x, int y)
	{
		if (x < 0 && x >= this.width) throw new IllegalArgumentException();
		if (y < 0 && y >= this.height) throw new IllegalArgumentException();

		if (x == 0) return 1000;
		if (y == 0) return 1000;
		if (x == this.width - 1) return 1000;
		if (y == this.height - 1) return 1000;

		Color colorX1 = new Color(this.colors[x-1][y]);
		Color colorX2 = new Color(this.colors[x+1][y]);
		Color colorY1 = new Color(this.colors[x][y-1]);
		Color colorY2 = new Color(this.colors[x][y+1]);

		double xGradSquare = Math.pow(colorX2.getRed() - colorX1.getRed(), 2);
		xGradSquare += Math.pow(colorX2.getGreen() - colorX1.getGreen(), 2);
		xGradSquare += Math.pow(colorX2.getBlue() - colorX1.getBlue(), 2);

		double yGradSquare = Math.pow(colorY2.getRed() - colorY1.getRed(), 2);
		yGradSquare += Math.pow(colorY2.getGreen() - colorY1.getGreen(), 2);
		yGradSquare += Math.pow(colorY2.getBlue() - colorY1.getBlue(), 2);

		return Math.sqrt(xGradSquare + yGradSquare);
	}

	private int col(int i)
	{
		return i % this.width;
	}

	private int row(int i)
	{
		return i / this.width;
	}

	// sequence of indices for horizontal seam
	public int[] findHorizontalSeam()
	{
		// Temporary transpose matrix of energies
		double[][] energiesT = new double[this.height][this.width];
		for (int x = 0; x < this.height; x++)
			for (int y = 0; y < this.width; y++)
				energiesT[x][y] = this.energies[y][x];
		double[][] tmp = this.energies;
		this.energies = energiesT;

		// Get seam
		int[] seam = findVerticalSeam();

		// Roll back matrix of energies
		this.energies = tmp;
		energiesT = null;

		return seam;
	}

	// sequence of indices for vertical seam
	public int[] findVerticalSeam()
	{
		int ewidth = this.energies.length;
		int eheight = this.energies[0].length;

		// Preparing sum of energies matrix
		double[][] energyTo = new double[ewidth][eheight];
		for (int x = 0; x < ewidth; x++)
			energyTo[x][0] = 1000;
		for (int y = 1; y < eheight; y++)
			for (int x = 0; x < ewidth; x++)
				energyTo[x][y] = Double.POSITIVE_INFINITY;

		// Calculating set of low energy sequences
		// Choose for every two or three next lower energy
		int[][] edgeTo = new int[ewidth][eheight];
		for (int y = 0; y < eheight-1; y++)
		{
			for (int x = 0; x < ewidth; x++)
			{
				int z; int z1;
				if (x > 0) z = x-1; else z = x;
				if (x == ewidth-1) z1 = x; else z1 = x+1;
				for (; z <= z1; z++)
				{
					double e = this.energies[z][y+1] + energyTo[x][y];
					if (e < energyTo[z][y+1])
					{  // Current energy better than last energy
						energyTo[z][y+1] = e;
						edgeTo[z][y+1] = y*ewidth+x;
					}
				}
			}
		}

		// Find the lowest energy sequence end point
		double minEnergy = energyTo[0][eheight-1];
		int minEnergyCol = 0;
		for (int x = 1; x < ewidth; x++)
		{
			if (energyTo[x][eheight-1] < minEnergy)
			{
				minEnergy = energyTo[x][eheight-1];
				minEnergyCol = x;
			}
		}

		// Calculating seam
		int[] seam = new int[eheight];
		seam[eheight-1] = minEnergyCol;
		for (int y = eheight-1; y > 0; y--)
			seam[y-1] = edgeTo[seam[y]][y] % ewidth;

		return seam;
	}

	private boolean isDistanceValid(int[] seam)
	{
		boolean valid = true;
		for (int i = 0; i < seam.length - 1; i++)
		{
			int diff = seam[i+1] - seam[i];
			if (diff == 0) continue;
			if (Math.abs(diff) == 1) continue;
			if (Math.abs(diff) > 1) { valid = false; break; }
		}
		return valid;
	}

	// remove horizontal seam from current picture
	public void removeHorizontalSeam(int[] seam)
	{
		if (this.height <= 1) throw new IllegalArgumentException();
		if (seam == null) throw new IllegalArgumentException();
		if (seam.length != this.width) throw new IllegalArgumentException();
		if (!isDistanceValid(seam)) throw new IllegalArgumentException();

		int[][] newColors = new int[this.width][this.height-1];
		double[][] newEnergies = new double[this.width][this.height-1];
		for (int x = 0; x < this.width; x++)
		{
			// Shift color matrix
			System.arraycopy(this.colors[x], 0, newColors[x], 0, seam[x]);
			System.arraycopy(this.colors[x], seam[x]+1, newColors[x], seam[x], this.height-seam[x]-1);
			// Shift energy matrix
			System.arraycopy(this.energies[x], 0, newEnergies[x], 0, seam[x]);
			System.arraycopy(this.energies[x], seam[x]+1, newEnergies[x], seam[x], this.height-seam[x]-1);
		}
		this.colors = newColors;
		this.height--;

		// Recalculate energy along the seams
		for (int x = 0; x < this.width; x++)
		{
			if (seam[x] > 0)
				newEnergies[x][seam[x]-1] = energy(x, seam[x]-1);
			else
				newEnergies[x][0] = 1000;
			if (seam[x] < this.height)
				newEnergies[x][seam[x]] = energy(x, seam[x]);
		}
		this.energies = newEnergies;
	}

	// remove vertical seam from current picture
	public void removeVerticalSeam(int[] seam)
	{
		if (this.width <= 1) throw new IllegalArgumentException();
		if (seam == null) throw new IllegalArgumentException();
		if (seam.length != this.height) throw new IllegalArgumentException();
		if (!isDistanceValid(seam)) throw new IllegalArgumentException();

		int[][] newColors = new int[this.width-1][this.height];
		double[][] newEnergies = new double[this.width-1][this.height];
		for (int y = 0; y < this.height; y++)
		{
			for (int x = 0; x < this.width-1; x++)
			{
				if (x < seam[y])
				{
					newColors[x][y] = this.colors[x][y];
					newEnergies[x][y] = this.energies[x][y];
				}
				else
				{
					newColors[x][y] = this.colors[x+1][y];
					newEnergies[x][y] = this.energies[x+1][y];
				}
			}
		}
		this.colors = newColors;
		this.width--;

		// Recalculate energy along the seams
		for (int y = 0; y < this.height; y++)
		{
			if (seam[y] > 0)
				newEnergies[seam[y]-1][y] = energy(seam[y]-1, y);
			else
				newEnergies[0][y] = 1000;
			if (seam[y] < this.width)
				newEnergies[seam[y]][y] = energy(seam[y], y);
		}
		this.energies = newEnergies;
	}
}