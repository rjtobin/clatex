/* ========================================================================
   CLaTeX
   A simple library to handle LaTeX output from C++.
  
   Josh Tobin (tobinrj@tcd.ie), 2015
   ======================================================================== */

#include "clatex.hpp"

#include <iostream>
using namespace std;

CText::CText()
 : mPrev(NULL), mNext(nullptr)
{
}

CText::~CText()
{
}

CText::CText(string text)
 : mPrev(NULL), mNext(nullptr), mText(text)
{ 
}

void CText::dbg_print()
{
  CText* cur = this;
  while(cur != NULL)
  {
    cout << "A node: " << cur << " " << cur->mNext.get() << endl;
    string s;
    cur->getText(s);
    cout << s << endl;
    cur = cur->mNext.get();
  }
}

CText& CText::addText(string text)
{
  mText += text;
  return *this;
}

CText& CText::addText(CText* text)
{
  text->mPrev = this;
  text->mNext.reset(nullptr);
  text->mNext = move(this->mNext);
  this->mNext.reset(text);
  
  if(this->mNext->mNext != nullptr)
  {
    this->mNext->mNext->mPrev = this->mNext.get();
  }
  return *text;
}

/*void CText::centeredDrawing(CDrawing* d)
{
  CText& centered = matchedCmd("center");
  centered.addText(d);
  }*/

void CText::matchedText(CText* text, std::string cmd)
{
  CText& matched = matchedCmd(cmd);
  matched.addText(text);
}

CText& CText::prependText(string text)
{
  mText = text + mText;
  return *this;
}

CText& CText::prependText(CText* text)
{
  text->mNext.reset(nullptr);
  if(mPrev)
    text->mNext = move(mPrev->mNext);
  else
    text->mNext.reset(this);

  if(mPrev)
    text->mPrev = mPrev;
  mPrev = text;
  return *text;
}

void CText::getText(string& s)
{
  s += mText;
  if(mNext)
    mNext->getText(s);
}

CText& CText::matchedCmd(string command)
{
  mCmd("begin", command);
  CText& ret = addText(new CText());
  CText& end = addText(new CText());
  end.mCmd("end", command);

  return ret;
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
  mNext.reset(new CText());
  mEnd = mNext.get();
}

CText& CSection::addText(CText* next)
{
  next->mPrev = mEnd;
  next->mNext.reset(nullptr);
  next->mNext = move(mEnd->mNext);
  mEnd->mNext.reset(next);
  mEnd = mEnd->mNext.get();
  return *mEnd;
}

CText& CSection::addText(string text)
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

CText& CSection::matchedCmd(string command)
{
  mCmd("begin", command);
  CText& ret = addText(new CText());
  CText& end = addText(new CText());
  end.mCmd("end", command);

  return ret;
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

void CGrid::draw(string& out)
{
  //   \draw[step=1,gray,very thin] (0,0) grid (10,7); 

  out += "\\draw[step=" + to_string(step) + ",gray,very thin, ";
  out += color.name + "]";
  out += "(" + to_string(bot_left.x) + "," + to_string(bot_left.y) + ")";
  out += " grid ";
  out += "(" + to_string(top_right.x) + "," + to_string(top_right.y) + ");\n";
}

CPlot::CPlot()
{
}

int CPlot::getSize()
{
  return mPts.size();
}

void CPlot::setSize(int x)
{
  mPts.resize(x);
}

void CPlot::setPoint(int i, double x, double y)
{
  if(i<0 || i>= mPts.size())
    return;
  mPts[i].x = x;
  mPts[i].y = y;
}

double CPlot::getX(int i)
{
  return mPts[i].x; // XXX: bounds checking 
}

double CPlot::getY(int i)
{
  return mPts[i].y; // XXX: bounds checking 
}

void CPlot::draw(string& out)
{
  out += "\\datavisualization [";
  out += "scientific axes, visualize as smooth line";
  out += "]\n";

  out += "data {";
  out += "x,y\n";
  for(vector<CPoint>::iterator i=mPts.begin(); i!=mPts.end(); i++)
  {
    out += to_string(i->x) + "," + to_string(i->y) + "\n";
  }
  out += "};\n";
}

CDrawing::CDrawing()
{
  mNext.reset(nullptr);
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
  mCmd(out, "usetikzlibrary", "datavisualization");

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

CSection& Clatex::newSection(string title, bool number)
{
  mSectionTitles.push_back(title);
  CSection* section = new CSection();
  mSections.push_back(section);
  mSectionNumber.push_back(number);
  return *section;
}

CSection& Clatex::newSection(string title)
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

CTable::CTable(unsigned int cols, std::string just)
 : mCols(cols), mJust(just)
{
}

CTable::~CTable()
{
  mFreeRows();
}

void CTable::setNumCols(unsigned int cols)
{
  mCols = cols;
  mFreeRows();
  mJust = "";
}

void CTable::setJust(std::string just)
{
  mJust = just;
}

void CTable::addRow(vector<CText*>& row)
{
  if(row.size() != mCols)
    return;

  mRows.push_back(row);
}

void CTable::addHLine()
{
  int curRow = mRows.size();
  mHLines[curRow]++;
}

void CTable::getText(std::string& s)
{
  s += "\\begin{tabular}{";
  s += mJust;
  s += "}\n";

  int curRow = 0;
  for(deque< vector<CText*> >::iterator i=mRows.begin(); ; i++, curRow++)
  {
    if(curRow > 0)
      s += "\\\\ \n";
    if(mHLines.count(curRow))
    {
      for(int j=0; j<mHLines[curRow]; j++)
        s+= "\\hline ";
    }
    if(i==mRows.end())
      break;
    for(vector<CText*>::iterator j = i->begin(); j!=i->end(); j++)
    {
      if(j != i->begin())
        s += " & ";
      (*j)->getText(s);
    }
  }
  
  s += "\\end{tabular}\n";

  if(mNext)
    mNext->getText(s);
}

void CTable::mFreeRows()
{
  for(deque< vector<CText*> >::iterator i=mRows.begin(); i!=mRows.end(); i++)
  {
    for(vector<CText*>::iterator j = i->begin(); j!=i->end(); j++)
    {
      delete (*j);
    }
  }
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

// XXX fix grid so that it is lined up with integer values
// XXX add tics and labels

void DrawPlots(CDrawing* d, CPoint** plots, CColor* colors, int* length,
               int n, double start_x, double end_x, double start_y,
               double end_y, double x_size, double y_size, bool grid)
{
  if(grid)
    d->addShape(new CGrid(1,0,0,x_size,y_size));

  double x_scale = x_size / (end_x - start_x);
  double y_scale = y_size / (end_y - start_y);

  CPoint p1, p2;
  
  for(int i=0; i<n; i++)
  {
    for(int j=0; j<length[i]-1; j++)
    {
      p1.x = x_scale * (plots[i][j].x - start_x);
      p1.y = y_scale * (plots[i][j].y - start_y);
      p2.x = x_scale * (plots[i][j+1].x - start_x);
      p2.y = y_scale * (plots[i][j+1].y - start_y);
      
      CLine* nl = new CLine(p1,p2, colors[i]);
      nl->thick = true;
      d->addShape(nl);
    }
  }

  CLine* border = new CLine(CPoint(0,0), CPoint(x_size,0), CColor(C_BLACK));
  d->addShape(border);
  border = new CLine(CPoint(0,0), CPoint(0,y_size), CColor(C_BLACK));
  d->addShape(border);
  border = new CLine(CPoint(x_size,0), CPoint(x_size,y_size), CColor(C_BLACK));
  d->addShape(border);
  border = new CLine(CPoint(0,y_size), CPoint(x_size,y_size), CColor(C_BLACK));
  d->addShape(border);
  
}

// XXX: may our children forgive us (this is so bad...)

void AddEnumerate(CText& cl, CText* items, int n, size_t size)
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
  cl.addText(text);
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
