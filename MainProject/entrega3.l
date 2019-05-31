%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "entrega3.tab.h"
int symbol;
sym_table table;
%}

%x comment
%x string

%%

"calc on"		return(CALC_ON);
"calc off"		return(CALC_OFF);
"HALT"			return(HALT);

"not"			return(NOT);
"and"			return(AND);
"or"			return(OR);

"if"			return(IF);
"then"			return(THEN);
"elsif"			return(ELSIF);
"else"			return(ELSE);
"fi"			return(FI);
"while"			return(WHILE);
"do"			return(DO);
"done"			return(DONE);
"repeat"		return(REPEAT);
"until"			return(UNTIL);
"for"			return(FOR);
"in"			return(IN);
".."			return(RANGE);

">"				return(GT);
">="			return(GE);
"=>"			return(GE);
"<"				return(LT);
"<="			return(LE);
"=<"			return(LE);
"="				return(EQ);
"<>"			return(NEQ);

"+"				return(ADD);
"-"				return(SUB);
"*"				return(MUL);
"/"				return(DIV);
"**"			return(POW);
"%"				return(MOD);

"("				return(PARENTH_INI);
")"				return(PARENTH_END);
":="			return(ASSIGN);
\"								BEGIN(string);
"/*"							BEGIN(comment);
<comment>"*/"					BEGIN(INITIAL);
(\/\/.*)						{};
<comment>(.|\n)					{};
"\n"			return(NEXT_LINE);


[0-9]+	{
	yylval.table.valType = INT_TYPE;
	yylval.table.intType = atoi( yytext );
	return TOK_INT;
}

[0-9]+\.[0-9]+  {
	yylval.table.valType = FLOAT_TYPE;
	yylval.table.floatType = atof( yytext );
	return TOK_FLOAT;
}

"TRUE" {
	yylval.table.valType = BOOL_TYPE;
	yylval.table.boolType = VAL_TRUE;
	return TOK_BOOL;
}

"FALSE" {
	yylval.table.valType = BOOL_TYPE;
	yylval.table.boolType = VAL_FALSE;
	return TOK_BOOL;
}

<string>{
	"\""		BEGIN(INITIAL);
	[^\n"]*	{ 	
				yylval.table.valType = STRING_TYPE; 
				yylval.table.stringType = strdup( yytext ); 
				return TOK_STRING; 
			}
}

[a-zA-Z0-9]* {
	yylval.table.valType = ID_TYPE;
	yylval.table.idType = strdup( yytext );
	symbol = sym_lookup(yylval.table.idType, &table);
	if (symbol == SYMTAB_NOT_FOUND)
	{
		return ASSIGN_ID;
	}
	else
	{	
		return TOK_ID;
	}
	
}

.	{};

%%