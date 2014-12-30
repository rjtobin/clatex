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

class Clatex;

class CShape
{
friend class CDrawing;
private:
  virtual void draw(std::ofstream& out) = 0;
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
  void draw(std::ofstream& out);

};

/*class CLine : public CShape
{
public:
  CPoint p1, p2;
private:
  void draw(std::ofstream& out);
  };*/

class CDrawing
{
//friend class CShape;
public:
  CDrawing(Clatex* parent);
  ~CDrawing();
  void addShape(CShape* shape);
  void draw();
private:
  std::deque<CShape*> mShapes;
  Clatex* mParent;
};

class Clatex
{
public:
  Clatex();
  Clatex(std::string filename, std::string title, std::string author);

  void open(std::string filename);
  void open(std::string filename, std::string title, std::string author);

  std::ofstream& getOutputFile();
  
  CDrawing& newDrawing();

  void close();
  
  
private:
  std::string mFilename;
  std::string mTitle;
  std::string mAuthor;
  
  bool mInit;
  std::ofstream mOut;
  bool mOutTitle;

  void mInitOutput();
  void mFinOutput();
  void mCmd(std::string cmd, std::string arg_sq, std::string arg_brace);
  void mCmd(std::string cmd, std::string arg_brace);
  void mCmd(std::string cmd);

  std::deque<CDrawing> mDrawings;
};

static void mCmd(std::ofstream& mOut, std::string cmd, std::string arg_sq, std::string arg_brace);
static void mCmd(std::ofstream& mOut, std::string cmd, std::string arg_brace);
static void mCmd(std::ofstream& mOut, std::string cmd);


#endif
