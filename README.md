CLaTeX
==============

A lightweight library for outputting latex from C++, 
with a particular emphasis on drawing graphs nicely.
Designed for use with some other projects (like grapt).
Very much a work in progress, see test.pdf for a 
demonstration of some current features.

The main goal is to provide a simple interface to
output mathematical documents that are generated 
procedurally, for example a report or randomly
generated homework questions.


Compilation
==============

To compile the reference code, use

g++ -std=c++11 test.cpp clatex.cpp

which will output a document test.tex.
