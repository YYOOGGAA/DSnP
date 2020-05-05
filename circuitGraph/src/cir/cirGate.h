/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;
class Edge;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
public:
   CirGate(int id, unsigned l, string type) : 
      gateId(id), lineNo(l), typeStr(type), color("white"), pre(-1) {}
   virtual ~CirGate() {}
   
   friend class CirMgr;
   // Basic access methods
   string getTypeStr() const { return typeStr; }
   unsigned getLineNo() const { return lineNo; }
   bool symbolExist() const{if(gateSymbol != "\0") return true; return false;}

   // Printing functions
   virtual void printGate() const = 0;
   void reportGate() const;
   void reportFanin(int level);
   void reportFanout(int level);

private:
   string typeStr;
   int gateId;
   int flevel;
   string gateSymbol;
   unsigned lineNo;

   vector<Edge*> fanin;
   vector<Edge*> fanout;

   //DFS
   string color;
   int pre;
   void preorderCirWalk_in(vector<Edge*>, int, int, string, bool, string&);
   void preorderCirWalk_out(vector<Edge*>, int, int, string,bool, string&);
   void colorIn(vector<Edge*>, int, int, string type, bool, string&);
   void colorOut(vector<Edge*>, int, int, string, bool, string&);

protected:
};

class CirPI : public CirGate
{
public:
   CirPI(int id, unsigned l): CirGate(id, l, "PI") {}
   ~CirPI() {}

   void printGate() const {}
};

class CirPO : public CirGate
{
public:
   CirPO(int id, unsigned l): CirGate(id, l, "PO") {}
   ~CirPO() {}

   void printGate() const {}

};

class CirAIG : public CirGate
{
public:
   CirAIG(int id, unsigned l): CirGate(id, l, "AIG") {}
   ~CirAIG() {}

   void printGate() const {}

};

class CirConst : public CirGate
{
public:
   CirConst(): CirGate(0, 0, "CONST") {}
   ~CirConst() {}

   void printGate() const {}

};

class CirUnDef : public CirGate
{
public:
   CirUnDef(int id): CirGate(id, 0, "UNDEF") {}
   ~CirUnDef() {}

   void printGate() const {}

};

class Edge
{
public:
   Edge(CirGate* c, bool inv): 
      connect(c), invert(inv)  {}
   ~Edge() {}
   
   CirGate* getConnect() {return connect;}
   bool getInv() {return invert;}
   
private:
   CirGate* connect;
   bool invert;
};

#endif // CIR_GATE_H
