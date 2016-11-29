class MacroStmtNotDef{
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    public int run(){
	    int var;
	    var = 0;
	    ONE();
	    return var;
    }
}   
