cls
C:\VulkanSDK\VulkanSDK\Bin\glslangValidator.exe -V -H -o shader.vert.spv shader.vert

C:\VulkanSDK\VulkanSDK\Bin\glslangValidator.exe -V -H -o shader.frag.spv shader.frag

cl.exe /c /EHsc /I "C:\\VulkanSDK\\VulkanSDK\\Include" VK.cpp darshan_module.cpp

rc.exe VK.rc

link.exe /SUBSYSTEM:WINDOWS /LIBPATH:"C:\VulkanSDK\VulkanSDK\Lib" VK.obj darshan_module.obj VK.res user32.lib gdi32.lib 
