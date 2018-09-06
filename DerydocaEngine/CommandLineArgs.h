#pragma once
#include <map>
#include <vector>
#include <string>

using namespace std;

using argumentCollection = map<string, vector<string>>;

class CommandLineArgs
{
public:
	CommandLineArgs(int const& argc, char* argv[]);
	~CommandLineArgs();

	string getExecutablePath() const { return m_executablePath; }
	bool keyExists(string const& key);
	vector<string> getValues(string const& key);
private:
	void processArguments(int const& argc, char* argv[]);

	string m_executablePath;
	argumentCollection m_arguments;
};

