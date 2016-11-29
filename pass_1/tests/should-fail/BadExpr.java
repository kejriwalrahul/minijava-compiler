class BadExpr {
    public static void main(String[] a){
        System.out.println(new B().run());
    }
}

class B {
    public int run() {
    	return 2+ -3;
    }
}
