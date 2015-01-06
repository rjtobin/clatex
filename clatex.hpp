#if !defined(CLATEX_H)
/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
   
   Josh Tobin (tobinrj@tcd.ie), 2015
   ======================================================================== */

// XXX: this mEnd business is a mess
// XXX: add file options (font size etc) before opening
// XXX: ugh mCmd adding text is crazy
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

/*class CText
{
public:
  void getText(std::string& s);
private:
  CText* next;
  };*/

class CText
{
friend class CSection;
//friend class CDrawing;
public:
  CText();
  CText(std::string text);
  virtual ~CText();
  
  //CDrawing& newDrawing();

  virtual CText* addText(std::string text);
  virtual CText* addText(CText* next);

  virtual CText* prependText(std::string text);
  virtual CText* prependText(CText* next);

  virtual void getText(std::string& s);

  virtual CText* matchedCmd(std::string command);
  
  virtual void mCmd(std::string cmd, std::string arg_sq, std::string arg_brace);
  virtual void mCmd(std::string cmd, std::string arg_brace);
  virtual void mCmd(std::string cmd);

protected:  
  
  CText* mPrev;
  CText* mNext;
  std::string mText;

};

class CSection : public CText
{
public:
  CSection();
  CText* addText(CText* next);
  CText* addText(std::string text);
  void mCmd(std::string cmd, std::string arg_sq, std::string arg_brace);
  void mCmd(std::string cmd, std::string arg_brace);
  void mCmd(std::string cmd);
  CText* matchedCmd(std::string command);
  
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
  
private:
  void draw(std::string& text);
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

  CSection* newSection(std::string title);
  CSection* newSection(std::string title, bool number);
  
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

//void DrawTable(CText* text, 

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

static void string_cmd(std::string& s, std::string cmd, std::string arg_sq, std::string arg_brace);
static void string_cmd(std::string& s, std::string cmd, std::string arg_brace);
static void string_cmd(std::string& s, std::string cmd);


#endif
