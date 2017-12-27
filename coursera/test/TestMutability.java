import edu.princeton.cs.algs4.StdOut;

public class TestMutability {
	private int me;
	public TestMutability(int me) {
		this.me = me;
	}

	private void mutateMe(TestMutability taget) {
		taget.me = 4;
	}


	// do unit testing of this class
	public static void main(String[] args) {
		TestMutability testme = new TestMutability(2);
		StdOut.println(testme.me);
		testme.mutateMe(testme);
		StdOut.println(testme.me);
	}

}