all : lex.yy.c entrega3.tab.c
	cc lex.yy.c entrega3.tab.c operations.c symtab.c -o entrega3.exe -lfl -lm

lex.yy.c  : entrega3.l	
	flex entrega3.l	

entrega3.tab.c: entrega3.y
	bison -d entrega3.y

clean : 
	rm -f lex.yy.c entrega3.exe logs.out entrega3.tab.c entrega3.tab.h 

run : entrega3.exe
	./entrega3.exe ${FILE}