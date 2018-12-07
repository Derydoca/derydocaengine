@ECHO OFF
SETLOCAL EnableDelayedExpansion

SET exampleProjectDirName=engineResources

:: Get the full path to the example project
SET sourceExampleProjectDir=%~dp0%exampleProjectDirName%

:: List all possible paths that an executable could be found in, in order of priority
SET buildDirectories=%~dp0bin\x64-Debug\ %~dp0bin\x64-Release\ %~dp0bin\Win32-Debug\ %~dp0bin\Win32-Release\

:: Iterate through all executable paths to find one with a valid exe file
(for %%a in (%buildDirectories%) do (
    SET buildExampleProjectDir=%%a%exampleProjectDirName%

    ECHO "Deleting !buildExampleProjectDir!"
    RD /S /Q !buildExampleProjectDir!

    ECHO "Copying the project folder"
    XCOPY /I /Q %sourceExampleProjectDir% !buildExampleProjectDir! /E
))

ECHO "Done copying the example project to the build directories."
