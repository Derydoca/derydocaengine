@ECHO OFF
SETLOCAL EnableDelayedExpansion

SET exampleProjectDirName=engineResources

:: Get the full path to the example project
SET sourceExampleProjectDir=%~dp0%exampleProjectDirName%

:: List all possible paths that an executable could be found in, in order of priority
SET buildDirectories=%~dp0bin\Debug-windows-x86_64\ %~dp0bin\Release-windows-x86_64\ %~dp0bin\Dist-windows-x86_64\

:: Iterate through all executable paths to find one with a valid exe file
(for %%a in (%buildDirectories%) do (
    SET buildExampleProjectDir=%%a%exampleProjectDirName%

    ECHO "Deleting !buildExampleProjectDir!"
    RD /S /Q !buildExampleProjectDir!

    ECHO "Copying the project folder"
    XCOPY /I /Q %sourceExampleProjectDir% !buildExampleProjectDir! /E
))

ECHO "Done copying the example project to the build directories."
