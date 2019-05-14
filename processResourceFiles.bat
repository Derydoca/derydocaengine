@ECHO OFF
SETLOCAL EnableDelayedExpansion

:: List all possible paths that an executable could be found in, in order of priority
SET buildDirectories=%~dp0bin\Debug-windows-x86_64\ %~dp0bin\Release-windows-x86_64\ %~dp0bin\Dist-windows-x86_64\

:: Name of the executable to run
SET executableName=DerydocaEngine.Editor.UI.exe

:: Arguments to pass to the executable
SET args=-processDirectory %~dp0engineResources %~dp0exampleProject

:: Iterate through all executable paths to find one with a valid exe file
(for %%a in (%buildDirectories%) do (
    SET fullExecutablePath=%%aDerydocaEngine.Editor.UI/%executableName%
    if exist !fullExecutablePath! (
        ECHO "Found the exe at !fullExecutablePath!."
        ECHO "Processing files..."

        :: Run the command
        !fullExecutablePath! %args%

        :: And leave
        ECHO "Done!"
        goto :eof
    ) else (
        ECHO "Unable to find executable at path !fullExecutablePath!.";
    )
))

ECHO "Unable to find a suitable version of the application in the build directories. Please compile at least one version of this application first.\n"
