#include "simpletui.hpp"
#include <exception>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv)
{
	try
	{
		Simpletui tui;
		tui.msg("hello world\n\nthis line is separated by an additional space");
		{
			SimpletuiTempMessage msg(tui, "this line will be shown for 5 seconds\n(until some other command has finished)...");
			system("sleep 5");
		}
		if(tui.yesno("continue?"))
		{
			string input=tui.inputString("please enter a string\nsecond line\nthird line",20);
			tui.msg("you entered "+input);
			string psw=tui.inputString("please enter a password\n2nd line",20,true);
			//tui.inputInt("please enter a number");
			vector<string> choices= {"choice1","choice2"};
			int result=tui.choiceFew("please choose\n2nd line",choices);
			tui.msg("you chose "+choices[result]);
			result=tui.choice("please choose\n2nd line",choices);
			tui.msg("you chose "+choices[result]);
			vector<string> choices2={"first"};
			for(size_t i=0;i<100;i++)
				choices2.push_back("something");
			result=tui.choice("please choose\2nd line",choices2);
		}
	}
	catch(STActionAborted &e)
	{
	}
}
