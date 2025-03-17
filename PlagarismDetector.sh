#!/bin/bash
FILES=./Examples/*
rm -f tokens.txt
make
for f in $FILES
do
  echo "Tokenizing $(basename "$f") file..."
  # Lex the C file to generate tokens
  ./scanner < "$f" > scanner_out.txt
  echo -ne "$(basename "$f")" >> tokens.txt
  echo -ne " " >> tokens.txt
  cat scanner_out.txt >> tokens.txt
  echo "" >> tokens.txt
done

echo "Detecting Plagiarism, this may take a few minutes."
# Run the C++ program with the generated tokens
./cmos < tokens.txt
echo "Done. Results stored in PlagarismReport.txt"
make clean