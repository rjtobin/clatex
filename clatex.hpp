#if !defined(CLATEX_H)
/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
   
   Josh Tobin (tobinrj@tcd.ie), 2015
   ======================================================================== */

// XXX: add multiple plots and color support to CPlot 
// XXX: add file options (font size etc) before opening
// XXX: mCmd fixes, no random latex commands
// XXX: add packages dynamically, rather than in bulk
// XXX: better name for CSection? or some proper generic text thing

#define CLATEX_H

#include <deque>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Clatex; // XXX: should this be CLatex?
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
friend class CSection;
//friend class CDrawing;
public:
  CText();
  CText(std::string text);
  virtual ~CText();
  
  virtual CText& addText(std::string text);
  virtual CText& addText(CText* next);
//  virtual void centeredDrawing(CDrawing* d);
  virtual void matchedText(CText* text, std::string cmd);

  virtual CText& prependText(std::string text);
  virtual CText& prependText(CText* next);

  virtual void getText(std::string& s);


  virtual CText& matchedCmd(std::string command);
  
  virtual void mCmd(std::string cmd, std::string arg_sq, std::string arg_brace);
  virtual void mCmd(std::string cmd, std::string arg_brace);
  virtual void mCmd(std::string cmd);

  void dbg_print();

protected:  
  
  CText* mPrev;
  std::unique_ptr<CText> mNext;
  std::string mText;

};

class CSection : public CText
{
public:
  CSection();
  CText& addText(CText* next);
  CText& addText(std::string text);
  void mCmd(std::string cmd, std::string arg_sq, std::string arg_brace);
  void mCmd(std::string cmd, std::string arg_brace);
  void mCmd(std::string cmd);
  CText& matchedCmd(std::string command);
  
  //CText* prependText(CText* next);
  //CText* prependText(std::string text);
private:
  CText* mEnd;
};

class CShape
{
friend class CDrawing;
public:
  virtual ~CShape() {};
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
  CLine(double x1, double y1, double x2, double y2)
   : p1(CPoint(x1,y1)), p2(CPoint(x2,y2)) {};
  CLine(double x1, double y1, double x2, double y2, CColor Color)
   : p1(CPoint(x1,y1)), p2(CPoint(x2,y2)) {color = Color;};
  CLine(double x1, double y1, double x2, double y2, CColor Color, bool Thick)
   : p1(CPoint(x1,y1)), p2(CPoint(x2,y2)), thick(Thick) {color = Color;};

private:
  void draw(std::string& text);
};

class CGrid : public CShape
{
public:
  CPoint bot_left, top_right;
  int step;

  CGrid() : step(0) {color = CColor(C_GRAY);};
  CGrid(int st, double x1, double y1, double x2, double y2)
          : step(st), bot_left(CPoint(x1,y1)), top_right(CPoint(x2,y2))
  {color = CColor(C_GRAY);};

private:
  void draw(std::string& text);
};

class CPlot : public CShape
{
public:
  CPlot();
  
  int getSize();
  void setSize(int x);
  
  void setPoint(int i, double x, double y);
  double getX(int i);
  double getY(int i);

private:
  std::vector<CPoint> mPts; //XXX: should this be CPoint?
  void draw(std::string& out);
};

class CDrawing : public CText
{
//friend class CShape;
public:
  CDrawing();
  ~CDrawing();
  void addShape(CShape* shape);
  void getText(std::string& s);
private:
  std::deque<CShape*> mShapes;
};

class Clatex : public CText
{
public:
  Clatex();
  ~Clatex();
  
  void setTitle(std::string title, std::string author);  
  
  void write(std::string filename);

  CSection& newSection(std::string title);
  CSection& newSection(std::string title, bool number);
  
//private:
  std::string mTitle;
  std::string mAuthor;
  bool mOutTitle;
  
  std::deque<CSection*> mSections;
  std::deque<std::string> mSectionTitles;
  std::deque<bool> mSectionNumber;
  
  void mCmd(std::ofstream& mOut, std::string cmd, std::string arg_sq, std::string arg_brace);
  void mCmd(std::ofstream& mOut, std::string cmd, std::string arg_brace);
  void mCmd(std::ofstream& mOut, std::string cmd);
};

class CTable : public CText
{
public:
  CTable() : mCols(0), mJust("") {};
  CTable(unsigned int cols, std::string just);  
  ~CTable();

  void setNumCols(unsigned int cols);
  unsigned int getNumCols() {return mCols; };
  void setJust(std::string just);

  void addHLine();
  void addRow(std::vector<CText*>& row);
  
  void getText(std::string& s);
  
private:
  unsigned int mCols;
  std::string mJust;
  std::deque< std::vector<CText*> > mRows;
  void mFreeRows();
  std::map<unsigned int, unsigned int> mHLines;
};

// XXX: this is no longer...
void AddAxes(CDrawing* d, CPoint origin, double x_start, double x_end,
             double y_start, double y_end, double big_grading,
             double small_grading);
void DrawPlots(CDrawing* d, CPoint** plots, CColor* colors, int* length,
               int n, double start_x, double end_x, double start_y,
               double end_y, double x_size, double y_size, bool grid);

// XXX: oh man, this has to go
template <size_t m, size_t n>
void DrawMatrix(CText& cl, std::string (&mat)[m][n])
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
  cl.addText(text);
}

void AddEnumerate(CText& cl, CText* items, int n, size_t t);

static void string_cmd(std::string& s, std::string cmd, std::string arg_sq, std::string arg_brace);
static void string_cmd(std::string& s, std::string cmd, std::string arg_brace);
static void string_cmd(std::string& s, std::string cmd);

#endif
