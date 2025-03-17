#!/bin/bash

flex trust.l

gcc lex.yy.c -lfl -o bills

./bills < ./Examples/bills_26.c >> tokens.txt