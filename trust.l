%{
#include <stdio.h>
%}

/* Rules section */
%%
"//".*                                         { /* Skip single-line comments */ }
"/*"(.|\n)*?"*/"                               { /* Skip multi-line comments */ }

"#include"                                     { printf("INCLUDE "); }
"<"[^>]*">"                                    { printf("HEADER(%s) ", yytext); }

"void"                                         { printf("VOID "); }
"int"                                          { printf("INT "); }
"char"                                         { printf("CHAR "); }
"return"                                       { printf("RETURN "); }
"do"                                           { printf("DO ");}
"while"                                        { printf("WHILE "); }
"if"                                           { printf("IF "); }
"else"                                         { printf("ELSE "); }

"("                                            { printf("LPAREN "); }
")"                                            { printf("RPAREN "); }
"{"                                            { printf("LBRACE "); }
"}"                                            { printf("RBRACE "); }
","                                            { printf("COMMA "); }
";"                                            { printf("SEMICOLON "); }

"=="                                           { printf("EQ "); }
"!="                                           { printf("NE "); }
"<="                                           { printf("LE "); }
">="                                           { printf("GE "); }
"<"                                            { printf("LT "); }
">"                                            { printf("GT "); }
"="                                            { printf("ASSIGN "); }

"+"                                            { printf("PLUS "); }
"-"                                            { printf("MINUS "); }
"*"                                            { printf("TIMES "); }
"/"                                            { printf("DIVIDE "); }
"%"                                            { printf("MOD "); }
"&"[A-Za-z_][A-Za-z0-9_]*                      { printf("ADDRESS ");}
"*"[A-Za-z_][A-Za-z0-9_]*                      { printf("POINTER ");}

[0-9]+                                         { printf("NUMBER(%s) ", yytext); }
\"([^\\\n]|(\\.))*?\"                          { printf("STRING(%s) ", yytext); }

[A-Za-z_][A-Za-z0-9_]*                         { printf("IDENTIFIER(%s) ", yytext); }
[ \t\n]+                                       { /* Skip whitespace */ }
.                                              { printf("UNKNOWN(%s) ", yytext); }
%%

/* Main function */
int main(void)
{
    yylex();
    return 0;
}
