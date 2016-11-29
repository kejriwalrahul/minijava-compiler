#define DIFF(a, b) (a-b)

class SimpleMacroExprArgs {
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    public int run(){
	    int a;
	    int b;
	    a = 3;
	    b = 7;
	    return DIFF(b, a);
    }
} 
