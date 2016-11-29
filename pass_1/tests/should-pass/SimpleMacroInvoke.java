#define INVOKE(C, fn, arg) ( new C().fn(arg) )

class SimpleMacroInvoke {
    public static void main(String[] a){
        System.out.println(INVOKE(A, run, 3));
    }
}

class A {
    public int run(int val){
    	return val + 1;
    }
} 
