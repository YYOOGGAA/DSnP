/****************************************************************************
  FileName     [ cmdParser.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command parsing member functions for class CmdParser ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "util.h"
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    External funcitons
//----------------------------------------------------------------------
void mybeep();


//----------------------------------------------------------------------
//    Member Function for class cmdParser
//----------------------------------------------------------------------
// return false if file cannot be opened
// Please refer to the comments in "DofileCmd::exec", cmdCommon.cpp
bool
CmdParser::openDofile(const string& dof)
{
   // TODO...
   if (_dofileStack.size() >= 1024) return false;
   if (_dofile != 0) _dofileStack.push(_dofile); 
   _dofile = new ifstream(dof.c_str());
   if (!(*_dofile))
   {
      delete _dofile;
      _dofile = 0;
      if (!_dofileStack.empty())
      {
        _dofile = _dofileStack.top();
        _dofileStack.pop();
      }
      return false;
   } 
   else return true;
}

// Must make sure _dofile != 0
void
CmdParser::closeDofile()
{
   assert(_dofile != 0);
   // TODO...
   delete _dofile;
   _dofile = 0;
   if (!_dofileStack.empty())
   {
       _dofile = _dofileStack.top();
       _dofileStack.pop();
   }
}

// Return false if registration fails
bool
CmdParser::regCmd(const string& cmd, unsigned nCmp, CmdExec* e)
{
   // Make sure cmd hasn't been registered and won't cause ambiguity
   string str = cmd;
   unsigned s = str.size();
   if (s < nCmp) return false;
   while (true) {
      if (getCmd(str)) return false;
      if (s == nCmp) break;
      str.resize(--s);
   }

   // Change the first nCmp characters to upper case to facilitate
   //    case-insensitive comparison later.
   // The strings stored in _cmdMap are all upper case
   //
   assert(str.size() == nCmp);  // str is now mandCmd
   string& mandCmd = str;
   for (unsigned i = 0; i < nCmp; ++i)
      mandCmd[i] = toupper(mandCmd[i]);
   string optCmd = cmd.substr(nCmp);
   assert(e != 0);
   e->setOptCmd(optCmd);

   // insert (mandCmd, e) to _cmdMap; return false if insertion fails.
   return (_cmdMap.insert(CmdRegPair(mandCmd, e))).second;
}

// Return false on "quit" or if excetion happens
CmdExecStatus
CmdParser::execOneCmd()
{
   bool newCmd = false;
   if (_dofile != 0)
      newCmd = readCmd(*_dofile);
   else
      newCmd = readCmd(cin);

   // execute the command
   if (newCmd) {
      string option;
      CmdExec* e = parseCmd(option);
      if (e != 0)
         return e->exec(option);
   }
   return CMD_EXEC_NOP;
}

// For each CmdExec* in _cmdMap, call its "help()" to print out the help msg.
// Print an endl at the end.
void
CmdParser::printHelps() const
{
   // TODO...
   CmdExec* e;
   e = cmdMgr->getCmd("dbap");
   e -> help();
   e = cmdMgr->getCmd("dbav");
   e -> help();
   e = cmdMgr->getCmd("dbc");
   e -> help();
   e = cmdMgr->getCmd("dbma");
   e -> help();
   e = cmdMgr->getCmd("dbmi");
   e -> help();
   e = cmdMgr->getCmd("dbp");
   e -> help();
   e = cmdMgr->getCmd("dbr");
   e -> help();
   e = cmdMgr->getCmd("dbso");
   e -> help();
   e = cmdMgr->getCmd("dbsu");
   e -> help();
   e = cmdMgr->getCmd("do");
   e -> help();
   e = cmdMgr->getCmd("hel");
   e -> help();
   e = cmdMgr->getCmd("his");
   e -> help();
   e = cmdMgr->getCmd("q");
   e -> help();
   cout << endl;
}

void
CmdParser::printHistory(int nPrint) const
{
   assert(_tempCmdStored == false);
   if (_history.empty()) {
      cout << "Empty command history!!" << endl;
      return;
   }
   int s = _history.size();
   if ((nPrint < 0) || (nPrint > s))
      nPrint = s;
   for (int i = s - nPrint; i < s; ++i)
      cout << "   " << i << ": " << _history[i] << endl;
}


//
// Parse the command from _history.back();
// Let string str = _history.back();
//
// 1. Read the command string (may contain multiple words) from the leading
//    part of str (i.e. the first word) and retrive the corresponding
//    CmdExec* from _cmdMap
//    ==> If command not found, print to cerr the following message:
//        Illegal command!! "(string cmdName)"
//    ==> return it at the end.
// 2. Call getCmd(cmd) to retrieve command from _cmdMap.
//    "cmd" is the first word of "str".
// 3. Get the command options from the trailing part of str (i.e. second
//    words and beyond) and store them in "option"
//
CmdExec*
CmdParser::parseCmd(string& option)
{
   assert(_tempCmdStored == false);
   assert(!_history.empty());
   string str = _history.back();

   // TODO...
   assert(str[0] != 0 && str[0] != ' ');
   string cmd;
   myStrGetTok(str, cmd);
   CmdExec* e = getCmd(cmd);
   if (e == 0)
   {
      cerr << "Illegal command!! (" << str << ")" << endl;
      return NULL;
   }
   else 
   {
      option = str.erase(0,cmd.size());
      return e;
   }
}

// Remove this function for TODO...
//
// This function is called by pressing 'Tab'.
// It is to list the partially matched commands.
// "str" is the partial string before current cursor position. It can be 
// a null string, or begin with ' '. The beginning ' ' will be ignored.
//
// Several possibilities after pressing 'Tab'
// (Let $ be the cursor position)
// 1. LIST ALL COMMANDS
//    --- 1.1 ---
//    [Before] Null cmd
//    cmd> $
//    --- 1.2 ---
//    [Before] Cmd with ' ' only
//    cmd>     $
//    [After Tab]
//    ==> List all the commands, each command is printed out by:
//           cout << setw(12) << left << cmd;
//    ==> Print a new line for every 5 commands
//    ==> After printing, re-print the prompt and place the cursor back to
//        original location (including ' ')
//
// 2. LIST ALL PARTIALLY MATCHED COMMANDS
//    --- 2.1 ---
//    [Before] partially matched (multiple matches)
//    cmd> h$                   // partially matched
//    [After Tab]
//    HELp        HIStory       // List all the parially matched commands
//    cmd> h$                   // and then re-print the partial command
//    --- 2.2 ---
//    [Before] partially matched (multiple matches)
//    cmd> h$llo                // partially matched with trailing characters
//    [After Tab]
//    HELp        HIStory       // List all the parially matched commands
//    cmd> h$llo                // and then re-print the partial command
//
// 3. LIST THE SINGLY MATCHED COMMAND
//    ==> In either of the following cases, print out cmd + ' '
//    ==> and reset _tabPressCount to 0
//    --- 3.1 ---
//    [Before] partially matched (single match)
//    cmd> he$
//    [After Tab]
//    cmd> heLp $               // auto completed with a space inserted
//    --- 3.2 ---
//    [Before] partially matched with trailing characters (single match)
//    cmd> he$ahah
//    [After Tab]
//    cmd> heLp $ahaha
//    ==> Automatically complete on the same line
//    ==> The auto-expanded part follow the strings stored in cmd map and
//        cmd->_optCmd. Insert a space after "heLp"
//    --- 3.3 ---
//    [Before] fully matched (cursor right behind cmd)
//    cmd> hElP$sdf
//    [After Tab]
//    cmd> hElP $sdf            // a space character is inserted
//
// 4. NO MATCH IN FITST WORD
//    --- 4.1 ---
//    [Before] No match
//    cmd> hek$
//    [After Tab]
//    ==> Beep and stay in the same location
//
// 5. FIRST WORD ALREADY MATCHED ON FIRST TAB PRESSING
//    --- 5.1 ---
//    [Before] Already matched on first tab pressing
//    cmd> help asd$gh
//    [After] Print out the usage for the already matched command
//    Usage: HELp [(string cmd)]
//    cmd> help asd$gh
//
// 6. FIRST WORD ALREADY MATCHED ON SECOND AND LATER TAB PRESSING
//    ==> Note: command usage has been printed under first tab press
//    ==> Check the word the cursor is at; get the prefix before the cursor
//    ==> So, this is to list the file names under current directory that
//        match the prefix
//    ==> List all the matched file names alphabetically by:
//           cout << setw(16) << left << fileName;
//    ==> Print a new line for every 5 commands
//    ==> After printing, re-print the prompt and place the cursor back to
//        original location
//    Considering the following cases in which prefix is empty:
//    --- 6.1.1 ---
//    [Before] if prefix is empty, and in this directory there are multiple
//             files and they do not have a common prefix,
//    cmd> help $sdfgh
//    [After] print all the file names
//    .               ..              Homework_3.docx Homework_3.pdf  Makefile
//    MustExist.txt   MustRemove.txt  bin             dofiles         include
//    lib             mydb            ref             src             testdb
//    cmd> help $sdfgh
//    --- 6.1.2 ---
//    [Before] if prefix is empty, and in this directory there are multiple
//             files and all of them have a common prefix,
//    cmd> help $orld
//    [After]
//    ==> auto insert the common prefix and make a beep sound
//    // e.g. in hw3/ref
//    cmd> help mydb-$orld
//    ==> DO NOT print the matched files
//    ==> If "tab" is pressed again, see 6.2
//    --- 6.1.3 ---
//    [Before] if prefix is empty, and only one file in the current directory
//    cmd> help $ydb
//    [After] print out the single file name followed by a ' '
//    // e.g. in hw3/bin
//    cmd> help mydb $
//    ==> If "tab" is pressed again, make a beep sound and DO NOT re-print 
//        the singly-matched file
//    --- 6.2 ---
//    [Before] with a prefix and with mutiple matched files
//    cmd> help M$Donald
//    [After]
//    Makefile        MustExist.txt   MustRemove.txt
//    cmd> help M$Donald
//    --- 6.3 ---
//    [Before] with a prefix and with mutiple matched files,
//             and these matched files have a common prefix
//    cmd> help Mu$k
//    [After]
//    ==> auto insert the common prefix and make a beep sound
//    ==> DO NOT print the matched files
//    cmd> help Must$k
//    --- 6.4 ---
//    [Before] with a prefix and with a singly matched file
//    cmd> help MustE$aa
//    [After] insert the remaining of the matched file name followed by a ' '
//    cmd> help MustExist.txt $aa
//    --- 6.5 ---
//    [Before] with a prefix and NO matched file
//    cmd> help Ye$kk
//    [After] beep and stay in the same location
//    cmd> help Ye$kk
//
//    [Note] The counting of tab press is reset after "newline" is entered.
//
// 7. FIRST WORD NO MATCH
//    --- 7.1 ---
//    [Before] Cursor NOT on the first word and NOT matched command
//    cmd> he haha$kk
//    [After Tab]
//    ==> Beep and stay in the same location
void
CmdParser::listCmd(const string& str)
{
   	// TODO...
   	string cmd[13] = {"DBAPpend", "DBAVerage", "DBCount", "DBMAx", "DBMIn",
                      "DBPrint",  "DBRead"   , "DBSOrt",  "DBSUm", "DOfile",
                      "HELp"   ,  "HIStory",   "Quit"};
   	string mandcmd[13] = {"DBAPPEND", "DBAVERAGE", "DBCOUNT", "DBMAX", "DBMIN",
                          "DBPRINT",  "DBREAD"   , "DBSORT",  "DBSUM", "DOFILE",
                          "HELP"   ,  "HISTORY",   "QUIT"};
   	int ptr = _readBufPtr - _readBuf;
   	string nstr;
   	myStrGetTok(str.substr(0, ptr), nstr);
   	size_t found = nstr.find_first_not_of(" ");
   	string mandstr = nstr;
   	for (unsigned j = 0; j < mandstr.size(); ++j) mandstr[j] = toupper(mandstr[j]);

    if (_tabPressCount == 1 && _run == true) 
    {
        cout << endl;
        CmdExec* e;
        e = cmdMgr->getCmd(nstr);
        e -> usage(cout);
        reprintCmd();
        _run1 = false;
    }
    else if (_tabPressCount >= 2 && _run1 == false)
    {
    	vector<string> filename;
    	if (str[ptr-1] == ' ')
    	{
    		listDir(filename, "", ".");
    		if (filename.size() == 1)
    		{
    			for (int i = 0; i < filename[0].size(); i++)
    			{
                	insertChar(filename[0][i]);
    			}
    			insertChar(' ');
    		}
    		else
    		{
    			cout << endl;
    			for (int i = 0; i < filename.size(); i++)
    			{
    				cout << setw(16) << left << filename[i];
    				if ((i+1) % 5 == 0 && i !=0) cout << endl;
    			}
    			reprintCmd();
    		}
    	}
    	else
    	{
    		int j;
    		for (j = 1; j <= ptr; j++)
    		{
    			if (str[ptr-j] == ' ') break;
    		}
    		string prefix = str.substr(ptr-j+1, j-1);
    		listDir(filename, prefix, ".");
    		if (filename.empty()) mybeep();
    		else if (filename.size() == 1)
    		{
    			for (int i = 0; i < prefix.size(); i++) {moveBufPtr(_readBufPtr - 1);deleteChar();}
    			for (int i = 0; i < filename[0].size(); i++)
    			{
                	insertChar(filename[0][i]);
    			}
    			insertChar(' ');
    		}
    		else
    		{
    			cout << endl;
    			for (int i = 0; i < filename.size(); i++)
    			{
    				cout << setw(16) << left << filename[i];
    				if ((i+1) % 4 == 0 && i != 0) cout << endl;
    			}
    			reprintCmd();
    		}
    	}

    }

    else
    {
    	if (!nstr.size() || found == -1)
   		{
      		cout << endl;
      		for (int i = 0; i < 13; i++)
      		{
        		cout << setw(12) << left << cmd[i];
        		if ((i+1)%5 == 0) cout << endl;
      		}
      		cout << endl;
      		resetBufAndPrintPrompt();
   		}

   		else
   		{
      		int k = 0;//用來數幾個符合
      		int pos = 0;
      		for (int i = 0; i < 13; i++)
      		{
         		if (mandstr == mandcmd[i].substr(0,mandstr.size()))
         		{
            		k++;
            		pos = i;
        		}
    		}
      		if (k == 0) {mybeep();}
      		else
      		{
         		if (k == 1) //single
         		{
         			if ((ptr - nstr.size())>0)
         			{
         				cout << endl;
        				CmdExec* e;
        				e = cmdMgr->getCmd(nstr);
        				e -> usage(cout);
        				reprintCmd();
        				_run1 = false;
         			}
         			else
         			{
                    	_tabPressCount = 0;
            			if (mandstr == mandcmd[pos]) // fully match
            			{
               				insertChar(' ');
            			}
            			else
            			{
               				for (int i = ptr; i < cmd[pos].size(); i++)
               				{
                  				insertChar(cmd[pos][i]);
               				}
               				insertChar(' ');
            			}
            			_run = true;
         			}
        		}
         		else
         		{
            		int p = 0;
            		cout << endl;
            		for (int i = 0; i < 13; i++)
            		{
               			if (mandstr == mandcmd[i].substr(0,mandstr.size()))
               			{
                  			cout << setw(12) << left << cmd[i];
                  			p++;
                  		}
                  		if (p > 0 && p%5 == 0) cout << endl;
               		}
                	reprintCmd();
                	_run1 = true;
            	}
        	}
    	}	
    }
}

// cmd is a copy of the original input
//
// return the corresponding CmdExec* if "cmd" matches any command in _cmdMap
// return 0 if not found.
//
// Please note:
// ------------
// 1. The mandatory part of the command string (stored in _cmdMap) must match
// 2. The optional part can be partially omitted.
// 3. All string comparison are "case-insensitive".
//
CmdExec*
CmdParser::getCmd(string cmd)
{
   CmdExec* e = 0;
   // TODO...
   CmdMap::iterator it;
   unsigned i=1;
   for (unsigned j = 0; j < cmd.size(); ++j) cmd[j] = toupper(cmd[j]);
   while (i <= cmd.size())
   {
      it = _cmdMap.find(cmd.substr(0,i));
      if (it != _cmdMap.end()) break;
      else
      {
         if (i == cmd.size()) return 0;
      }
      i++;
   }
   
   e = _cmdMap[cmd.substr(0,i)];
   if (i == cmd.size()) return e; //剩下的是空的

   for (unsigned j = i; j < cmd.size(); ++j) cmd[j] = tolower(cmd[j]); //把剩下的轉成小寫

   unsigned k=i;
   string _opt = _cmdMap[cmd.substr(0,i)] -> getOptCmd();
   if ((cmd.size()-k) > _opt.size()) return 0;
   for (unsigned j = 0; j < _opt.size(); j++)
   {
      if (cmd[k] != _opt[j]) return 0;
      k++;
      if (k == cmd.size()) return e;
   }
}


//----------------------------------------------------------------------
//    Member Function for class CmdExec
//----------------------------------------------------------------------
// return false if option contains an token
bool
CmdExec::lexNoOption(const string& option) const
{
   string err;
   myStrGetTok(option, err);
   if (err.size()) {
      errorOption(CMD_OPT_EXTRA, err);
      return false;
   }
   return true;
}

// Return false if error options found
// "optional" = true if the option is optional XD
// "optional": default = true
//
bool
CmdExec::lexSingleOption
(const string& option, string& token, bool optional) const
{
   size_t n = myStrGetTok(option, token);
   if (!optional) {
      if (token.size() == 0) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
   }
   if (n != string::npos) {
      errorOption(CMD_OPT_EXTRA, option.substr(n));
      return false;
   }
   return true;
}

// if nOpts is specified (!= 0), the number of tokens must be exactly = nOpts
// Otherwise, return false.
//
bool
CmdExec::lexOptions
(const string& option, vector<string>& tokens, size_t nOpts) const
{
   string token;
   size_t n = myStrGetTok(option, token);
   while (token.size()) {
      tokens.push_back(token);
      n = myStrGetTok(option, token, n);
   }
   if (nOpts != 0) {
      if (tokens.size() < nOpts) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
      if (tokens.size() > nOpts) {
         errorOption(CMD_OPT_EXTRA, tokens[nOpts]);
         return false;
      }
   }
   return true;
}

CmdExecStatus
CmdExec::errorOption(CmdOptionError err, const string& opt) const
{
   switch (err) {
      case CMD_OPT_MISSING:
         cerr << "Error: Missing option";
         if (opt.size()) cerr << " after (" << opt << ")";
         cerr << "!!" << endl;
      break;
      case CMD_OPT_EXTRA:
         cerr << "Error: Extra option!! (" << opt << ")" << endl;
      break;
      case CMD_OPT_ILLEGAL:
         cerr << "Error: Illegal option!! (" << opt << ")" << endl;
      break;
      case CMD_OPT_FOPEN_FAIL:
         cerr << "Error: cannot open file \"" << opt << "\"!!" << endl;
      break;
      default:
         cerr << "Error: Unknown option error type!! (" << err << ")" << endl;
      exit(-1);
   }
   return CMD_EXEC_ERROR;
}

