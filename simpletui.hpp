#include <string>
using std::string;
#include <vector>
using std::vector;
#include <set>
using std::set;
#include <exception>

class STActionAborted : public std::exception
{
public:
	virtual const char* what();
};

class Simpletui
{
private:
	static set<Simpletui*> instances;
	static void staticCleanup();
	static void staticInit();
	static char **splitString(string str);
public:
	Simpletui();
	~Simpletui();
	void msg(string text);
	string inputString(string text, int maxlen=20);
	string inputPassword(string text, int maxlen=20);
	bool yesno(string text);
	int choiceFew(string text, vector<string> choices);
	int choice(string text, vector<string> choices);
};
