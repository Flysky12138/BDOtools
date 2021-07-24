%~d0
cd "%~dp0"

.\bin\otfccdump.exe --ignore-hints -o base.otd "%~1"
.\bin\pseudo-sc.exe base.otd

@echo off
if %ERRORLEVEL% NEQ 0 ( 
	echo [31mFAILED![0m
	del base.otd
	pause
) ELSE (
	if exist prestringtable\font rmdir /s /q prestringtable\font
	mkdir prestringtable\font	
	.\bin\otfccbuild.exe -q -O3 -o prestringtable\font\pearl.ttf base.otd
	del base.otd
	pause
)
