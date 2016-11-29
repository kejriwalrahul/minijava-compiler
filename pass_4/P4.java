import syntaxtree.*;
import visitor.*;

public class P4 {
   public static void main(String [] args) {
      try {
         Node root = new MiniIRParser(System.in).Goal();
         TempCount t = new TempCount();
         root.accept(t);
         // System.out.println(t.maxTemp);
         irReduce c = new irReduce();
         c.currTemp = t.maxTemp + 1;
         root.accept(c);
      }
      catch (ParseException e) {
         System.out.println(e.toString());
      }
   }
} 