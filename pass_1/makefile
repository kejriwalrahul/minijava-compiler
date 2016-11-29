all:
	bison -d P1.y
	flex P1.l
	gcc P1.tab.c lex.yy.c -lfl -o P1

clean:
	rm P1.tab.h P1.tab.c lex.yy.c P1 