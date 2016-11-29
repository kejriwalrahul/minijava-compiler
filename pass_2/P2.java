import syntaxtree.*;
import visitor.*;

public class P2 {
   public static void main(String [] args) {
      try {
         Node root = new MiniJavaParser(System.in).Goal();
         TypeCheck s = new TypeCheck();
         // First Pass
         root.accept(s);
         s.printdump();
         //	Yet to do second pass         
         root.accept(s);
         System.out.println("Program type checked successfully");
      }
      catch (ParseException e) {
         System.out.println(e.toString());
      }
   }
} 



