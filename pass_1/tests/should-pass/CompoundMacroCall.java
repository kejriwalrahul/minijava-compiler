#define SQUARE(a) (a*a)
#define SUM(a, b) (a+b)

class CompoundMacroCall {
	public static void main(String[] a){
		System.out.println(new A().run());
	}
}

class A {
	public int run(){
		return SUM(SQUARE(3), SQUARE(4));
	}
} 
