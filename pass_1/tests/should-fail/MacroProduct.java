#define ONE() (1)

class MacroProduct{
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    public int run(){
	    return ONE() * ONE();
    }
} 
