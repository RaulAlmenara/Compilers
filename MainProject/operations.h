int yyerror(char *s);
void add(sym_table *result, sym_table variable1, sym_table variable2, int modeNum);
void sub(sym_table *result, sym_table variable1, sym_table variable2, int modeNum);
void negative(sym_table *result, sym_table variable1, int modeNum);
void mul(sym_table *result, sym_table variable1, sym_table variable2, int modeNum);
void power(sym_table *result, sym_table variable1, sym_table variable2, int modeNum);
void divide(sym_table *result, sym_table variable1, sym_table variable2, int modeNum);
void mod(sym_table *result, sym_table variable1, sym_table variable2, int modeNum);
void biggerThan(sym_table *result, sym_table variable1, sym_table variable2, int equalVariable, int modeNum);
void lessThan(sym_table *result, sym_table variable1, sym_table variable2, int equalVariable, int modeNum);
void equal(sym_table *result, sym_table variable1, sym_table variable2, int notEqual, int modeNum);
void createList(list_Int *result, int gotoLine);
void fusiona(list_Int *result, list_Int list1, list_Int list2);
void emit(char* line);
void emitAssignation(char* id, char* expression);
char* temporalOperation(char* C3A1, char* op, char* C3A2);
void completeGoto(list_Int list, int line);
void printC3A();
int getLine();