import syntaxtree.*;
import visitor.*;

public class P3 {
   public static void main(String [] args) {
      try {
         Node root = new MiniJavaParser(System.in).Goal();
         // Get classes structure         
         ClassStructure cs = new ClassStructure();
         root.accept(cs);
         CodeGen cg = new CodeGen();
         cg.structure = cs.structure;
         root.accept(cg);
      }
      catch (ParseException e) {
         System.out.println(e.toString());
      }
   }
} 