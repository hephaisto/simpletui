#include "simpletui.hpp"

int main(int argc, char** argv)
{
	Simpletui tui;
	tui.msg("hello world");
	if(tui.yesno("continue?"))
	{
		tui.inputString("please enter a string");
		tui.inputInt("please enter a number");
		tui.choice("please choose", {"choice1","choice2"});
	}
}
