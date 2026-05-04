@echo off
cd /d "%~dp0"

echo ======================================
echo   Defense Calculator v2.0 - Clean Build
echo ======================================
echo.

set "PATH=F:\Qt\Tools\mingw1310_64\bin;F:\Qt\6.11.0\mingw_64\bin;%PATH%"

echo [0/6] Cleaning old build files...
del /Q build\*.o 2>nul
del /Q build\*.exe 2>nul
del /Q build\moc_*.cpp 2>nul
del /Q build\qrc_*.cpp 2>nul
echo        Clean done.

echo [1/6] Running MOC...
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\MainWindow.h -o build\moc_MainWindow.cpp
if errorlevel 1 goto :moc_fail
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\TypeButton.h -o build\moc_TypeButton.cpp
if errorlevel 1 goto :moc_fail
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\TypeSelector.h -o build\moc_TypeSelector.cpp
if errorlevel 1 goto :moc_fail
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\InputRow.h -o build\moc_InputRow.cpp
if errorlevel 1 goto :moc_fail
F:\Qt\6.11.0\mingw_64\bin\moc.exe src\ResultPanel.h -o build\moc_ResultPanel.cpp
if errorlevel 1 goto :moc_fail
echo        MOC done.

echo [2/6] Running RCC...
F:\Qt\6.11.0\mingw_64\bin\rcc.exe resources\icons.qrc -o build\qrc_icons.cpp
if errorlevel 1 goto :rcc_fail
echo        RCC done.

echo [3/6] Compiling C++ sources...
set "CXXFLAGS=-std=c++17 -O2 -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_CORE_LIB -DQT_GUI_LIB -finput-charset=UTF-8 -fexec-charset=UTF-8"
set "INCS=-IF:/Qt/6.11.0/mingw_64/include -IF:/Qt/6.11.0/mingw_64/include/QtCore -IF:/Qt/6.11.0/mingw_64/include/QtWidgets -IF:/Qt/6.11.0/mingw_64/include/QtGui -Isrc"

g++.exe %CXXFLAGS% %INCS% -c src\main.cpp -o build\main.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\TypeChart.cpp -o build\TypeChart.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\TypeButton.cpp -o build\TypeButton.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\TypeSelector.cpp -o build\TypeSelector.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\InputRow.cpp -o build\InputRow.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\OffenseCalculator.cpp -o build\OffenseCalculator.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\ResultPanel.cpp -o build\ResultPanel.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c src\MainWindow.cpp -o build\MainWindow.o
if errorlevel 1 goto :fail
echo        Source compilation done.

echo [4/6] Compiling MOC outputs...
g++.exe %CXXFLAGS% %INCS% -c build\moc_MainWindow.cpp -o build\moc_MainWindow.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c build\moc_TypeButton.cpp -o build\moc_TypeButton.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c build\moc_TypeSelector.cpp -o build\moc_TypeSelector.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c build\moc_InputRow.cpp -o build\moc_InputRow.o
if errorlevel 1 goto :fail
g++.exe %CXXFLAGS% %INCS% -c build\moc_ResultPanel.cpp -o build\moc_ResultPanel.o
if errorlevel 1 goto :fail
echo        MOC compilation done.

echo [5/6] Compiling resource file...
g++.exe %CXXFLAGS% %INCS% -c build\qrc_icons.cpp -o build\qrc_icons.o
if errorlevel 1 goto :fail
echo        Resource compilation done.

echo [6/6] Linking...
g++.exe build\main.o build\TypeChart.o build\TypeButton.o build\TypeSelector.o build\InputRow.o build\OffenseCalculator.o build\ResultPanel.o build\MainWindow.o build\moc_MainWindow.o build\moc_TypeButton.o build\moc_TypeSelector.o build\moc_InputRow.o build\moc_ResultPanel.o build\qrc_icons.o -LF:/Qt/6.11.0/mingw_64/lib -lQt6Widgets -lQt6Gui -lQt6Core -o build\team-defense-calc.exe
if errorlevel 1 goto :link_fail
echo        Linking done.

echo [BONUS] Copying data files...
copy /Y TypeChart.json build\ >nul
echo        TypeChart.json copied.

echo.
echo ======================================
echo   Build SUCCESS! (v2.0)
echo   Output: build\team-defense-calc.exe
echo ======================================
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
