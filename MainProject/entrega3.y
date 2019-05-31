%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "operations.h"
#include "entrega3.tab.h"

FILE *logs;
extern FILE *yyin;
extern FILE *yyout;
extern int yylex(void);
int yylineno;
char * mode;
int modeNum;
char * auxLine;

sym_value_type table; 
%}

%union{
  sym_table table;
}

%token <table> TOK_INT TOK_FLOAT TOK_STRING TOK_BOOL TOK_ID
%token <table> ASSIGN_ID
%token <table> NEXT_LINE


%token ADD SUB MUL POW DIV MOD
%token GT GE LT LE EQ NEQ NOT AND OR
%token IF THEN ELSE ELSIF FI WHILE DO DONE REPEAT UNTIL FOR IN RANGE
%token PARENTH_INI
%token PARENTH_END
%token ASSIGN
%token HALT
%token CALC_ON CALC_OFF


%type <table> list_seq
%type <table> sequence
%type <table> line
%type <table> line_calc
%type <table> line_prog
%type <table> priorityOP3
%type <table> priorityOP2
%type <table> priorityOP1
%type <table> comparation
%type <table> compare
%type <table> andOP
%type <table> notOP
%type <table> boolOP
%type <table> assignation
%type <table> getNumLine
%type <table> generateGoTo

%start execution


%%

execution:
	| CALC_ON calc_on list_seq HALT {printf("END OF EXECUTION");}
	| CALC_OFF calc_off list_seq HALT {completeGoto($3.next_list, getLine()); emit("HALT");  printC3A();}

/* modeNum s'utilitza per indicar si estem en mode calculadora o programa per a passar a les funcions d'operació, 0 es calculadora i 1 es programa */

calc_on: {mode = "CALC"; modeNum = 0;};   
calc_off: {mode = "PROG"; modeNum = 1;};


getNumLine: {$$.intType = getLine();}
generateGoTo: {createList(&$$.next_list, getLine()); emit("GOTO");}


list_seq: sequence {$$ = $1;}
	| list_seq getNumLine sequence {if(mode == "PROG"){completeGoto($1.next_list, $2.intType); $$ = $3;}}


sequence: NEXT_LINE	{$$.next_list.size = 0;}
	| line NEXT_LINE {$$ = $1;}


line: line_calc {$$ = $1;}
	| assignation {$$ = $1; if(mode == "PROG"){createList(&$$.next_list, -1);}}
	| line_prog {$$ = $1;}


line_calc: priorityOP3	
	{
		if(mode == "PROG")
		{
			auxLine = (char *) malloc (40);
			sprintf(auxLine,"PUT %s",$1.C3A);
			emit(auxLine);
		}

		if($1.valType == INT_TYPE && mode == "CALC")
		{
			printf("%d\t EXPRESSIO (INT) = %i\n", yylineno, $1.intType);
		}
		else if($1.valType == FLOAT_TYPE && mode == "CALC")
		{
			printf("%d\t EXPRESSIO (FLOAT) = %f\n", yylineno, $1.floatType);
		}
		else if($1.valType == STRING_TYPE && mode == "CALC")
		{
			printf("%d\t EXPRESSIO (STRING) = %s\n", yylineno, $1.stringType);
		}
		yylineno++;
		$$ = $1;
	}

	| comparation
;

priorityOP3: priorityOP3 ADD priorityOP2 	{add(&$$, $1, $3, modeNum);}
		   	| priorityOP3 SUB priorityOP2 	{sub(&$$, $1, $3, modeNum);}
		   	| SUB priorityOP3 				{negative(&$$, $2, modeNum);}
		   	| priorityOP2					{$$ = $1;}	
;

priorityOP2: priorityOP2 POW priorityOP1	{power(&$$, $1, $3, modeNum);}
			| priorityOP2 MUL priorityOP1	{mul(&$$, $1, $3, modeNum);}
		   	| priorityOP2 DIV priorityOP1 	{divide(&$$, $1, $3, modeNum);}
		   	| priorityOP2 MOD priorityOP1 	{mod(&$$, $1, $3, modeNum);}
		   	| priorityOP1					{$$ = $1;}
;

priorityOP1: PARENTH_INI priorityOP3 PARENTH_END	{$$ = $2;}
			| TOK_INT {$$ = $1; if(mode == "PROG"){auxLine =(char *) malloc(15); sprintf(auxLine,"%d",$1.intType); $$.C3A = auxLine;}}
			| TOK_FLOAT {$$ = $1; if(mode == "PROG"){auxLine =(char *) malloc(15); sprintf(auxLine,"%f",$1.floatType); $$.C3A = auxLine;}}
			| TOK_STRING {$$ = $1; if(mode == "PROG"){$$.C3A = $1.stringType;}}

			| TOK_ID {
			 if ((sym_lookup($1.idType, &table) != SYMTAB_NOT_FOUND) && (table.valType == INT_TYPE))
			 	{
				 $$.intType = table.intType;  $$.valType = table.valType;
				 if(mode == "PROG") {$$.C3A = table.idType;}
			 	}
			 else if ((sym_lookup($1.idType, &table) != SYMTAB_NOT_FOUND) && (table.valType == FLOAT_TYPE))
			 	{
				 $$.floatType = table.floatType;  $$.valType = table.valType;
				 if(mode == "PROG") {$$.C3A = table.idType;}
			 	}
			  else if ((sym_lookup($1.idType, &table) != SYMTAB_NOT_FOUND) && (table.valType == STRING_TYPE))
			 	{
				 $$.stringType = table.stringType;  $$.valType = table.valType;
				 if(mode == "PROG") {$$.C3A = table.idType;}
			 	} 
			 }
;

comparation: compare {
	if(mode == "CALC")
		{ 
			printf("%d \t EXPRESSIO (BOOL) => %s",yylineno,($1.boolType == VAL_TRUE) ? "TRUE" : "FALSE");
			yylineno++;
		}
	$$ = $1;
}

compare: compare OR getNumLine andOP {$$.boolType = ($1.boolType == VAL_FALSE && $3.boolType == VAL_FALSE) ? VAL_FALSE : VAL_TRUE;
										if(mode =="PROG")
										{
											completeGoto($1.false_list, $3.intType);
						   					fusiona(&$$.true_list, $1.true_list, $4.true_list);
						   					$$.false_list = $4.false_list;
										}
									}
			| andOP					 {$$ = $1;}
;

andOP: andOP AND getNumLine notOP {$$.boolType = ($1.boolType == VAL_TRUE && $3.boolType == VAL_TRUE) ? VAL_TRUE : VAL_FALSE;
										if(mode =="PROG")
										{
											completeGoto($1.true_list, $3.intType);
											$$.true_list = $4.true_list;
						   					fusiona(&$$.false_list, $1.false_list, $4.false_list);
										}
									}
	  | notOP					{$$ = $1;}	

notOP: NOT boolOP			{$$.boolType = ($2.boolType == VAL_FALSE) ? VAL_TRUE : VAL_FALSE;
								if(mode =="PROG")
								{
									$$.true_list = $2.false_list;
								 	$$.false_list = $2.true_list;
								}
							}
	  | boolOP					{$$ = $1;}
;

boolOP: PARENTH_INI compare PARENTH_END	{$$ = $2;}
	   | priorityOP3 GT priorityOP3		{biggerThan(&$$, $1, $3, 0, modeNum);}
	   | priorityOP3 GE priorityOP3 	{biggerThan(&$$, $1, $3, 1, modeNum);}
	   | priorityOP3 LT priorityOP3 	{lessThan(&$$, $1, $3, 0, modeNum);}
	   | priorityOP3 LE priorityOP3 	{lessThan(&$$, $1, $3, 1, modeNum);}
	   | priorityOP3 EQ priorityOP3 	{equal(&$$, $1, $3, 0, modeNum);}
	   | priorityOP3 NEQ priorityOP3 	{equal(&$$, $1, $3, 1, modeNum); $$.boolType = ($$.boolType == VAL_FALSE) ? VAL_TRUE : VAL_FALSE;}
	   
	   | TOK_ID 
	   {
			 if ((sym_lookup($1.idType, &table) != SYMTAB_NOT_FOUND) && (table.valType == BOOL_TYPE))
			{
				$$ = table;
				if(mode == "PROG") {$$.C3A = table.idType;}
			}
		}

		| TOK_BOOL	
		{ $$ = $1; 
		  if(mode == "PROG")
		   {
			   if($1.boolType == VAL_TRUE) createList(&$$.true_list, getLine());
			   else createList(&$$.false_list, getLine());
			   emit("GOTO");
		   }
		}
;

assignation: ASSIGN_ID ASSIGN priorityOP3
		 	{
				table = $3;
				sym_add($1.idType, &table);
				if($3.valType == INT_TYPE)
				{
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (INT): %s := %d\n", yylineno, $1.idType, $3.intType);
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);
					}
				}
				else if($3.valType == FLOAT_TYPE)
				{
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (FLOAT): %s := %f\n", yylineno, $1.idType, $3.floatType);
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);					
					}
				}
			 	else if($3.valType == STRING_TYPE)
			 	{
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (STRING): %s := %s\n", yylineno, $1.idType, $3.stringType);
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);					
					}
				} 
			yylineno++;
			}
		| ASSIGN_ID ASSIGN compare
			{
				table = $3;
				sym_add($1.idType, &table);
				if($3.valType == BOOL_TYPE)
			 	{
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (BOOL): %s := %s\n", yylineno, $1.idType, ($3.boolType == VAL_TRUE) ? "TRUE" : "FALSE");
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);
					}
				}
				yylineno++;
			}	
		| TOK_ID ASSIGN priorityOP3 
			{
				if($3.valType == INT_TYPE)
				{
					table.intType = $3.intType;
					sym_enter($1.idType, &table);
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (INT): %s := %d\n", yylineno, $1.idType, $3.intType);
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);
					}
					yylineno++;
				}
				else if($3.valType == FLOAT_TYPE)
				{
					table.floatType = $3.floatType;
					sym_enter($1.idType, &table);
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (FLOAT): %s := %d\n", yylineno, $1.idType, $3.floatType);
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);
					}
					yylineno++;
				}
				else if($3.valType == STRING_TYPE)
				{
					table.stringType = $3.stringType;
					sym_enter($1.idType, &table);
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (STRING): %s := %d\n", yylineno, $1.idType, $3.stringType);
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);
					}
					yylineno++;
				}
				else if($3.valType == BOOL_TYPE)
				{
					table.boolType = $3.boolType;
					sym_enter($1.idType, &table);
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (BOOL): %s := %s\n", yylineno, $1.idType, ($3.boolType == VAL_TRUE) ? "TRUE" : "FALSE");
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);
					}
					yylineno++;
				}
			}
		| TOK_ID ASSIGN compare
			{
				table.boolType = $3.boolType;
					sym_enter($1.idType, &table);
					if(mode == "CALC")
					{
						printf("%d \t ASSIGN (BOOL): %s := %s\n", yylineno, $1.idType, ($3.boolType == VAL_TRUE) ? "TRUE" : "FALSE");
					}
					else
					{
						auxLine = (char *) malloc(40);
						sprintf(auxLine, "%s := %s", $1.idType, $3.C3A);
						emit(auxLine);
					}
				yylineno++;
			}			
;


line_prog: IF compare THEN getNumLine list_seq FI {
			completeGoto($2.true_list, $4.intType);
			fusiona(&$$.next_list, $2.false_list, $5.false_list);
		}
		| IF compare THEN getNumLine list_seq generateGoTo ELSE getNumLine list_seq FI {
			completeGoto($2.true_list, $4.intType);
	    	completeGoto($2.false_list, $8.intType);
			fusiona(&$$.next_list, $6.next_list, $9.next_list);
			fusiona(&$$.next_list, $$.next_list, $5.next_list);
   		 }
		 | WHILE getNumLine compare DO getNumLine list_seq DONE {
		 	completeGoto($6.next_list, $2.intType);
			completeGoto($3.true_list, $5.intType);
			$$.next_list = $3.false_list;
			auxLine = (char *) malloc (10);
			sprintf(auxLine,"GOTO %d",$2.intType);
			emit(auxLine);
		 }
		 | REPEAT getNumLine list_seq UNTIL getNumLine comparation {
		 	completeGoto($3.next_list, $5.intType);
			completeGoto($6.true_list, $2.intType);
			$$.next_list = $6.false_list;
		 }
;


%%