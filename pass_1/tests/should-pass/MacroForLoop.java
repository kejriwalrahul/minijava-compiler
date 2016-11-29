#define FORLOOP(it, start, end, fn, tvar) {
	it = start;
	while ((it <= end)&&(it != end)) {
	    tvar = this.fn(it);
	    it = it + 1;
    }
}

class MacroForLoop {
    public static void main(String[] a){
        System.out.println(new A().run());
    }
}

class A {
    int sum;

    public int sumAcc(int val) {
	    sum = sum + val;
	    return sum;
    }

    public int run() {
		int tvar;
	    int i;
        sum = 0;
	    FORLOOP(i, 1, 5, sumAcc, tvar);
	    return sum;
    }
}
