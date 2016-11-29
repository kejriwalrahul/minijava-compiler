import syntaxtree.*;
import visitor.*;

public class P6 {
   public static void main(String [] args) {
      try {
         Node root = new MiniRAParser(System.in).Goal();
         Translator t = new Translator();
         System.out.println(root.accept(t));
      }
      catch (ParseException e) {
         System.out.println(e.toString());
      }
   }
}