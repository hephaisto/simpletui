#include <string>
using std::string;
#include <vector>
using std::vector;
#include <set>
using std::set;

class STActionAborted
{
};

class Simpletui
{
private:
	static set<Simpletui*> instances;
	static void staticCleanup();
	static void staticInit();
public:
	Simpletui();
	~Simpletui();
	void msg(string text);
	string inputString(string text);
	int inputInt(string text);
	bool yesno(string text);
	int choice(string text, vector<string> choices);
};
