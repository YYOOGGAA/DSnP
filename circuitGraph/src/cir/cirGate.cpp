/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
   int Idl = 1;
   int linel = 1;
   int space;
   int k = gateId;
   while(k/10 != 0) {Idl++; k /= 10;}
   k = getLineNo();
   while(k/10 != 0) {linel++; k /= 10;}

   for(int i = 0; i < 50; i++)
   {
      cout << "=";
   }
   cout << endl;
   cout << "= " << getTypeStr() << "(" << gateId << ")";
   if(symbolExist())
   {
      cout << "\"" << gateSymbol << "\"";
   }
   cout << ", line " << getLineNo();
   space = 39 - typeStr.size() - Idl - linel; 
   if(symbolExist())
   {
      space = space - gateSymbol.size()-2;
   }
   cout << setw(space) << "=";
   cout << endl;
   for(int i = 0; i < 50; i++)
   {
      cout << "=";
   }
   cout << endl;
}

void
CirGate::reportFanin(int level)
{
   assert (level >= 0);
   flevel = level;
   preorderCirWalk_in(fanin,level,gateId,getTypeStr(),false,color);
   colorIn(fanin,level,gateId,getTypeStr(),false,color);
}

void
CirGate::reportFanout(int level)
{
   assert (level >= 0);
   flevel = level;
   preorderCirWalk_out(fanout,level,gateId,getTypeStr(),false,color);
   colorOut(fanout,level,gateId,getTypeStr(),false,color);
}

void
CirGate::preorderCirWalk_in(vector<Edge*> fin, int l, int id, string type, bool inv, string& gatecolor)
{
   CirGate* in;
   if(l < 0) return;

   for(int i = 0; i < flevel-l; i++) cout << "  ";
   if(inv == true) cout << "!";
   cout << type << " " << id;
   if(gatecolor == "gray" && l != 0 && !fin.empty())
   {
      cout << " (*)" << endl;
      return;
   }
   cout << endl;
   if(l != 0) gatecolor = "gray";

   if(fin.empty()) return;
   for(int i = 0; i < fin.size(); i++)
   {
      in = fin[i]->getConnect();
      preorderCirWalk_in(in->fanin, l-1, in->gateId, in->getTypeStr(), 
   	                     fin[i]->getInv(), in->color);
   }
}

void
CirGate::colorIn(vector<Edge*> fin, int l, int id, string type, bool inv, string& gatecolor)
{
   CirGate* in;
   if(l < 0) return;
   if(gatecolor == "white" && l != 0 && !fin.empty()) return;
   if(l != 0) gatecolor = "white";
   if(fin.empty()) return;
   for(int i = 0; i < fin.size(); i++)
   {
      in = fin[i]->getConnect();
      colorIn(in->fanin, l-1, in->gateId, in->getTypeStr(), 
   	          fin[i]->getInv(), in->color);
   }
}

void
CirGate::preorderCirWalk_out(vector<Edge*> fout, int l, int id, string type, bool inv, string& gatecolor)
{
   CirGate* out;
   int k;
   if(l < 0) return;

   for(int i = 0; i < flevel-l; i++) cout << "  ";
   if(inv == true) cout << "!";
   cout << type << " " << id ;
   if(gatecolor == "gray" && l != 0 && !fout.empty())
   {
      cout << " (*)" << endl;
      return;
   }
   cout << endl;
   if(l != 0) gatecolor = "gray";

   if(fout.empty()) return;
   for(int i = 0; i < fout.size(); i++)
   {
   	  out = fout[i]->getConnect();
      preorderCirWalk_out(out->fanout, l-1, out->gateId, out->getTypeStr(), 
   	                      fout[i]->getInv(), out->color);
   }
}

void
CirGate::colorOut(vector<Edge*> fout, int l, int id, string type, bool inv, string& gatecolor)
{
   CirGate* out;
   if(l < 0) return;
   if(gatecolor == "white" && l != 0 && !fout.empty()) return;
   if(l != 0) gatecolor = "white";
   if(fout.empty()) return;
   for(int i = 0; i < fout.size(); i++)
   {
   	  out = fout[i]->getConnect();
      colorOut(out->fanout, l-1, out->gateId, out->getTypeStr(), 
   	           fout[i]->getInv(), out->color);
   }
}