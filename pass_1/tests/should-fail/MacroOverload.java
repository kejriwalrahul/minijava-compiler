#define ONE() (1+0)
#define ONE(a) (a/a)

class MacroOverload{
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    public int run(){
	    return ONE();
    }
}  
