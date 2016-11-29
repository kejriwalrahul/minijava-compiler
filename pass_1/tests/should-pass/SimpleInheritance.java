class SimpleInheritance {
    public static void main(String[] a){
        System.out.println(new B().run());
    }
}

class A {
    int a;
    public int run(){
	    a = 1;
        return a;
    }
}

class B extends A {
    int b;
    public int run() {
	    a = 1;
	    b = 541;
	    return b - a;
    }
} 

