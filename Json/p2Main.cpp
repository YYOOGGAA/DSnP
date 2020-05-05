/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"

using namespace std;

int main()
{
   Json json;

   // Read in the csv(???) file. Do NOT change this part of code.
   string jsonFile;
   cout << "Please enter the file name: ";
   cin >> jsonFile;
   if (json.read(jsonFile))
      cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
   else {
      cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
      exit(-1); // jsonFile does not exist.
   }

   // TODO read and execute commands
   // cout << "Enter command: ";
   while (true) 
   {
      string command;
      int a;
      cout << "Enter command: ";
      cin>>command;
      if(command=="EXIT") break;
      else if(command=="PRINT") a=1;
      else if(command=="SUM") a=2;
      else if(command=="AVE") a=3;
      else if(command=="MAX") a=4;
      else if(command=="MIN") a=5;
      else if(command=="ADD") a=6;
      else a=0;
      switch(a)
      {
         case 0:
            cout<<"Error: unknown command: \""<<command<<"\""<<endl;
            break;
         case 1:
            json.print();
            break;
         case 2:
            json.sum();
            break;
         case 3:
            json.ave();
            break;
         case 4:
            json._max();
            break;
         case 5:
            json._min();
            break;
         case 6:
            json._add();
      }
   }
}
