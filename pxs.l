%{
#include <stdio.h>
#include <string.h>
#define YYSTYPE char *
#include "pxs.tab.h"
%}
ALPHA   [a-zA-Z]
NUMERIC [0-9]
ALNUM   {ALPHA}|{NUMERIC}
ASC     [\x00-\x7f]
ASCN    [\x00-\t\v-\x7f]
U       [\x80-\xbf]
U2      [\xc2-\xdf]
U3      [\xe0-\xef]
U4      [\xf0-\xf4]

UANY    {ASC}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}
UANYN   {ASCN}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U} 
UONLY   {U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}


%%

,                     return COMMA;
var                   return VAR;
\(                    return OPENPAREN;
\)                    return CLOSEPAREN;
\{                    return OPENSQUIGGLY;
\}                    return CLOSESQUIGGLY;
;                     return SEMICOLON;
=                     return EQUALS;
\+                    return PLUS;
\-                    return MINUS;
\*                    return MUL;
\/                    return DIV;
{NUMERIC}+            { yylval = strdup(yytext); return NUMBER; }
({NUMERIC}+)(\.{NUMERIC}+)?e(\+|\-)?({NUMERIC}+) { yylval = strdup(yytext); return NUMBER; }
({NUMERIC}+)(\.{NUMERIC}+)? { yylval = strdup(yytext); return NUMBER; }
({ALPHA}|{UONLY})?({ALNUM}|{UONLY})+ { yylval = strdup(yytext); return IDENTIFIER; }




%%
