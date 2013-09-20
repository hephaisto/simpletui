#include <cdk/cdk.h>
#include "simpletui.hpp"

const char* STActionAborted::what()
{
	return "action aborted by user";
}


string inputStringInternal(string text,int maxlen, EDisplayType type);

static CDKSCREEN *cdk;
static WINDOW *cw;
Simpletui::Simpletui()
{
	if(instances.empty())
		staticInit();
	instances.insert(this);
}
Simpletui::~Simpletui()
{
	set<Simpletui*>::iterator it=instances.find(this);
	if(it!=instances.end())
		instances.erase(it);
	if(instances.empty())
		staticCleanup();
}

void Simpletui::staticInit()
{
	cw=initscr();
	cdk=initCDKScreen(cw);
}
void Simpletui::staticCleanup()
{
	endCDK();
}

void Simpletui::msg(string text)
{
	choiceFew(text, {"OK"});
	//CDKLABEL *label=newCDKLabel(cdk,CENTER,CENTER,text.c_str(),1,TRUE,FALSE);
}
string Simpletui::inputString(string text,int maxlen)
{
	return inputStringInternal(text,maxlen,vMIXED);
}
string Simpletui::inputPassword(string text,int maxlen)
{
	return inputStringInternal(text,maxlen,vHMIXED);
}
string inputStringInternal(string text,int maxlen, EDisplayType type)
{
	CDKENTRY *entry=newCDKEntry(cdk,CENTER,CENTER,const_cast<char*>(text.c_str()),const_cast<char*>(""),A_NORMAL,'_',type,maxlen,0 , maxlen,TRUE,FALSE);
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
	CDKDIALOG *dia=newCDKDialog(cdk,CENTER,CENTER,const_cast<char**>(&textChar),1,const_cast<char**>(choiceChars),choices.size(), A_STANDOUT,TRUE,TRUE,FALSE);
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
	CDKSCROLL *scroll=newCDKScroll(cdk,CENTER,CENTER,RIGHT,choices.size()+3,width,const_cast<char*>(text.c_str()),const_cast<char**>(choiceChars),choices.size(),FALSE,A_STANDOUT,TRUE,FALSE);
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

set<Simpletui*> Simpletui::instances;
