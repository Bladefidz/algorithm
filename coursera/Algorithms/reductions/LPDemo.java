/******************************************************************************
 *  Compilation:  javac -cp .:or124.jar LPDemo.java
 *  Execution:    java  -cp .:or124.jar LPDemo
 *  Dependencies: or124.jar
 *  
 *  max      13x1 + 23x2
 *            5x1 + 15x2   <=   480
 *            4x1 +  4x2   <=   160
 *           35x1 + 20x2   <=  1190
 *             x1 ,   x2   >=     0
 *
 *  Copied from http://opsresearch.com/OR-Objects/faq.html
 *
 *  % java LPDemo
 *  800.0
 *  Vector[2] = 
 *  [0] 11.999999999999998
 *  [1] 28.0
 *
 *  The optimal value is 800 with x1 = 12 and x2 = 28.
 *
 ******************************************************************************/


import drasys.or.mp.Problem; 
import drasys.or.mp.lp.DenseSimplex; 

public class LPDemo { 
    public static void main(String[] args) throws Exception {
//        final byte LTE = Constraint.LESS;
//        final byte GTE = Constraint.GREATER;
//        prob.newConstraint("c2").setType(LTE).setRightHandSide( 100.0);
//        prob.newConstraint("c3").setType(LTE).setRightHandSide(6000.0);

        Problem prob = new Problem(3, 2);
        prob.getMetadata().put("lp.isMaximize", "true");
        prob.newVariable("x1").setObjectiveCoefficient(13.0);
        prob.newVariable("x2").setObjectiveCoefficient(23.0);
        prob.newConstraint("corn").setRightHandSide(480.0);
        prob.newConstraint("hops").setRightHandSide(160.0);
        prob.newConstraint("malt").setRightHandSide(1190.0);
        
        prob.setCoefficientAt("corn", "x1",  5.0);
        prob.setCoefficientAt("corn", "x2", 15.0);
        prob.setCoefficientAt("hops", "x1",  4.0);
        prob.setCoefficientAt("hops", "x2",  4.0);
        prob.setCoefficientAt("malt", "x1", 35.0);
        prob.setCoefficientAt("malt", "x2", 20.0);
 
        StdOut.println(prob);
        DenseSimplex lp = new DenseSimplex(prob);
        StdOut.println(lp.solve());
        StdOut.println(lp.getSolution());
    }
}
