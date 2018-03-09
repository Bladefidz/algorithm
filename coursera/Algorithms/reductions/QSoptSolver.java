/******************************************************************************
 *  Compilation:  javac -cp .:qsopt.jar QSoptSolver.java
 *  Execution:    java  -cp .:qsopt.jar QSoptSolver input.lp 
 *  Dependencies: https://algs4.cs.princeton.edu/65reductions/qpopt.jar
 *  Reference:    http://www2.isye.gatech.edu/~wcook/qsopt/software/java.htm
 *
 *  % java  -cp .:qsopt.jar QSoptSolver beer.lp
 *  Optimal profit = 800.0
 *  Optimal primal solution (nonzeros only): 
 *    A = 12.000000
 *    B = 28.000000
 *  Optimal dual solution (nonzeros only): 
 *    corn = -1.000000
 *    hops = -2.000000
 *  
 ******************************************************************************/

import java.io.IOException;
import qs.Problem;
import qs.QS;
import qs.QSException;
import qs.Reporter;

public class QSoptSolver {

    public static void main(String[] args) throws QSException, IOException {
        Problem problem = Problem.read(args[0], false);
        problem.opt_primal();
        if (problem.get_status() == QS.LP_OPTIMAL) {
            StdOut.println("Optimal " + problem.get_objname() + " = " + problem.get_objval());
            StdOut.println("Optimal primal solution (nonzeros only): ");
            problem.print_x(new Reporter(System.out), true, 6);
            StdOut.println("Optimal dual solution (nonzeros only): ");
            problem.print_pi(new Reporter(System.out), true, 6);
        } 
        else {
            System.out.println("Problem maybe unbounded or infeasible.\n");
        }
    }    
}
