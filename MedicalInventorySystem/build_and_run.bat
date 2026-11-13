@echo off
title Medical Inventory Management System - DS Lab
color 0b
echo ==================================================================
echo         MEDICAL INVENTORY MANAGEMENT SYSTEM (DS LAB)
echo ==================================================================
echo.
echo Compiling source code (main.cpp + modules/*.cpp)...
g++ -std=c++17 main.cpp modules/*.cpp -o inventory.exe
if %errorlevel% neq 0 (
    color 0c
    echo.
    echo [ERROR] Compilation failed!
    echo Please make sure MinGW (g++) is installed and added to your PATH.
    echo.
    pause
    exit /b %errorlevel%
)

echo.
echo [SUCCESS] Build succeeded: inventory.exe
echo Starting Console Application...
echo.
inventory.exe

echo.
echo Launching Interactive GUI in your default browser...
start gui.html
pause
