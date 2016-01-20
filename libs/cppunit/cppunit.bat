@echo off
set xConfig=%1
set xInputName=%2
set xCurrentDir=%CD%
	
if %xConfig% == nt_vc8d   set xDebugRunner=..\cppunit\lib\nt_vc8\DllPlugInTesterd_dll.exe
if %xConfig% == nt_vc8 set xDebugRunner=..\cppunit\lib\nt_vc8\DllPlugInTester_dll.exe


pushd %~dp0

cd ..\bin

echo %xInputName:~1,-4%xml
%xDebugRunner% -b --xml %xInputName:~1,-4%xml -c %2

popd


