package visitor;
import syntaxtree.*;
import java.util.*;

public class Line{
	// public String statement = "";
	
	public ArrayList<NodeToken> statement = new ArrayList<NodeToken>();
	public int lno;
	public String label = "";
	public Node root;
	
	public HashSet<String> use = new HashSet<String>();
	public HashSet<String> def = new HashSet<String>();
	
	public HashSet<String> in = new HashSet<String>();
	public HashSet<String> out = new HashSet<String>();

	public HashSet<Line> succ = new HashSet<Line>();
	public HashSet<Line> pred = new HashSet<Line>();
} 