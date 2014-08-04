%{
#include <stdio.h>
#include <string.h>
#define YYSTYPE char *

%}


%token OPENPAREN CLOSEPAREN OPENSQUIGGLY CLOSESQUIGGLY VAR COMMA
%token SEMICOLON
%token EQUALS
%token NUMBER IDENTIFIER
%left  PLUS MINUS
%left  DIV MUL

o%%

file       : statements ;

statements :
           | statements statement SEMICOLON
           ;

statement : decl  { printf("; decl\n"); }
          | assign { printf("got assignment\n"); }
          | add_expr { printf("expression statement\n"); }
          ;

assign  : lvalue EQUALS add_expr { printf("ASSIGN %s = %s\n", $1, $3); }


add_expr: mul_expr
        | add_expr PLUS mul_expr  { printf("PLUS %s %s\n", $1, $3); }
        | add_expr MINUS mul_expr { printf("MINUS %s %s\n", $1, $3); }
        ;

mul_expr: primary
        | mul_expr MUL primary { printf("MUL %s %s\n", $1, $3); }
        | mul_expr DIV primary { printf("DIV %s %s\n", $1, $3); }
        ;

primary : NUMBER { $$ = $1; }
        | IDENTIFIER { $$ = $1; }
        ;


decl    : VAR varlist { printf("\tDECLARE\t%s\n", $2); }

varlist : lvalue                { $$ = $1; }
        | lvalue COMMA varlist  { $$ = $3; }
        ;

lvalue  : IDENTIFIER { $$ = $1; }
        ;
%%

extern const char *exename;

#if 0
#include <stdio.h>
char *progname;


main( argc, argv )
char *argv[];
{
  progname = argv[0];
  yyparse();
}

#endif // 0


int yyerror(char *s)
{
  warning( s , ( char * )0 );
  yyparse();
}

int warning (char *s, char *t)
{
  fprintf( stderr ,"%s: %s\n" , exename , s );
  if ( t )
    fprintf( stderr , " %s\n" , t );
}

