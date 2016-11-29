class NewInt {
    public static void main(String[] a){
        System.out.println((new A().run())[0]);
    }
}

class A {
    public int[] run() {
	    int[] arr;
	    int a;
		int b;
		int c;
		a = 4;
		b = 8;
		c = 120;
		arr = new int[(a*b)-(c/a)];
	    arr[0] = 13;
	    arr[1] = 31;
	    return arr;
    }
}

