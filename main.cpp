/*******************************************
 * File: main.cpp                          *
 * Original Author: S. Blythe              *
 * Date: 12/2022                           *
 * PURPOSE: driver file for project 1  
 *******************************************/

  /******************************************
 Modified by: Connor Marshall
 Computer Systems II Project 2
 02-06-23
 ********************************************/

#include <iostream>
#include <fstream>

#include "Token.hpp"
#include "Grammer.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  // check to make sure an input file was specified. 
  if (argc!=2)
    {
      cerr << "USAGE: ./grammerfy <file>" << endl;
      return -1;
    }

  // open input file
  ifstream ifile(argv[1]);

  // if open was not successful, let user know. 
  if (!ifile)
    {
      cerr << "ERROR: Could not open file:\"" << argv[1] << "\"" <<endl;
      return -1;
    }

  //MAIN FILE TO CHECK GRAMMER
  
    string bProgram = program(ifile);
    if(validCode())
    {
      cout << "#################################" << endl;
      cout << "#        VALID PROGRAM          #" << endl;
      cout << "#################################" << endl;
      cout << bProgram << endl << endl;
    }
    else
    {
      cout << "#################################" << endl;
      cout << "#       INVALID PROGRAM         #" << endl;
      cout << "#################################" << endl << endl;
    }

  return 0;
}





