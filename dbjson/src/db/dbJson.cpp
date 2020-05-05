/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)//read
{
   // TODO: to read in data from Json file and store them in a DB 
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
   assert(j._obj.empty());

   string k;
   char a,b=',';
   int v,i=0;
   is>>k;
   while(b==',')
   {
      is>>k;
        if(k=="}") break;
      k.erase(k.begin());
      k.erase(k.end()-1);
      is>>a>>v>>b;
      DBJsonElem elem(k,v);
      j.add(elem);
   }

   j.readin = true;
   return is;
}

ostream& operator << (ostream& os, const DBJson& j)//print
{
   // TODO
   os<<"{"<<endl;
   int a=j.size();
   for(unsigned i=0;i<a;i++)
   {
      if(i<a-1) os<<"  "<<j._obj[i]<<","<<endl;
      else os<<"  "<<j._obj[i]<<endl;
   }
   os<<"}"<<endl;
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()//丟掉原本的東西
{
   // TODO
   while(!empty()){_obj.pop_back();}
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
   for(int i = 0; i < size(); i++)
   {
      if(_obj[i].key() == elm.key()) return false;
   }
   _obj.push_back(elm);
   return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
  int sum=0;
  for(unsigned i=0;i<size();i++)
  {
      sum+=_obj[i].value();
  }
  float ave=(float)sum/size();
  return ave;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::_max(size_t& idx) const
{
   // TODO
   int maxN = INT_MIN;
   if(size() != 0)
   {
      int k = 0;
      if (size() == 1) k=0;
      else
      {
         maxN = max(_obj[0].value(), _obj[1].value());
         if(_obj[1].value() > _obj[0].value()) k=1;
         for(unsigned i = 2; i < size(); i++)
         {
           if(maxN < _obj[i].value()) k = i;
           maxN = max( maxN , _obj[i].value() );
         }
      }
      idx = k;
   }
   else{ idx = size(); }
   return  maxN;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::_min(size_t& idx) const
{
   // TODO
   int minN = INT_MAX;
   if(size() == 0) idx = size();
   else
   {
      int k = 0;
      if(size() == 1) k=0;
      else
      {
         minN = min(_obj[0].value(),_obj[1].value());
         if(_obj[1].value() < _obj[0].value()) k=1;
         for(unsigned i=2; i < size(); i++)
         {
          if(minN > _obj[i].value()) k=i;
          minN = min(minN, _obj[i].value());
         }
      }
      idx = k;
   }
   return  minN;
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   int s = 0;
   for(unsigned i = 0; i < size(); i++)
   {
         s += _obj[i].value();
   }
   return s;
}
