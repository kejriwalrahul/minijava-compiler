class WhileIf {
	public static void main(String[] a){
		System.out.println(new A().run(10));
	}
}

class A {
	public int run(int n) {
		int sum;
		int count;
		boolean cond;
		cond = false;
		sum = 0;
		count = 0;

		while (!cond) {
			sum = sum + count;
			if (count <= n)
				count = count + 1;
			else
				cond = false;
		}
		return sum;
	}
} 
