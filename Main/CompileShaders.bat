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

:: Compile Vertex Shader
echo [Compiling] Vertex Shader: shader.vert -> shader.vert.spv
echo.
%GLSLANG_VALIDATOR% -V -H -o shaders\shader.vert.spv shaders\shader.vert
if %ERRORLEVEL% neq 0 (
    echo [Error] Failed to compile shader.vert
    pause
    exit /b
)
echo [Success] Vertex shader compiled successfully.
echo.

:: Compile Fragment Shader
echo [Compiling] Fragment Shader: shader.frag -> shader.frag.spv
echo.
%GLSLANG_VALIDATOR% -V -H -o shaders\shader.frag.spv shaders\shader.frag
if %ERRORLEVEL% neq 0 (
    echo [Error] Failed to compile shader.frag
    pause
    exit /b
)
echo [Success] Fragment shader compiled successfully.
echo.

echo ===============================================================================================
echo                             Shader Compilation Complete 
echo ===============================================================================================
pause
