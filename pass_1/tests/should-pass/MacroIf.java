#define MAXVAL(a, b, rval) { rval = a; if ((a <= b)&&(a != b)) rval = b; else {} } 

class MacroIf {
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    public int run(){
	    int a;
	    int b;
	    int max;
	    a = 5;
	    b = 3;
	    MAXVAL(a, b, max);
	    return max;
    }
}
