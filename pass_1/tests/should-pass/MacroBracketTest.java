#define SUM(a, b) (a+b)

class MacroBracketTest{
	public static void main(String[] a){
		System.out.println(new A().run());
	}
}

class A {
	public int run(){
		return (SUM(2,0)) * (SUM(1,3));
	}
} 
