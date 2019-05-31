#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "symtab.h"
#include "operations.h"


extern FILE *yyin;
extern FILE *yyout;
FILE * logs;
extern int yylineno;
int numLine = 1;
char * C3A[1000];
char * line;
char * operationString;
int tempVar;



int yyerror(char *s)
{
	printf("Error: %s Line=> %d",s,yylineno);
	return -1;
}


/* OPERACIONS ARITMETIQUES */

void add(sym_table *result, sym_table variable1, sym_table variable2, int modeNum)
{
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "ADDI";
			result-> valType = INT_TYPE;
			result-> intType = variable1.intType + variable2.intType;
	}
	else if(variable1.valType == INT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			operationString = "ADDF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.intType + variable2.floatType;
			if(modeNum)
			{
				variable1.C3A = temporalOperation("","I2F",variable1.C3A);
			}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "ADDF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.floatType + variable2.intType;
			if(modeNum)
			{
				variable2.C3A = temporalOperation("","I2F",variable2.C3A);
			}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			operationString = "ADDF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.floatType + variable2.floatType;
	}
	else if(variable1.valType == INT_TYPE && variable2.valType == STRING_TYPE)
	{
			char * stringConcat = (char *) malloc(strlen(variable2.stringType) + 20);
			sprintf(stringConcat, "%d%s", variable1.intType, variable2.stringType);
			result-> valType = STRING_TYPE;
			result-> stringType = stringConcat;
	}
	else if(variable1.valType == STRING_TYPE && variable2.valType == INT_TYPE)
	{
			char * stringConcat = (char *) malloc(strlen(variable1.stringType) + 20);
			sprintf(stringConcat, "%s%d", variable1.stringType, variable2.intType);
			result-> valType = STRING_TYPE;
			result-> stringType = stringConcat;
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == STRING_TYPE)
	{
			char * stringConcat = (char *) malloc(strlen(variable2.stringType) + 20);
			sprintf(stringConcat, "%f%s", variable1.floatType, variable2.stringType);
			result-> valType = STRING_TYPE;
			result-> stringType = stringConcat;
	}
	else if(variable1.valType == STRING_TYPE && variable2.valType == FLOAT_TYPE)
	{
			char * stringConcat = (char *) malloc(strlen(variable1.stringType) + 20);
			sprintf(stringConcat, "%s%f", variable1.stringType, variable2.floatType);
			result-> valType = STRING_TYPE;
			result-> stringType = stringConcat;
	}
	else if(variable1.valType == STRING_TYPE && variable2.valType == STRING_TYPE)
	{
			char * stringConcat = (char *) malloc(strlen(variable1.stringType) + strlen(variable2.stringType) + 1);
			strcpy(stringConcat, variable1.stringType);
			strcat(stringConcat, variable2.stringType);
			result-> valType = STRING_TYPE;
			result-> stringType = stringConcat;
	}
	else
	{
		printf("Incompatyble elements type to ADD at: Line %d\n", yylineno);
	}

	if(modeNum)
	{
		result->C3A = temporalOperation(variable1.C3A, operationString, variable2.C3A);
	}
}


void sub(sym_table *result, sym_table variable1, sym_table variable2, int modeNum)
{
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "SUBI";
			result-> valType = INT_TYPE;
			result-> intType = variable1.intType - variable2.intType;
	}
	else if(variable1.valType == INT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			operationString = "SUBF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.intType - variable2.floatType;
			if(modeNum)
			{
				variable1.C3A = temporalOperation("","I2F",variable1.C3A);
			}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "SUBF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.floatType - variable2.intType;
			if(modeNum)
			{
				variable2.C3A = temporalOperation("","I2F",variable2.C3A);
			}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			operationString = "SUBF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.floatType - variable2.floatType;
	}
	else
	{
		printf("Incompatyble elements type to SUBSTRACT at: Line %d\n", yylineno);
	}

	if(modeNum)
	{
		result->C3A = temporalOperation(variable1.C3A, operationString, variable2.C3A);
	}
}


void negative(sym_table *result, sym_table variable1, int modeNum)
{
	if(variable1.valType == INT_TYPE)
	{
			operationString = "CHSI";
			result-> valType = INT_TYPE;
			result-> intType = -variable1.intType;
	}
	else if(variable1.valType == FLOAT_TYPE)
	{
			operationString = "CHSF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = -variable1.floatType;
	}
	else
	{
		printf("Incompatyble elements type to NEGATE at: Line %d\n", yylineno);
	}

	if(modeNum)
	{
		result->C3A = temporalOperation("", operationString, variable1.C3A);
	}
}


void mul(sym_table *result, sym_table variable1, sym_table variable2, int modeNum)
{
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "MULI";
			result-> valType = INT_TYPE;
			result-> intType = variable1.intType * variable2.intType;
	}
	else if(variable1.valType == INT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			operationString = "MULF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.intType * variable2.floatType;
			if(modeNum)
			{
				variable1.C3A = temporalOperation("","I2F",variable1.C3A);
			}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "MULF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.floatType * variable2.intType;
			if(modeNum)
			{
				variable2.C3A = temporalOperation("","I2F",variable2.C3A);
			}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			operationString = "MULF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.floatType * variable2.floatType;
	}
	else
	{
		printf("Incompatyble elements type to MULTIPLY at: Line %d\n", yylineno);
	}

	if(modeNum)
	{
		result->C3A = temporalOperation(variable1.C3A, operationString, variable2.C3A);
	}
}


void power(sym_table *result, sym_table variable1, sym_table variable2, int modeNum)
{
	int power = 1;
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
		operationString = "POWI";
		result-> valType = INT_TYPE;
		for(int i = 0; i < variable2.intType; i++)
			{
				power = power * variable1.intType;
			}
		result -> intType = power;
	}
	else
	{
		printf("Incompatyble elements type to POWER at: Line %d\n", yylineno);
	}

	if(modeNum)
	{
		result->C3A = temporalOperation(variable1.C3A, operationString, variable2.C3A);
	}
}


void divide(sym_table *result, sym_table variable1, sym_table variable2, int modeNum)
{
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "DIVI";
			result-> valType = INT_TYPE;
			result-> intType = variable1.intType / variable2.intType;
	}
	else if(variable1.valType == INT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			operationString = "DIVF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.intType / variable2.floatType;
			if(modeNum)
			{
				variable1.C3A = temporalOperation("","I2F",variable1.C3A);
			}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "DIVF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.floatType / variable2.intType;
			if(modeNum)
			{
				variable2.C3A = temporalOperation("","I2F",variable2.C3A);
			}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			operationString = "DIVF";
			result-> valType = FLOAT_TYPE;
			result-> floatType = variable1.floatType / variable2.floatType;
	}
	else
	{
		printf("Incompatyble elements type to DIVIDE at: Line %d\n", yylineno);
	}

	if(modeNum)
	{
		result->C3A = temporalOperation(variable1.C3A, operationString, variable2.C3A);
	}
}


void mod(sym_table *result, sym_table variable1, sym_table variable2, int modeNum)
{
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
			operationString = "MODI";
			result-> valType = INT_TYPE;
			result-> intType = variable1.intType % variable2.intType;
	}
	else
	{
		printf("Incompatyble elements type to MODUL at: Line %d\n", yylineno);
	}

	if(modeNum)
	{
		result->C3A = temporalOperation(variable1.C3A, operationString, variable2.C3A);
	}
}


void biggerThan(sym_table *result, sym_table variable1, sym_table variable2, int equalVariable, int modeNum)
{
	result-> valType = BOOL_TYPE;
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
			result-> boolType = (variable1.intType > variable2.intType) ? VAL_TRUE : VAL_FALSE;
	}
	else if(variable1.valType == INT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			result-> boolType = (variable1.intType > variable2.floatType) ? VAL_TRUE : VAL_FALSE;
			if(modeNum)
				{
					variable1.C3A = temporalOperation("","I2F",variable1.C3A);
					variable1.valType = FLOAT_TYPE;
				}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == INT_TYPE)
	{
			result-> boolType = (variable1.floatType > variable2.intType) ? VAL_TRUE : VAL_FALSE;
			if(modeNum)
				{
					variable2.C3A = temporalOperation("","I2F",variable2.C3A);
					variable2.valType = FLOAT_TYPE;
				}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			result-> boolType = (variable1.intType > variable2.intType) ? VAL_TRUE : VAL_FALSE;
	}
	else
	{
		printf("Incompatyble elements type to check BIGGER_THAN at: Line %d\n", yylineno);
	}

	if(result->boolType == VAL_FALSE && equalVariable == 1) 
	{
		equal(result, variable1, variable2, 0, modeNum);
	}
	else if(modeNum)
	{
		if (equalVariable == 0) operationString = "BG";
		else operationString = "BGE"; 
		line = (char *) malloc (50);
		sprintf(line,"IF %s %s %s GOTO",variable1.C3A ,operationString ,variable2.C3A);
		createList(&result->true_list, numLine);
		emit(line);
		createList(&result->false_list, numLine);
		emit("GOTO");
	}
}


void lessThan(sym_table *result, sym_table variable1, sym_table variable2, int equalVariable, int modeNum)
{
	result-> valType = BOOL_TYPE;
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
			result-> boolType = (variable1.intType < variable2.intType) ? VAL_TRUE : VAL_FALSE;
	}
	else if(variable1.valType == INT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			result-> boolType = (variable1.intType < variable2.floatType) ? VAL_TRUE : VAL_FALSE;
			if(modeNum)
				{
					variable1.C3A = temporalOperation("","I2F",variable1.C3A);
					variable1.valType = FLOAT_TYPE;
				}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == INT_TYPE)
	{
			result-> boolType = (variable1.floatType < variable2.intType) ? VAL_TRUE : VAL_FALSE;
			if(modeNum)
				{
					variable2.C3A = temporalOperation("","I2F",variable2.C3A);
					variable2.valType = FLOAT_TYPE;
				}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			result-> boolType = (variable1.intType < variable2.intType) ? VAL_TRUE : VAL_FALSE;
	}
	else
	{
		printf("Incompatyble elements type to check LESS_THAN at: Line %d\n", yylineno);
	}

	if(result->boolType == VAL_FALSE && equalVariable == 1) 
	{
		equal(result, variable1, variable2, 0, modeNum);
	}
	else if(modeNum)
	{
		if (equalVariable == 0) operationString = "LT";
		else operationString = "LTE"; 
		line = (char *) malloc (50);
		sprintf(line,"IF %s %s %s GOTO",variable1.C3A ,operationString ,variable2.C3A);
		createList(&result->true_list, numLine);
		emit(line);
		createList(&result->false_list, numLine);
		emit("GOTO");
	}
}


void equal(sym_table *result, sym_table variable1, sym_table variable2, int notEqual, int modeNum)
{
	result-> valType = BOOL_TYPE;
	if(variable1.valType == INT_TYPE && variable2.valType == INT_TYPE)
	{
			result-> boolType = (variable1.intType == variable2.intType) ? VAL_TRUE : VAL_FALSE;
	}
	else if(variable1.valType == INT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			result-> boolType = (variable1.intType == variable2.floatType) ? VAL_TRUE : VAL_FALSE;
			if(modeNum)
				{
					variable1.C3A = temporalOperation("","I2F",variable1.C3A);
					variable1.valType = FLOAT_TYPE;
				}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == INT_TYPE)
	{
			result-> boolType = (variable1.floatType == variable2.intType) ? VAL_TRUE : VAL_FALSE;
			if(modeNum)
				{
					variable2.C3A = temporalOperation("","I2F",variable2.C3A);
					variable2.valType = FLOAT_TYPE;
				}
	}
	else if(variable1.valType == FLOAT_TYPE && variable2.valType == FLOAT_TYPE)
	{
			result-> boolType = (variable1.intType == variable2.intType) ? VAL_TRUE : VAL_FALSE;
	}
	else
	{
		printf("Incompatyble elements type to check EQUAL at: Line %d\n", yylineno);
	}

	if(modeNum)
		{
			if(notEqual == 0){operationString = "EQ";}
			else{ operationString = "NEQ";}
			line = (char *) malloc (50);
			sprintf(line,"IF %s %s %s GOTO",variable1.C3A ,operationString ,variable2.C3A);
			createList(&result->true_list, numLine);
			emit(line);
			createList(&result->false_list, numLine);
			emit("GOTO");
		}
}

/* OPERACIONS C3A */

void createList(list_Int *result, int gotoLine)
{
	list_Int aux;
    aux.values = malloc(sizeof(int*));
    aux.size = 1;
    aux.values[0] = gotoLine;
    *result = aux;
}


void fusiona(list_Int *result, list_Int list1, list_Int list2)
{
    list_Int aux;
    aux.values = (int*) calloc(list1.size + list2.size, sizeof(int));
    int y = 0;
    
    for(int x = 0; x < list1.size; x++)
    {
        aux.values[x] = list1.values[x];
        y++;
    }

    for(int x = 0; x < list2.size; x++)
    {
        aux.values[y] = list2.values[x];
        y++;
    }
    aux.size = list1.size + list2.size;
    *result = aux;
}


void emit(char* line)
{
    C3A[numLine] = line;
   	numLine++;
}


char* temporalOperation(char* C3A1, char* operation, char* C3A2){
	line = (char *) malloc(50); 
	sprintf(line, "$t%d := %s %s %s", tempVar, C3A1, operation, C3A2);
	emit(line);
	line = (char *) malloc (10); 
	sprintf(line,"$t%d", tempVar);
	tempVar++;
	return line;
}


void completeGoto(list_Int list, int completeGoToLine)
{
    for(int x = 0; x < list.size; x++)
    {
    	line = (char *) malloc(40);
        sprintf(line,"%s %d", C3A[list.values[x]], completeGoToLine);
        C3A[list.values[x]] = line;
    }
}


void printC3A()
{
	for(int x = 1; x < numLine; x++)
	{
			printf("%d \t %s \n", x, C3A[x]);
	}	
}


int getLine()
{
	return numLine;
}



/* MAIN */

int main(int argc, char **argv)
	{ 
		numLine = 1;
		if (argc>1)
		{ 
			yyin=fopen(argv[1],"rt");
			if(yyin == NULL) 
			{
				printf("Error reading file\n");
				return -1;
			}	
			else 
			{
				logs = fopen("logs.out", "w+");
			}
		}
		else
		{ 
			printf("Missing input file\n");
			return -2;
		}
		yyparse();
		return 0;
	}