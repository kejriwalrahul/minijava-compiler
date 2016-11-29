import syntaxtree.*;
import visitor.*;

public class P5 {
   public static void main(String [] args) {
      try {
         Node root = new microIRParser(System.in).Goal();
         BuildCFG b = new BuildCFG();
         root.accept(b);
      }
      catch (ParseException e) {
         System.out.println(e.toString());
      }
   }
} 


