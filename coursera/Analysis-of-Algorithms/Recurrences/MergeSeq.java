public class MergeSeq implements Sequence
{
   private final double[] c;

   public MergeSeq(int maxN)
   {
      c = new double[maxN+1];
      c[0] = 0;
      for (int N = 1; N <= maxN; N++)
         c[N] = N + c[N/2] + c[N-(N/2)];
   }

   public double eval(int N)
   {  return c[N];  }

   public static void main(String[] args)
   {
      int maxN = Integer.parseInt(args[0]);
      MergeSeq m = new MergeSeq(maxN);
      Values.show(m, maxN);
   }
}
