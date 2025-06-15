@echo off

setlocal

set /p SKETCH_PATH="path: "

set FQBN=arduino:avr:mega
set PORT=COM3
set BUILD_DIR=build

echo.
echo === Compiling sketch ===
arduino-cli compile --fqbn %FQBN% --output-dir %BUILD_DIR% "%SKETCH_PATH%"
if errorlevel 1 (
    echo Compilation failed.
    exit /b 1
)

echo.
echo === Uploading to %PORT% ===
arduino-cli upload -p %PORT% --fqbn %FQBN% --input-dir %BUILD_DIR% "%SKETCH_PATH%"
if errorlevel 1 (
    echo Upload failed.
    exit /b 1
)

echo.
echo === Upload successful! ===
exit /b 0