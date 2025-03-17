#!/bin/bash
rm -f tokens.txt

flex cmos.l

gcc lex.yy.c -lfl -o bills

./bills < ./Examples/bills_26.c >> tokens.txt

rm -f lex.yy.c

rm -f ./bills