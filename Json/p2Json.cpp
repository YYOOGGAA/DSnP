/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "p2Json.h"

using namespace std;

// Implement member functions of class Row and Table(???) here
bool
Json::read(const string& jsonFile)
{
   fstream file;
   file.open(jsonFile);
   if(!file) return false;
   string k;
   char a,b=',';
   int v,i=0;
   file>>k;
   while(b==',')
   {
   	  file>>k;
        if(k=="}") break;
   	  k.erase(k.begin());
   	  k.erase(k.end()-1);
   	  file>>a>>v>>b;
      JsonElem elem(k,v);
      _obj.push_back(elem);
   }

   return true; // TODO
}

void
Json::print()
{
   cout<<"{"<<endl;
   int a=_obj.size();
   for(unsigned i=0;i<a;i++)
   {
   	  if(i<a-1) cout<<"  "<<_obj[i]<<","<<endl;
   	  else cout<<"  "<<_obj[i]<<endl;
   }
   cout<<"}"<<endl;
   
}

void
Json::sum()
{
   int sum=0;
   if(_obj.size()==0) cout<<"Error: No element found!"<<endl;
   else
   {
      for(unsigned i=0;i<_obj.size();i++)
      {
         sum+=_obj[i].value();
      }
      cout<<"The summation of the values is: "<<sum<<"."<<endl;
   }
}

void
Json::ave()
{
   if(_obj.size()==0) cout<<"Error: No element found!"<<endl;
   else
   {  
   	  int sum=0;
   	  for(unsigned i=0;i<_obj.size();i++)
      {
         sum+=_obj[i].value();
      }
   	  double ave=(double)sum/_obj.size();
      cout<<"The average of the values is: "<<fixed<<setprecision(1)<<ave<<"."<<endl;
   }
}

void
Json::_max()
{
   //int max=*max_element(_v.begin(),_v.end());
   if(_obj.size()==0) cout<<"Error: No element found!"<<endl;
   else
   {
      int a=0,k=0;
      if(_obj.size()==1) k=0;
      else
      {
         a=max(_obj[0].value(),_obj[1].value());
         if(_obj[1].value()>_obj[0].value()) k=1;
         for(unsigned i=2;i<_obj.size();i++)
         {
   	      if(a<_obj[i].value()) k=i;
   	      a=max(a,_obj[i].value());
         }
      }
      cout<<"The maximum element is: { "<<_obj[k]<<" }."<<endl;
   }
}

void
Json::_min()
{
   //int min=*min_element(_v.begin(),_v.end());
   if(_obj.size()==0) cout<<"Error: No element found!"<<endl;
   else
   {
      int a=0,k=0;
      if(_obj.size()==1) k=0;
      else
      {
         a=min(_obj[0].value(),_obj[1].value());
         if(_obj[1].value()<_obj[0].value()) k=1;
         for(unsigned i=2;i<_obj.size();i++)
         {
   	      if(a>_obj[i].value()) k=i;
   	      a=min(a,_obj[i].value());
         }
      }
      cout<<"The minimum element is: { "<<_obj[k]<<" }."<<endl;
   }
}

void
Json::_add()
{
   int b;
   string k;
   cin>>k>>b;
   JsonElem newelem(k,b);
   _obj.push_back(newelem);
}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

