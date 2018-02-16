#include "Editor.h"
#include "EngineSettings.h"
#include "ObjectLibrary.h"

int main(int argc, char* argv[])
{
	if (argc > 2 && strcmp(argv[1], "/processDirectory") == 0)
	{
		// If the command to process a directory was passed in, process it and then immediately exit
		ObjectLibrary::getInstance().initializeDirectory(argv[2]);

		return 0;
	}
	else
	{
		// All other commands create the editor window
		Editor* editor = new Editor();
		return editor->Run();
	}
}