import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.BorderLayout;
import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.block.BlockBorder;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.chart.title.TextTitle;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import org.jfree.ui.RefineryUtilities;
import edu.princeton.cs.algs4.StdOut;

public class PlotXYSeries extends JFrame {
	private XYSeriesCollection dataset;
	private boolean autoSort = false;
	private int width;
	private int height;

	public PlotXYSeries(String applicationTitle, double width, double height)
	{
		super(applicationTitle);
		dataset = new XYSeriesCollection();
		width = width;
		height = height;
	}

	public void addData(String name, double[] values) {
		XYSeries series = new XYSeries(name, autoSort);
		for (int i = 0; i < values.length; i++)
			series.add(i, values[i]);
		dataset.addSeries(series);
	}

	public void createChartPanel(String chartTitle, String xTitle, String yTitle) {
		JFreeChart chart = ChartFactory.createXYLineChart(
			chartTitle,
			xTitle,
			yTitle,
			dataset);
		ChartPanel chartPanel = new ChartPanel(chart);
		add(chartPanel, BorderLayout.CENTER);

		setSize(640, 480);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setLocationRelativeTo(null);
	}

	public static void main(String[] args) {
		int N = Integer.parseInt(args[0]);
		PeriodicRecurrence P = new PeriodicRecurrence(N);

		SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                PlotXYSeries chart = new PlotXYSeries("Test Plot", 640, 380);
				chart.addData("An", P.getRecurrenceSeries());
				// chart.addData("Periodic", P.getPeriodicSeries());
				chart.createChartPanel("Periodic Recurrence Chart", "X", "Y");
				chart.setVisible(true);
            }
        });
	}
}