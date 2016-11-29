#define SQUARE(a) (a*a)
#define SUM(a, b) (a+b)
#define SUMSQUARES(a, b) (SUM(SQUARE(a), SQUARE(b)))

class CompoundMacroDef {
	public static void main(String[] a){
		System.out.println(new A().run());
	}
}

class A {
	public int run(){
		return SUMSQUARES(3, 4);
	}
} 
