%{
#include <stdio.h>
#include <string.h>
#include "parse.h"

#define YYSTYPE char *

int yyerror(void **pout, char *s);
int warning (char *s, char *t);
int yylex (void);

%}


%parse-param { void **output_node }

%token OPENPAREN CLOSEPAREN OPENSQUIGGLY CLOSESQUIGGLY VAR COMMA
%token SEMICOLON TRUEVAL FALSEVAL
%token EQUALS
%token FOR WHILE DO IF ELSE ELSEIF FUNCTION RETURN BREAK CONTINUE
%token NUMBER IDENTIFIER
%left  EQUALITY IDENTITY INEQUALITY NOTIDENTITY
%left  GT GTE LT LTE
%left  PLUS MINUS
%left  MOD
%left  DIV MUL

%%

file       : statements  { *output_node = $1; }
           ;

statements : /* empty */          { $$ = first_statement(); }
           | statements statement { add_statement($1, $2);  }
           ;

statement : decl SEMICOLON        { $$ = $1; }
          | assign SEMICOLON      { $$ = $1; }
          | expr SEMICOLON        { $$ = $1; }
          | return_stmt SEMICOLON { $$ = $1; }
          | for_loop              { $$ = $1; }
          | while_loop            { $$ = $1; }
          | if_stmt               { $$ = $1; }
          | cont_stmt SEMICOLON   { $$ = $1; }
          | break_stmt SEMICOLON  { $$ = $1; }
          | function_decl         { $$ = $1; }
          ;


cont_stmt : CONTINUE              { $$ = continue_node(); }
          ;

break_stmt : BREAK                { $$ = break_node();    }
           ;


if_stmt  : IF OPENPAREN expr CLOSEPAREN OPENSQUIGGLY statements CLOSESQUIGGLY else_chain
         {
             $$ = if_statement_node($3, $6, $8);
         }
         ;

else_chain : /* empty */ { $$ = NULL; }
           | elseif OPENPAREN expr CLOSEPAREN OPENSQUIGGLY statements CLOSESQUIGGLY else_chain
           {
               $$ = elseif($3, $6, $8);
           }
           | ELSE OPENSQUIGGLY statements CLOSESQUIGGLY
           {
               $$ = danglingelse($3);
           }
           ;

elseif   : ELSEIF 
         | ELSE IF
         ;

for_loop : FOR OPENPAREN assign  SEMICOLON expr SEMICOLON assign_expr CLOSEPAREN OPENSQUIGGLY statements CLOSESQUIGGLY
         {
             $$ = for_loop_node($3, $5, $7, $10);
         }
         ;

while_loop : WHILE OPENPAREN expr CLOSEPAREN OPENSQUIGGLY statements CLOSESQUIGGLY
           {
               $$ = while_loop_node($3, $6);
           }
           ;

assign_expr : assign { $$ = $1; }
            | expr   { $$ = $1; }
            ;


assign  : lvalue EQUALS expr { $$ = create_assignment($1, $3); }

expr    : OPENPAREN expr CLOSEPAREN { $$ = $2; }
        | op_expr { $$ = $1; }
        | function_call { $$ = $1; }
        ;

expr_emptyok : /* empty ok */    { $$ = NULL; }
             | expr              { $$ = $1;   }
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
        | PLUS NUMBER { $$ = value_node($2); }
        | MINUS NUMBER { $$ = negative_const_value_node($2); }
        | lvalue   { $$ = value_node($1); }
        | PLUS lvalue   { $$ = value_node($2); }
        | MINUS lvalue   { $$ = negative_identifier_value_node($2); }
        | TRUEVAL  { $$ = value_node(strdup("true")); }
        | FALSEVAL { $$ = value_node(strdup("false")); }
        ;


return_stmt : RETURN expr_emptyok { $$ = return_node($2); }
            ;



function_decl: FUNCTION IDENTIFIER OPENPAREN paramlist_empty CLOSEPAREN OPENSQUIGGLY statements CLOSESQUIGGLY 
             {
                 $$ = function_declaration($2, $4, $7);
             }
             ;




function_call : IDENTIFIER OPENPAREN arglist CLOSEPAREN 
              { 
                  $$ = function_call_node($1, $3);
              }
              ;

arglist : /* empty */           { $$ = NULL; }
        | expr                  { $$ = first_func_arg($1); }
        | expr COMMA arglist    { $$ = add_func_arg($3, $1); }
        ;

decl    : VAR varlist { $$ = $2; }
        ;

paramlist_empty : /* empty ok */      { $$ = NULL; }
                | paramlist           { $$ = $1;   }
                ;

paramlist : lvalue                 { $$ = first_func_param($1);   }
          | lvalue COMMA paramlist { $$ = add_func_param($3, $1); }
          ;

varlist : lvalue                { $$ = first_decl($1);   }
        | lvalue COMMA varlist  { $$ = add_decl($3, $1); }
        ;

lvalue  : IDENTIFIER { $$ = $1; }
        ;


%%


extern const char *exename;
extern int   yylineno;


int yyerror(void **pout, char *s)
{
  warning( s , ( char * )0 );
  yyparse(pout);
  return 0;
}


int warning (char *s, char *t)
{
  fprintf( stderr ,"%s: %s (line %d)\n" , exename , s, yylineno );
  if ( t )
    fprintf( stderr , " %s\n" , t );
  return 0;
}



