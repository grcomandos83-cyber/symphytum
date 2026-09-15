@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

title Symphytum - Build and Deploy Script

echo ======================================================================
echo                  Symphytum Build and Deploy Script
echo ======================================================================
echo.

:: 1. Define paths
set "PROJECT_ROOT=%~dp0"
set "SOURCE_DIR=%PROJECT_ROOT%"
set "BUILD_DIR=%PROJECT_ROOT%build"
set "QT_BIN=C:\Qt\5.15.2\mingw81_64\bin"
set "MINGW_BIN=C:\Qt\Tools\mingw810_64\bin"

:: 2. Check prerequisites
if not exist "%QT_BIN%\qmake.exe" (
    echo [ERROR] Qt 5.15.2 was not found at: %QT_BIN%
    echo Please make sure Qt is installed.
    echo.
    pause
    exit /b 1
)

if not exist "%MINGW_BIN%\g++.exe" (
    echo [ERROR] MinGW was not found at: %MINGW_BIN%
    echo Please make sure MinGW is installed.
    echo.
    pause
    exit /b 1
)

:: Check if Symphytum is currently running and close it so the linker can write
tasklist /fi "imagename eq symphytum.exe" 2>nul | find /i "symphytum.exe" >nul
if %errorlevel% equ 0 (
    echo [INFO] To Symphytum ektelite afti ti stigmi. Termatismos gia ananeosi...
    taskkill /im symphytum.exe /f >nul 2>&1
    ping 127.0.0.1 -n 2 >nul
)

:: Set PATH environment
set "PATH=%QT_BIN%;%MINGW_BIN%;%PATH%"

echo [1/5] Updating translations (lrelease)...
if exist "%QT_BIN%\lrelease.exe" (
    "%QT_BIN%\lrelease.exe" "%SOURCE_DIR%\Symphytum.pro" >nul 2>&1
    if exist "%SOURCE_DIR%\stuff\translations\symphytum_el.qm" (
        copy /y "%SOURCE_DIR%\stuff\translations\symphytum_el.qm" "%SOURCE_DIR%\resources\languages\" >nul 2>&1
    )
)
echo      Translations updated successfully.
echo.

:: 3. Prepare build directory
echo [2/5] Preparing build directory...
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)
cd /d "%BUILD_DIR%"

:: 4. Run qmake
echo [3/5] Running qmake...
qmake.exe "%SOURCE_DIR%\Symphytum.pro" -spec win32-g++
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] qmake failed with error code %errorlevel%!
    echo.
    pause
    exit /b %errorlevel%
)
echo      qmake completed successfully.
echo.

:: 5. Compile with mingw32-make
echo [4/5] Compiling Symphytum (Release build using %NUMBER_OF_PROCESSORS% threads)...
mingw32-make.exe -j%NUMBER_OF_PROCESSORS% release
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation failed! Check error messages above.
    echo.
    pause
    exit /b %errorlevel%
)
echo      Compilation completed successfully!
echo.

:: 6. Deploy Qt DLLs and dependencies
echo [5/5] Deploying dependencies (windeployqt)...
cd /d "%BUILD_DIR%"
if exist "release\symphytum.exe" (
    windeployqt.exe release\symphytum.exe >nul 2>&1
    copy /y "%MINGW_BIN%\libgcc_s_seh-1.dll" "release\" >nul 2>&1
    copy /y "%MINGW_BIN%\libstdc++-6.dll" "release\" >nul 2>&1
    copy /y "%MINGW_BIN%\libwinpthread-1.dll" "release\" >nul 2>&1
    echo      Dependencies deployed successfully.
) else (
    echo [ERROR] Executable release\symphytum.exe was not found!
    pause
    exit /b 1
)

echo.
echo ======================================================================
echo                    BUILD COMPLETED SUCCESSFULLY!
echo ======================================================================
echo Executable location:
echo %BUILD_DIR%\release\symphytum.exe
echo.

:: Prompt to run
set /p RUN_APP="Thelete na ekinisete tin efarmogi tora; (Y/N, Enter=Y): "
if /i "%RUN_APP%"=="N" goto END
if /i "%RUN_APP%"=="NO" goto END

echo Ekinisi tou Symphytum...
start "" "%BUILD_DIR%\release\symphytum.exe"

:END
echo.
echo Telos ergasias.
ping 127.0.0.1 -n 3 >nul 2>&1
exit /b 0
