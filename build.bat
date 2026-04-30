@echo off
cd /d "%~dp0"

echo =======================================
echo   Defense Calculator - Clean Build
echo =======================================
echo.

set "PATH=F:\Qt\Tools\mingw1310_64\bin;F:\Qt\6.11.0\mingw_64\bin;%PATH%"

echo [0/5] Cleaning old build files...
del /Q build\*.o 2>nul
del /Q build\*.exe 2>nul
del /Q build\moc_*.cpp 2>nul
del /Q build\qrc_*.cpp 2>nul
echo        Clean done.

echo [1/5] Running MOC...
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\MainWindow.h -o build\moc_MainWindow.cpp
if errorlevel 1 goto :moc_fail
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\TypeButton.h -o build\moc_TypeButton.cpp
if errorlevel 1 goto :moc_fail
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\TypeSelector.h -o build\moc_TypeSelector.cpp
if errorlevel 1 goto :moc_fail
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\SpiritRow.h -o build\moc_SpiritRow.cpp
if errorlevel 1 goto :moc_fail
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\ResultPanel.h -o build\moc_ResultPanel.cpp
if errorlevel 1 goto :moc_fail
echo        MOC done.

echo [2/5] Running RCC...
F:\Qt\6.11.0\mingw_64\bin\rcc.exe resources\icons.qrc -o build\qrc_icons.cpp
if errorlevel 1 goto :rcc_fail
echo        RCC done.

echo [3/5] Compiling C++ sources...
set "CXXFLAGS=-std=c++17 -O2 -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_CORE_LIB -DQT_GUI_LIB"
set "INCS=-IF:/Qt/6.11.0/mingw_64/include -IF:/Qt/6.11.0/mingw_64/include/QtCore -IF:/Qt/6.11.0/mingw_64/include/QtWidgets -IF:/Qt/6.11.0/mingw_64/include/QtGui -Isrc"

g++.exe %CXXFLAGS% %INCS% -c src\main.cpp -o build\main.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\TypeChart.cpp -o build\TypeChart.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\TypeButton.cpp -o build\TypeButton.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\TypeSelector.cpp -o build\TypeSelector.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\SpiritRow.cpp -o build\SpiritRow.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\ResultPanel.cpp -o build\ResultPanel.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\MainWindow.cpp -o build\MainWindow.o
if errorlevel 1 goto :fail
echo        Source compilation done.

echo        Compiling MOC outputs...
g++.exe %CXXFLAGS% %INCS% -c build\moc_MainWindow.cpp -o build\moc_MainWindow.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c build\moc_TypeButton.cpp -o build\moc_TypeButton.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c build\moc_TypeSelector.cpp -o build\moc_TypeSelector.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c build\moc_SpiritRow.cpp -o build\moc_SpiritRow.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c build\moc_ResultPanel.cpp -o build\moc_ResultPanel.o
if errorlevel 1 goto :fail
echo        MOC compilation done.

echo        Compiling resource file...
g++.exe %CXXFLAGS% %INCS% -c build\qrc_icons.cpp -o build\qrc_icons.o
if errorlevel 1 goto :fail
echo        All compilation done.

echo [4/5] Linking...
g++.exe build\main.o build\TypeChart.o build\TypeButton.o build\TypeSelector.o build\SpiritRow.o build\ResultPanel.o build\MainWindow.o build\moc_MainWindow.o build\moc_TypeButton.o build\moc_TypeSelector.o build\moc_SpiritRow.o build\moc_ResultPanel.o build\qrc_icons.o -LF:/Qt/6.11.0/mingw_64/lib -lQt6Widgets -lQt6Gui -lQt6Core -o build\team-defense-calc.exe
if errorlevel 1 goto :link_fail
echo        Linking done.

echo [5/5] Copying data files...
copy /Y TypeChart.json build\ >nul
echo        TypeChart.json copied.

echo.
echo =======================================
echo   Build SUCCESS!
echo   Output: build\team-defense-calc.exe
echo =======================================
exit /b 0

:moc_fail
echo [ERROR] MOC failed
pause
exit /b 1

:rcc_fail
echo [ERROR] RCC failed
pause
exit /b 1

:fail
echo [ERROR] Compilation failed! Check error above.
pause
exit /b 1

:link_fail
echo [ERROR] Linking failed! Check error above.
pause
exit /b 1
