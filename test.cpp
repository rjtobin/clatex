/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
   
   Josh Tobin (tobinrj@tcd.ie), 2015
   ======================================================================== */

#include <cmath>
#include "clatex.hpp"

#define PI 3.141592

using namespace std;

// constants for our sine graph plot
const double dx = 0.005;
const int n = 5;
int length[n];
int l = 4. * M_PI / dx;
CPoint *pts[n];


void initialise_sines()
{
  for(int i=0; i<n; i++)
    pts[i] = new CPoint[l];
  for(int i=0; i<n; i++)
  {
    length[i] = l;
    for(int j=0; j<l; j++)
    {
      pts[i][j].x = dx * j;
      pts[i][j].y = sin(dx * j * (i+1));
    }
  }
}

int main()
{
  Clatex cl;

  // Make the title and a few sections
  
  cl.setTitle("My Test Document", "Test Author");
 
  CSection& introduction = cl.newSection("Introduction", false);
  CSection& details = cl.newSection("Details", false);
  
  CDrawing* d = new CDrawing();
  details.matchedText(d, "center");
  // Draw many sine functions
  
  CColor col[n];
  col[0].name = C_BLACK;
  col[1].name = C_CYAN;
  col[2].name = C_OLIVE;
  col[3].name = C_VIOLET;
  col[4].name = C_BROWN;

  initialise_sines();
   
  //DrawPlots(d, pts, col, length, n, 0 - 0.1, 4*M_PI + 0.1, -1. - 0.1, 1. + 0.1, 10., 5., false);

  CPlot* plot = new CPlot();
  plot->setSize(length[1]);
  for(int i=0; i<length[1]; i++)
  {
    plot->setPoint(i,pts[1][i].x, pts[1][i].y);
  }
  d->addShape(plot);
  
  // Add text to the sections 
  
  introduction.addText("This is a test article.  It is a              \
   \\LaTeX ~document generated through C++.  Below is an example image. ");

  introduction.addText("Here is some more text.");
  
  details.addText("\nHere is a test matrix:\n");

  // Make a matrix 
  
  details.mCmd("[");
  string test[3][3];
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      test[i][j] = "x^{" + to_string(i+j) + "}";

  DrawMatrix(details, test);
  details.mCmd("]");

  CSection sec[4];
  for(int i=1; i<=4; i++)
    sec[i-1].addText("This is the " + to_string(i) + "th item.");

  AddEnumerate(details, sec, 4, sizeof(CSection));

  details.addText("Finally, here is a table: \\\\");
  
  CTable* table;
  table = new CTable(3, "||c|c|c||");
  table->addHLine();
  
  vector<CText*> r(3);
  r[0] = new CText("r11");
  r[1] = new CText("r12");
  r[2] = new CText("r13");
  table->addRow(r);

  r[0] = new CText("r21");
  r[1] = new CText("r22");
  r[2] = new CText("r23");
  table->addRow(r);
  table->addHLine();

  
  details.matchedText(table, "center");
  
  cl.write("test.tex");
  
  for(int i=0; i<n; i++)
    delete[] pts[i];
  
  return 0;
}
