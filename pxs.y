%{
#include <stdio.h>
#include <string.h>
#include "parse.h"

#define YYSTYPE char *

%}


%token OPENPAREN CLOSEPAREN OPENSQUIGGLY CLOSESQUIGGLY VAR COMMA
%token SEMICOLON TRUEVAL FALSEVAL
%token EQUALS
%token FOR WHILE DO IF ELSE ELSEIF
%token NUMBER IDENTIFIER
%left  EQUALITY IDENTITY INEQUALITY NOTIDENTITY
%left  GT GTE LT LTE
%left  PLUS MINUS
%left  MOD
%left  DIV MUL

%%

file       : statements  { printnode($1); }
           ;

statements : /* empty */                    { $$ = first_statement(); }
           | statements statement { add_statement($1, $2);  }
           ;

statement : decl SEMICOLON     { $$ = $1; }
          | assign SEMICOLON   { $$ = $1; }
          | expr SEMICOLON     { }
          | for_loop { }
          | if_stmt  { $$ = $1; }
          ;


if_stmt  : IF OPENPAREN expr CLOSEPAREN OPENSQUIGGLY statements CLOSESQUIGGLY
         {
             $$ = if_statement_node($3, $6);
         }
         ;

for_loop : FOR OPENPAREN statement SEMICOLON expr SEMICOLON assign OPENSQUIGGLY statements CLOSESQUIGGLY
         | FOR OPENPAREN statement SEMICOLON expr SEMICOLON expr OPENSQUIGGLY statements CLOSESQUIGGLY
         ;


assign  : lvalue EQUALS expr { $$ = create_assignment($1, $3); }

expr    : OPENPAREN expr CLOSEPAREN { $$ = $2; }
        | op_expr { $$ = $1; }
        ;

op_expr    : primary
           | expr EQUALITY expr
           {
               $$ = expression_node("TESTEQ", $1, $3);
           }
           | expr INEQUALITY expr
           {
               $$ = expression_node("TESTNEQ", $1, $3);
           }
           | expr IDENTITY expr
           {
               $$ = expression_node("TESTIDENT", $1, $3);
           }
           | expr NOTIDENTITY expr
           {
               $$ = expression_node("TESTNIDENT", $1, $3);
           }
           | expr GT expr
           {
               $$ = expression_node("TESTGT", $1, $3);
           }
           | expr GTE expr
           {
               $$ = expression_node("TESTGTE", $1, $3);
           }
           | expr LT expr
           {
               $$ = expression_node("TESTLT", $1, $3);
           }
           | expr LTE expr
           {
               $$ = expression_node("TESTLTE", $1, $3);
           }
           | expr PLUS expr
           {
               $$ = expression_node("ADD", $1, $3);
           }
           | expr MINUS expr
           {
               $$ = expression_node("MINUS", $1, $3);
           }
           | expr MUL expr
           {
               $$ = expression_node("MUL", $1, $3);
           }
           | expr DIV expr
           {
               $$ = expression_node("DIV", $1, $3);
           }
           | expr MOD expr
           {
               $$ = expression_node("MOD", $1, $3);
           }
           ;

primary : NUMBER   { $$ = value_node($1); }
        | lvalue   { $$ = value_node($1); }
        | TRUEVAL  { $$ = value_node($1); }
        | FALSEVAL { $$ = value_node($1); }
        ;


decl    : VAR varlist { $$ = $2; }

varlist : lvalue                { $$ = first_decl($1);   }
        | lvalue COMMA varlist  { $$ = add_decl($3, $1); }
        ;

lvalue  : IDENTIFIER { $$ = $1; }
        ;


%%


extern const char *exename;
extern int   yylineno;

int yyerror(char *s)
{
  warning( s , ( char * )0 );
  yyparse();
}

int warning (char *s, char *t)
{
  fprintf( stderr ,"%s: %s (line %d)\n" , exename , s, yylineno );
  if ( t )
    fprintf( stderr , " %s\n" , t );
}



