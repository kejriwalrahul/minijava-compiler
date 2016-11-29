class SimpleFunCall {
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    public int swap(int x, int y) {
        x = x+y; 
        y = x-y; 
        x = x-y;
	return x;
    }
    public int run(){
        int a;
        int b;
        a = 3;
        b = 5;
        a = swap(a, b);
        return a;
    }
} 

