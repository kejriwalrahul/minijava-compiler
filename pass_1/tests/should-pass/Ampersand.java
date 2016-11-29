class Ampersand {
	public static void main(String[] a){
		System.out.println(new A().run());
	}
}

class A {
	public boolean run() {
		if(true && true) System.out.println(1);			
		if(true && false) System.out.println(0);
		if(false && true) System.out.println(0);
		if(false && false) System.out.println(0);
		return 0;
	}
}

