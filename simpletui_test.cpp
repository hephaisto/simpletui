#include <simpletui.hpp>
#include <exception>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
	try
	{
		Simpletui tui;
		tui.msg("hello world");
		if(tui.yesno("continue?"))
		{
			string input=tui.inputString("please enter a string\nsecond line\nthird line",20);
			tui.msg("you entered "+input);
			string psw=tui.inputString("please enter a password",20,true);
			//tui.inputInt("please enter a number");
			vector<string> choices= {"choice1","choice2"};
			int result=tui.choiceFew("please choose",choices);
			tui.msg("you chose "+choices[result]);
			result=tui.choice("please choose",choices);
			tui.msg("you chose "+choices[result]);
			vector<string> choices2;
			for(size_t i=0;i<100;i++)
				choices2.push_back("something");
			result=tui.choice("please choose",choices2);
		}
	}
	catch(STActionAborted &e)
	{
	}
}
