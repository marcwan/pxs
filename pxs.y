%{
#include <stdio.h>
#include <string.h>
#include "parse.h"
#define YYSTYPE char *

%}


%token OPENPAREN CLOSEPAREN OPENSQUIGGLY CLOSESQUIGGLY VAR COMMA
%token SEMICOLON
%token EQUALS
%token NUMBER IDENTIFIER
%left  PLUS MINUS
%left  DIV MUL

%%

file       : statements ;

statements:
           | statements statement SEMICOLON
           ;

statement : decl  {  }
          | assign { if (have_tmps()) printf("%s", remove_tmps()); }
          | add_expr { }
          ;

assign  : lvalue EQUALS add_expr { printf("\tSET\t%s, %s\n", $1, $3); }


add_expr: mul_expr
        | add_expr PLUS mul_expr
        {
            char *tmp = get_temp();
            printf("\tADD\t%s, %s, %s\n", $1, $3, tmp);
            $$ = tmp;
        }
        | add_expr MINUS mul_expr
        {
            char *tmp = get_temp();
            printf("\tSUB\t%s, %s, %s\n", $1, $3, tmp);
            $$ = tmp;
        }
        ;

mul_expr: primary
        | mul_expr MUL primary
        {
            char *tmp = get_temp();
            printf("\tMUL\t%s, %s, %s\n", $1, $3, tmp);
            $$ = tmp;
        }
        | mul_expr DIV primary
        {
            char *tmp = get_temp();
            printf("\tDIV\t%s, %s, %s\n", $1, $3, tmp);
            $$ = tmp;
        }
        ;

primary : NUMBER { $$ = $1; }
        | IDENTIFIER { $$ = $1; }
        ;


decl    : VAR varlist
        {
            printf("%s", pop_decls());
        }

varlist : lvalue                { push_decl($1); $$ = $1; }
        | lvalue COMMA varlist  { push_decl($1); $$ = $1; }
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



