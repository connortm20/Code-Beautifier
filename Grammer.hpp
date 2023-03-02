/******************************************
 Modified by: Connor Marshall
 Computer Systems II Project 2
 02-06-23
 ********************************************/

#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

#include <fstream>
#include <iostream>

using namespace std;

bool validCode();
string tabStrings();

string program(istream &is);
string declaration(istream &is);
string idlist(istream &is);
string type(istream &is);
string compound(istream &is);
string stmtlist(istream &is);
string stmt(istream &is);
string exprlist(istream &is);
string expr(istream &is);
string simpexpr(istream &is);
string term(istream &is);
string factor(istream &is);


#endif
