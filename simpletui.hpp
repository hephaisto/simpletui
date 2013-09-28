#include <string>
using std::string;
#include <vector>
using std::vector;
#include <exception>

class STActionAborted : public std::exception
{
public:
	virtual const char* what();
};

class Simpletui
{
private:
	Simpletui(const Simpletui &other);
	Simpletui& operator=(const Simpletui &other);

	static size_t instance_count;
	static void *cdk;
	static void *cw;

	static void staticCleanup();
	static void staticInit();
public:
	Simpletui();
	virtual ~Simpletui();

	// tui functions
	void msg(string text);
	string inputString(string text, int maxlen=20, bool password=false);
	bool yesno(string text);
	int choiceFew(string text, vector<string> choices);
	int choice(string text, vector<string> choices);
	friend class SimpletuiTempMessage;
};

class SimpletuiTempMessage
{
public:
	SimpletuiTempMessage(Simpletui &tui, string text);
	virtual ~SimpletuiTempMessage();
private:
	SimpletuiTempMessage();
	SimpletuiTempMessage(SimpletuiTempMessage &other);
	SimpletuiTempMessage& operator=(const SimpletuiTempMessage &other);
	void *label;
};
