@echo off
setlocal

REM Set the project root directory
set PROJECT_ROOT=%~dp0

REM Compile C++ files
echo Compiling C++ files...

set CPP_FILES=logger.cpp client_generator.cpp  REM Add other necessary .cpp files here

for %%f in (%CPP_FILES%) do (
    echo Compiling %%f...
    g++ %%f -o %%~nf.exe
    if errorlevel 1 (
        echo Error compiling %%f. Exiting...
        exit /b 1
    )
)

echo C++ compilation completed successfully.

REM Execute Python scripts
echo Executing Python scripts...

set PYTHON_FILES=config_reader.py file_manager.py logger.py validator.py  REM Add other necessary .py files here

for %%f in (%PYTHON_FILES%) do (
    echo Executing %%f...
    python %%f
    if errorlevel 1 (
        echo Error executing %%f. Exiting...
        exit /b 1
    )
)

echo Python scripts executed successfully.

REM Build Go files
echo Building Go files...

set GO_FILES=validator.go  REM Add other necessary .go files here

for %%f in (%GO_FILES%) do (
    echo Building %%f...
    go build %%f
    if errorlevel 1 (
        echo Error building %%f. Exiting...
        exit /b 1
    )
)

echo Go files built successfully.

echo Compilation process completed successfully.
endlocal
