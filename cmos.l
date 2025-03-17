%{
#include <stdio.h>
%}

%option noyywrap

%%

/* Ignore comments and whitespace */
"//".*                             { /* ignore C++ style comments */ }
"/*"([^*]|\*+)*"*/"            { /* ignore C style comments */ }
[ \t\n]+                           { /* ignore whitespace */ }

/* Keywords */


/* Numeric constants */
[0-9]+(\.[0-9]+)?                  { printf("202 "); }

/* String literals and character constants */

/* Identifiers (map all identifiers to a generic token) */
[a-zA-Z_][a-zA-Z0-9_]*             { printf("200 "); }

/* Multi-character operators */


/* Single-character operators and punctuation */


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
