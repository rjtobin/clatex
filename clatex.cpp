/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
  
   Josh Tobin (tobinrj@tcd.ie), 2014
   ======================================================================== */

#include "clatex.hpp"

using namespace std;

CDrawing& CText::newDrawing()
{
  mDrawings.push_front(CDrawing(this));
  return mDrawings.front();
}

void CText::addText(string text)
{
  mText += text;
}

void CText::mCmd(string cmd, string arg_sq, string arg_brace)
{
  mText += "\\" + cmd;
  if(arg_sq != "")
    mText += "[" + arg_sq + "]";
  if(arg_brace != "")
    mText += "{" + arg_brace + "}";
}

void CText::mCmd(string cmd, string arg_brace)
{
  mCmd(cmd, "", arg_brace);
}

void CText::mCmd(string cmd)
{
  mCmd(cmd, "", "");
}


void CPoint::draw(string& out)
{
  out += "\\filldraw (" + to_string(x) + "," + to_string(y) + ") circle [radius=0.08];\n";
}

void CLine::draw(string& out)
{
  out += "\\draw (" + to_string(p1.x) + "," + to_string(p1.y) + ") -- (" + to_string(p2.x) + "," + to_string(p2.y) + ");\n";
}

CDrawing::CDrawing(CText* parent)
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
  string& text = mParent->mText;
  mParent->mCmd("begin","tikzpicture");
  for(deque<CShape*>::iterator i=mShapes.begin(); i!=mShapes.end(); i++)
    (*i)->draw(text);
  mParent->mCmd("end","tikzpicture");
}

Clatex::Clatex()
 : mOutTitle(false)  
{
}

Clatex::~Clatex()
{
  for(deque<CText*>::iterator sec=mSections.begin();
      sec != mSections.end(); sec++)
  {
    delete (*sec);
  }  
}

void Clatex::setTitle(string title, string author)
{
  mTitle = title;
  mAuthor = author;
  mOutTitle = true;
}

void Clatex::write(string filename)
{
  ofstream out(filename);
  
  mCmd(out, "documentclass", "11pt", "article");
  mCmd(out, "usepackage", "tikz");
  if(mOutTitle)
  {
    mCmd(out, "title", mTitle);
    mCmd(out, "author", mAuthor);
  }

  mCmd(out, "begin", "document");
  if(mOutTitle)
    mCmd(out, "maketitle");

  deque<string>::iterator title = mSectionTitles.begin();
  deque<bool>::iterator number = mSectionNumber.begin();
  for(deque<CText*>::iterator sec=mSections.begin();
      sec != mSections.end(); sec++, title++, number++)
  {
    if(*number)
      mCmd(out, "section", *title);
    else
      mCmd(out, "section*", *title);
    out << (*sec)->mText << endl;
  }
      
  mCmd(out, "end", "document");

  out.close();
}

CText* Clatex::newSection(string title, bool number)
{
  mSectionTitles.push_back(title);
  CText* section = new CText();
  mSections.push_back(section);
  mSectionNumber.push_back(number);
  return section;
}

CText* Clatex::newSection(string title)
{
  return newSection(title, false);
}

/*void Clatex::mCmd(std::string cmd, std::string arg_sq, std::string arg_brace)
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
}*/

void Clatex::mCmd(std::ofstream& mOut, string cmd, string arg_sq, string arg_brace)
{
  mOut << "\\" << cmd;
  if(arg_sq != "")
    mOut << "[" << arg_sq << "]";
  if(arg_brace != "")
    mOut << "{" << arg_brace << "}";
  mOut << endl;
}

void Clatex::mCmd(std::ofstream& mOut, string cmd, string arg_brace)
{
  mOut << "\\" << cmd;
  if(arg_brace != "")
    mOut << "{" << arg_brace << "}";
  mOut << endl;
}

void Clatex::mCmd(std::ofstream& mOut, string cmd)
{
  mOut << "\\" << cmd;
  mOut << endl;
}
