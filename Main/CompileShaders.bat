@echo off
cls

echo.
echo ===============================================================================================
echo                               Vulkan Shader Compilation
echo ===============================================================================================
echo.

:: Delete previous SPIR-V files
echo [Cleaning] Removing old SPIR-V files...
del /Q "*.spv" >nul 2>&1
echo [Cleaning] Done.
echo.

:: Set GLSLangValidator path (change this if Vulkan SDK is installed elsewhere)
set GLSLANG_VALIDATOR="C:\VulkanSDK\VulkanSDK\Bin\glslangValidator.exe"

:: Check if glslangValidator exists
if not exist %GLSLANG_VALIDATOR% (
    echo [Error] glslangValidator not found at %GLSLANG_VALIDATOR%
    echo Please make sure the Vulkan SDK is installed and path is correct.
    pause
    exit /b
)

@REM for /r %%i in (*.vert) do (
@REM     set "INPUT=%%i"
@REM     set "FILENAME=%%~nxi"
@REM     set "OUTPUT=bin\%%~nxi.spv"

@REM     echo [Compiling] %INPUT% -> %OUTPUT%
@REM     %GLSLANG_VALIDATOR% -V -o "%OUTPUT%" "%INPUT%"
@REM     echo [Success] %FILENAME% compiled.
@REM     echo.
@REM )
@REM echo [Success] Vertex shader compiled successfully.
@REM echo.

@REM for /r %%i in (*.frag) do (
@REM     set "INPUT=%%i"
@REM     set "FILENAME=%%~nxi"
@REM     set "OUTPUT=bin\%%~nxi.spv"

@REM     echo [Compiling] %INPUT% -> %OUTPUT%
@REM     %GLSLANG_VALIDATOR% -V -o "%OUTPUT%" "%INPUT%"
@REM     echo [Success] %FILENAME% compiled.
@REM     echo.
@REM )
@REM echo [Success] Fragment shader compiled successfully.
@REM echo.

@echo off
setlocal enabledelayedexpansion

for /r %%i in (*.vert) do (
    set "INPUT=%%i"
    set "FILENAME=%%~nxi"
    set "OUTPUT=bin\%%~nxi.spv"

    echo [Compiling] !INPUT! -> !OUTPUT!
    "%GLSLANG_VALIDATOR%" -V -o "!OUTPUT!" "!INPUT!"
    if !errorlevel! neq 0 (
        echo [Error] Failed to compile !FILENAME!
    ) else (
        echo [Success] !FILENAME! compiled.
    )
    echo.
)
echo [Success] Vertex shaders compiled successfully.
echo.

for /r %%i in (*.frag) do (
    set "INPUT=%%i"
    set "FILENAME=%%~nxi"
    set "OUTPUT=bin\%%~nxi.spv"

    echo [Compiling] !INPUT! -> !OUTPUT!
    "%GLSLANG_VALIDATOR%" -V -o "!OUTPUT!" "!INPUT!"
    if !errorlevel! neq 0 (
        echo [Error] Failed to compile !FILENAME!
    ) else (
        echo [Success] !FILENAME! compiled.
    )
    echo.
)
echo [Success] Fragment shaders compiled successfully.
echo.
pause


echo ===============================================================================================
echo                             Shader Compilation Complete 
echo ===============================================================================================
pause
