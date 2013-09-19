#include "simpletui.hpp"

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
}
void Simpletui::staticCleanup()
{
}

void Simpletui::msg(string text)
{
}
string Simpletui::inputString(string text)
{
}
int Simpletui::inputInt(string text)
{
}
bool Simpletui::yesno(string text)
{
}
int Simpletui::choice(string text, vector<string> choices)
{
}

set<Simpletui*> Simpletui::instances;
