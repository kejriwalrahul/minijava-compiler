class IfElse {
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    int var1;
    int var2;
    public int run(){
    	var1 = 2;
		var2 = 3;	
		if(var1 <= 2) {
			if(var2 <= 3) var2 = 4;
		}
		else {
			if(var2 <= 2) var1 = 5;
		}
        return var1;
    }
}
