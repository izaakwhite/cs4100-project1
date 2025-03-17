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
%}

/* Rules section */
%%
"//".*                                         { /* Skip single-line comments */ }
"/*"(.|\n)*?"*/"                               { /* Skip multi-line comments */ }

"#include"                                     { fprintf(yyout, "%03d", T_INCLUDE); }
"<"[^>]*">"                                    { fprintf(yyout, "%03d", T_HEADER); }

"void"                                         { fprintf(yyout, "%03d", T_VOID); }
"int"                                          { fprintf(yyout, "%03d", T_INT); }
"char"                                         { fprintf(yyout, "%03d", T_CHAR); }
"return"                                       { fprintf(yyout, "%03d", T_RETURN); }
"do"                                           { fprintf(yyout, "%03d", T_DO); }
"while"                                        { fprintf(yyout, "%03d", T_WHILE); }
"if"                                           { fprintf(yyout, "%03d", T_IF); }
"else"                                         { fprintf(yyout, "%03d", T_ELSE); }

"("                                            { fprintf(yyout, "%03d", T_LPAREN); }
")"                                            { fprintf(yyout, "%03d", T_RPAREN); }
"{"                                            { fprintf(yyout, "%03d", T_LBRACE); }
"}"                                            { fprintf(yyout, "%03d", T_RBRACE); }
","                                            { fprintf(yyout, "%03d", T_COMMA); }
";"                                            { fprintf(yyout, "%03d", T_SEMICOLON); }

"=="                                           { fprintf(yyout, "%03d", T_EQ); }
"!="                                           { fprintf(yyout, "%03d", T_NE); }
"<="                                           { fprintf(yyout, "%03d", T_LE); }
">="                                           { fprintf(yyout, "%03d", T_GE); }
"<"                                            { fprintf(yyout, "%03d", T_LT); }
">"                                            { fprintf(yyout, "%03d", T_GT); }
"="                                            { fprintf(yyout, "%03d", T_ASSIGN); }

"+"                                            { fprintf(yyout, "%03d", T_PLUS); }
"-"                                            { fprintf(yyout, "%03d", T_MINUS); }
"*"                                            { fprintf(yyout, "%03d", T_TIMES); }
"/"                                            { fprintf(yyout, "%03d", T_DIVIDE); }
"%"                                            { fprintf(yyout, "%03d", T_MOD); }

"&"[A-Za-z_][A-Za-z0-9_]*                      { fprintf(yyout, "%03d", T_ADDRESS); }
"*"[A-Za-z_][A-Za-z0-9_]*                      { fprintf(yyout, "%03d", T_POINTER); }

[0-9]+                                         { fprintf(yyout, "%03d", T_NUMBER); }
\"([^\\\n]|(\\.))*?\"                          { fprintf(yyout, "%03d", T_STRING); }

[A-Za-z_][A-Za-z0-9_]*                         { fprintf(yyout, "%03d", T_IDENTIFIER); }
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
