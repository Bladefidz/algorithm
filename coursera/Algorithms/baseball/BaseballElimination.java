import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.ST;
import edu.princeton.cs.algs4.SET;
import edu.princeton.cs.algs4.Edge;
import edu.princeton.cs.algs4.FlowEdge;
import edu.princeton.cs.algs4.FlowNetwork;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.FordFulkerson;

/**
 * There are two cases how we can tell a team eliminated:
 * 	Trivial		: In the case that we can easily show a team should be
 * 		eliminated only by show there is a team which has more current win
 * 		point than maximum win point that a team could achieve.
 * 	Non Trivial	: In the case that we can not easily show a team should be
 * 		eliminated because there are may be two or more teams have probability
 * 		to having higher maximum win than maximum win that a team could
 * 		achieve.
 *
 * This case has special characteristics:
 * 	It is bipartite, so we can always found a match and minimum vertex cover
 * 	that could divide our graph into two subset by removing its minimum vertex
 * 	covers.
 */
public class BaseballElimination
{
	private class Eliminator
	{
		private Queue<String> q;
		Eliminator()
		{
			this.q = new Queue<>();
		}

		private void add(String team)
		{
			this.q.enqueue(team);
		}

		private int size()
		{
			return this.q.size();
		}

		private Queue<String> get()
		{
			return this.q;
		}
	}

	private int V;
	private String[] teamName;
	private ST<String, Integer> teams;
	private int[][] scores;
	private int matchesNum;  // Number of matches excluding one team
	private int[] matches;
	private Eliminator[] R;

	// create a baseball division from given filename in format specified below
	public BaseballElimination(String filename)
	{
		In in = new In(filename);
		this.V = in.readInt();
		this.teamName = new String[this.V];
		this.teams = new ST<>();
		this.scores = new int[this.V][3];
		this.matchesNum = sum(this.V-1);
		this.matches = new int[this.matchesNum];
		this.R = new Eliminator[this.V];

		int m = 0;
		for (int v = 0; v < this.V; v++)
		{
            String team = in.readString();
            this.teamName[v] = team;
            this.teams.put(team, v);

            for (int i = 0; i < 3; i++)
            	this.scores[v][i] = in.readInt();

            for (int i = 0; i < this.V; i++)
            {
            	int against = in.readInt();
            	if (i <= v) continue;
            	this.matches[m++] = against;
            }
        }

        // // Test for matches combination
        // for (int i = 0; i < this.matchesNum; i++)
        // 	StdOut.println(this.matches[i]);
        // StdOut.println();

        for (int v = 0; v < this.V; v++)
        	if (!trivialElimination(v))
        		nonTrivialEliminiation(v);
	}

	private int sum(int n)
	{
		if (n <= 0) return 0;
		return n + sum(n-1);
	}

	private boolean trivialElimination(int v)
	{
		boolean solved = false;
		int mx = this.scores[v][0] + this.scores[v][2];
		for (int w = 0; w < this.V; w++)
		{
			if (w != v)
			{
				if (mx < this.scores[w][0])
				{
					this.R[v] = new Eliminator();
					this.R[v].add(this.teamName[w]);
					solved = true;
					break;
				}
			}
		}
		return solved;
	}

	private void nonTrivialEliminiation(int v)
	{
		int m = this.matchesNum - this.V + 1;
		int n = this.matchesNum + 1;
		int mx = this.scores[v][0] + this.scores[v][2];
		FlowNetwork GF = new FlowNetwork(n+1);

		// StdOut.println(); // Test for new line
		int colNum = this.V-1;
		int offset = 0;
		int s = 1;
		int t1 = m+1;
		for (int i = 0; i < colNum; i++)
		{
			int length = offset + colNum - i;
			// StdOut.println(length);  // Check for length in matches combination
			if (i != v)
			{
				int col = i+1;
				int t2 = t1+1;
				for (int k = offset; k < length; k++)
				{
					if (col != v)
					{
						// Edge from s to s'
						FlowEdge e = new FlowEdge(
							0, s, this.matches[k], 0.0);
						// StdOut.println(e); // Check for edge
						GF.addEdge(e);

						// Edges from s' to t
						e = new FlowEdge(
							s, t1, Double.POSITIVE_INFINITY, 0.0);
						// StdOut.println(e); // Check for edge
						GF.addEdge(e);
						e = new FlowEdge(
							s, t2, Double.POSITIVE_INFINITY, 0.0);
						// StdOut.println(e); // Check for edge
						GF.addEdge(e);

						s++;
						t2++;
					}
					col++;
				}
				t1++;
			}
			offset = length;
		}

		for (int i = 0; i < this.V; i++)
		{
			FlowEdge e;
			if (i < v)
				e = new FlowEdge(m+i+1, n, mx-this.scores[i][0], 0.0);
			else if (i > v)
				e = new FlowEdge(m+i, n, mx-this.scores[i][0], 0.0);
			else continue;
			// StdOut.println(e); // Check for edge
			GF.addEdge(e);
		}

		// // Check for flow network
		// StdOut.println("Flow network for " + this.teamName[v]);
		// StdOut.println(GF);

		FordFulkerson FF = new FordFulkerson(GF, 0, n);
		// Check for st-cut
		for (s = m+1; s < n; s++)
		{
			// StdOut.println(s);
			if (FF.inCut(s))
			{
				int t = s-m-1;
				if (t>=v) t += 1;
				if (this.R[v] == null) this.R[v] = new Eliminator();
				this.R[v].add(this.teamName[t]);
			}
		}
	}

	// number of teams
	public int numberOfTeams()
	{
		return this.V;
	}

	// all teams
	public Iterable<String> teams()
	{
		return this.teams.keys();
	}

	private boolean isRegisteredTeam(String team)
	{
		return this.teams.get(team) != null;
	}

	// number of wins for given team
	public int wins(String team)
	{
		if (!isRegisteredTeam(team))
			throw new java.lang.IllegalArgumentException();
		int t = this.teams.get(team);
		return this.scores[t][0];
	}

	// number of losses for given team
	public int losses(String team)
	{
		if (!isRegisteredTeam(team))
			throw new java.lang.IllegalArgumentException();
		int t = this.teams.get(team);
		return this.scores[t][1];
	}

	// number of remaining games for given team
	public int remaining(String team)
	{
		if (!isRegisteredTeam(team))
			throw new java.lang.IllegalArgumentException();
		int t = this.teams.get(team);
		return this.scores[t][2];
	}

	private int diagonalRow(int i)
	{
		if (i == 0) return 0;
		return this.V - i + diagonalRow(i-1);
	}

	private int diagonalIndex(int i, int j)
	{
		int offset = diagonalRow(i);
		return offset + j - i - 1;
	}

	// number of remaining games between team1 and team2
	public int against(String team1, String team2)
	{
		if (!isRegisteredTeam(team1) || !isRegisteredTeam(team2))
			throw new java.lang.IllegalArgumentException();

		int t1 = this.teams.get(team1);
		int t2 = this.teams.get(team2);
		if (t1 == t2) return 0;

		int tmp = t1;
		if (t1>t2)
		{
			t1 = t2;
			t2 = tmp;
		}

		return this.matches[diagonalIndex(t1, t2)];
	}

	// is given team eliminated?
	public boolean isEliminated(String team)
	{
		if (!isRegisteredTeam(team))
			throw new java.lang.IllegalArgumentException();
		int t = this.teams.get(team);
		return this.R[t] != null;
	}

	// subset R of teams that eliminates given team; null if not eliminated
	public Iterable<String> certificateOfElimination(String team)
	{
		if (!isRegisteredTeam(team))
			throw new java.lang.IllegalArgumentException();
		int t = this.teams.get(team);
		if (this.R[t] == null) return null;
		return this.R[t].get();
	}

	public static void main(String[] args)
	{
		BaseballElimination division = new BaseballElimination(args[0]);
		for (String team : division.teams()) {
			if (division.isEliminated(team)) {
				StdOut.print(team + " is eliminated by the subset R = { ");
				for (String t : division.certificateOfElimination(team)) {
					StdOut.print(t + " ");
				}
				StdOut.println("}");
			}
			else {
				StdOut.println(team + " is not eliminated");
			}
		}

		// 	// Test each matche
		// for (String t1 : division.teams())
		// {
		// 	for (String t2 : division.teams())
		// 		StdOut.println(
		// 			t1 + " vs " + t2 + " "+ division.against(t1, t2));
		// }

		// // Test for null certificateOfElimination of not eliminated
		// for (String team : division.teams())
		// {
		// 	if (!division.isEliminated(team))
		// 		StdOut.println(division.certificateOfElimination(team));
		// }
	}
}