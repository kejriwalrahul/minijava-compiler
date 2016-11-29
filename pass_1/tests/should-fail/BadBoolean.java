class Ampersand {
	public static void main(String[] a){
		System.out.println(new A().run());
	}
}

class A {
	public boolean run() {
		if(true & true) {
			System.out.println(1);
		}
		if(true | true) {
			System.out.println(1);
		}
		if(1 < 2) {
			System.out.println(1);
		}
		return 0;
	}
}

