/******************************************
 Modified by: Connor Marshall
 Computer Systems II Project 2
 02-06-23
 ********************************************/
#include "Grammer.hpp"

#include <cstdlib>

#include "Token.hpp"

static bool codeFailed = false; //static stores if a code sentence ever failed grammer

static int tabs = 0; //store current tab ammount
string tabString()//returns a string with the current tabs
{
  string rString = "";
  for(int i=0; i<tabs; i++)
    rString += "  ";

  return rString;
}

bool validCode()
{
  return !codeFailed;
}

string program(istream &is)
{
  int pos = is.tellg();

  string dVal = declaration(is);

  //if it starts with a proper declaration must be ->declaration program
  if(dVal != "DECLARATION_ERROR")
  {
    string pVal = program(is);
    return dVal + "\n" + pVal;
  }

  //must be ->compound
  is.clear();
  is.seekg(pos);

  string cVal = compound(is);
  if(cVal == "COMPOUND_ERROR")
    codeFailed = true;

  return "\n" + cVal;
}

string declaration(istream &is)
{
  string tVal = type(is);

  if(tVal=="TYPE_ERROR")
    return "DECLARATION_ERROR";

  string ilValue = idlist(is);

  Token next;
  next.get(is);

  if(next.type()!=SEMICOLON)
  {
    return "DECLARATION_ERROR";
  }

  //Last Token is SEMICOLON therefore ->type idlist SEMICOLON
  return tVal + " " + ilValue + next.value();
}

string idlist(istream &is)
{
  Token next;
  next.get(is);

  //if first token is not an ID
  if(next.type()!=ID)
  {
    codeFailed = true;
    return "IDLIST_ERROR";
  }

  string idVal = next.value();

  int pos = is.tellg();

  //MUST JUST BE ->ID
  next.get(is);
  if(next.type()!= COMMA)
  {
    is.clear();
    is.seekg(pos);
    return idVal;
  }

  //MUST BE ->ID COMMA idlist
  string idlVal = idlist(is);

  return idVal + next.value() + idlVal;

}

string type(istream &is)
{
  Token next;
  next.get(is);

  if(next.type()==INTEGER || next.type() == FLOAT || next.type() == VOID)
    return next.value();

  //if not then error in format
  return "TYPE_ERROR";
}

string compound(istream &is)
{
  //MUST BE -> BEGIN stmtlist END
  Token next;
  next.get(is);

  if(next.type()!=BEGIN)
  {
    codeFailed = true;
    return "COMPOUND_ERROR";
  }

  string bVal = next.value();
    tabs++;

  string slVal = stmtlist(is);
    

  next.get(is);
  if(next.type()!=END)
  {
    codeFailed = true;
    return "COMPOUND_ERROR";
  }

  tabs--;
  string eVal = tabString() + next.value();
  

  return bVal+ "\n" + slVal + "\n"+ eVal;
}

string stmtlist(istream &is)
{
  string sVal = stmt(is);
  int pos = is.tellg();

  Token semi;
  semi.get(is);

  //IF NOT SEMICOLON MUST BE ->stmt
  if(semi.type() != SEMICOLON)
  {
    is.clear();
    is.seekg(pos);
    return sVal;
  }

  //MUST BE ->stmt SEMICOLON stmtlist
  string semVal = semi.value();

  string slVal = stmtlist(is);

  return sVal + semVal+ "\n" + slVal;
}

string stmt(istream &is)
{
  int pos = is.tellg();
  Token next;
  next.get(is);

  //first  token is ID
  if(next.type() == ID)
  {
    string ival = tabString() + next.value();
    int pos = is.tellg();

    next.get(is);

    //->ID LPAREN exprlist RPAREN
    if(next.type()==LPAREN)
    {
      string lVal = next.value();
      string elVal = exprlist(is);
     
      next.get(is);
      string rVal = next.value();

      return ival + lVal + elVal + rVal;
    }

    //->ID ASSIGNOP expr
    if(next.type()==ASSIGNOP)
    {
      string opVal = next.value();
      string eVal = expr(is);

      return ival + " " + opVal + " " + eVal;
    }

    is.clear();
    is.seekg(pos);
    return ival;
  }

  //first token is IF
  if(next.type()==IF)
  {
    string iVal = tabString() + next.value();

    string exVal = expr(is);

    next.get(is);

    if(next.type()!= THEN)
    {
      codeFailed = true;
      return "STMT ERROR";
    }

    string thVal = next.value();
    
    tabs++;
    string cVal = tabString() + compound(is);
    tabs--;

    next.get(is);

    if(next.type()!=ELSE)
    {
      codeFailed = true;
      return "STMT ERROR";
    }

    string elVal = tabString() + next.value();

    tabs++;
    string cVal2 = tabString() + compound(is);
    tabs--;

    return iVal + exVal + " " + thVal + "\n" + cVal +"\n" + elVal + "\n" + cVal2;
  }

  //first token is WHILE  therefore ->WHILE LPAREN expr RPAREN compound
  if(next.type()==WHILE)
  {
    string wVal = tabString() + next.value();

    next.get(is);
    if(next.type()!=LPAREN)
    {
      codeFailed = true;
      return "STMT_ERROR";
    }
    //second token is LPAREN
    string lVal = next.value();

    string eVal = expr(is);

    next.get(is);
    if(next.type()!=RPAREN)
    {
      codeFailed = true;
      return "STMT_ERROR";
    }
    //fourth token is RPAREN
    string rVal = next.value();

    tabs++;
    string cVal = tabString() + compound(is);
    tabs--;

    return wVal + lVal + eVal + rVal + "\n" + cVal;
  }

  //Must be ->compound
  is.clear();
  is.seekg(pos);

  string cVal = tabString() + compound(is);
  return cVal;
}

string exprlist(istream &is)
{
  string eVal = expr(is);
  int pos = is.tellg();

  Token op;
  op.get(is);

  //->expr
  if(op.type()!=COMMA)
  {
    is.clear();
    is.seekg(pos);
    return eVal;
  }

  //->expr COMMA exprlist
  string com = op.value();
  string elVal = exprlist(is);
  return eVal + com + elVal;

}

string expr(istream &is)
{
  string sval = simpexpr(is);
  int pos = is.tellg();

  Token op;
  op.get(is);

  //if next token is not operator then must be expr->simpexpr
  if(op.type() != RELOP)
  {
//cout << "expr returned " << op.value() << endl;
    is.clear();
    is.seekg(pos);
    return sval;
  }

  //expr->simpexpr RELOP simpexpr
  string rVal = op.value();
  string sval2 = simpexpr(is);

  return sval + " " + rVal + " " + sval2;
}

string simpexpr(istream &is)
{
  string tval = term(is);
  int pos = is.tellg();

  Token op;
  op.get(is);

  //if next token is not operator then must be simpexpr->term
  if(op.type() != ADDOP)
  {
    is.clear();
    is.seekg(pos);
    return tval;
  }

  //simpexpr->term ADDOP simpexpr
  string opVal = op.value();
  string sval = simpexpr(is);

  return tval + " " + opVal + " " + sval;
}

string term(istream &is)
{
  string fval = factor(is);

  int pos = is.tellg();

  Token op;
  op.get(is);

  //if no operator then it must be term->factor
  if(op.type() != MULOP)
  {
    is.clear();
    is.seekg(pos);
    return fval;
  }

  //term->factor MULOP term
  string opVal = op.value();
  string tval = term(is);

  return fval + " " + opVal + " " + tval;
}

string factor(istream &is)
{
  //retrieve next token
  Token next;
  next.get(is);

  //return ID
  if(next.type()==ID)
  {
    string idVal = next.value();
    int pos = is.tellg();

    next.get(is);
    //must be just ->ID
    if(next.type()!=LPAREN)
    {
      is.clear();
      is.seekg(pos);
      return idVal;
    }
    //must be ID LPAREN exprlist RPAREN
    string lpVal = next.value();
    string elVal = exprlist(is);

    next.get(is);
    if(next.type()!=RPAREN)
    {
      return "FACTOR ERROR";
      codeFailed = true;
    }

    string rpVal = next.value();

    return idVal + lpVal + elVal + rpVal; 
  }


  //NUM_REAL
  if(next.type() == NUM_REAL)
    return next.value();

  //NUM_INT
  if(next.type() == NUM_INT)
    return next.value();

  //->LPAREN expr RPAREN
  if(next.type() == LPAREN)
  {
    string lVal = next.value();

    string eVal = expr(is);
    
    Token next;
    next.get(is);

    if(next.type() != RPAREN)
    {
      codeFailed = true;
      return "FACTOR_ERROR";
    }

    return lVal + eVal + next.value();
  }

  return "FACTOR_ERROR";
}