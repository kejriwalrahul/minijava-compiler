class IdType {
    public static void main(String[] a){
        System.out.println(new C().run());
    }
}

class A {
    public int run(){
        return 125;
    }
}

class B {
    public int run() {
	    return 126;
    }
}

class C {
    public int run() {
	    A a;
	    B b;
		a = new A();
		b = new B();
	    return (a.run()) + (b.run());
    }
}
