@echo off
set RootPath=%CD%
set SolutionFile=%RootPath%\projects\riftwarrior\proj.wp8-xaml\riftwarrior.sln
REM TODO: Might use the binary path and object path in future.
REM set BinPath=%CD%.bin
REM set ObjPath=%CD%.obj

doskey ..=pushd ..
doskey ...=pushd ..\..
doskey binary=pushd %BinPath%
doskey root=pushd %RootPath%
doskey ide=start vswinexpress.exe %SolutionFile%
