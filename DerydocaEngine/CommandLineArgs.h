#pragma once
#include <map>
#include <vector>
#include <string>

using namespace std;

using argumentCollection = map<string, vector<string>>;

class CommandLineArgs
{
public:
	CommandLineArgs(int argc, char* argv[]);
	~CommandLineArgs();

	string getExecutablePath() const { return m_executablePath; }
	bool keyExists(string key);
	vector<string> getValues(string key);
private:
	void processArguments(int argc, char* argv[]);

	string m_executablePath;
	argumentCollection m_arguments;
};

