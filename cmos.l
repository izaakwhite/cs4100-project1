%{
#include <stdio.h>
#include <stdlib.h>
%}

%option noyywrap

%%

/* Ignore comments and whitespace */
"//".*                             { /* ignore C++ style comments */ }
"/*"([^*]|\*+[^*/])*\*+\/           { /* ignore C style comments */ }
[ \t\n]+                           { /* ignore whitespace */ }

/* Keywords */
"auto"                             { printf("101 "); }
"break"                            { printf("102 "); }
"case"                             { printf("103 "); }
"char"                             { printf("104 "); }
"const"                            { printf("105 "); }
"continue"                         { printf("106 "); }
"default"                          { printf("107 "); }
"do"                               { printf("108 "); }
"double"                           { printf("109 "); }
"else"                             { printf("110 "); }
"enum"                             { printf("111 "); }
"extern"                           { printf("112 "); }
"float"                            { printf("113 "); }
"for"                              { printf("114 "); }
"goto"                             { printf("115 "); }
"if"                               { printf("116 "); }
"int"                              { printf("117 "); }
"long"                             { printf("118 "); }
"register"                         { printf("119 "); }
"return"                           { printf("120 "); }
"short"                            { printf("121 "); }
"signed"                           { printf("122 "); }
"sizeof"                           { printf("123 "); }
"static"                           { printf("124 "); }
"struct"                           { printf("125 "); }
"switch"                           { printf("126 "); }
"typedef"                          { printf("127 "); }
"union"                            { printf("128 "); }
"unsigned"                         { printf("129 "); }
"void"                             { printf("130 "); }
"volatile"                         { printf("131 "); }
"while"                            { printf("132 "); }

/* Numeric constants */
[0-9]+(\.[0-9]+)?                  { printf("202 "); }

/* String literals and character constants */
\"([^\\\n]|(\\.))*?\"              { printf("203 "); }
\'([^\\\n]|(\\.))*?\'              { printf("204 "); }

/* Identifiers (map all identifiers to a generic token) */
[a-zA-Z_][a-zA-Z0-9_]*             { printf("200 "); }

/* Multi-character operators */
"=="                               { printf("308 "); }
"!="                               { printf("309 "); }
">="                               { printf("312 "); }
"<="                               { printf("313 "); }
"++"                               { printf("306 "); }
"--"                               { printf("307 "); }
"&&"                               { printf("314 "); }
"||"                               { printf("315 "); }
"+="                               { printf("333 "); }
"-="                               { printf("334 "); }
"*="                               { printf("335 "); }
"/="                               { printf("336 "); }
"%="                               { printf("337 "); }
"<<="                              { printf("341 "); }
">>="                              { printf("342 "); }

/* Single-character operators and punctuation */
"+"                                { printf("301 "); }
"-"                                { printf("302 "); }
"*"                                { printf("303 "); }
"/"                                { printf("304 "); }
"%"                                { printf("305 "); }
"="                                { printf("332 "); }
">"                                { printf("310 "); }
"<"                                { printf("311 "); }
"!"                                { printf("316 "); }
"&"                                { printf("317 "); }
"|"                                { printf("318 "); }
"^"                                { printf("319 "); }
"~"                                { printf("320 "); }
"?"                                { printf("321 "); }
":"                                { printf("322 "); }
";"                                { printf("323 "); }
","                                { printf("324 "); }
"("                                { printf("325 "); }
")"                                { printf("326 "); }
"{"                                { printf("327 "); }
"}"                                { printf("328 "); }
"["                                { printf("329 "); }
"]"                                { printf("330 "); }
"."                                { printf("331 "); }

/* Catch-all: ignore any other character */
.                                  { /* do nothing */ }

%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}

int yywrap(void) {
    return 1;
}
