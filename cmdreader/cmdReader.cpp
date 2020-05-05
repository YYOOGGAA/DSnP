/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      ParseChar pch = getChar(istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : moveBufPtr(_readBufPtr-1);deleteChar();
                               break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt();
                               //cout<<endl;cout<<"history="<<_history[0]<<endl;
                                break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1);/*cout<<endl;cout<<"buf=" <<_readBuf<<endl;*/ break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1);/*cout<<endl;cout<<"buf=" <<_readBuf<<endl;*/ break;
         case ARROW_RIGHT_KEY: moveBufPtr(_readBufPtr+1);break;//cursor的位置往右
         case ARROW_LEFT_KEY : moveBufPtr(_readBufPtr-1); break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : insertChar(' ');
                               while((_readBufPtr-_readBuf)%8 !=0) insertChar(' ');
                               break;
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch));/*cout<<endl;cout<<"buf=" <<_readBuf<<endl;*/break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}
          

// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO...
  if(ptr<_readBuf||ptr>_readBufEnd){mybeep();return false;}
  else
  {
    if(ptr==_readBuf)
    { 
      int k=0;
      while(k!=_readBufPtr - _readBuf){cout<<'\b';k++;}
      cout.flush();}
    else if(ptr==_readBufEnd)
    {
      for(int i=_readBufPtr - _readBuf;i<_readBufEnd-_readBuf;i++){cout<<_readBuf[i];}
      cout.flush();
    }
    else if(ptr==_readBufPtr+1)
    {
      cout<<_readBuf[_readBufPtr - _readBuf];
      cout.flush();
    }
    else if(ptr==_readBufPtr-1){cout<<'\b';cout.flush();}
    _readBufPtr=ptr;
    return true;
  }
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // TODO...
   int k=0;
   if(_readBufPtr == _readBufEnd) {mybeep();return false;}
   for(int i=_readBufPtr - _readBuf+1;i<_readBufEnd - _readBuf;i++){cout<<_readBuf[i];}
   cout<<" ";
   while(k< _readBufEnd- _readBufPtr) {cout<<'\b';k++;}
   cout.flush();
   for(int i=_readBufPtr- _readBuf;i<_readBufEnd-_readBuf-1;i++){_readBuf[i]=_readBuf[i+1];}
    *(_readBufEnd - 1) = '\0';
   // _readBuf[_readBufEnd - _readBuf+1]=' ';
   _readBufEnd--;
   return true;
}

// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
   // TODO...
   cout<<ch;
   if(_readBufPtr!=_readBufEnd)
   {
     for(int i=_readBufPtr - _readBuf;i<_readBufEnd-_readBuf;i++){cout<<_readBuf[i];}
    char buf[_readBufEnd- _readBufPtr+1] = {0};
     int k=0,j=0;
     for(int i=_readBufPtr - _readBuf;i<_readBufEnd-_readBuf+1;i++)
     {
       buf[k]=_readBuf[i];
       if(i==_readBufPtr - _readBuf) _readBuf[i]=ch;
       else _readBuf[i]=buf[k-1];
       k++;
     }
     while(j<_readBufEnd- _readBufPtr){cout<<'\b';j++;}
   }
   else {_readBuf[_readBufPtr- _readBuf]=ch;}

   _readBufPtr++;
   _readBufEnd++;
   *_readBufEnd=0;
   cout.flush();

   
   
   assert(repeat >= 1);
}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
   // TODO...
  memset(_readBuf,0,sizeof(_readBuf));
  if(_readBufPtr!=_readBuf) moveBufPtr(_readBuf);
  while(_readBufEnd!=_readBuf){deleteChar();}
  _readBufEnd=_readBufPtr=_readBuf;
  *_readBufEnd = 0;
  *_readBufPtr=0;
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
   // TODO...
   if(index< _historyIdx)
   {
      if(_historyIdx==0) mybeep();
      else 
      {
        if(_tempCmdStored==false && _historyIdx==_history.size()) {_history.push_back(_readBuf);_tempCmdStored=true;}
        if(_tempCmdStored==true && _historyIdx==_history.size()-1) {_history.pop_back();_history.push_back(_readBuf);}
        if(index<0) index=0;
        _historyIdx=index;
        retrieveHistory();
      }
   }
   else if(index>_historyIdx)
   {
      if(_historyIdx==_history.size()) mybeep();
      else if(_tempCmdStored==true && _historyIdx==_history.size()-1) mybeep();
      else
      {
        if(index>=_history.size()) index=_history.size()-1;
        _historyIdx=index;
        retrieveHistory();
      }
   }
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf V
// 2. If not a null string, add string to _history. V
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//
void
CmdParser::addHistory()
{
   // TODO...
    if(_tempCmdStored==true) {_history.pop_back();_tempCmdStored=false;}
    int k=0,n=0;
    while(_readBuf[_readBufEnd-_readBuf-1]==' ') _readBufEnd--;
    if(_readBufEnd!=_readBuf)
    {
      while(_readBuf[k]==' ') k++;
      *_readBufEnd=0;

      int _size=_readBufEnd-_readBuf-k+1;
      char _buf[_size]={0};
      for(int i=k;i<_readBufEnd-_readBuf;i++) {_buf[n]=_readBuf[i];n++;}
      _buf[sizeof(_buf)-1]=0; 
      _history.push_back(_buf);
    }
      _historyIdx=_history.size();
    
    //cout<<"_readBufEnd="<<*(_readBufEnd-1)<<endl;
    //cout<<"_readBufPtr="<<*(_readBufPtr)<<endl;
    //cout<<"_buf="<<_buf<<endl;
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
