/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
  
   Josh Tobin (tobinrj@tcd.ie), 2015
   ======================================================================== */

#include "clatex.hpp"

using namespace std;

CText::CText()
 : mPrev(NULL), mNext(NULL)
{
}

CText::~CText()
{
  if(mNext)
    delete mNext;
}

CText::CText(string text)
 : mPrev(NULL), mNext(NULL), mText(text)
{ 
}

CText* CText::addText(string text)
{
  mText += text;
  return this;
}

CText* CText::addText(CText* text)
{
  if(mNext)
    text->mNext = mNext;
  mNext = text;
  text->mPrev = text;
  return text;
}

CText* CText::prependText(string text)
{
  mText = text + mText;
  return this;
}

CText* CText::prependText(CText* text)
{
  if(mPrev)
    text->mPrev = mPrev;
  mPrev = text;
  text->mNext = this;
  return text;
}

void CText::getText(string& s)
{
  s += mText;
  if(mNext)
    mNext->getText(s);
}

CText* CText::matchedCmd(string command)
{
  mCmd("begin", command);
  CText* mid = new CText();
  addText(mid);
  CText* end = new CText();
  mid->addText(end);
  end->mCmd("end", command);
  return mid;
}

void CText::mCmd(string cmd, string arg_sq, string arg_brace)
{
  string_cmd(mText, cmd, arg_sq, arg_brace);
}

void CText::mCmd(string cmd, string arg_brace)
{
  string_cmd(mText, cmd, arg_brace);
}

void CText::mCmd(string cmd)
{
  string_cmd(mText, cmd);
}

CSection::CSection()
{
  mEnd = mNext = new CText();
}

CText* CSection::addText(CText* next)
{
  next->mPrev = mEnd;
  mEnd->mNext = next;
  mEnd = next;
  return next;
}

CText* CSection::addText(string text)
{
  CText* next = new CText(text);
  return addText(next);
}

void CSection::mCmd(string cmd, string arg_sq, string arg_brace)
{
  mEnd->mCmd(cmd, arg_sq, arg_brace);
}

void CSection::mCmd(string cmd, string arg_brace)
{
  mEnd->mCmd(cmd, arg_brace);
}

void CSection::mCmd(string cmd)
{
  mEnd->mCmd(cmd);
}

CText* CSection::matchedCmd(string command)
{
  mCmd("begin", command);
  CText* mid = new CText();
  addText(mid);
  CText* end = new CText();
  addText(end);
  end->mCmd("end", command);
  return mid;
}

void CPoint::draw(string& out)
{
  out += "\\filldraw[" + color.name + "]";
  out += " (" + to_string(x) + "," + to_string(y) + ") circle [radius=0.08];\n";
}

void CLine::draw(string& out)
{
  out += "\\draw[";
  if(thick)
    out += "thick,";
  if(dashed)
    out += "dashed,";
  out += color.name + "] (" + to_string(p1.x) + "," + to_string(p1.y) + ") -- (" + to_string(p2.x) + "," + to_string(p2.y) + ");\n";
}

CDrawing::CDrawing()
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

void CDrawing::getText(string& text)
{
  string_cmd(text,"begin","tikzpicture");
  for(deque<CShape*>::iterator i=mShapes.begin(); i!=mShapes.end(); i++)
  {
    (*i)->draw(text);
  }
  string_cmd(text,"end","tikzpicture");
  if(mText != "")
    text += mText;
  if(mNext)
    mNext->getText(text);
}

Clatex::Clatex()
 : mOutTitle(false)  
{
}

Clatex::~Clatex()
{
  for(deque<CSection*>::iterator sec=mSections.begin();
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
  mCmd(out, "usepackage", "amsmath");

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
  for(deque<CSection*>::iterator sec=mSections.begin();
      sec != mSections.end(); sec++, title++, number++)
  {
    if(*number)
      mCmd(out, "section", *title);
    else
      mCmd(out, "section*", *title);
    string sec_text;
    (*sec)->getText(sec_text);
    out << sec_text << endl;
  }
      
  mCmd(out, "end", "document");

  out.close();
}

CSection* Clatex::newSection(string title, bool number)
{
  mSectionTitles.push_back(title);
  CSection* section = new CSection();
  mSections.push_back(section);
  mSectionNumber.push_back(number);
  return section;
}

CSection* Clatex::newSection(string title)
{
  return newSection(title, false);
}

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

void AddAxes(CDrawing* d, CPoint origin, double x_start, double x_end,
             double y_start, double y_end, double big_grading,
             double small_grading)
{
  d->addShape(new CLine(x_start, origin.y, x_end, origin.y));
  d->addShape(new CLine(origin.x, y_start, origin.x, y_end));

  for(double x=x_start; x<=x_end; x+=big_grading)
  {
    d->addShape(new CLine(x,origin.y-0.07,x,origin.y+0.07));
  }

  for(double y=y_start; y<=y_end; y+=big_grading)
  {
    d->addShape(new CLine(origin.x-0.07,y,origin.x+0.07,y));
  }

  for(double x=x_start; x<=x_end; x+=small_grading)
  {
    d->addShape(new CLine(x,origin.y-0.025,x,origin.y+0.025));
  }

  for(double y=y_start; y<=y_end; y+=small_grading)
  {
    d->addShape(new CLine(origin.x-0.025,y,origin.x+0.025,y));
  }
  
}

void DrawPlots(CDrawing* d, CPoint** plots, CColor* colors, int* length, int n)
{
  for(int i=0; i<n; i++)
  {
    for(int j=0; j<length[i]-1; j++)
    {
      CLine* nl = new CLine(plots[i][j], plots[i][j+1], colors[i]);
      nl->thick = true;
      d->addShape(nl);
    }
  }
}

// XXX: may our children forgive us (this is so bad...)

void AddEnumerate(CText* cl, CText* items, int n, size_t size)
{
  string text;
  text = "\\begin{enumerate}\n";
  for(int i=0; i<n; i++)
  {
    string res;
    items->getText(res);
    text += "\\item \n" + res + "\n";
    items = (CText*) ( ((char*) items) + size); // :(
  }
  text += "\\end{enumerate}\n\n";
  cl->addText(text);
}

static void string_cmd(string& s, string cmd, string arg_sq, string arg_brace)
{
  s += "\\" + cmd;
  if(arg_sq != "")
    s += "[" + arg_sq + "]";
  if(arg_brace != "")
    s += "{" + arg_brace + "}";
}

static void string_cmd(string& s, string cmd, string arg_brace)
{
  string_cmd(s, cmd, "", arg_brace);
}

static void string_cmd(string& s, string cmd)
{
  string_cmd(s, cmd, "", "");
}
