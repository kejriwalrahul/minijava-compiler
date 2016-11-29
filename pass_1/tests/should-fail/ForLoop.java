class ForLoop{
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    public int run(){
	    int var;
	    int sum;
	    sum = 0;
	    for(var = 1; var < 10; var++){
	        sum = sum + var;
	    }
	    return sum;
    }
}    
