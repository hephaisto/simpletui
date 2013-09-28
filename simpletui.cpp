#include <cdk/cdk.h>
#include "simpletui.hpp"

const char* STActionAborted::what()
{
	return "action aborted by user";
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
	const char *textChar=text.c_str();
	CDKDIALOG *dia=newCDKDialog(reinterpret_cast<CDKSCREEN*>(cdk),CENTER,CENTER,const_cast<char**>(&textChar),1,const_cast<char**>(choiceChars),choices.size(), A_STANDOUT,TRUE,TRUE,FALSE);
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
	CDKSCROLL *scroll=newCDKScroll(reinterpret_cast<CDKSCREEN*>(cdk),CENTER,CENTER,RIGHT,choices.size()+3,width,const_cast<char*>(text.c_str()),const_cast<char**>(choiceChars),choices.size(),FALSE,A_STANDOUT,TRUE,FALSE);
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
	char *texts[]={const_cast<char*>(text.c_str())};
	label=newCDKLabel(reinterpret_cast<CDKSCREEN*>(tui.cdk), CENTER, CENTER, texts, 1, TRUE, FALSE);
	refreshCDKScreen(reinterpret_cast<CDKSCREEN*>(tui.cdk));
}
SimpletuiTempMessage::~SimpletuiTempMessage()
{
	destroyCDKLabel(reinterpret_cast<CDKLABEL*>(label));
}
