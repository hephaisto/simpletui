#include <cdk/cdk.h>
#include "simpletui.hpp"

const char* STActionAborted::what()
{
	return "action aborted by user";
}

class SplitString
{
	vector<string> lines;
	char **buf;
public:
	SplitString(string s);
	~SplitString();
	char** getPtr();
	size_t getLength();
};

SplitString::SplitString(string s)
{
	size_t endpos=0;
	size_t startpos=0;
	do
	{
		endpos=s.find("\n",startpos);
		string substr=s.substr(startpos,endpos-startpos);
		lines.push_back(substr);
		startpos=endpos+1;
		//cerr<<substr<<"\n";
	}
	while(endpos!=string::npos);
	buf=new char*[lines.size()];
	for(size_t i=0;i<lines.size();i++)
		buf[i]=const_cast<char*>(lines[i].c_str());
}
SplitString::~SplitString()
{
	delete[] buf;
}
char** SplitString::getPtr()
{
	return buf;
}
size_t SplitString::getLength()
{
	return lines.size();
}


string inputStringInternal(string text,int maxlen, EDisplayType type);

Simpletui::Simpletui()
{
	if(!(instance_count++))
		staticInit();
}
Simpletui::~Simpletui()
{
	if(!(--instance_count))
		staticCleanup();
}

void Simpletui::staticInit()
{
	cw=initscr();
	cdk=initCDKScreen(reinterpret_cast<WINDOW*>(cw));
}
void Simpletui::staticCleanup()
{
	destroyCDKScreen(reinterpret_cast<CDKSCREEN*>(cdk));
	endCDK();
}

void Simpletui::msg(string text)
{
	choiceFew(text, {"OK"});
}
string Simpletui::inputString(string text,int maxlen, bool password)
{
	CDKENTRY *entry=newCDKEntry(reinterpret_cast<CDKSCREEN*>(cdk),CENTER,CENTER,const_cast<char*>(text.c_str()),const_cast<char*>(""),A_NORMAL,'_',password?vHMIXED:vMIXED,maxlen,0 , maxlen,TRUE,FALSE);
	char* resulttext=activateCDKEntry(entry,NULL);
	if(resulttext==NULL)
	{
		destroyCDKEntry(entry);
		throw STActionAborted();
	}
	string result(resulttext);
	destroyCDKEntry(entry);
	return result;
}
bool Simpletui::yesno(string text)
{
	return (choiceFew(text, {"yes","no"})==0);
}
int Simpletui::choiceFew(string text, vector<string> choices)
{
	const char **choiceChars=new const char*[choices.size()];
	for(size_t i=0;i<choices.size();i++)
		choiceChars[i]=choices[i].c_str();

	SplitString sText(text);

	CDKDIALOG *dia=newCDKDialog(reinterpret_cast<CDKSCREEN*>(cdk),CENTER,CENTER,sText.getPtr(),sText.getLength(),const_cast<char**>(choiceChars),choices.size(), A_STANDOUT,TRUE,TRUE,FALSE);
	int result=activateCDKDialog(dia,NULL);
	if(dia->exitType!=vNORMAL)
	{
		destroyCDKDialog(dia);
		throw STActionAborted();
	}
	destroyCDKDialog(dia);
	delete[] choiceChars;	// deep-delete unnecessary, references will be destroyed when strings are
	return result;
}
int Simpletui::choice(string text, vector<string> choices)
{
	const char **choiceChars=new const char*[choices.size()];
	unsigned int width=0;
	for(size_t i=0;i<choices.size();i++)
	{
		choiceChars[i]=choices[i].c_str();
		if(choices[i].length()>width)
			width=choices[i].length();
	}
	SplitString sText(text);
	CDKSCROLL *scroll=newCDKScroll(reinterpret_cast<CDKSCREEN*>(cdk),CENTER,CENTER,RIGHT,choices.size()+sText.getLength()+2,width,const_cast<char*>(text.c_str()),const_cast<char**>(choiceChars),choices.size(),FALSE,A_STANDOUT,TRUE,FALSE);
	int result=activateCDKScroll(scroll,NULL);
	if(scroll->exitType!=vNORMAL)
	{
		destroyCDKScroll(scroll);
		throw STActionAborted();
	}
	destroyCDKScroll(scroll);
	delete[] choiceChars;	// deep-delete unnecessary, references will be destroyed when strings are
	return result;
}

size_t Simpletui::instance_count;
void *Simpletui::cdk;
void *Simpletui::cw;

SimpletuiTempMessage::SimpletuiTempMessage(Simpletui &tui, string text)
{
	SplitString sText(text);
	label=newCDKLabel(reinterpret_cast<CDKSCREEN*>(tui.cdk), CENTER, CENTER, sText.getPtr(), sText.getLength(), TRUE, FALSE);
	refreshCDKScreen(reinterpret_cast<CDKSCREEN*>(tui.cdk));
}
SimpletuiTempMessage::~SimpletuiTempMessage()
{
	destroyCDKLabel(reinterpret_cast<CDKLABEL*>(label));
}
