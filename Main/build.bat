@echo off
cls

echo.
echo ===============================================================================================
echo                                 Vulkan Application Build
echo ===============================================================================================
echo.

:: Define project name
set PROJECT_NAME=VkMain

:: Define Vulkan SDK path (update if needed)
set VULKAN_SDK="C:\VulkanSDK\Vulkan"
set OPENAL_LIB="./external/openal/libs/Win64"

:: Step 1: Cleanup previous build artifacts
echo [Cleaning] Removing previous build files...
del /Q %PROJECT_NAME%.exe >nul 2>&1
del /Q ".\\bin\\*.obj" >nul 2>&1
del /Q %PROJECT_NAME%.res >nul 2>&1
echo [Cleaning] Done.
echo.

:: Step 2: Compile the C++ source file
echo [Compiling] C++ source: %PROJECT_NAME%.cpp
for /r "src" %%i in ("*.cpp") do (
    cl.exe /Fo".\\bin\\" /c /EHsc "%%i" /I "%VULKAN_SDK%\Include"
    if %ERRORLEVEL% neq 0 (
        echo [Error] Compilation failed!
        pause
        exit /b
    )
)
echo [Success] Compilation complete.
echo.

:: Step 3: Compile the resource file
echo [Compiling] Resource file: %PROJECT_NAME%.rc
rc.exe "src\\%PROJECT_NAME%.rc"
if %ERRORLEVEL% neq 0 (
    echo [Error] Resource compilation failed!
    pause
    exit /b
)
echo [Success] Resource compilation complete.
echo.

:: Step 4: Link everything into executable
echo [Linking] Creating executable: %PROJECT_NAME%.exe
link.exe /out:%PROJECT_NAME%.exe ./bin/*.obj "src\\%PROJECT_NAME%.res" user32.lib gdi32.lib ^
    /LIBPATH:"%VULKAN_SDK%\Lib" /LIBPATH:"%OPENAL_LIB%" OpenAL32.lib /SUBSYSTEM:CONSOLE
if %ERRORLEVEL% neq 0 (
    echo [Error] Linking failed!
    pause
    exit /b
)
echo [Success] Linking complete.
echo.

:: Step 5: Run the application (optional)
echo ===============================================================================================
echo                               Build Finished Successfully
echo ===============================================================================================
echo.
echo Launching application...
echo.
start %PROJECT_NAME%.exe

pause
