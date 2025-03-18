%{
#include <stdio.h>

/* Token definitions with numeric codes */
#define T_INCLUDE      1
#define T_HEADER       2
#define T_VOID         3
#define T_INT          4
#define T_CHAR         5
#define T_RETURN       6
#define T_DO           7
#define T_WHILE        8
#define T_IF           9
#define T_ELSE         10
#define T_LPAREN       11
#define T_RPAREN       12
#define T_LBRACE       13
#define T_RBRACE       14
#define T_COMMA        15
#define T_SEMICOLON    16
#define T_EQ           17
#define T_NE           18
#define T_LE           19
#define T_GE           20
#define T_LT           21
#define T_GT           22
#define T_ASSIGN       23
#define T_PLUS         24
#define T_MINUS        25
#define T_TIMES        26
#define T_DIVIDE       27
#define T_MOD          28
#define T_ADDRESS      29
#define T_POINTER      30
#define T_NUMBER       31
#define T_STRING       32
#define T_IDENTIFIER   33
#define T_PRINTF       34
#define T_SCANF        35
%}

/* Rules section */
%%
"//".*                                         { /* Skip single-line comments */ }
"/*"(.|\n)*?"*/"                               { /* Skip multi-line comments */ }

"#include"                                     { fprintf(yyout, "%02d", T_INCLUDE); }
"<"[^>]*">"                                    { fprintf(yyout, "%02d", T_HEADER); }

"void"                                         { fprintf(yyout, "%02d", T_VOID); }
"int"                                          { fprintf(yyout, "%02d", T_INT); }
"char"                                         { fprintf(yyout, "%02d", T_CHAR); }
"return"                                       { fprintf(yyout, "%02d", T_RETURN); }
"do"                                           { fprintf(yyout, "%02d", T_DO); }
"while"                                        { fprintf(yyout, "%02d", T_WHILE); }
"if"                                           { fprintf(yyout, "%02d", T_IF); }
"else"                                         { fprintf(yyout, "%02d", T_ELSE); }

"("                                            { fprintf(yyout, "%02d", T_LPAREN); }
")"                                            { fprintf(yyout, "%02d", T_RPAREN); }
"{"                                            { fprintf(yyout, "%02d", T_LBRACE); }
"}"                                            { fprintf(yyout, "%02d", T_RBRACE); }
","                                            { fprintf(yyout, "%02d", T_COMMA); }
";"                                            { fprintf(yyout, "%02d", T_SEMICOLON); }

"=="                                           { fprintf(yyout, "%02d", T_EQ); }
"!="                                           { fprintf(yyout, "%02d", T_NE); }
"<="                                           { fprintf(yyout, "%02d", T_LE); }
">="                                           { fprintf(yyout, "%02d", T_GE); }
"<"                                            { fprintf(yyout, "%02d", T_LT); }
">"                                            { fprintf(yyout, "%02d", T_GT); }
"="                                            { fprintf(yyout, "%02d", T_ASSIGN); }

"+"                                            { fprintf(yyout, "%02d", T_PLUS); }
"-"                                            { fprintf(yyout, "%02d", T_MINUS); }
"*"                                            { fprintf(yyout, "%02d", T_TIMES); }
"/"                                            { fprintf(yyout, "%02d", T_DIVIDE); }
"%"                                            { fprintf(yyout, "%02d", T_MOD); }

"printf"                                       { fprintf(yyout, "%02d", T_PRINTF); }
"scanf"                                        { fprintf(yyout, "%02d", T_SCANF); }

"&"[A-Za-z_][A-Za-z0-9_]*                      { fprintf(yyout, "%02d", T_ADDRESS); }
"*"[A-Za-z_][A-Za-z0-9_]*                      { fprintf(yyout, "%02d", T_POINTER); }

[0-9]+                                         { fprintf(yyout, "%02d", T_NUMBER); }
\"([^\\\n]|(\\.))*?\"                          { fprintf(yyout, "%02d", T_STRING); }

[A-Za-z_][A-Za-z0-9_]*                         { fprintf(yyout, "%02d", T_IDENTIFIER); }
[ \t\n]+                                       { /* Ignore whitespace */ }
.                                              { /* Optionally ignore unknown characters */ }
%%
// In my Print Out I Opted to just remove the white space before handing it off to the next file
int main(void)
{
    yylex();
    return 0;
}

int yywrap() { return 1; }
