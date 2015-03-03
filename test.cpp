/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
   
   Josh Tobin (tobinrj@tcd.ie), 2015
   ======================================================================== */

#include <cmath>
#include "clatex.hpp"

#define PI 3.141592

using namespace std;

int main()
{
  Clatex cl;
  CSection *introduction, *details;

  // Make the title and a few sections
  
  cl.setTitle("My Test Document", "Test Author");

  introduction = cl.newSection("Introduction", false);
  details = cl.newSection("Details", false);

  CText* centered = details->matchedCmd("center");
  
  CDrawing* d = new CDrawing;
  centered->addText(d);

  // Draw many sine functions 

  double dx = 0.005;
  int n = 5;

  CColor col[n];
  col[0].name = C_BLACK;
  col[1].name = C_CYAN;
  col[2].name = C_OLIVE;
  col[3].name = C_VIOLET;
  col[4].name = C_BROWN;

  int length[n];
  int l = 4. * M_PI / dx;
  CPoint *pts[n];
  for(int i=0; i<n; i++)
    pts[i] = new CPoint[l];
  for(int i=0; i<n; i++)
  {
    length[i] = l;
    for(int j=0; j<l; j++)
    {
      pts[i][j].x = dx * j;
      pts[i][j].y = sin(dx * j * i);
    }
  }
  /*length[0] = length[1] = l;
  for(int i=0; i<l; i++)
  {
    
    pts[1][i].x = 10 * dx * i;
    pts[1][i].y = 5 * pow(2., 3. - 2. * ceil(2./(dx * i)));
    }*/

  AddAxes(d, CPoint(0,0), 0,5,0,2,0.5,0.1);

  for(int n=3; n<70; n++)
  {
    double y = 12. * pow(2.,3- 2. * n);
    double x1 = 5. * 2. / n;
    double x2 = 5. * 2. / (n-1);
    d->addShape(new CLine(x1,y,x2,y, CColor(C_ORANGE),true));
  }
  
  //DrawPlots(d, pts, col, length, n);

  // Add text to the sections 
  
  introduction->addText("This is a test article.  It is a             \
   \\LaTeX ~document generated through C++.  Below is an example image.");

  introduction->addText("Here is some more text.");
  
  details->addText("\nHere is a test matrix:\n");

  // Make a matrix 
  
  details->mCmd("[");
  string test[3][3];
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      test[i][j] = "x^{" + to_string(i+j) + "}";

  DrawMatrix(details, test);
  details->mCmd("]");

  CSection sec[4];
  for(int i=0; i<4; i++)
    sec[i].addText("This is the " + to_string(i) + "th item.");

  AddEnumerate(details, sec, 4, sizeof(CSection));
  
  cl.write("test.tex");
  
  for(int i=0; i<n; i++)
    delete[] pts[i];
  
  return 0;
}
