class ArrayReturn {
    public static void main(String[] a){
        System.out.println((new A().run())[0]);
    }
}

class A {
    public int[] run() {
	    int[] arr;
	    arr = new int[2];
	    arr[0] = 13;
	    arr[1] = 31;
	    return arr;
    }
}

