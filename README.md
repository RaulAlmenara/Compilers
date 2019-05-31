# Compilers
This repository contains the final version of the main project from the Compilers subject of the Computer Engineering degree.

The project consists on a lexical analyzer which identifies differents kinds of tokens from an input file and works depending on the chosen mode, Calc on or Calc off.

Calc on mode reads assignations and gives the result of simple mathematical expressions such as add, substract, multiply, divide, power and mod; or boolean operations such as biggerThan, lessThan, biggerThanEquals, lessThanEquals, Equals or notEquals.

Calc off creates and prints three adress code from the assignments and expressions read on the file.

To compile the project you must use a cygwin64 terminal on the project's directory and type "make".
When the project is compiled you have to execute "./NameOfFileToRead.txt" and the terminal will print the result.
