/* ========================================================================
   FILENAME $
   DESCRIPTION $
   PROJECT $
   Josh Tobin (tobinrj@tcd.ie), 2015
   ======================================================================== */

#include <cmath>
#include "clatex.hpp"

using namespace std;

template <size_t m, size_t n>
void Color(string (&mat)[m][n], int col, int d1, int d2)
{
  if(col)
  {
    for(int i=0; i<n; i++)
    {
      mat[d1][i] = "\\textcolor{red}{" + mat[d1][i] + "}"; 
      mat[d2][i] = "\\textcolor{green}{" + mat[d2][i] + "}"; 

    }
  }
  else
  {
    for(int j=0; j<m; j++)
    {
      mat[j][d1] = "\\textcolor{red}{" + mat[j][d1] + "}"; 
      mat[j][d2] = "\\textcolor{green}{" + mat[j][d2] + "}"; 
    }
  }
}

template <size_t m, size_t n>
void Mark(string (&mat)[m][n], int row, int col)
{
  mat[row][col] = "\\boxed{" + mat[row][col] + "}";
}


template <size_t m, size_t n>
void MakeDominatedMatrix(string (&mat)[m][n], int col, int dom, int& good)
{
  int mat_int[m][n];
  //for(int i=0; i<m; i++)
  //  mat[i] = new int[n];
  
  for(int i=0; i<m; i++)
    for(int j=0; j<n; j++)
    {
      mat_int[i][j] = (int)(drand48() * 30) - 15;
      //mat[i][j] = to_string(mat_int[i][j]);
    }
  
  if(col)
  {
    good = dom;
    do {
      good = (drand48() * m);
    } while(good == dom); 
 
    for(int i=0; i<m; i++)
    {
      int maxi = mat_int[good][i];
      mat_int[dom][i] = maxi + (int) ((drand48())*10);
    }
  }
  else
  {
    good = dom;
    do {
      good = (drand48() * n);
    } while(good == dom); 
    for(int i=0; i<m; i++)
    {
      int mini = mat_int[i][good];
      mat_int[i][dom] = mini - (int) ((drand48())*10);
    }
  }

  int adj = (int) (drand48() * 20) - 10;
  
  for(int i=0; i<m; i++)
    for(int j=0; j<n; j++)
      mat[i][j] = to_string(mat_int[i][j] + adj);

  
  return;
}

template <size_t m, size_t n>
void MakeSaddleMatrix(string (&mat)[m][n], int row, int col)
{
  int mat_int[m][n];
  //for(int i=0; i<m; i++)
  //  mat[i] = new int[n];
  
  for(int i=0; i<m; i++)
    for(int j=0; j<n; j++)
    {
      mat_int[i][j] = (int)(drand48() * 30) - 15;
      //mat[i][j] = to_string(mat_int[i][j]);
    }
  
  int x = mat_int[row][col];
  
  //int maxi = -10000, mini = 10000;
  for(int i=0; i<n; i++)
    //if(mat_int[row][i] < mini)
      if(i != col)
        mat_int[row][i] = x + (int) (drand48() * 7.);

   for(int i=0; i<m; i++)
     // if(mat_int[i][col] > maxi)
      if(i != row)
        mat_int[i][col] = x - (int) (drand48() * 7.);

   
  
  int adj = (int) (drand48() * 20) - 10;
  
  for(int i=0; i<m; i++)
    for(int j=0; j<n; j++)
      mat[i][j] = to_string(mat_int[i][j] + adj);

  
  return;
}


int main()
{
  Clatex cl, cla;
  cla.setTitle("Some 152 Solutions", "Josh Tobin");
  cl.setTitle("Some 152 Questions", "Josh Tobin");

  CSection *qs = cl.newSection("Domination Questions", false);
  CSection *as = cla.newSection("Domination Answers", false);
  
  qs->addText("In the following zero--sum matrix games, at least one row/column is dominated.  Find it.  ");

  int n = 200;
  
  CSection* questions = new CSection[n];
  CSection* answers = new CSection[n];
  
  string m[5][5];
  for(int i=0; i<n; i++)
  {
    if(drand48() < 0.7)
    {
      int col = drand48() * 2;
      int dom = drand48() * 5;
      int good;
      MakeDominatedMatrix(m,col,dom,good);
      questions[i].addText("\\[");
      DrawMatrix(&questions[i], m);
      questions[i].addText("\\]");

      answers[i].addText("\\[");
      Color(m,col,dom,good);
      DrawMatrix(&answers[i], m);
      answers[i].addText("\\]");
    }
    else
    {
      int row = drand48() * 5;
      int col = drand48() * 5;
      MakeSaddleMatrix(m,row,col);
      questions[i].addText("\\[");
      DrawMatrix(&questions[i], m);
      questions[i].addText("\\]");

      answers[i].addText("\\[");
      Mark(m,row,col);
      DrawMatrix(&answers[i], m);
      answers[i].addText("\\]");

    }
  }

  AddEnumerate(qs, questions, n, sizeof(CSection));
  AddEnumerate(as, answers, n, sizeof(CSection));
  
  cl.write("152.tex");
  cla.write("152_answers.tex");
 
  return 0;
}
