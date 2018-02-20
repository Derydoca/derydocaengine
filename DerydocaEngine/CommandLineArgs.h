#pragma once
#include <map>
#include <list>
#include <string>

using namespace std;

using argumentCollection = map<string, list<string>>;

class CommandLineArgs
{
public:
	CommandLineArgs(int argc, char* argv[]);
	~CommandLineArgs();

	string getExecutablePath() const { return m_executablePath; }
	bool keyExists(string key);
	list<string> getValues(string key);
private:
	void processArguments(int argc, char* argv[]);

	string m_executablePath;
	argumentCollection m_arguments;
};

