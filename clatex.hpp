#if !defined(CLATEX_H)
/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
   
   Josh Tobin (tobinrj@tcd.ie), 2014
   ======================================================================== */

// XXX: add file options (font size etc) before opening
// XXX: handle matched commands in a more elegant/natural way?
// XXX: add some mInit checks?
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
private:
  virtual void draw(std::string& text) = 0;
};

class CPoint : public CShape
{
public:
  int x, y;
  
  CPoint()
   : x(0), y(0) {}; 
  CPoint(int X, int Y)
   : x(X), y(Y) {};
private:
  void draw(std::string& out);
};

class CLine : public CShape
{
public:
  CPoint p1, p2;
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

#endif
