/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
   
   Josh Tobin (tobinrj@tcd.ie), 2014
   ======================================================================== */

#include "clatex.hpp"

int main()
{
  Clatex cl;
  cl.open("test.tex", "My Test Document", "Josh Tobin");
  CDrawing& d = cl.newDrawing();
  d.addShape(new CPoint(0,0));
  d.addShape(new CPoint(0,1));
  d.addShape(new CPoint(1,0));
  d.addShape(new CPoint(1,1));
  d.draw();
  cl.close();

  return 0;
}
