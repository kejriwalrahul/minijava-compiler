package visitor;
import syntaxtree.*;
import java.util.*;

public class Proc{
	public ArrayList<Line> proc;
	public String label = "";
	public int argc = 0;
	
	public int stackSpace = 0;
	public int maxCallArgc = 0;
	
	public ArrayList<LiveRange> args;
	public ArrayList<LiveRange> liveRanges;
	public HashSet<String> used;
	public Node root;
	
	// public ArrayList<NodeToken> returnval;
	
	/*
	public HashSet<String> use = new HashSet<String>();
	public HashSet<String> def = new HashSet<String>();
	
	public HashSet<String> in = new HashSet<String>();
	public HashSet<String> out = new HashSet<String>();
	*/
} 