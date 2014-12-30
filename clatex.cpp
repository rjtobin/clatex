/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
  
   Josh Tobin (tobinrj@tcd.ie), 2014
   ======================================================================== */

#include "clatex.hpp"

using namespace std;

void CPoint::draw(std::ofstream& out)
{
  out << "\\filldraw (" << x << "," << y << ") circle [radius=0.08];" << endl;
}

CDrawing::CDrawing(Clatex* parent)
 : mParent(parent)
{
}

CDrawing::~CDrawing()
{
  for(deque<CShape*>::iterator i=mShapes.begin(); i!=mShapes.end(); i++)
    delete *i;
}

void CDrawing::addShape(CShape* shape)
{
  mShapes.push_back(shape);
}

void CDrawing::draw()
{
  ofstream& out = mParent->getOutputFile();
  mCmd(out,"begin","tikzpicture");
  for(deque<CShape*>::iterator i=mShapes.begin(); i!=mShapes.end(); i++)
    (*i)->draw(out);
  mCmd(out,"end","tikzpicture");
}

Clatex::Clatex()
        : mInit(false)
{
}

Clatex::Clatex(string filename, string title, string author)
        : mInit(true), mFilename(filename), mAuthor(author), mOutTitle(true)
{
  mOut.open(filename.c_str());
  mInitOutput();
}

void Clatex::open(string filename)
{
  if(mInit)
    return;
  mFilename = filename;
  mInit = true;
  mOutTitle = false;

  mOut.open(filename.c_str());
  mInitOutput();
}

void Clatex::close()
{
  mFinOutput();
  mOut.close();
}

void Clatex::open(string filename, string title, string author)
{
  if(mInit)
    return;
  mFilename = filename;
  mTitle = title;
  mAuthor = author;
  mInit = true;

  mOut.open(filename.c_str());
  mInitOutput();
}

ofstream& Clatex::getOutputFile()
{
  return mOut;
}

CDrawing& Clatex::newDrawing()
{
  mDrawings.push_front(CDrawing(this));
  return mDrawings.front();
}

void Clatex::mInitOutput()
{
  mCmd("documentclass", "11pt", "article");
  mCmd("usepackage", "tikz");
  if(mOutTitle)
  {
    mCmd("title", mTitle);
    mCmd("author", mAuthor);
  }

  mCmd("begin","document");
  if(mOutTitle)
    mCmd("maketitle");
}

void Clatex::mFinOutput()
{
  mCmd("end","document");
}

void Clatex::mCmd(std::string cmd, std::string arg_sq, std::string arg_brace)
{
  ::mCmd(mOut, cmd, arg_sq, arg_brace);
}

void Clatex::mCmd(std::string cmd, std::string arg_brace)
{
  ::mCmd(mOut, cmd, arg_brace);
}

void Clatex::mCmd(std::string cmd)
{
  ::mCmd(mOut, cmd);
}

static void mCmd(std::ofstream& mOut, string cmd, string arg_sq, string arg_brace)
{
  mOut << "\\" << cmd;
  if(arg_sq != "")
    mOut << "[" << arg_sq << "]";
  if(arg_brace != "")
    mOut << "{" << arg_brace << "}";
  mOut << endl;
}

static void mCmd(std::ofstream& mOut, string cmd, string arg_brace)
{
  mOut << "\\" << cmd;
  if(arg_brace != "")
    mOut << "{" << arg_brace << "}";
  mOut << endl;
}

static void mCmd(std::ofstream& mOut, string cmd)
{
  mOut << "\\" << cmd;
  mOut << endl;
}
