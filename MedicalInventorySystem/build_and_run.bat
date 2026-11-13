@echo off
cd /d "%~dp0"
title Medical Inventory Management System - DS-PBL
color 0b

echo ==================================================================
echo         MEDICAL INVENTORY MANAGEMENT SYSTEM (DS-PBL)
echo ==================================================================
echo.
echo Compiling source code...

g++ -std=c++17 main.cpp modules\*.cpp -o inventory.exe
if errorlevel 1 goto build_failed

echo.
echo [SUCCESS] Build succeeded: inventory.exe
echo Starting Console Application...
echo.

inventory.exe
goto end

:build_failed
color 0c
echo.
echo [ERROR] Compilation failed!
echo Please make sure MinGW compiler is installed and added to PATH.
echo.
pause
exit /b 1

:end
pause
