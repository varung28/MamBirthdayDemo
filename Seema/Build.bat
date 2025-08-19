cls
del VK.exe
del VK.obj
del VK.res

cl.exe /c /EHsc /I C:\VulkanSDK\VULKAN\Include VK.cpp

rc.exe VK.rc

link.exe VK.obj VK.res user32.lib gdi32.lib /LIBPATH:C:\VulkanSDK\VULKAN\Lib /SUBSYSTEM:WINDOWS

