@echo off
cls

echo.
echo ===============================================================================================
echo                               Vulkan Shader Compilation
echo ===============================================================================================
echo.

:: Delete previous SPIR-V files
echo [Cleaning] Removing old SPIR-V files...
del /Q "shaders\shader.vert.spv" >nul 2>&1
del /Q "shaders\shader.frag.spv" >nul 2>&1
echo [Cleaning] Done.
echo.

:: Set GLSLangValidator path (change this if Vulkan SDK is installed elsewhere)
set GLSLANG_VALIDATOR="C:\VulkanSDK\vulkan\Bin\glslangValidator.exe"

:: Check if glslangValidator exists
if not exist %GLSLANG_VALIDATOR% (
    echo [Error] glslangValidator not found at %GLSLANG_VALIDATOR%
    echo Please make sure the Vulkan SDK is installed and path is correct.
    pause
    exit /b
)

for /r %%i in (*.vert) do (
    set "INPUT=%%i"
    set "FILENAME=%%~nxi"
    set "OUTPUT=bin\%%~nxi.spv"

    echo [Compiling] %INPUT% -> %OUTPUT%
    %GLSLANG_VALIDATOR% -V -o "%OUTPUT%" "%INPUT%"
    echo [Success] %FILENAME% compiled.
    echo.
)
echo [Success] Vertex shader compiled successfully.
echo.

for /r %%i in (*.frag) do (
    set "INPUT=%%i"
    set "FILENAME=%%~nxi"
    set "OUTPUT=bin\%%~nxi.spv"

    echo [Compiling] %INPUT% -> %OUTPUT%
    %GLSLANG_VALIDATOR% -V -o "%OUTPUT%" "%INPUT%"
    echo [Success] %FILENAME% compiled.
    echo.
)
echo [Success] Fragment shader compiled successfully.
echo.

echo ===============================================================================================
echo                             Shader Compilation Complete 
echo ===============================================================================================
pause
