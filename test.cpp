/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
   
   Josh Tobin (tobinrj@tcd.ie), 2014
   ======================================================================== */

#include "clatex.hpp"

int main()
{
  Clatex cl;
  CText *introduction, *details;

  cl.setTitle("My Test Document", "Josh Tobin");

  introduction = cl.newSection("Introduction", false);
  details = cl.newSection("Details", false);

  CDrawing& d = details->newDrawing();
  d.addShape(new CPoint(0,0));
  d.addShape(new CPoint(0,1));
  d.addShape(new CPoint(1,0));
  d.addShape(new CPoint(1,1));
  d.draw();

  introduction->addText("This is a test paragraph.  Here is\
                             some test in this test paragraph.");

  
  cl.write("test.tex");

  return 0;
}
