/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {}
   
   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const {
    if(gid > ric.size() || ric[gid] == 0) return 0;
    return ric[gid]; 
  }

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&);

private:
   map<char,int> header;
   map<int,pair<int,int>> _and;
   map<int,int> aigline;
   vector<string> _symbol;
   vector<int> input;
   vector<int> output;

   IdList   notUsedGate;
   IdList   UDId;

   GateList ric;
   GateList _dfsList;

   void readIO(fstream&, int, vector<int>&);
   void readAnd(fstream&, int);
   void readSymbol(fstream&, vector<string>&);
   void createGate();
   void connectGate();
   void gateNotUsed();
   void DFS(CirGate*);
};

#endif // CIR_MGR_H
