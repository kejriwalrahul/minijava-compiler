package visitor;
import syntaxtree.*;
import java.util.*;

public class LiveRange{
	public int start;
	public int end;
	
	public String temp;
	public String location = "";	
	public boolean stacked = false;
	public int stackLoc;
	public String argLoc = "";
	
	public String toString(){
		return "Start: " + Integer.toString(start) + " & End: " + Integer.toString(end) + " & TEMP " + temp + " & stacked?: " + stacked + " & location: " + location + " & stackLoc: " + stackLoc + "\n";
	}
} 