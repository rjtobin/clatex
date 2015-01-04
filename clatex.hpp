#if !defined(CLATEX_H)
/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
   
   Josh Tobin (tobinrj@tcd.ie), 2015
   ======================================================================== */

// XXX: add file options (font size etc) before opening
// XXX: handle matched commands in a more elegant/natural way?
//  PPP:  -- maybe have another higher level API
// XXX: use unique_ptr for CImage
// XXX: add packages dynamically, rather than in bulk

#define CLATEX_H

#include <fstream>
#include <deque>
#include <string>

class Clatex;
class CShape;
class CText;
class CDrawing;

#define C_RED "red"
#define C_GREEN "green"
#define C_BLUE "blue"
#define C_CYAN "cyan"
#define C_MAGENTA "magenta"
#define C_YELLOW "yellow"
#define C_BLACK "black"
#define C_GRAY "gray"
#define C_DARKGRAY "darkgray"
#define C_LIGHTGRAY "light_gray"
#define C_BROWN "brown"
#define C_LIME "lime"
#define C_OLIVE "olive"
#define C_ORANGE "orange"
#define C_PINK "pink"
#define C_PURPLE "purple"
#define C_TEAL "teal"
#define C_VIOLET "violet"
#define C_WHITE "white"

class CColor
{
public:
  std::string name;
  CColor() : name(C_BLACK) {};
  CColor(std::string name_) : name(name_) {};
};

class CText
{
friend class Clatex;
friend class CDrawing;
public:
  CDrawing& newDrawing();

  void addText(std::string text);
  
  void mCmd(std::string cmd, std::string arg_sq, std::string arg_brace);
  void mCmd(std::string cmd, std::string arg_brace);
  void mCmd(std::string cmd);
  
private:  
  
  std::deque<CDrawing> mDrawings;
  std::string mText;
};

class CShape
{
friend class CDrawing;
public:
  CColor color;
  virtual void draw(std::string& text) = 0;
};

class CPoint : public CShape
{
public:
  double x, y;
  
  CPoint()
   : x(0), y(0) {}; 
  CPoint(double X, double Y)
   : x(X), y(Y) {};
private:
  void draw(std::string& out);
};

class CLine : public CShape
{
public:
  CPoint p1, p2;
  bool thick;
  bool dashed;

  CLine()
   : thick(false), dashed(false) {};
  CLine(CPoint P1, CPoint P2)
   : p1(P1), p2(P2), thick(false), dashed(false) {};
  CLine(CPoint P1, CPoint P2, CColor Color)
   : p1(P1), p2(P2), thick(false), dashed(false) { color = Color;};
  
private:
  void draw(std::string& text);
};

class CDrawing
{
//friend class CShape;
public:
  CDrawing(CText* parent);
  ~CDrawing();
  void addShape(CShape* shape);
  void draw();
private:
  std::deque<CShape*> mShapes;
  CText* mParent;
};

class Clatex : public CText
{
public:
  Clatex();
  ~Clatex();
  
  void setTitle(std::string title, std::string author);  
  
  void write(std::string filename);

  CText* newSection(std::string title);
  CText* newSection(std::string title, bool number);
  
private:
  std::string mTitle;
  std::string mAuthor;
  bool mOutTitle;
  
  std::deque<CText*> mSections;
  std::deque<std::string> mSectionTitles;
  std::deque<bool> mSectionNumber;
  
  void mCmd(std::ofstream& mOut, std::string cmd, std::string arg_sq, std::string arg_brace);
  void mCmd(std::ofstream& mOut, std::string cmd, std::string arg_brace);
  void mCmd(std::ofstream& mOut, std::string cmd);
};

void DrawPlots(CDrawing* d, CPoint** plots, CColor* colors, int* length, int n);

template <size_t m, size_t n>
void DrawMatrix(CText* cl, std::string (&mat)[m][n])
{
  std::string text;
  text = "\\left( \\begin{array}{";
  for(int i=0; i<n; i++)
    text += "c";
  text += "}\n";
  for(int i=0; i<m; i++)
  {
    for(int j=0; j<n; j++)
    {
      text += mat[i][j];
      if(j<n-1)
        text+="&";
    }
    if(i<m-1)
      text+="\\\\";
    text+="\n";
  }
  text+="\\end{array}\\right)";
  cl->addText(text);
}


#endif
