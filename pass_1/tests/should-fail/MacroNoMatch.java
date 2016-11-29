#define ONE() (1+0)

class MacroNoMatch{
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    public int run(){
	    int var;
	    var = 1;
	    return ONE(var);
    }
}   
