//=================================================================================================================
// 
// Two2DShape Colored
// shader as same as ortho not with inverted y
// 
// To run shader
// >C:\VulkanSDK\VULKAN\Bin\glslangValidator.exe -V -H -o Shader.vert.spv Shader.vert
// >C:\VulkanSDK\VULKAN\Bin\glslangValidator.exe -V -H -o Shader.frag.spv Shader.frag
//=================================================================================================================

// Common header files
#include<windows.h>  // win32 api
#include<stdio.h>    // For file io 
#include<stdlib.h>   // For exit

// user define header file
#include "VK.h"      

//=================================================================================================================
// vulkan realted header
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

// glm related macro and header files
#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// concept: about above line
// hi line sarv thikani add karychi. Sarv platform la
// pn tyla sangav lagt apn kontya OS var ahot. so ya line chya var ek macro mentioned karva lagto. aapn kontya OS la ahot
// KHR = khronous
// ya line ne fkt desire platform sathi che macro use kele jatil.

//=================================================================================================================

//=================================================================================================================
// Vulkan related lab
#pragma comment (lib, "vulkan-1.lib")

//=================================================================================================================

// Macros
#define WIN_WIDTH  800
#define WIN_HEIGHT 600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const char* gpszAppName = "ARTR";

DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev;

BOOL gbFullscreen = FALSE;
BOOL bWindowMimized = FALSE;
BOOL gbActive = FALSE;

FILE* gpFile = NULL;

HWND ghwnd = NULL;

HDC ghdc = NULL;
HGLRC ghrc = NULL;

//=================================================================================================================
// vulkan related global variables

//instance extenstion related variables
//VK_KHR_SUREFACE_EXTENSTION_NAME and VK_KHR_WIN32_SURFACE_EXTENSION_NAME ya 2 sathi khalche variables declare kele
uint32_t enabledInstanceExtenstionCount = 0;
const char* enabledInstanceExtenstionNames_array[3]; // before it VK_EXT_DEBUG_REPORT_EXTENSTION_NAME
// unsinged int. valukan he compulsary 64 bit ahe. tymule 32 wale se

//Vulkan instance
VkInstance vkInstance = VK_NULL_HANDLE;

//vulkan PresentationSurface related variables
//PresentationSurface Step 1. Declare global variable to hold presentation surface object
VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE; // null la set karycha

//PhysicalDevice
//PhysicalDevice Step 1 : Declare 3 Global variable. 1 for selected physical device. 2 for selected queue family 
//						 index. 3 for physical device's memory property required late

VkPhysicalDevice vkPhysicalDevice_selected = VK_NULL_HANDLE;
uint32_t graphicsQueueFamilyIndex_selected = UINT32_MAX;
VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;

//PrintVKInfo
//PrintVKInfo Step 01 : Remove local declaration of physicalDeviceCount and physicalDeviceArray from getPhysicalDevice() and do it globally.
VkPhysicalDevice* vkPhysicalDevice_array = NULL; // for step 3rd
uint32_t physicalDeviceCount = 0;

// Device Extenstion
//VK_KHR_SWAPCHAIN_EXTENSTION_NAME
uint32_t enabledDeviceExtenstionCount = 0;
const char* enabledDeviceExtenstionNames_array[1];

// 08-VulkanDevice
VkDevice vkDevice = VK_NULL_HANDLE;

// 09 DeviceQueue
VkQueue vkQueue = VK_NULL_HANDLE;

//10 Surface color format and color space
VkFormat vkFormat_color = VK_FORMAT_UNDEFINED;
VkColorSpaceKHR vkColorSpaceKHR = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; // minimum ha tri bhetala pahije

// 11 PresentationMode
VkPresentModeKHR vkPresentModeKHR = VK_PRESENT_MODE_FIFO_KHR;

// 12 Swapchain
int winWidth = WIN_WIDTH;
int winHeight = WIN_HEIGHT;

VkSwapchainKHR vkSwapchainKHR = VK_NULL_HANDLE;
VkExtent2D vkExtent2D_swapchain;

// 13-SwapchainImagesAndViews
// FOR COLOR Images

uint32_t swapchainImageCount = UINT32_MAX;
VkImage* swapchainImage_array = VK_NULL_HANDLE;
VkImageView* swapchainimageView_array = VK_NULL_HANDLE;

// FOR DEPTH IMAGES

VkFormat vkFormat_depth = VK_FORMAT_UNDEFINED;
VkImage vkImage_depth = VK_NULL_HANDLE;
VkImageView vkImageView_depth = VK_NULL_HANDLE;
VkDeviceMemory vkDeviceMemory_depth = VK_NULL_HANDLE;

//14 CommandPool
VkCommandPool vkCommandPool = VK_NULL_HANDLE;

//15 commandbuffer
VkCommandBuffer* vkCommandBuffer_array = VK_NULL_HANDLE;

//16 Render pass
VkRenderPass vkRenderPass = VK_NULL_HANDLE;

//17 Framebuffer
VkFramebuffer* vkFramebuffer_array = VK_NULL_HANDLE;

//18-FencesAndSemaphores
VkSemaphore vkSemaphore_backBuffer = VK_NULL_HANDLE;
VkSemaphore vkSemaphore_renderComplete = VK_NULL_HANDLE;

VkFence* vkFence_array = NULL;

// 19 create comand buffer
VkClearColorValue vkClearColorValue;
VkClearDepthStencilValue vkClearDepthStensilValue;

// 20 Render : Bluescreen

BOOL bInitialised = FALSE;
uint32_t currentImageIndex = UINT32_MAX;

// Validation
BOOL bValidation = TRUE;
uint32_t enabledValidationLayerCount = 0;
const char* enabledValidationLayerName_array[1];  //for VK_LAYER_KHRONOS_validation
VkDebugReportCallbackEXT vkDebugReportCallbackEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT_fnptr = NULL;

// Vetex Buffer related variables.

typedef struct
{
	VkBuffer vkBuffer;
	VkDeviceMemory vkDeviceMemory;
} VetextData;

// position related variable

VetextData vetextData_street_light_middle_bar_position;
VetextData vetextData_street_light_middle_bar_color;//vetextData_street_light_middle_bar_color

VetextData vetextData_position_street_light_lamp; //vetextData_position_street_light_lamp
VetextData vetextData_color_street_light_lamp;// vetextData_color_street_light_lamp

//uniform releated declaration
struct MyUniformData
{
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};

struct UniformData
{
	VkBuffer vkBuffer;
	VkDeviceMemory vkDeviceMemory;
};

UniformData uniformData_street_light_middle_bar;
UniformData uniformData_street_light_horizontal_top_bar;
UniformData uniformData_street_light_lamp_1;
UniformData uniformData_street_light_lamp_2;

// 23 Shaders

// Step 02 from shader
VkShaderModule vkShaderModule_vertex_shader		 = VK_NULL_HANDLE;
VkShaderModule vkShaderModule_fragment_shader	 = VK_NULL_HANDLE;

//24-DescriptorSetLayout
//Step 01: Globally decalre vulkan object of type VkDescriptorSetLayout and initilaise it to null
VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

//25-PipelineLayout'
VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

// descriptor pool
VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;

// descriptor set
VkDescriptorSet vkDescriptorSet_street_light_middle_bar = VK_NULL_HANDLE;
VkDescriptorSet vkDescriptorSet_street_light_horizontal_top_bar = VK_NULL_HANDLE;
VkDescriptorSet vkDescriptorSet_street_light_lamp_1 = VK_NULL_HANDLE;
VkDescriptorSet vkDescriptorSet_street_light_lamp_2 = VK_NULL_HANDLE; 

//26-Pipeline 07th May 2025

VkViewport vkViweport;
VkRect2D vkRect2D_scissor;
VkPipeline vkPipeline = VK_NULL_HANDLE;

//=================================================================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function declaration
	VkResult initialise(void);
	void uninitialise(void);
	VkResult display(void);
	void update(void);


	// local variable declaration
	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[255]; // artr - 1
	int iResult = 0;
	BOOL bDone = FALSE;

	VkResult vkResult = VK_SUCCESS;
	// Code

	// File IO
	gpFile = fopen("Log.txt", "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log file cannot be open"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}

	fprintf(gpFile, "WinMain() -> Program Started Successfully \n");


	// artr 2
	wsprintf(szAppName, TEXT("%s"), gpszAppName); // gpszAppName hi copy keli local var madhe copy keli

	// initialise WNDCLASSEX

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	RegisterClassEx(&wndClass);

	// centering of window
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int xPos = (screenWidth - WIN_WIDTH) / 2;
	int yPos = (screenHeight - WIN_HEIGHT) / 2;

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("Vulkan On Windows | Object : StreetLight"), 
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		xPos,
		yPos,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	// initialisation
	vkResult = initialise();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "WinMain() -> initialise() is failed. \n");
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fprintf(gpFile, "WinMain() -> initialise() is successed. \n");
	}

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	SetForegroundWindow(hwnd);// mazya windows la foregroud la aan z order madhe
	SetFocus(hwnd);

	// Game loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActive == TRUE)
			{
				if (bWindowMimized == FALSE)
				{
					vkResult = display();
					if (vkResult != VK_FALSE && vkResult != VK_SUCCESS && vkResult != VK_ERROR_OUT_OF_DATE_KHR && vkResult != VK_SUBOPTIMAL_KHR)
					{
						fprintf(gpFile, "WinMain() -> call to display failed. \n");
						bDone = TRUE;
					}
					update();
				}
			}
			
		}
	}

	uninitialise();

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declaration
	void ToggleFullscreen(void);
	VkResult resize(int, int);

	// code
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			bWindowMimized = TRUE;
		}
		else
		{
			resize(LOWORD(lParam), HIWORD(lParam));
			bWindowMimized = FALSE;
		}
		
		break;

	//case WM_ERASEBKGND: return(0);  // ya mule window madhe yeun rahte fullscreen la

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{

		case 'F':
		case 'f':
			if (gbFullscreen == FALSE)
			{
				ToggleFullscreen();
				gbFullscreen = TRUE;
			}
			else
			{
				ToggleFullscreen();
				gbFullscreen = FALSE;
			}
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE: 
		//wpPrev ch memset
		//memset
		//ZeroMemory((void*)d3d11InputElementDescriptor, sizeof(D3D11_INPUT_ELEMENT_DESC) * _ARRAYSIZE(d3d11InputElementDescriptor));

		wpPrev.length = sizeof(WINDOWPLACEMENT);
		break;

	default: break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	// local variable declaration
	MONITORINFO mi = { sizeof(MONITORINFO) };

	// Code
	if (gbFullscreen == FALSE)
	{
		// get current window style
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		// mala window cha long type cha variable de. DWORD And Long same
		// 1. hwnd 2. style de

		if (dwStyle & WS_OVERLAPPEDWINDOW) // contains sathi &. dwstyle madhe to style ahe ka
		{
			// MonitorFromWindow : HMONITOR handle deto monitor cha, 2nd parametr : primary monitor de. MONITORINFOF_PRIMARY macro : last F  is flage
			// get current window placement, get monitor width, height
			if (GetWindowPlacement(ghwnd, &wpPrev)
				&& GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)) //GetWindowPlacement ::  kontya window chi placement, kasha madhe deu
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos( // Pos = position, 2nd parameter ne WS_OVERLAPPED bhetate 
					ghwnd,
					HWND_TOP, // z ordermadhe top la 
					mi.rcMonitor.left,  // client area cha left
					mi.rcMonitor.top,  //
					mi.rcMonitor.right - mi.rcMonitor.left, // rundi
					mi.rcMonitor.bottom - mi.rcMonitor.top, // unchi
					SWP_NOZORDER | SWP_FRAMECHANGED
				);
			}
		}
		ShowCursor(FALSE); // cursor hide karne
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED); //setWindowPos has greater priory tha setWindowStyle and Placement
		// normal aanta window halu nko
		// normal la anta size halu nko,
		// window chya owner z order badlu nko
		// window chi z order badlu nko
		// frame cha size claculate
		ShowCursor(TRUE);
	}
}

VkResult initialise(void)
{
	// function declaration
	VkResult createVulkanInstance(void);
	VkResult getSupportedSurface(void);
	VkResult getPhysicalDevice(void);
	VkResult printVkInfo(void);
	VkResult deviceExtenstion(void);
	VkResult createVulkanDevice(void);
	void	 getDeviceQueue(void);
	VkResult createSwapchain(VkBool32);
	VkResult createImagesAndImageView(void);
	VkResult createCommandPool(void);
	VkResult createCommandBuffer(void);
	VkResult createVertexBuffer(void);
	VkResult createUniformBuffer(void);
	VkResult createShaders(void);
	VkResult createDecriptorSetLayout(void);
	VkResult createPipelineLayout(void);
	VkResult createDescriptorPool(void);
	VkResult createDescriptorSet(void);
	VkResult createRenderPass(void);
	VkResult createPipeline(void);
	VkResult createFramebuffers(void);
	VkResult createSemaphores(void);
	VkResult createFences(void);
	VkResult buildCommandBuffer(void);
	
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	vkResult = createVulkanInstance();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createVulkanInstance() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createVulkanInstance() is successed. \n");
	}

	// Create vulkan presentation surface
	vkResult = getSupportedSurface();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> getSupportedSurface() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> getSupportedSurface() is successed. \n");
	}

	// selected required physical devices and its queuefamily index
	vkResult = getPhysicalDevice();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> getPhysicalDevice() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> getPhysicalDevice() is successed. \n");
	}

	//Print Vulkan Info
	vkResult = printVkInfo();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> printVkInfo() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> printVkInfo() is successed. \n");
	}

	// create Vulkan device (logical)
	vkResult = createVulkanDevice();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createVulkanDevice() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createVulkanDevice() is successed. \n");
	}

	// device queue
	getDeviceQueue();

	// 10 and 11th step moved to 12 step swapchain

	// Step 12 : Swapchain
	vkResult = createSwapchain(VK_FALSE);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createSwapchain() is failed. %d \n", vkResult);
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createSwapchain() is successed. \n");
	}

	// 13-SwapchainImagesAndViews
	vkResult = createImagesAndImageView();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createImagesAndImageView() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createImagesAndImageView() is successed. \n");
	}

	//14 CommandPool
	vkResult = createCommandPool();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createCommandPool() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createCommandPool() is successed. \n");
	}

	//VkResult createCommandBuffer(void);
	vkResult = createCommandBuffer();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createCommandBuffer() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createCommandBuffer() is successed. \n");
	}

	// vertex buffer
	
	vkResult = createVertexBuffer();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createVertexBuffer() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createVertexBuffer() is successed. \n");
	}
	
	//createUniformBuffer

	vkResult = createUniformBuffer();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createUniformBuffer() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createUniformBuffer() is successed. \n");
	}

	// 23 Shaders createShaders
	vkResult = createShaders();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createShaders() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createShaders() is successed. \n");
	}

	//24.
	//Step 02: in intialise declare and call udf createDecriptorSetLayout() maintiaing the convention of decalre and calling after createShader() before createRenderPass()
	vkResult = createDecriptorSetLayout();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createDecriptorSetLayout() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createDecriptorSetLayout() is successed. \n");
	}


	vkResult = createPipelineLayout();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createPipelineLayout() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createPipelineLayout() is successed. \n");
	}

	//VkResult createDescriptorPool(void);
	vkResult = createDescriptorPool();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createDescriptorPool() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createDescriptorPool() is successed. \n");
	}
	
	//VkResult createDescriptorSet(void);
	vkResult = createDescriptorSet();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createDescriptorSet() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createDescriptorSet() is successed. \n");
	}

	// 16 create render pass
	vkResult = createRenderPass();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createRenderPass() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createRenderPass() is successed. \n");
	}

	//createPipeline
	vkResult = createPipeline();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createPipeline() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createPipeline() is successed. \n");
	}

	//17 createFramebuffers(void)
	vkResult = createFramebuffers();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createFramebuffers() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createFramebuffers() is successed. \n");
	}
	
	//18 create semaphore
	vkResult = createSemaphores();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createSemaphores() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createSemaphores() is successed. \n");
	}

	//18 create fences
	vkResult = createFences();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> createFences() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> createFences() is successed. \n");
	}

	// 19.
	// initialise clear color value
	memset((void*)&vkClearColorValue, 0 , sizeof(VkClearColorValue));

	vkClearColorValue.float32[0] = 0.5f;
	vkClearColorValue.float32[1] = 0.5f;
	vkClearColorValue.float32[2] = 0.5f;
	vkClearColorValue.float32[3] = 1.0f;
	
	// set default clear depth 
	memset((void*)&vkClearDepthStensilValue, 0, sizeof(VkClearDepthStencilValue));

	vkClearDepthStensilValue.depth = 1.0f; // hi value float ahe

	// set default stensil value
	vkClearDepthStensilValue.stencil = 0; // stensil value is unit32 ahe

	vkResult = buildCommandBuffer();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "initialise() -> buildCommandBuffer() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "initialise() -> buildCommandBuffer() is successed. \n");
	}

	// 20. Render Bluescreen
	// initialisation is completed
	bInitialised = TRUE;
	fprintf(gpFile, "initialise() -> initialisation is successed. \n");
	return (vkResult);
}

VkResult resize(int width, int height)
{
	// function declaration
	VkResult createSwapchain(VkBool32);
	VkResult createImagesAndImageView(void);
	VkResult createCommandBuffer(void);
	VkResult createPipelineLayout(void);
	VkResult createPipeline(void);
	VkResult createRenderPass(void);
	VkResult createFramebuffers(void);
	VkResult buildCommandBuffer(void);

	// Variable declaration
	VkResult vkResult = VK_SUCCESS;

	// code
	if (height <= 0 )
		height = 1;

	// check the bInitialised
	if (bInitialised == FALSE)
	{
		fprintf(gpFile, "\n resize() -> bInitialised is yet not completed or failed. \n");
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return vkResult;
	}

	// has recreation of swapchain needed we are going to repeat many steps of initialise again Hence set bInitialised = false again
	bInitialised = FALSE;

	//1. set global winWidth winHeight variables
	
	winWidth = width;
	winHeight = height;

	// destroy 8 thing and recreate again

	// wait for device to complete in hand task
	if (vkDevice)
	{
		vkDeviceWaitIdle(vkDevice);
		fprintf(gpFile, "resize() -> vkDevice vkDeviceWaitIdle() is done. \n");
	}

	// destroy framebuffer array
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		vkDestroyFramebuffer(vkDevice, vkFramebuffer_array[i], NULL);
	}
	if (vkFramebuffer_array)
	{
		free(vkFramebuffer_array);
		vkFramebuffer_array = NULL;
		fprintf(gpFile, "resize() -> vkDestroyFramebuffer() is done. \n");
	}

	// destroy command buffer
	for (uint32_t i = 0; i < swapchainImageCount; i++)
		vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_array[i]);

	fprintf(gpFile, "resize() -> vkFreeCommandBuffers() is done. \n");

	if (vkCommandBuffer_array)
	{
		free(vkCommandBuffer_array);
		vkCommandBuffer_array = NULL;
		fprintf(gpFile, "resize() -> vkCommandBuffer_array is free. \n");
	}

	// Graphics pipeline
	if (vkPipeline)
	{
		vkDestroyPipeline(vkDevice, vkPipeline, NULL);
		vkPipeline = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyPipeline() is done. \n");
	}

	//destroy pipelinelayout
	if (vkPipelineLayout)
	{
		vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, NULL);
		vkPipelineLayout = VK_NULL_HANDLE;
		fprintf(gpFile, "resize() -> vkDestroyPipelineLayout() is done.\n");
	}

	//destroy render pass
	if (vkRenderPass)
	{
		vkDestroyRenderPass(vkDevice, vkRenderPass, NULL);
		fprintf(gpFile, "resize() -> vkDestroyRenderPass() is done. \n");
		vkRenderPass = NULL;
	}

	// ---------------------
	// Depth related
	if (vkImageView_depth) // depth image
	{
		vkDestroyImageView(vkDevice, vkImageView_depth, NULL);
		vkImageView_depth = VK_NULL_HANDLE;
		//	fprintf(gpFile, "resize() -> vkDestroyCommandPool() is done for depth : vkImageView_depth. \n");
	}

	if (vkDeviceMemory_depth) // destroy depth memory
	{
		vkFreeMemory(vkDevice, vkDeviceMemory_depth, NULL);
		vkDeviceMemory_depth = VK_NULL_HANDLE;
		//fprintf(gpFile, "resize() -> vkFreeMemory() is done for depth : vkDeviceMemory_depth. \n");
	}

	if (vkImage_depth)  //destroy depth image
	{
		vkDestroyImage(vkDevice, vkImage_depth, NULL);
		vkImage_depth = VK_NULL_HANDLE;
		//fprintf(gpFile, "resize() -> vkDestroyImage() is done for depth : vkImage_depth. \n");
	}
	//--------------------------------------------------------

	// swapnchain
	for (uint32_t i = 0; i < swapchainImageCount; i++)
		vkDestroyImageView(vkDevice, swapchainimageView_array[i], NULL);

	fprintf(gpFile, "resize() -> vkDestroyImageView() is done. \n");

	if (swapchainimageView_array)
	{
		free(swapchainimageView_array);
		swapchainimageView_array = NULL;
	}

	fprintf(gpFile, "resize() -> swapchainimageView_array is free. \n");

	if (swapchainImage_array)
	{
		free(swapchainImage_array);
		swapchainImage_array = NULL;
		fprintf(gpFile, "resize() -> swapchainImage_array is free. \n");
	}

	// check presence of swap chain
	if (vkSwapchainKHR == VK_NULL_HANDLE)
	{
		fprintf(gpFile, "resize() -> vkSwapchainKHR is already null can not procced \n");
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return vkResult;
	}

	// destroy swapchain
	if (vkSwapchainKHR)
	{
		vkDestroySwapchainKHR(vkDevice, vkSwapchainKHR, NULL);
		vkSwapchainKHR = VK_NULL_HANDLE;
		fprintf(gpFile, "resize() -> vkDestroySwapchainKHR() is done. \n");
	}


	//===========================================================================================
	//								RECREATE FOR RESIZE

	// create swapchain
	vkResult = createSwapchain(VK_FALSE);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "resize() -> createSwapchain() is failed. %d \n", vkResult);
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}

	//createImagesAndImageView
	vkResult = createImagesAndImageView();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "resize() -> createImagesAndImageView() is failed. %d \n", vkResult);
		return (vkResult);
	}

	// 16 create render pass
	vkResult = createRenderPass();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "resize() -> createRenderPass() is failed. %d \n", vkResult);
		return (vkResult);
	}

	// pipeline layout
	vkResult = createPipelineLayout();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "resize() -> createPipelineLayout() is failed. %d \n", vkResult);
		return (vkResult);
	}

	//createPipeline
	vkResult = createPipeline();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "resize() -> createPipeline() is failed. %d \n", vkResult);
		return (vkResult);
	}

	//17 createFramebuffers(void)
	vkResult = createFramebuffers();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "resize() -> createFramebuffers() is failed. %d \n", vkResult);
		return (vkResult);
	}

	//createCommandBuffer
	vkResult = createCommandBuffer();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "resize() -> createCommandBuffer() is failed. %d \n", vkResult);
		return (vkResult);
	}

	vkResult = buildCommandBuffer();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "resize() -> buildCommandBuffer() is failed. %d \n", vkResult);
		return (vkResult);
	}

	bInitialised = TRUE;

	return (vkResult);
}

VkResult display(void)
{
	// 20. Render Bluescreen
	VkResult resize(int, int);
	VkResult updateUniformBuffer(void);

	// variable
	VkResult vkResult = VK_SUCCESS;

	// code 
	// if control comes here before initiialsation is completed return false
	if (bInitialised == FALSE)
	{
		//fprintf(gpFile, "\n display() -> initiialsation is not yet completed. \n");
		return (VkResult)VK_FALSE;
	}

	// accqure the index of next swapchain image
	/*VkResult vkAcquireNextImageKHR(
		VkDevice                                    device,
		VkSwapchainKHR                              swapchain,
		uint64_t                                    timeout,
		VkSemaphore                                 semaphore,
		VkFence                                     fence,
		uint32_t * pImageIndex);*/

	vkResult = vkAcquireNextImageKHR(vkDevice, vkSwapchainKHR, UINT64_MAX, vkSemaphore_backBuffer, VK_NULL_HANDLE, &currentImageIndex);
	if (vkResult != VK_SUCCESS)
	{
		if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR)
		{
			resize(winWidth, winHeight);
		}
		else
		{
			fprintf(gpFile, "display() -> vkAcquireNextImageKHR() is failed %d. \n", vkResult);
			return vkResult;
		}
	}

	//use fence to allow to host to wait for completion of execution of previous command buffer
	/*VkResult vkWaitForFences(
		VkDevice                                    device,
		uint32_t                                    fenceCount,
		const VkFence * pFences,
		VkBool32                                    waitAll,
		uint64_t                                    timeout);*/ 

	vkResult = vkWaitForFences(vkDevice, 1, &vkFence_array[currentImageIndex], VK_TRUE, UINT64_MAX);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "display() -> vkWaitForFences() is failed %d. \n", vkResult);
		return vkResult;
	}

	//make fences ready for execution of next command buffer
	/*VkResult vkResetFences(
		VkDevice                                    device,
		uint32_t                                    fenceCount,
		const VkFence * pFences);*/
	vkResult = vkResetFences(vkDevice, 1, &vkFence_array[currentImageIndex]);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "display() -> vkResetFences() is failed %d. \n", vkResult);
		return vkResult;
	}

	// one of the member of VkSubmitInfo structure requires array of pipeline stages. 
	// We have only one of completion of color attachment output. Still we need one member array.

	const VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	/*typedef struct VkSubmitInfo {
		VkStructureType                  sType;
		const void*						 pNext;
		uint32_t						 waitSemaphoreCount;
		const VkSemaphore*				 pWaitSemaphores;
		const VkPipelineStageFlags*		 pWaitDstStageMask;
		uint32_t                         commandBufferCount;
		const VkCommandBuffer*			 pCommandBuffers;
		uint32_t						 signalSemaphoreCount;
		const VkSemaphore*				 pSignalSemaphores;
	} VkSubmitInfo;*/

	// declare memset, and initialise VkSubmitInfo structre
	VkSubmitInfo vkSubmitInfo;
	memset((void*)&vkSubmitInfo, 0, sizeof(VkSubmitInfo));

	vkSubmitInfo.sType					 = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.pNext					 = NULL;
	vkSubmitInfo.pWaitDstStageMask		 = &waitDstStageMask;
	vkSubmitInfo.waitSemaphoreCount		 = 1;
	vkSubmitInfo.pWaitSemaphores		 = &vkSemaphore_backBuffer;
	vkSubmitInfo.commandBufferCount		 = 1;
	vkSubmitInfo.pCommandBuffers		 = &vkCommandBuffer_array[currentImageIndex];
	vkSubmitInfo.signalSemaphoreCount	 = 1;
	vkSubmitInfo.pSignalSemaphores		 = &vkSemaphore_renderComplete;

	// now submit above work to the queue
	/*VkResult vkQueueSubmit(
		VkQueue                                     queue,
		uint32_t                                    submitCount,
		const VkSubmitInfo * pSubmits,
		VkFence                                     fence);*/

	vkResult = vkQueueSubmit(vkQueue, 1, &vkSubmitInfo, vkFence_array[currentImageIndex]);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "display() -> vkQueueSubmit() is failed %d. \n", vkResult);
		return vkResult;
	}

	// we are going to present rendered image after declaring, initialisating VkPresentInfoKHR structure
	/*typedef struct VkPresentInfoKHR {
		VkStructureType          sType;
		const void* pNext;
		uint32_t                 waitSemaphoreCount;
		const VkSemaphore* pWaitSemaphores;
		uint32_t                 swapchainCount;
		const VkSwapchainKHR* pSwapchains;
		const uint32_t* pImageIndices;
		VkResult* pResults;
	} VkPresentInfoKHR;*/


	VkPresentInfoKHR vkPresentInfoKHR;
	memset((void*)&vkPresentInfoKHR, 0, sizeof(VkPresentInfoKHR));
	
	vkPresentInfoKHR.sType				 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	vkPresentInfoKHR.pNext				 = NULL;
	vkPresentInfoKHR.swapchainCount		 = 1;
	vkPresentInfoKHR.pSwapchains		 = &vkSwapchainKHR;
	vkPresentInfoKHR.pImageIndices		 = &currentImageIndex;
	vkPresentInfoKHR.waitSemaphoreCount  = 1;
	vkPresentInfoKHR.pWaitSemaphores	 = &vkSemaphore_renderComplete;
//	vkPresentInfoKHR.pResults			 = ;

	// now present the quque
	vkResult = vkQueuePresentKHR(vkQueue,&vkPresentInfoKHR);
	if (vkResult != VK_SUCCESS)
	{
		if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR)
		{
			resize(winWidth, winHeight);
		}
		else
		{
			fprintf(gpFile, "display() -> vkQueuePresentKHR() is failed %d. \n", vkResult);
			return vkResult;
		}
	}

	vkResult = updateUniformBuffer();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "display() -> updateUniformBuffer() is failed %d. \n", vkResult);
		return vkResult;
	}

	vkDeviceWaitIdle(vkDevice);
	
	return vkResult;
}

void update(void)
{
	// code
}

void uninitialise(void)
{
	// function declarations
	void ToggleFullscreen(void);

	if (gbFullscreen == TRUE)
	{
		ToggleFullscreen();
		gbFullscreen = FALSE;
	}

	// release hdc
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// destroy window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}
	
	// no need to destroy/ unitialise device queue
	/*VulkanDevice Step 05: destroy this device when done
		Imp * **Before destroying device ensure that all operation on that device are finsied.Till then wait on the device.*/
	if (vkDevice)
	{
		//VkResult vkDeviceWaitIdle(VkDevice device); // vulkan's first synchronisation function
		vkDeviceWaitIdle(vkDevice);
		fprintf(gpFile, "uninitialise() -> vkDeviceWaitIdle() is done. \n");
	}

	//-------------------------------------------------------------------------------
	// do uninitalise regarding vulkan below if (vkDevice)
	
	//18-FencesAndSemaphores
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		/*void vkDestroyFence(
			VkDevice                                    device,
			VkFence                                     fence,
			const VkAllocationCallbacks * pAllocator);*/

		vkDestroyFence(vkDevice, vkFence_array[i], NULL);
	}
	
	fprintf(gpFile, "uninitialise() -> vkDestroyFence() is done. \n");

	if (vkFence_array)
	{
		free(vkFence_array);
		vkFence_array = NULL;
	}
	
	// destroy semaphore
	/*void vkDestroySemaphore(
		VkDevice                                    device,
		VkSemaphore                                 semaphore,
		const VkAllocationCallbacks * pAllocator);*/

	if (vkSemaphore_renderComplete)
	{
		vkDestroySemaphore(vkDevice, vkSemaphore_renderComplete, NULL);
		fprintf(gpFile, "uninitialise() -> vkDestroySemaphore() is done. for vkSemaphore_renderComplete\n");
	}
	
	if (vkSemaphore_backBuffer)
	{
		vkDestroySemaphore(vkDevice, vkSemaphore_backBuffer, NULL);
		fprintf(gpFile, "uninitialise() -> vkDestroySemaphore() is done. for vkSemaphore_backBuffer\n");
	}
	
	//17 Framebuffers
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		/*void vkDestroyFramebuffer(
			VkDevice                                    device,
			VkFramebuffer                               framebuffer,
			const VkAllocationCallbacks * pAllocator);*/

		vkDestroyFramebuffer(vkDevice, vkFramebuffer_array[i], NULL);
	}
	
	if (vkFramebuffer_array)
	{
		free(vkFramebuffer_array);
		vkFramebuffer_array = NULL;
		fprintf(gpFile, "uninitialise() -> vkDestroyFramebuffer() is done. \n");
	}

	// Graphics pipeline
	if (vkPipeline)
	{
		vkDestroyPipeline(vkDevice, vkPipeline, NULL);
		vkPipeline = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyPipeline() is done. \n");
	}

	//16 : Step 06: in unitialise the destroy the render pass by using vkDestroyRenderPass()
	/*void vkDestroyRenderPass(
		VkDevice                                    device,
		VkRenderPass                                renderPass,
		const VkAllocationCallbacks * pAllocator);*/

	if (vkRenderPass)
	{
		vkDestroyRenderPass(vkDevice, vkRenderPass, NULL);
		fprintf(gpFile, "uninitialise() -> vkDestroyRenderPass() is done. \n");
		vkRenderPass = NULL;
	}

	//destroy vkDescriptorPool // descriptor set vegala destroy karyla lagat nhi
	if (vkDescriptorPool)
	{
		vkDestroyDescriptorPool(vkDevice, vkDescriptorPool, NULL);
		fprintf(gpFile, "uninitialise() -> vkDestroyDescriptorPool() is done. vkDescriptorPool and vkDescriptorSet are destroy successfully.\n");
		vkDescriptorPool = VK_NULL_HANDLE;
		vkDescriptorSet_street_light_lamp_1 = VK_NULL_HANDLE;
	}

	if (vkPipelineLayout)
	{
		/*void vkDestroyPipelineLayout(
			VkDevice                                    device,
			VkPipelineLayout                            pipelineLayout,
			const VkAllocationCallbacks* pAllocator);*/

		vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, NULL);
		vkPipelineLayout = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyPipelineLayout() is done.\n");
	}

	//Step 05: in unitialise call vkDestroyDescriptorSetLayout() vulkan api to destroy this vulkan object
	if (vkDescriptorSetLayout)
	{
		/*void vkDestroyDescriptorSetLayout(
			VkDevice                                    device,
			VkDescriptorSetLayout                       descriptorSetLayout,
			const VkAllocationCallbacks * pAllocator);*/
		vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout, NULL);
		vkDescriptorSetLayout = VK_NULL_HANDLE;
	}

	fprintf(gpFile, "uninitialise() -> vkDescriptorSetLayout() is done.\n");
	// destroy shader module
		
	if (vkShaderModule_vertex_shader)
	{
		vkDestroyShaderModule(vkDevice, vkShaderModule_fragment_shader, NULL);
		vkShaderModule_fragment_shader = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyShaderModule() is done. for  vkShaderModule_fragment_shader\n");
	}

	if (vkShaderModule_vertex_shader)
	{
		vkDestroyShaderModule(vkDevice, vkShaderModule_vertex_shader, NULL);
		vkShaderModule_vertex_shader = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyShaderModule() is done. for  vkShaderModule_vertex_shader\n");
	}

	// destroy uniform buffer
	// For rectangle 1
	if (uniformData_street_light_lamp_1.vkBuffer)
	{
		vkDestroyBuffer(vkDevice, uniformData_street_light_lamp_1.vkBuffer, NULL);
		uniformData_street_light_lamp_1.vkBuffer = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyBuffer() is done for uniformData_street_light_lamp_1.vkBuffer\n");
	}

	if (uniformData_street_light_lamp_1.vkDeviceMemory)
	{
		vkFreeMemory(vkDevice, uniformData_street_light_lamp_1.vkDeviceMemory, NULL);
		uniformData_street_light_lamp_1.vkDeviceMemory = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkFreeMemory() is done for uniformData_street_light_lamp_1.vkDeviceMemory\n");
	}

	// for rectangle 2
	if (uniformData_street_light_lamp_2.vkBuffer)
	{
		vkDestroyBuffer(vkDevice, uniformData_street_light_lamp_2.vkBuffer, NULL);
		uniformData_street_light_lamp_2.vkBuffer = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyBuffer() is done for uniformData_street_light_lamp_2.vkBuffer\n");
	}

	if (uniformData_street_light_lamp_2.vkDeviceMemory)
	{
		vkFreeMemory(vkDevice, uniformData_street_light_lamp_2.vkDeviceMemory, NULL);
		uniformData_street_light_lamp_2.vkDeviceMemory = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkFreeMemory() is done for uniformData_street_light_lamp_2.vkDeviceMemory\n");
	}
	//-------------------------------------------------


	// destroy vetextData_color_street_light_lamp 
	if (vetextData_color_street_light_lamp.vkDeviceMemory)
	{
		vkFreeMemory(vkDevice, vetextData_color_street_light_lamp.vkDeviceMemory, NULL);
		vetextData_color_street_light_lamp.vkDeviceMemory = VK_NULL_HANDLE;
	}

	if (vetextData_color_street_light_lamp.vkBuffer)
	{
		vkDestroyBuffer(vkDevice, vetextData_color_street_light_lamp.vkBuffer, NULL);
		vetextData_color_street_light_lamp.vkBuffer = VK_NULL_HANDLE;
	}

	// destroy vetextData_position_street_light_lamp
	if (vetextData_position_street_light_lamp.vkDeviceMemory)
	{
		vkFreeMemory(vkDevice, vetextData_position_street_light_lamp.vkDeviceMemory, NULL);
		vetextData_position_street_light_lamp.vkDeviceMemory = VK_NULL_HANDLE;
	}

	if (vetextData_position_street_light_lamp.vkBuffer)
	{
		vkDestroyBuffer(vkDevice, vetextData_position_street_light_lamp.vkBuffer, NULL);
		vetextData_position_street_light_lamp.vkBuffer = VK_NULL_HANDLE;
	}

	//15. command buffer
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		/*void vkFreeCommandBuffers(
			VkDevice                                    device,
			VkCommandPool                               commandPool,
			uint32_t                                    commandBufferCount,
			const VkCommandBuffer * pCommandBuffers);*/

		vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &vkCommandBuffer_array[i]);
	}
	
	fprintf(gpFile, "uninitialise() -> vkFreeCommandBuffers() is done. \n");

	//15 CommandBuffer : step 05: free the actual command buffer array
	if (vkCommandBuffer_array)
	{
		free(vkCommandBuffer_array);
		vkCommandBuffer_array = NULL;
		fprintf(gpFile, "uninitialise() -> vkCommandBuffer_array is free. \n");
	}

	// 14 : step 03: in uninialise destroy command pool using vkDestroyCommandPool()
	if (vkCommandPool)
	{
		/*void vkDestroyCommandPool(
			VkDevice                                    device,
			VkCommandPool                               commandPool,
			const VkAllocationCallbacks * pAllocator);*/

		vkDestroyCommandPool(vkDevice, vkCommandPool, NULL);
		vkCommandPool = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyCommandPool() is done. \n");

	}

	// ---------------------
	// Depth related
	if (vkImageView_depth) // depth image
	{
		vkDestroyImageView(vkDevice, vkImageView_depth, NULL);
		vkImageView_depth = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyCommandPool() is done for depth : vkImageView_depth. \n");
	}

	if (vkDeviceMemory_depth) // destroy depth memory
	{
		vkFreeMemory(vkDevice, vkDeviceMemory_depth, NULL);
		vkDeviceMemory_depth = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkFreeMemory() is done for depth : vkDeviceMemory_depth. \n");
	}

	if (vkImage_depth)  //destroy depth image
	{
		vkDestroyImage(vkDevice, vkImage_depth, NULL);
		vkImage_depth = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyImage() is done for depth : vkImage_depth. \n");
	}
	//--------------------------------------------------------
	// 
	//13 :  09 destroy image views
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		/*void vkDestroyImageView(
								VkDevice                                    device,
								VkImageView                                 imageView,
								const VkAllocationCallbacks*                pAllocator);*/

		vkDestroyImageView(vkDevice, swapchainimageView_array[i], NULL);
	}

	fprintf(gpFile, "uninitialise() -> vkDestroyImageView() is done. \n");
	
	//13:10 free 
	if (swapchainimageView_array)
	{
		free(swapchainimageView_array);
		swapchainimageView_array = NULL;
		
	}
	fprintf(gpFile, "uninitialise() -> swapchainimageView_array is free. \n");

	////13: 07 free swapchain image
	/*for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		vkDestroyImage(vkDevice, swapchainImage_array[i], NULL);
		fprintf(gpFile, "uninitialise() -> vkDestroyImage() is done. \n");
		fclose(gpFile);
	}*/

	//13: 08 
	if (swapchainImage_array)
	{
		free(swapchainImage_array);
		swapchainImage_array = NULL;
		fprintf(gpFile, "uninitialise() -> swapchainImage_array is free. \n");
	}

	//========================================================================================================
	if (vkSwapchainKHR)
	{
		vkDestroySwapchainKHR(vkDevice, vkSwapchainKHR, NULL);
		vkSwapchainKHR = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroySwapchainKHR() is done. \n");
	}

	if (vkDevice)
	{
		vkDestroyDevice(vkDevice, NULL);
		vkDevice = VK_NULL_HANDLE;

	}

	//PhysicalDevice Step 12 : There is no need to free / unitialise / destroy the selected physical device.why ? becouse later we will create vulkan
	//						   logical device which need to be destroy and its destruction will automatically destroy selected device
	
	//PresentationSurface Step 5 : Destroy VkInstance in uninitialise function.
	if (vkSurfaceKHR)
	{
		vkDestroySurfaceKHR(vkInstance, vkSurfaceKHR, NULL); // vkDestroySurfaceKHR is generic not platform specific
		vkSurfaceKHR = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroySurfaceKHR() succeeded\n");
	}

	if (vkDebugReportCallbackEXT && vkDestroyDebugReportCallbackEXT_fnptr)
	{
		vkDestroyDebugReportCallbackEXT_fnptr(vkInstance, vkDebugReportCallbackEXT, NULL);
		vkDebugReportCallbackEXT				= VK_NULL_HANDLE;
		vkDestroyDebugReportCallbackEXT_fnptr   = NULL;
		fprintf(gpFile, "uninitialise() -> vkDebugReportCallbackEXT && vkDestroyDebugReportCallbackEXT_fnptr succeeded\n");
	}

	//InstanceCreation : Step 5: Destroy VkInstance in uninitialise function.
	if (vkInstance)
	{
		vkDestroyInstance(vkInstance, NULL);
		vkInstance = VK_NULL_HANDLE;
		fprintf(gpFile, "uninitialise() -> vkDestroyInstance() succeeded\n");
	}

	// close the log file
	if (gpFile)
	{
		fprintf(gpFile, "uninitialise() -> Program Ended Successfully........ \n");
		fclose(gpFile);
		gpFile = NULL;
	}
}

//=================================================================================================================
// defination of Vulkan related functions

VkResult createVulkanInstance(void)
{
	// function declarations
	VkResult fillInstanceExtenstionsNames(void);
	VkResult fillValidationLayerNames(void);
	VkResult createValidationCallbackFunction(void);

	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code

	//Step 1: InstanceCreation : As explain before fill and initialise required extenstion names and count in global variable (varcha 8stpes)
	vkResult = fillInstanceExtenstionsNames();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVulkanInstance() -> fillInstanceExtenstionsNames() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVulkanInstance() -> fillInstanceExtenstionsNames() is successed. \n");
	}

	// fillValidationLayerNames
	if (bValidation == TRUE)
	{
		vkResult = fillValidationLayerNames();
		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "createVulkanInstance() -> fillValidationLayerNames() is failed. \n");
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "createVulkanInstance() -> fillValidationLayerNames() is successed. \n");
		}
	}


	//Step 2: InstanceCreation : Initialise struct VkApplicationInfo
	VkApplicationInfo vkApplicationInfo;
	memset((void*) &vkApplicationInfo, 0, sizeof(VkApplicationInfo));

	vkApplicationInfo.sType				 = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkApplicationInfo.pNext				 = NULL;
	vkApplicationInfo.pApplicationName   = gpszAppName;
	vkApplicationInfo.applicationVersion = 1;
	vkApplicationInfo.pEngineName		 = gpszAppName;
	vkApplicationInfo.engineVersion		 = 1;
	vkApplicationInfo.apiVersion		 = VK_API_VERSION_1_3; // version update kela vulkan ch ki hita 1_4 change karne

	//Step 3: InstanceCreation : Initialise struct VkInstanceCreateInfo by using information from step 1 and 2
	VkInstanceCreateInfo vkInstanceCreateInfo;
	memset((void*)&vkInstanceCreateInfo, 0, sizeof(VkInstanceCreateInfo));

	vkInstanceCreateInfo.sType					 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pApplicationInfo		 = &vkApplicationInfo;
	vkInstanceCreateInfo.enabledExtensionCount   = enabledInstanceExtenstionCount;
	vkInstanceCreateInfo.ppEnabledExtensionNames = enabledInstanceExtenstionNames_array;

	if (bValidation == TRUE)
	{
		vkInstanceCreateInfo.enabledLayerCount   = enabledValidationLayerCount;
		vkInstanceCreateInfo.ppEnabledLayerNames = enabledValidationLayerName_array;
	}
	else
	{
		vkInstanceCreateInfo.enabledLayerCount	 = 0;
		vkInstanceCreateInfo.ppEnabledLayerNames = NULL;
	}

	//Step 4: InstanceCreation : Call vkCreateInstance() to get 'Vk' instance in global variable and do error checking.
	vkResult = vkCreateInstance(&vkInstanceCreateInfo, NULL, &vkInstance);
	// 2nd para,eter : custom mempry allocator nahi tuza vapar.
	
	if (vkResult == VK_ERROR_INCOMPATIBLE_DRIVER)
	{
		fprintf(gpFile, "createVulkanInstance() -> vkCreateInstance() is failed due to incompatible driver. (%d) \n", vkResult);
		return (vkResult);
	}
	else if (vkResult == VK_ERROR_EXTENSION_NOT_PRESENT) 
	{
		fprintf(gpFile, "createVulkanInstance() -> vkCreateInstance() is failed due to extension not present. (%d) \n", vkResult);
		return (vkResult);
	}
	else if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVulkanInstance() -> vkCreateInstance() is failed due to unknown reason.(%d) \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVulkanInstance() -> vkCreateInstance() is successed. \n");
	}

	// do for validation call back
	
	if (bValidation == TRUE)
	{
		vkResult = createValidationCallbackFunction();
		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "createVulkanInstance() -> createValidationCallbackFunction() is failed. \n");
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "createVulkanInstance() -> createValidationCallbackFunction() is successed. \n");
		}
	}


	return (vkResult);
}

VkResult fillInstanceExtenstionsNames(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Step 1 : Find how many instanceextenstion are supported by the vulkan driver of/for this version.
	//          and keep it in local varibale.

	uint32_t instanceExtenstionCount = 0; // sarv extenstion cha count
	
	// first vulkan api
	vkResult = vkEnumerateInstanceExtensionProperties(NULL, &instanceExtenstionCount, NULL);
	// 1st : kontya layer chi extenston havi. Layer ch naav. Pn Aplya purn driver chi extenstion havi. so NULL
	// 2nd : jatna rikama yetana count gheun yenar
	// 3rd : VkExtensionProperties cha array. Pn aplya kade count ch nahi. Mg array chi size kadhi tharvnar? 
	//		 to count aplya 2nd para madhun bhetato. ya call nanatr instanceExtenstionCount madhe count yeil. 
	//		 mg prt call karycha ya ch function la. 

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> 1st call to vkEnumerateInstanceExtensionProperties() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> 1st call to vkEnumerateInstanceExtensionProperties() is successed. \n");
	}

	// step 2: Allocate and fill struct VKExtenstionProperties array corresponding to above count.
	VkExtensionProperties* vkExtensionProperties_array = NULL;

	// allocate
	vkExtensionProperties_array = (VkExtensionProperties *) malloc(instanceExtenstionCount * sizeof(VkExtensionProperties));
	
	// do error checking for malloc. Assignment madhe thik ahe. Pn real project he error checking must ahe.

	vkResult = vkEnumerateInstanceExtensionProperties(NULL, &instanceExtenstionCount, vkExtensionProperties_array);
	// hita 2nd parameter aapn NULL pathau shakto. karn varti count ghetala ahe. Pn tri send kara.

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> 2nd call to vkEnumerateInstanceExtensionProperties() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> 2nd call to vkEnumerateInstanceExtensionProperties() is successed. \n");
	}

	// Step 3: Fill and display the local string array of extenstions names obtained by VKExtenstionProperties struct array

	char** instanceExtenstionName_array = NULL;

	// string chya array la size
	instanceExtenstionName_array = (char**)malloc(sizeof(char*) * instanceExtenstionCount);

	// malloc error checking do later

	// aata string chya array madhil pratek string la size dene

	for (uint32_t i = 0; i < instanceExtenstionCount; i++)
	{
		// allocate memory for each string from string array
		instanceExtenstionName_array[i] = (char*)malloc(sizeof(char) * 
			strlen(vkExtensionProperties_array[i].extensionName) + 1 );

		//aplya array madhe ya struct madhla naav copy karn
		memcpy(instanceExtenstionName_array[i], vkExtensionProperties_array[i].extensionName, strlen(vkExtensionProperties_array[i].extensionName) + 1);

		fprintf(gpFile, "fillInstanceExtenstionsNames() -> Vulkan Extension Name = %s\n", instanceExtenstionName_array[i]);
	}

	// Step 4: As not required here onword free the VKExtenstionProperties array
	free(vkExtensionProperties_array);
	vkExtensionProperties_array = NULL;

	// Step 5: Find whether above extenstion names contain our required 2 extenstions.
	VkBool32 vulkanSurfaceExtenstionFound = VK_FALSE; //VkBool32 : vulkan cha bool
	VkBool32 win32SurfaceExtenstionFound  = VK_FALSE; 
	VkBool32 debugReportExtenstionFound   = VK_FALSE; 

	for (uint32_t i = 0; i < instanceExtenstionCount; i++)
	{
		if (strcmp(instanceExtenstionName_array[i], VK_KHR_SURFACE_EXTENSION_NAME) == 0 )
		{
			vulkanSurfaceExtenstionFound = VK_TRUE;
			enabledInstanceExtenstionNames_array[enabledInstanceExtenstionCount++] = VK_KHR_SURFACE_EXTENSION_NAME;
			// ya varcha line ne global array bhartoy tyach veli to increment kartoy. index aivaji to count cha var
			// use krt ahe
		}

		if (strcmp(instanceExtenstionName_array[i], VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0)
		{
			win32SurfaceExtenstionFound = VK_TRUE;
			enabledInstanceExtenstionNames_array[enabledInstanceExtenstionCount++] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
		}

		if (strcmp(instanceExtenstionName_array[i], VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0)
		{
			debugReportExtenstionFound = VK_TRUE;

			if (bValidation == TRUE)
			{
				enabledInstanceExtenstionNames_array[enabledInstanceExtenstionCount++] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
			}
			else
			{
				// array will not have entry of VK_EXT_DEBUG_REPORT_EXTENSTION_NAME
			}
		}
	}

	// Steps 6: As not needed here forth hence free the local string array.

	for (uint32_t i = 0; i < instanceExtenstionCount; i++)
	{
		free(instanceExtenstionName_array[i]);
	}

	free(instanceExtenstionName_array);
	instanceExtenstionName_array = NULL;

	// Step 7: Print whether our vulkan dirver supports our required name or not
	if (vulkanSurfaceExtenstionFound == VK_FALSE)
	{
		//Concept : 
		vkResult = VK_ERROR_INITIALIZATION_FAILED; // return hardcoded failure
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> VK_KHR_SURFACE_EXTENSION_NAME not found \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> VK_KHR_SURFACE_EXTENSION_NAME found \n");
	}

	if (win32SurfaceExtenstionFound == VK_FALSE)
	{
		//Concept : 
		vkResult = VK_ERROR_INITIALIZATION_FAILED; // return hardcoded failure
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> VK_KHR_WIN32_SURFACE_EXTENSION_NAME not found \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> VK_KHR_WIN32_SURFACE_EXTENSION_NAME found \n");
	}

	if (debugReportExtenstionFound == VK_FALSE)
	{
		if (bValidation == TRUE)
		{
			vkResult = VK_ERROR_INITIALIZATION_FAILED; // return hardcoded failure
			fprintf(gpFile, "fillInstanceExtenstionsNames() -> validation is on but required VK_EXT_DEBUG_REPORT_EXTENSTION_NAME is not supported \n");
			return (vkResult);
		}
		else
		{
			//bValidation is false
			fprintf(gpFile, "fillInstanceExtenstionsNames() -> validation is off and VK_EXT_DEBUG_REPORT_EXTENSTION_NAME is not supported \n");
		}
	}
	else
	{
		if (bValidation == TRUE)
		{
			fprintf(gpFile, "fillInstanceExtenstionsNames() -> validation is on and required VK_EXT_DEBUG_REPORT_EXTENSTION_NAME is also supported \n");			
		}
		else
		{
			//bValidation is false
			fprintf(gpFile, "fillInstanceExtenstionsNames() -> validation is off and VK_EXT_DEBUG_REPORT_EXTENSTION_NAME is supported \n");
		}
	}


	// Step 8: Print only supported extenstions names.
	for (uint32_t i = 0; i < enabledInstanceExtenstionCount; i++)
	{
		fprintf(gpFile, "fillInstanceExtenstionsNames() -> Enabaled vulkan instance extenstion name : %s \n", enabledInstanceExtenstionNames_array[i]);
	}

	return vkResult;
}

VkResult fillValidationLayerNames(void)
{
	VkResult vkResult = VK_SUCCESS;
	uint32_t validationLayerCount = 0;

	fprintf(gpFile, "\n======================================================================\n");

	vkResult = vkEnumerateInstanceLayerProperties(&validationLayerCount, NULL);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "fillValidationLayerNames() -> 1st call to vkEnumerateInstanceLayerProperties() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillValidationLayerNames() -> 1st call to vkEnumerateInstanceLayerProperties() is successed. \n");
	}

	VkLayerProperties* vkLayerProperties_array = NULL;
	vkLayerProperties_array = (VkLayerProperties*)malloc(validationLayerCount * sizeof(VkLayerProperties));

	vkResult = vkEnumerateInstanceLayerProperties(&validationLayerCount, vkLayerProperties_array);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "fillValidationLayerNames() -> 2nd call to vkEnumerateInstanceLayerProperties() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillValidationLayerNames() -> 2nd call to vkEnumerateInstanceLayerProperties() is successed. \n");
	}

	char** validationLayerName_array = NULL;
	validationLayerName_array = (char**)malloc(sizeof(char*) * validationLayerCount);

	for (uint32_t i = 0; i < validationLayerCount; i++)
	{
		validationLayerName_array[i] = (char*)malloc(sizeof(char) *
			strlen(vkLayerProperties_array[i].layerName) + 1);

		//aplya array madhe ya struct madhla naav copy karn
		memcpy(validationLayerName_array[i], vkLayerProperties_array[i].layerName, 
				strlen(vkLayerProperties_array[i].layerName) + 1);

		fprintf(gpFile, "fillValidationLayerNames() -> Vulkan validation layer Name = %s\n", 
				validationLayerName_array[i]);
	}

	free(vkLayerProperties_array);
	vkLayerProperties_array = NULL;

	//for required one validation layer
	VkBool32 validationLayerFound = VK_FALSE; 
	
	for (uint32_t i = 0; i < validationLayerCount; i++)
	{
		if (strcmp(validationLayerName_array[i], "VK_LAYER_KHRONOS_validation") == 0)
		{
			validationLayerFound = VK_TRUE;
			enabledValidationLayerName_array[enabledValidationLayerCount++] = "VK_LAYER_KHRONOS_validation";
		}
	}


	for (uint32_t i = 0; i < validationLayerCount; i++)
	{
		free(validationLayerName_array[i]);
	}

	free(validationLayerName_array);
	validationLayerName_array = NULL;

	if (validationLayerFound == VK_FALSE)
	{
		//Concept : 
		vkResult = VK_ERROR_INITIALIZATION_FAILED; // return hardcoded failure
		fprintf(gpFile, "fillValidationLayerNames() -> VK_LAYER_KHRONOS_validation not supported \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillValidationLayerNames() -> VK_LAYER_KHRONOS_validation supported \n");
	}

	// Step 8: Print only supported extenstions names.
	for (uint32_t i = 0; i < enabledValidationLayerCount; i++)
	{
		fprintf(gpFile, "fillValidationLayerNames() -> Enabaled vulkan validation layer name : %s \n", enabledValidationLayerName_array[i]);
	}


	fprintf(gpFile, "===========================================================\n\n");
	return vkResult;
}

VkResult createValidationCallbackFunction(void)
{
	fprintf(gpFile, "===========================================================\n\n");

	// function declarations
	VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallBack(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, 
													   uint64_t, size_t, int32_t, const char *, const char *, void*);
	
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT_fnptr = NULL;

	VkResult vkResult = VK_SUCCESS;

	// get the required function pointers
	vkCreateDebugReportCallbackEXT_fnptr = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vkInstance, "vkCreateDebugReportCallbackEXT");

	if (vkCreateDebugReportCallbackEXT_fnptr == NULL)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED; // return hardcoded failure
		fprintf(gpFile, "createValidationCallbackFunction() -> vkGetInstanceProcAddr failed to get the function pointer for vkCreateDebugReportCallbackEXT_fnptr\n");
		return (vkResult);
	}
	else
	{
		//bValidation is false
		fprintf(gpFile, "createValidationCallbackFunction() -> vkGetInstanceProcAddr successful for vkCreateDebugReportCallbackEXT_fnptr\n");
	}


	vkDestroyDebugReportCallbackEXT_fnptr = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugReportCallbackEXT");

	if (vkDestroyDebugReportCallbackEXT_fnptr == NULL)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED; // return hardcoded failure
		fprintf(gpFile, "createValidationCallbackFunction() -> vkGetInstanceProcAddr failed to get the function pointer for vkDestroyDebugReportCallbackEXT_fnptr\n");
		return (vkResult);
	}
	else
	{
		//bValidation is false
		fprintf(gpFile, "createValidationCallbackFunction() -> vkGetInstanceProcAddr successful for vkDestroyDebugReportCallbackEXT_fnptr\n");
	}

	// get the vulkan debug report call back object
	/*typedef struct VkDebugReportCallbackCreateInfoEXT {
		VkStructureType                 sType;
		const void* pNext;
		VkDebugReportFlagsEXT           flags;
		PFN_vkDebugReportCallbackEXT    pfnCallback;
		void* pUserData;
	} VkDebugReportCallbackCreateInfoEXT;*/

	VkDebugReportCallbackCreateInfoEXT vkDebugReportCallbackCreateInfoEXT;

	memset((void*)&vkDebugReportCallbackCreateInfoEXT, 0, sizeof(VkDebugReportCallbackCreateInfoEXT));

	vkDebugReportCallbackCreateInfoEXT.sType		 = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	vkDebugReportCallbackCreateInfoEXT.pNext		 = NULL;
	vkDebugReportCallbackCreateInfoEXT.flags		 = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	vkDebugReportCallbackCreateInfoEXT.pfnCallback	 = debugReportCallBack;
	vkDebugReportCallbackCreateInfoEXT.pUserData	 = NULL;

	vkResult = vkCreateDebugReportCallbackEXT_fnptr(vkInstance, &vkDebugReportCallbackCreateInfoEXT, NULL, &vkDebugReportCallbackEXT);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createValidationCallbackFunction() -> vkCreateDebugReportCallbackEXT_fnptr() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createValidationCallbackFunction() -> vkCreateDebugReportCallbackEXT_fnptr() is successed. \n");
	}

	fprintf(gpFile, "=========================================================================================\n\n");
	return vkResult;
}

VkResult getSupportedSurface(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	//PresentationSurface Step 2. Declare and memset platform specific (windows, linux, android etc) SurfaceCreateInfoStructure
	VkWin32SurfaceCreateInfoKHR vkWin32SurfaceCreateInfoKHR;
	memset((void*)&vkWin32SurfaceCreateInfoKHR, 0, sizeof(VkWin32SurfaceCreateInfoKHR));

	//PresentationSurface Step 3. Initialise it.Particularly its hInstance and hwnd member
	vkWin32SurfaceCreateInfoKHR.sType		 = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	vkWin32SurfaceCreateInfoKHR.pNext		 = NULL;
	vkWin32SurfaceCreateInfoKHR.flags		 = 0;
	vkWin32SurfaceCreateInfoKHR.hinstance	 = (HINSTANCE) GetWindowLongPtr(ghwnd, GWLP_HINSTANCE);
	vkWin32SurfaceCreateInfoKHR.hwnd		 = ghwnd;

	//PresentationSurface Step 4. Now call(function / api) VkCreateWin32SurfaceKHR() to create the presentation surface object
	/*VkResult vkCreateWin32SurfaceKHR(
		VkInstance                                  instance,
		const VkWin32SurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface);*/

	vkResult = vkCreateWin32SurfaceKHR(vkInstance, &vkWin32SurfaceCreateInfoKHR, NULL, &vkSurfaceKHR);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getSupportedSurface() -> vkCreateWin32SurfaceKHR() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "getSupportedSurface() -> vkCreateWin32SurfaceKHR() is successed. \n");
	}

	return (vkResult);
}

VkResult getPhysicalDevice(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	fprintf(gpFile, "\n\ngetPhysicalDevice");
	// remove globally because of PrintVKInfo Step 01 :
	/*
	VkPhysicalDevice* vkPhysicalDevice_array = NULL; // for step 3rd
	uint32_t physicalDeviceCount = 0;
	*/

	//PhysicalDevice Step 2 : Call vkEnumeratePhysicalDevices()  to get physical devices count
	/*VkResult vkEnumeratePhysicalDevices(
		VkInstance instance,
		uint32_t * pPhysicalDeviceCount,
		VkPhysicalDevice * pPhysicalDevices);*/

	
	vkResult = vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, NULL);
	// 3rd parameter : aata array nhi so NULL. pudhe yenar ahe
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDevice() -> 1st call to vkEnumeratePhysicalDevices() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else if (physicalDeviceCount == 0)
	{
		fprintf(gpFile, "getPhysicalDevice() -> 1st call to vkEnumeratePhysicalDevices() is resulted in 0 physical device count \n");
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevice() -> 1st call to vkEnumeratePhysicalDevices() is successed. \n");
	}


	//PhysicalDevice Step 3 : allocate VkPhysicalDevice array according to above count
	vkPhysicalDevice_array = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);
	// for the sake of bravity error checking krt nhi pn production code madh kara

	//PhysicalDevice Step 4 : call vkEnumeratePhysicalDevices() again to fill above array
	vkResult = vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, vkPhysicalDevice_array);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDevice() -> 2nd call to vkEnumeratePhysicalDevices() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevice() -> 2nd call to vkEnumeratePhysicalDevices() is successed. \n");
	}


	//PhysicalDevice Step 5 : start a loop using physicalDevice countand physical device array
	//					  	  Declare a boolean bFound variable before this loop.
	//						  It will decide whether we found the desired physical device or not



	VkBool32 bFound = VK_FALSE;

	for (uint32_t i = 0; i < physicalDeviceCount; i++)
	{
		//5.a : declare a local variable to hold queue count
		uint32_t queueCount = UINT32_MAX;

		//5.b : call VkGetPhysicalDeviceQueueFamilyProperities() to initialise above queueCount variable
		/*void vkGetPhysicalDeviceQueueFamilyProperties(
			VkPhysicalDevice                            physicalDevice,
			uint32_t * pQueueFamilyPropertyCount,
			VkQueueFamilyProperties * pQueueFamilyProperties);*/

		vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_array[i], &queueCount, NULL);


		//5.c : allocate VkQueueFamilyProperties array variable according to above count

		VkQueueFamilyProperties* vkQueueFamilyProperties_array = NULL;
		vkQueueFamilyProperties_array = (VkQueueFamilyProperties*) malloc(sizeof(VkQueueFamilyProperties) * queueCount);
		// error checking krt nhi itha pn production code madhe kara

		//5.d : call VkGetPhysicalDeviceQueueFamilyProperities() again to fill the above array
		vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_array[i], &queueCount, vkQueueFamilyProperties_array);


		//5.e : declare VkBool32 type array and allocate it using the same above queue count
		VkBool32 *isQueueSurfaceSupported_array = NULL;
		isQueueSurfaceSupported_array = (VkBool32*) malloc (sizeof(VkBool32) * queueCount);
		// error checking krt nhi itha pn production code madhe kara

		//5.f : start a nested loop and fill (jatana rikam  yetana bharun)above VKkBool32 array by calling vkGetPhysicalDeviceSurfaceSupportKHR

		for (uint32_t j = 0; j < queueCount; j++)
		{
			/*VkResult vkGetPhysicalDeviceSurfaceSupportKHR(
				VkPhysicalDevice                            physicalDevice,
				uint32_t                                    queueFamilyIndex,
				VkSurfaceKHR                                surface,
				VkBool32 * pSupported);*/
			vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice_array[i], j, vkSurfaceKHR, &isQueueSurfaceSupported_array[j]);
		}

		/*5.g : Not in above loop but in main loop and check wether physical device in its array with its queue family
			has graphics bit or not.If "yes" the this is selected physical device assign it global variable.
			Similary this index the selected queue family index and assign it global variable too. and set bFound to true
			and break from second nested loop.*/

		for (uint32_t j = 0; j < queueCount; j++)
		{
			if (vkQueueFamilyProperties_array[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				if (isQueueSurfaceSupported_array[j] == VK_TRUE)
				{
					vkPhysicalDevice_selected = vkPhysicalDevice_array[i];
					graphicsQueueFamilyIndex_selected = j;
					bFound = VK_TRUE;
					break;
				}
			}
		}

		if (isQueueSurfaceSupported_array)
		{
			free(isQueueSurfaceSupported_array);
			isQueueSurfaceSupported_array = NULL;
			fprintf(gpFile, "getPhysicalDevice() -> free(isQueueSurfaceSupported_array); is successed. \n");
		}

		if (vkQueueFamilyProperties_array)
		{
			free(vkQueueFamilyProperties_array);
			vkQueueFamilyProperties_array = NULL;
			fprintf(gpFile, "getPhysicalDevice() -> free(vkQueueFamilyProperties_array);; is successed. \n");
		}

		//5.i : still being in main loop according to bFound variable break out from main loop
		if (bFound == VK_TRUE)
			break;
	}

	//5.j : Free physical device array
	if (bFound == VK_TRUE)
	{
		fprintf(gpFile, "getPhysicalDevice() -> is successed to select required device with graphics enabled \n");
		
	}
	else
	{
		//PhysicalDevice Step 6 : do error checking according to bFound
		fprintf(gpFile, "getPhysicalDevice() -> failed to obtain get graphical supported physical device \n");
		if (vkPhysicalDevice_array)
		{
			free(vkPhysicalDevice_array);
			vkPhysicalDevice_array = NULL;
			fprintf(gpFile, "getPhysicalDevice() -> vkPhysicalDevice_array free done in else.\n");
		}
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}

	/*typedef struct VkPhysicalDeviceMemoryProperties {
		uint32_t        memoryTypeCount;
		VkMemoryType    memoryTypes[VK_MAX_MEMORY_TYPES];
		uint32_t        memoryHeapCount;
		VkMemoryHeap    memoryHeaps[VK_MAX_MEMORY_HEAPS];
	} VkPhysicalDeviceMemoryProperties;*/
	//PhysicalDevice Step 7 : memset the global 3rd varibale physical device's memory property structure and initialise it with
	memset((void*) &vkPhysicalDeviceMemoryProperties, 0, sizeof(VkPhysicalDeviceMemoryProperties));

	//PhysicalDevice Step 8 : initialise above structure above it with vkGetPhysicalDeviceMemoryProperties()
	/*void vkGetPhysicalDeviceMemoryProperties(
		VkPhysicalDevice                            physicalDevice,
		VkPhysicalDeviceMemoryProperties* pMemoryProperties);*/

	vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice_selected, &vkPhysicalDeviceMemoryProperties);

	/*PhysicalDevice Step 9 : declare a local structure variable VkPhysicalDeviceFeatures, memset itand initialise it with by calling
							 vkPhysicalDeviceFeatures()*/

	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
	memset((void*) &vkPhysicalDeviceFeatures, 0, sizeof(VkPhysicalDeviceFeatures));

	vkGetPhysicalDeviceFeatures(vkPhysicalDevice_selected, &vkPhysicalDeviceFeatures);

	//PhysicalDevice Step 10 : by using "TesselationShader member" of above struture check, selected device's Tesselation shader support
	if (vkPhysicalDeviceFeatures.tessellationShader)
	{
		fprintf(gpFile, "getPhysicalDevice() -> selected physical device support tessellationShader.\n");
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevice() -> selected physical device not support tessellationShader.\n");
	}

	//PhysicalDevice Step 11 : by using "GeometryShader member" of above struture check, selected device's GeometryShader support
	if (vkPhysicalDeviceFeatures.geometryShader)
	{
		fprintf(gpFile, "getPhysicalDevice() -> selected physical device support geometryShader.\n");
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevice() -> selected physical device not support geometryShader.\n");
	}

	fprintf(gpFile, "\n\n");
	return (vkResult);
}

VkResult printVkInfo(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	
	fprintf(gpFile, "\n");
	fprintf(gpFile, "================== Print Vulkan Info ========================\n");
	
	//PrintVKInfo Step 03 : Write printVKInfo() user define function with following steps.

	for (uint32_t i = 0; i < physicalDeviceCount; i++)
	{
	    //PrintVKInfo Step 03 A.Start a loop using global physicalDeviceCountand inside it declareand memset VkPhysicalDeviceProperty struct variable
		VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
		memset((void*)&vkPhysicalDeviceProperties, 0, sizeof(VkPhysicalDeviceProperties));

		//PrintVKInfo Step 03 B. Initialise this struct variable by calling vkGetPhysicalDeviceProperites() vulkcan api
		/*void vkGetPhysicalDeviceProperties(
			VkPhysicalDevice                            physicalDevice,
			VkPhysicalDeviceProperties * pProperties);*/
		vkGetPhysicalDeviceProperties(vkPhysicalDevice_array[i], &vkPhysicalDeviceProperties);

		//PrintVKInfo Step 03 C. Print vulkan api version using "apiVersionMember" of above struct this required 3 vulkan macros.

		uint32_t majorVersion = VK_API_VERSION_MAJOR(vkPhysicalDeviceProperties.apiVersion);
		uint32_t minorVersion = VK_API_VERSION_MINOR(vkPhysicalDeviceProperties.apiVersion);
		uint32_t patchVersion = VK_API_VERSION_PATCH(vkPhysicalDeviceProperties.apiVersion);

		fprintf(gpFile, "Api Version : %d.%d.%d \n", majorVersion, minorVersion, patchVersion);

		//PrintVKInfo Step 03 D. Print device name by using "deviceName" of above struct.
		fprintf(gpFile, "Device Name : %s \n", vkPhysicalDeviceProperties.deviceName);

		//E. Use "deviceType" member of above struct in switch case block and accrodingly print device type
		
		switch (vkPhysicalDeviceProperties.deviceType)
		{
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				fprintf(gpFile, "Device Type : INTEGRATED_GPU (iGPU) \n");
				break;

			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: 
				fprintf(gpFile, "Device Type : Discrte Gpu (dGPU) \n");
				break;

			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				fprintf(gpFile, "Device Type : virtual Gpu \n");
				break;

			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				fprintf(gpFile, "Device Type : CPU \n");
				break;

			case VK_PHYSICAL_DEVICE_TYPE_OTHER :
				fprintf(gpFile, "Device Type : Other \n");
				break;

			default: fprintf(gpFile, "Device Type : UNKNOWN \n");
		}

		//F. Print hexadeciaml vendor id of device using "vendorID" member of above struct.
		fprintf(gpFile, "vendorID : 0x%04x\n", vkPhysicalDeviceProperties.vendorID);
		
		//G.Print hexadeciaml device ID using "deviceID" member of above struct.
		fprintf(gpFile, "deviceID : 0x%04x\n", vkPhysicalDeviceProperties.deviceID);
	}


	//H. Free physicalDeviceArray which we removed from "if (bFound == VK_TRUE)" of getPhysicalDevice()
	if (vkPhysicalDevice_array)
	{
		free(vkPhysicalDevice_array);
		vkPhysicalDevice_array = NULL;
		fprintf(gpFile, "printVKInfo() -> vkPhysicalDevice_array free done \n");
	}

	fprintf(gpFile, "=============================================================\n\n");
	return (vkResult);
}

VkResult fillDeviceExtenstionsNames(void)
{
	fprintf(gpFile, "\n\n");

	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Step 1 : Find how many device Extenstion are supported by the vulkan driver of/for this version.
	//          and keep it in local varibale.

	uint32_t deviceExtenstionCount = 0; // sarv device extenstion cha count

	/*VkResult vkEnumerateDeviceExtensionProperties(
		VkPhysicalDevice                            physicalDevice,
		const char* pLayerName,
		uint32_t * pPropertyCount,
		VkExtensionProperties * pProperties);*/

	vkResult = vkEnumerateDeviceExtensionProperties(vkPhysicalDevice_selected, 
													NULL, // all layer mhunu null
													&deviceExtenstionCount, // jatama rikama
													NULL); // 4th null array mhnun
	

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "fillDeviceExtenstionsNames() -> 1st call to vkEnumerateDeviceExtensionProperties() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillDeviceExtenstionsNames() -> deviceExtenstionCount : %d \n", deviceExtenstionCount);
		fprintf(gpFile, "fillDeviceExtenstionsNames() -> 1st call to vkEnumerateDeviceExtensionProperties() is successed. \n");
	}

	
	// step 2: Allocate and fill struct VKExtenstionProperties array corresponding to above count.
	VkExtensionProperties* vkExtensionProperties_array = NULL;

	// allocate
	vkExtensionProperties_array = (VkExtensionProperties*)malloc(deviceExtenstionCount * sizeof(VkExtensionProperties));

	// do error checking for malloc. Assignment madhe thik ahe. Pn real project he error checking must ahe.

	vkResult = vkEnumerateDeviceExtensionProperties(vkPhysicalDevice_selected,
													NULL, // all layer mhunu null
													&deviceExtenstionCount, // jatama rikama
													vkExtensionProperties_array); // 4th null array mhnun

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "fillDeviceExtenstionsNames() -> 2nd call to vkEnumerateDeviceExtensionProperties() is failed. \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillDeviceExtenstionsNames() -> 2nd call to vkEnumerateDeviceExtensionProperties() is successed. \n");
	}

	// Step 3: Fill and display the local string array of extenstions names obtained by VKExtenstionProperties struct array

	char** deviceExtenstionName_array = NULL;

	// string chya array la size
	deviceExtenstionName_array = (char**)malloc(sizeof(char*) * deviceExtenstionCount);

	for (uint32_t i = 0; i < deviceExtenstionCount; i++)
	{
		// allocate memory for each string from string array
		deviceExtenstionName_array[i] = (char*)malloc(sizeof(char) *
			strlen(vkExtensionProperties_array[i].extensionName) + 1);

		//aplya array madhe ya struct madhla naav copy karn
		memcpy(deviceExtenstionName_array[i], vkExtensionProperties_array[i].extensionName, strlen(vkExtensionProperties_array[i].extensionName) + 1);

		fprintf(gpFile, "fillDeviceExtenstionsNames() -> Vulkan Device Extension Name = %s\n", deviceExtenstionName_array[i]);
	}

	// Step 4: As not required here onword free the VKExtenstionProperties array
	free(vkExtensionProperties_array);
	vkExtensionProperties_array = NULL;

	// Step 5: Find whether above extenstion names contain our required 2 extenstions.
	VkBool32 vulkanSwapChainExtenstionFound = VK_FALSE; //VkBool32 : vulkan cha bool
	
	for (uint32_t i = 0; i < deviceExtenstionCount; i++)
	{
		if (strcmp(deviceExtenstionName_array[i], VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			vulkanSwapChainExtenstionFound = VK_TRUE;
			enabledDeviceExtenstionNames_array[enabledDeviceExtenstionCount++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		}
	}

	// Steps 6: As not needed here forth hence free the local string array.

	for (uint32_t i = 0; i < deviceExtenstionCount; i++)
	{
		free(deviceExtenstionName_array[i]);
	}

	free(deviceExtenstionName_array);
	deviceExtenstionName_array = NULL;

	// Step 7: Print whether our vulkan dirver supports our required name or not
	if (vulkanSwapChainExtenstionFound == VK_FALSE)
	{
		//Concept : 
		vkResult = VK_ERROR_INITIALIZATION_FAILED; // return hardcoded failure
		fprintf(gpFile, "fillDeviceExtenstionsNames() -> VK_KHR_SWAPCHAIN_EXTENSION_NAME not found \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillDeviceExtenstionsNames() -> VK_KHR_SWAPCHAIN_EXTENSION_NAME found \n");
	}

	fprintf(gpFile, "fillDeviceExtenstionsNames() ->enabledDeviceExtenstionCount :: %d \n", enabledDeviceExtenstionCount);
	// Step 8: Print only supported extenstions names.
	for (uint32_t i = 0; i < enabledDeviceExtenstionCount; i++)
	{
		fprintf(gpFile, "fillDeviceExtenstionsNames() -> Enabaled vulkan device extenstion name : %s \n", enabledDeviceExtenstionNames_array[i]);
		//fprintf(gpFile, "fillDeviceExtenstionsNames() -> Enabaled vulkan device extenstion name : %s \n", enabledInstanceExtenstionNames_array[i]);
	}

	fprintf(gpFile, "\n\n");
	return vkResult;
}

//VulkanDevice Step 01: Create a user define function say "createVulkanDevice".
VkResult createVulkanDevice(void)
{
	VkResult fillDeviceExtenstionsNames(void);

	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	//VulkanDevice Step 02: Call previously created fillDeviceExtenstionsNames() in it.

	// device extenstion
	vkResult = fillDeviceExtenstionsNames();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVulkanDevice() -> fillDeviceExtenstionsNames() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVulkanDevice() -> fillDeviceExtenstionsNames() is successed. \n");
	}

	//VulkanDevice Step 03: Declare and initialise VkDeviceCreateInfo structure.Use previosuly obtrained 
	//						device extenstion count and deviceExtenstion array to initialise this structure
	/*
	typedef struct VkDeviceCreateInfo {
		VkStructureType                    sType;
		const void* pNext;
		VkDeviceCreateFlags                flags;
		uint32_t                           queueCreateInfoCount;
		
		const VkDeviceQueueCreateInfo* pQueueCreateInfos;
		// enabledLayerCount is deprecated and should not be used
		uint32_t                           enabledLayerCount;
		// ppEnabledLayerNames is deprecated and should not be used
		
		const char* const* ppEnabledLayerNames;
		uint32_t                           enabledExtensionCount;
		const char* const* ppEnabledExtensionNames;
		const VkPhysicalDeviceFeatures* pEnabledFeatures;
	} VkDeviceCreateInfo; */

	//=================================================================================================
	// newly added code on 12th Feb lecture

	float queuePriorties[1];
	queuePriorties[0] = 1.0f;

	VkDeviceQueueCreateInfo vkDeviceQueueCreateInfo;
	memset((void*)&vkDeviceQueueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));

	vkDeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vkDeviceQueueCreateInfo.pNext = NULL;
	vkDeviceQueueCreateInfo.flags = 0;
	vkDeviceQueueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex_selected;
	vkDeviceQueueCreateInfo.queueCount = 1;
	vkDeviceQueueCreateInfo.pQueuePriorities = queuePriorties;

	//=================================================================================================

	VkDeviceCreateInfo vkDeviceCreateInfo; //VkDeviceCreateInfo
	memset((void*)&vkDeviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));

	vkDeviceCreateInfo.sType				   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vkDeviceCreateInfo.pNext				   = NULL;
	vkDeviceCreateInfo.flags				   = 0;
	vkDeviceCreateInfo.enabledExtensionCount   = enabledDeviceExtenstionCount;
	vkDeviceCreateInfo.ppEnabledExtensionNames = enabledDeviceExtenstionNames_array;
	vkDeviceCreateInfo.enabledLayerCount	   = 0;
	vkDeviceCreateInfo.ppEnabledLayerNames	   = NULL;
	vkDeviceCreateInfo.pEnabledFeatures		   = NULL;
	
	vkDeviceCreateInfo.queueCreateInfoCount	   = 1;
	vkDeviceCreateInfo.pQueueCreateInfos	   = &vkDeviceQueueCreateInfo;
	

	//VulkanDevice Step 04: Now call vkCreateDevice() vulkan api to create actual vulkan device and do error checking
	/*VkResult vkCreateDevice(
		VkPhysicalDevice                            physicalDevice,
		const VkDeviceCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDevice * pDevice);*/
	
	vkResult = vkCreateDevice(vkPhysicalDevice_selected, &vkDeviceCreateInfo, NULL, &vkDevice); //VK_NULL_HANDLE

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVulkanDevice() -> vkCreateDevice() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVulkanDevice() -> vkCreateDevice() is successed. \n");
	}

	return vkResult;
}

void getDeviceQueue(void)
{
	// Code
	/*void vkGetDeviceQueue(
		VkDevice                                    device,
		uint32_t                                    queueFamilyIndex,
		uint32_t                                    queueIndex,
		VkQueue * pQueue);*/

	fprintf(gpFile, "\n\n");
	fprintf(gpFile, "getDeviceQueue() -> graphicsQueueFamilyIndex_selected %d \n", graphicsQueueFamilyIndex_selected);
	vkGetDeviceQueue(vkDevice, graphicsQueueFamilyIndex_selected, 0, &vkQueue);
	
	if (vkQueue == VK_NULL_HANDLE)
	{
		fprintf(gpFile, "getDeviceQueue() -> vkGetDeviceQueue() is returned null \n");
	}
	else
		fprintf(gpFile, "getDeviceQueue() -> vkGetDeviceQueue() is successful \n");
	
	fprintf(gpFile, "\n\n");
}

VkResult getPhysicalDeviceSurfaceFormatAndColorSpace(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;
	uint32_t formatCount = 0;

	// Code
	//SurfaceFormatAndColorSpace Step 01: Call vkGetPhysicalDeviceSurfaceFormatsKHR() first to retrive the count of supported formats
	// get the count of supported surface color format

	/*VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(
		VkPhysicalDevice                            physicalDevice,
		VkSurfaceKHR                                surface,
		uint32_t * pSurfaceFormatCount,
		VkSurfaceFormatKHR * pSurfaceFormats);*/

	fprintf(gpFile, "=====================================================================================\n\n");

	vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice_selected, vkSurfaceKHR, &formatCount, NULL);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace() -> vkGetPhysicalDeviceSurfaceFormatsKHR() 1st call  is failed. %d \n", vkResult);
		return (vkResult);
	}
	else if (formatCount == 0)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace() -> vkGetPhysicalDeviceSurfaceFormatsKHR() 1st call is fromat count is zero \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace() -> vkGetPhysicalDeviceSurfaceFormatsKHR() 1st call is successed. \n");
	}

	//SurfaceFormatAndColorSpace Step 02: declare and allocate array of VkSurfaceFormatsKHR struture corresponding to above count. Tis has 2
	//									  vkFormatand vkColorSpaceKHR
	
	/*typedef struct VkSurfaceFormatKHR {
		VkFormat           format;
		VkColorSpaceKHR    colorSpace;
	} VkSurfaceFormatKHR;*/

	VkSurfaceFormatKHR *vkSurfaceFormatKHR_array = (VkSurfaceFormatKHR *) malloc(formatCount * sizeof(VkSurfaceFormatKHR));
	
	//SurfaceFormatAndColorSpace Step 03: call the same above function again but now to fill the above array
	vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice_selected, vkSurfaceKHR, &formatCount, vkSurfaceFormatKHR_array);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace() -> vkGetPhysicalDeviceSurfaceFormatsKHR() 2nd call is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace() -> vkGetPhysicalDeviceSurfaceFormatsKHR() 2nd call  is successed. \n");
	}
	
	fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace() -> formatCount: %d \n", formatCount);
	//SurfaceFormatAndColorSpace Step 04: according to the contains of above filled array decide the surface color format and color sapce
	// decide the surface color format first
	if (formatCount == 1 && vkSurfaceFormatKHR_array[0].format == VK_FORMAT_UNDEFINED)
	{
		vkFormat_color = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else
	{
		vkFormat_color = vkSurfaceFormatKHR_array[0].format;
	}

	// decide the surface color space
	vkColorSpaceKHR = vkSurfaceFormatKHR_array[0].colorSpace;

	//SurfaceFormatAndColorSpace Step 05: Free the array
	if (vkSurfaceFormatKHR_array)
	{
		free(vkSurfaceFormatKHR_array);
		vkSurfaceFormatKHR_array = NULL;
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace() -> vkSurfaceFormatKHR_array free \n");
	}

	fprintf(gpFile, "=====================================================================================\n");
	return (vkResult);

}

VkResult getPhysicalDevicePresentMode(void)
{
	VkResult vkResult = VK_SUCCESS;
	uint32_t presentationModesCount = 0;

	// code
	fprintf(gpFile, "=====================================================================================\n\n");
	//PresentationMode Step 01: Call vkGetPhysicalDeviceSurfacePresentModesKHR() first to retrive the count of supported presentation mode
	/*VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(
		VkPhysicalDevice                            physicalDevice,
		VkSurfaceKHR                                surface,
		uint32_t * pPresentModeCount,
		VkPresentModeKHR * pPresentModes);*/

	vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice_selected, vkSurfaceKHR, &presentationModesCount, NULL);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDevicePresentMode() -> vkGetPhysicalDeviceSurfacePresentModesKHR() 1st call  is failed. %d \n", vkResult);
		return (vkResult);
	}
	else if (presentationModesCount == 0)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		fprintf(gpFile, "getPhysicalDevicePresentMode() -> vkGetPhysicalDeviceSurfacePresentModesKHR() 1st call is fromat count is zero \n");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevicePresentMode() -> vkGetPhysicalDeviceSurfacePresentModesKHR() 1st call is successed. \n");
	}

	fprintf(gpFile, "getPhysicalDevicePresentMode() -> presentationModesCount : %d \n", presentationModesCount);

	//PresentationMode Step 02: Declare and allocate array of VkPresentationModeKHR structure

	VkPresentModeKHR* vkPresentModeKHR_array = (VkPresentModeKHR *) malloc(sizeof(VkPresentModeKHR) * presentationModesCount);

	//PresentationMode Step 03: Call above function again but now fill the above array
	vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice_selected, vkSurfaceKHR, &presentationModesCount, vkPresentModeKHR_array);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDevicePresentMode() -> vkGetPhysicalDeviceSurfacePresentModesKHR() 2nd call  is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevicePresentMode() -> vkGetPhysicalDeviceSurfacePresentModesKHR() 2nd call is successed. \n");
	}

	// decide the presentation mode
	//PresentationMode Step 04: According to the contains of above filled array decide the presentation mode
	for (uint32_t i = 0; i < presentationModesCount; i++)
	{
		//fprintf(gpFile, "getPhysicalDevicePresentMode() -> for loop -> vkPresentModeKHR = %ld \n", vkPresentModeKHR_array[i]);
		if (vkPresentModeKHR_array[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			vkPresentModeKHR = VK_PRESENT_MODE_MAILBOX_KHR;
			fprintf(gpFile, "getPhysicalDevicePresentMode() -> for loop -> vkPresentModeKHR = VK_PRESENT_MODE_MAILBOX_KHR; \n");
			break;
		}
	}

	if (vkPresentModeKHR != VK_PRESENT_MODE_MAILBOX_KHR)
	{
		vkPresentModeKHR = VK_PRESENT_MODE_FIFO_KHR;
		fprintf(gpFile, "getPhysicalDevicePresentMode() -> vkPresentModeKHR = VK_PRESENT_MODE_FIFO_KHR \n");
	}

	//PresentationMode Step 05: Free the array
	if (vkPresentModeKHR_array)
	{
		free(vkPresentModeKHR_array);
		fprintf(gpFile, "getPhysicalDevicePresentMode() -> vkPresentModeKHR_array() free done\n");
		vkPresentModeKHR_array = NULL;
	}

	fprintf(gpFile, "\n=====================================================================================\n");
	return(vkResult);

}

VkResult createSwapchain(VkBool32 vsync) //vertical sync. mobile madhe use karychi garaj nhi. desktop la garaj aste
{
	// function declaration
	VkResult getPhysicalDeviceSurfaceFormatAndColorSpace(void);
	VkResult getPhysicalDevicePresentMode(void);

	// Code
	VkResult vkResult = VK_SUCCESS;

	// Swapchain Step 01: Get physical device surface supported color format and physical device sufrace supported color space using step No "10 SurfaceFormatAndColorSpace"
	// Step 10 : color format and color space
	vkResult = getPhysicalDeviceSurfaceFormatAndColorSpace();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createSwapchain() -> getPhysicalDeviceSurfaceFormatAndColorSpace() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createSwapchain() -> getPhysicalDeviceSurfaceFormatAndColorSpace() is successed. \n");
	}

	//Swapchain Step 02: Get phyical device capabilitiesusing vulkan api vkGetPhysicalDeviceSurfaceCapabilitiesKHR() and accordingly initialise VkCapabilitiesKHR structure
	VkSurfaceCapabilitiesKHR vkSurfaceCapabilitiesKHR;
	memset((void *)&vkSurfaceCapabilitiesKHR, 0, sizeof(VkSurfaceCapabilitiesKHR));

	/*
	VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		VkPhysicalDevice                            physicalDevice,
		VkSurfaceKHR                                surface,
		VkSurfaceCapabilitiesKHR * pSurfaceCapabilities);
	*/
	vkResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice_selected, vkSurfaceKHR, &vkSurfaceCapabilitiesKHR);
	
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createSwapchain() -> vkGetPhysicalDeviceSurfaceCapabilitiesKHR() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createSwapchain() -> vkGetPhysicalDeviceSurfaceCapabilitiesKHR() is successed. \n");
	}

	//Swapchain Step 03: By using minImageCount and maxImageCount members of above structure decide desire image count for swapchain
	uint32_t testingNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.minImageCount + 1; // why +1?
	uint32_t desiredNumberOfSwapchainImages = 0; // ha find out karycha ahe

	if (vkSurfaceCapabilitiesKHR.maxImageCount > 0 && vkSurfaceCapabilitiesKHR.maxImageCount < testingNumberOfSwapchainImages)
	{ 
		desiredNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.maxImageCount;
	}
	else
	{
		desiredNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.minImageCount;
	}

	//Swapchain Step 04: By using currentExtent.width and currentExtent.height memeber of above struture and comparing them with 
	//					 current width and height of window decide image width and height of swapchain

	memset((void*)&vkExtent2D_swapchain, 0, sizeof(VkExtent2D));

	if (vkSurfaceCapabilitiesKHR.currentExtent.width != UINT32_MAX)
	{
		vkExtent2D_swapchain.width  = vkSurfaceCapabilitiesKHR.currentExtent.width;
		vkExtent2D_swapchain.height = vkSurfaceCapabilitiesKHR.currentExtent.height;

		fprintf(gpFile, "createSwapchain() -> in if () swapchain image width X height = %d X %d \n", vkExtent2D_swapchain.width, vkExtent2D_swapchain.height);
	}
	else
	{
		//if surface size is already define the swapchain image size must match with it
		VkExtent2D vkExtent2D;
		memset((void*)&vkExtent2D, 0 , sizeof(VkExtent2D));

		vkExtent2D.width  = (uint32_t) winWidth;
		vkExtent2D.height = (uint32_t) winHeight;

		vkExtent2D_swapchain.width = 
			glm::max(vkSurfaceCapabilitiesKHR.minImageExtent.width, glm::min(vkSurfaceCapabilitiesKHR.maxImageExtent.width,
			vkExtent2D.width));

		vkExtent2D_swapchain.height =
			glm::max(vkSurfaceCapabilitiesKHR.minImageExtent.height, glm::min(vkSurfaceCapabilitiesKHR.maxImageExtent.height,
				vkExtent2D.height));
		fprintf(gpFile, "createSwapchain() -> in else swapchain image width X height = %d X %d \n", vkExtent2D_swapchain.width, vkExtent2D_swapchain.height);
	}

	// swapchain Step 05: Decide how we are going to use the swap chain images means whether we are going to store image data and use it later(defered rendering) or
	//					  or we are going to use it immediatley as color attachement

	// image usage flag

	VkImageUsageFlags vkImageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	// FBO, Compute shader madhe 2nd paramter useful hoto. aata triangle madhe lagat nhi. nhi dila tri chalel aata


	// swapchain Step 06: swapchain is cabale of storing transformed image before presentation which is called as "PreTransformed" while creating swapchain we can decide wheter to pre transformed or not.
	//					  Pretransformed also inculed filping of image
	VkSurfaceTransformFlagBitsKHR vkSurfaceTransformFlagBitsKHR; //enum

	if (vkSurfaceCapabilitiesKHR.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		vkSurfaceTransformFlagBitsKHR = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		fprintf(gpFile, "createSwapchain() -> if vkSurfaceTransformFlagBitsKHR = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR\n");
	}
	else
	{
		vkSurfaceTransformFlagBitsKHR = vkSurfaceCapabilitiesKHR.currentTransform;
		fprintf(gpFile, "createSwapchain() -> else vkSurfaceTransformFlagBitsKHR = vkSurfaceCapabilitiesKHR.currentTransform\n");
	}

	//Swapchain Step 07: Get presenation mode for swapchain images using Step "11.PresentationMode"
	// Step 11 : PresentationMode
	vkResult = getPhysicalDevicePresentMode();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createSwapchain() -> getPhysicalDevicePresentMode() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createSwapchain() -> getPhysicalDevicePresentMode() is successed. \n");
	}

	//Swapchain Step 08: According to above data declare, memset and initialise VkSwapchainCreateInforKHR structure
	VkSwapchainCreateInfoKHR  vkSwapchainCreateInforKHR;
	memset((void*) &vkSwapchainCreateInforKHR, 0, sizeof(VkSwapchainCreateInfoKHR));

	vkSwapchainCreateInforKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	vkSwapchainCreateInforKHR.pNext = NULL;
	vkSwapchainCreateInforKHR.flags = 0;
	vkSwapchainCreateInforKHR.surface = vkSurfaceKHR;
	vkSwapchainCreateInforKHR.minImageCount = desiredNumberOfSwapchainImages;
	vkSwapchainCreateInforKHR.imageFormat = vkFormat_color;
	vkSwapchainCreateInforKHR.imageColorSpace = vkColorSpaceKHR;
	vkSwapchainCreateInforKHR.imageExtent.width = vkExtent2D_swapchain.width;
	vkSwapchainCreateInforKHR.imageExtent.height = vkExtent2D_swapchain.height;
	vkSwapchainCreateInforKHR.imageUsage = vkImageUsageFlags;
	vkSwapchainCreateInforKHR.preTransform = vkSurfaceTransformFlagBitsKHR;
	vkSwapchainCreateInforKHR.imageArrayLayers = 1;
	vkSwapchainCreateInforKHR.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkSwapchainCreateInforKHR.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	vkSwapchainCreateInforKHR.presentMode = vkPresentModeKHR;
	vkSwapchainCreateInforKHR.clipped = VK_TRUE;

	vkResult = vkCreateSwapchainKHR(vkDevice, &vkSwapchainCreateInforKHR, NULL, &vkSwapchainKHR);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createSwapchain() -> vkCreateSwapchainKHR() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createSwapchain() -> vkCreateSwapchainKHR() is successed. \n");
	}

	return (vkResult);
}

VkResult createImagesAndImageView()
{
	// function declaration
	VkResult getSupportedDepthFormat(void);

	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createImagesAndImageView() : 06th March 2025\n\n");
	// code
	//step 01: Get Swapchain image count in global varibale using vkGetSwapchainImagesKHR() api

	/*VkResult vkGetSwapchainImagesKHR(
		VkDevice                                    device,
		VkSwapchainKHR                              swapchain,
		uint32_t * pSwapchainImageCount,
		VkImage * pSwapchainImages);*/

	vkResult = vkGetSwapchainImagesKHR(vkDevice, vkSwapchainKHR, &swapchainImageCount, NULL);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkGetSwapchainImagesKHR() 1st call  is failed. %d \n", vkResult);
		return (vkResult);
	}
	else if (swapchainImageCount == 0)
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkGetSwapchainImagesKHR() 1st call is swapchainImageCount count is zero \n");
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkGetSwapchainImagesKHR() 1st call is successed. And swapchain image count %d \n", swapchainImageCount);

	}

	//step 02: declare a global VkImageType array and allocate it to the size of swapchain image count using malloc
	
	swapchainImage_array = (VkImage*) malloc(sizeof(VkImage) * swapchainImageCount);
	// for the sake of bravity do error checking for malloc

	//step 03: now call the same function again from step1 and filled this array.
	vkResult = vkGetSwapchainImagesKHR(vkDevice, vkSwapchainKHR, &swapchainImageCount, swapchainImage_array);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkGetSwapchainImagesKHR() 2nd call  is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkGetSwapchainImagesKHR() 2nd call is successed\n");
	}

	//step 04: declare another global array of type VkImageView and allocate it to the size of swapchain image count using malloc

	swapchainimageView_array = (VkImageView*)malloc(sizeof(VkImageView) * swapchainImageCount);

	//step 05: declare and initialise VkImageViewCreateInfo except its ".image"

	/*typedef struct VkImageViewCreateInfo {
		VkStructureType            sType;
		const void*				   pNext;
		VkImageViewCreateFlags     flags;
		VkImage                    image;
		VkImageViewType            viewType;
		VkFormat                   format;
		VkComponentMapping         components;
		VkImageSubresourceRange    subresourceRange;
	} VkImageViewCreateInfo;*/

	VkImageViewCreateInfo vkImageViewCreateInfo;

	memset((void*)&vkImageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

	vkImageViewCreateInfo.sType								 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vkImageViewCreateInfo.pNext								 = NULL;
	vkImageViewCreateInfo.flags								 = 0;
	vkImageViewCreateInfo.format							 = vkFormat_color;

	vkImageViewCreateInfo.components.r						 = VK_COMPONENT_SWIZZLE_R;
	vkImageViewCreateInfo.components.g						 = VK_COMPONENT_SWIZZLE_G;
	vkImageViewCreateInfo.components.b						 = VK_COMPONENT_SWIZZLE_B;
	vkImageViewCreateInfo.components.a						 = VK_COMPONENT_SWIZZLE_A; //VkComponentSwizzle enum
	
	vkImageViewCreateInfo.subresourceRange.aspectMask		 = VK_IMAGE_ASPECT_COLOR_BIT;
	vkImageViewCreateInfo.subresourceRange.baseMipLevel		 = 0;
	vkImageViewCreateInfo.subresourceRange.levelCount		 = 1;
	vkImageViewCreateInfo.subresourceRange.baseArrayLayer	 = 0;
	vkImageViewCreateInfo.subresourceRange.layerCount	     = 1;
	
	vkImageViewCreateInfo.viewType							 = VK_IMAGE_VIEW_TYPE_2D;

	//step 06: now start a loop for swapchain image countand inside this loop initialise above ".image" memeber to the swapchain image array index we obtained above
	//		  and then call  () api to fill above ImageView array(step 4)
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		vkImageViewCreateInfo.image = swapchainImage_array[i];

		/*VkResult vkCreateImageView(
				VkDevice                                    device,
				const VkImageViewCreateInfo * pCreateInfo,
				const VkAllocationCallbacks * pAllocator,
				VkImageView * pView);*/

		vkResult = vkCreateImageView(vkDevice, &vkImageViewCreateInfo, NULL, &swapchainimageView_array[i]);

		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "createImagesAndImageView() -> vkCreateImageView() in loop  is failed at ith :  %d due to %d error \n", i, vkResult);
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "createImagesAndImageView() -> vkGetSwapchainImagesKHR() in loop is successed at ith : %d \n", i);
		}
	}

	// FOR DEPTH IMAGE

	vkResult = getSupportedDepthFormat();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createImagesAndImageView() -> getSupportedDepthFormat() failed due %d error \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createImagesAndImageView() -> getSupportedDepthFormat() is successed \n");
	}

	// for depth image initialise VkImageCreateInfo
	/*
	typedef struct VkImageCreateInfo {
							VkStructureType          sType;
							const void*              pNext;
							VkImageCreateFlags       flags;
							VkImageType              imageType;
							VkFormat                 format;
							VkExtent3D               extent;
							uint32_t                 mipLevels;
							uint32_t                 arrayLayers;
							VkSampleCountFlagBits    samples;
							VkImageTiling            tiling;
							VkImageUsageFlags        usage;
							VkSharingMode            sharingMode;
							uint32_t                 queueFamilyIndexCount;
							const uint32_t*          pQueueFamilyIndices;
							VkImageLayout            initialLayout;
	} VkImageCreateInfo; */

	VkImageCreateInfo vkImageCreateInfo;
	memset((void*)&vkImageCreateInfo, 0, sizeof(VkImageCreateInfo));

	vkImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	vkImageCreateInfo.pNext = NULL;
	vkImageCreateInfo.flags = 0;
	vkImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	vkImageCreateInfo.format = vkFormat_depth;
	vkImageCreateInfo.extent.width = winWidth;
	vkImageCreateInfo.extent.height = winHeight;
	vkImageCreateInfo.extent.depth = 1;
	vkImageCreateInfo.mipLevels = 1;
	vkImageCreateInfo.arrayLayers = 1;
	vkImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	vkImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	vkImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	vkResult = vkCreateImage(vkDevice, &vkImageCreateInfo, NULL, &vkImage_depth);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkCreateImage() failed for depth due %d error \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkCreateImage() is successed for depth \n");
	}

	// Memory required for depth image
	VkMemoryRequirements vkMemoryRequirements;
	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

	/*void vkGetBufferMemoryRequirements(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		VkMemoryRequirements * pMemoryRequirements);*/

	vkGetImageMemoryRequirements(vkDevice, vkImage_depth, &vkMemoryRequirements);

	/*typedef struct VkMemoryAllocateInfo {
		VkStructureType    sType;
		const void* pNext;
		VkDeviceSize       allocationSize;
		uint32_t           memoryTypeIndex;
	} VkMemoryAllocateInfo;*/

	VkMemoryAllocateInfo vkMemoryAllocateInfo;
	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext = NULL;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}

	/*VkResult vkAllocateMemory(
		VkDevice                                    device,
		const VkMemoryAllocateInfo * pAllocateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDeviceMemory * pMemory);*/

	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vkDeviceMemory_depth);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createImagesAndImageView()() -> vkAllocateMemory() is failed due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createImagesAndImageView()() -> vkAllocateMemory() \n");
	}

	/*Step 10: now we have our required device memeory handle as well as our vkBufferHandle.
			   Bind device memeory handle to vulkan memory handle by using vkBindBUfferMemory();*/
			   /*VkResult vkBindBufferMemory(
				   VkDevice                                    device,
				   VkBuffer                                    buffer,
				   VkDeviceMemory                              memory,
				   VkDeviceSize                                memoryOffset);*/

	vkResult = vkBindImageMemory(vkDevice, vkImage_depth, vkDeviceMemory_depth, 0);//vkBuffer, vetextData_position.vkDeviceMemory
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createImagesAndImageView()() -> vkBindBufferMemory() is failed due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createImagesAndImageView()() -> vkBindBufferMemory() \n");
	}

	// create image view for above depth image

	memset((void*)&vkImageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

	vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vkImageViewCreateInfo.pNext = NULL;
	vkImageViewCreateInfo.flags = 0;
	vkImageViewCreateInfo.format = vkFormat_depth;

	vkImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	vkImageViewCreateInfo.subresourceRange.levelCount = 1;
	vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	vkImageViewCreateInfo.subresourceRange.layerCount = 1;

	vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	vkImageViewCreateInfo.image = vkImage_depth;  // IMP For depth

	vkResult = vkCreateImageView(vkDevice, &vkImageViewCreateInfo, NULL, &vkImageView_depth);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkCreateImageView() for depth is failed due to %d error \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createImagesAndImageView() -> vkCreateImageView() is successed for depth\n");
	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return (vkResult);

}

VkResult getSupportedDepthFormat(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;
	VkFormat vkFormat_depth_array[] = {
		VK_FORMAT_D32_SFLOAT_S8_UINT, // 32 bit depth. heighest depth. 
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "getSupportedDepthFormat() :31st July 2025\n\n");

	for (uint32_t i = 0; i < sizeof(vkFormat_depth_array) / sizeof(vkFormat_depth_array[0]); i++)
	{
		VkFormatProperties vkFormatProperties;
		memset((void*)&vkFormatProperties, 0, sizeof(VkFormatProperties));

		/*typedef struct VkFormatProperties {
			VkFormatFeatureFlags    linearTilingFeatures;
			VkFormatFeatureFlags    optimalTilingFeatures;
			VkFormatFeatureFlags    bufferFeatures;
		} VkFormatProperties;*/

		/*void vkGetPhysicalDeviceFormatProperties(
			VkPhysicalDevice                            physicalDevice,
			VkFormat                                    format,
			VkFormatProperties * pFormatProperties);*/

		vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice_selected, vkFormat_depth_array[i], &vkFormatProperties);

		if (vkFormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			vkFormat_depth = vkFormat_depth_array[i];
			vkResult = VK_SUCCESS;
			break;
		}
	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return (vkResult);
}

VkResult createCommandPool(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createCommandPool() : 06th March 2025\n\n");

	//Step 01: declare and initialise VkCommandPoolCreateInfo struture
	/*typedef struct VkCommandPoolCreateInfo {
		VkStructureType             sType;
		const void*					pNext;
		VkCommandPoolCreateFlags    flags;
		uint32_t                    queueFamilyIndex;
	} VkCommandPoolCreateInfo;*/

	VkCommandPoolCreateInfo vkCommandPoolCreateInfo;
	memset((void*)&vkCommandPoolCreateInfo, 0, sizeof(VkCommandPoolCreateInfo));

	vkCommandPoolCreateInfo.sType				 = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vkCommandPoolCreateInfo.pNext				 = NULL;
	vkCommandPoolCreateInfo.flags				 = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	vkCommandPoolCreateInfo.queueFamilyIndex	 = graphicsQueueFamilyIndex_selected;


	//step 02: call vkCreateCommandPool() to create the command pool
	/*VkResult vkCreateCommandPool(
		VkDevice                                    device,
		const VkCommandPoolCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkCommandPool * pCommandPool);*/

	vkResult = vkCreateCommandPool(vkDevice, &vkCommandPoolCreateInfo, NULL, &vkCommandPool);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createCommandPool() -> vkCreateCommandPool() is failed. %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createCommandPool() -> vkCreateCommandPool()  successed \n");

	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return(vkResult);
}

VkResult createCommandBuffer(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createCommandBuffer() : 06th March 2025\n\n");

	//step 01: declare and initialise struct VkCommandBufferAllocateInfo 
	/*typedef struct VkCommandBufferAllocateInfo {
		VkStructureType         sType;
		const void*				pNext;
		VkCommandPool           commandPool;
		VkCommandBufferLevel    level;
		uint32_t                commandBufferCount;
	} VkCommandBufferAllocateInfo;*/

	VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo;
	memset((void*)&vkCommandBufferAllocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));

	vkCommandBufferAllocateInfo.sType				 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	vkCommandBufferAllocateInfo.pNext				 = NULL;
	vkCommandBufferAllocateInfo.commandPool			 = vkCommandPool;
	vkCommandBufferAllocateInfo.level				 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	vkCommandBufferAllocateInfo.commandBufferCount	 = 1;
	//vkCommandBufferAllocateInfo.flag	 = ;

	//step 02: declare a command buffer array globally and allocate it to the size of swapchain image count

	vkCommandBuffer_array = (VkCommandBuffer*) malloc(sizeof(VkCommandBuffer) * swapchainImageCount);

	//	step 03: in a loop which is equla to swapchain image count, allocate each command buffer in above array by using vkAllocateCommandBuffers().
	//		  	 remember at the time of allocation buffer are gong to empty later we will record graphics or compute command into them

	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		/*VkResult vkAllocateCommandBuffers(
			VkDevice                                    device,
			const VkCommandBufferAllocateInfo * pAllocateInfo,
			VkCommandBuffer * pCommandBuffers);*/
		vkResult = vkAllocateCommandBuffers(vkDevice, &vkCommandBufferAllocateInfo, &vkCommandBuffer_array[i]);
		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "createCommandBuffer() -> vkAllocateCommandBuffers() is failed at ith : %d due to error : %d \n", i, vkResult);
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "createCommandBuffer() -> vkAllocateCommandBuffers() successed at ith : %d\n", i);

		}

	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return(vkResult);
}

//Step 02: declare UDF createVertexBuffer().write its prototype below createCommandBuffer() and above createrenderPass() and also call it between the call's of it
VkResult createVertexBuffer(void)
{
	VkResult vkResult = VK_SUCCESS;

	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createVertexBuffer() : 17th April 2025\n\n");

	//Step 03: Implement this UDF. and inside it first declare our triangles position array

	//street_lamp_middle_bar and street_lamp_horizontal_top_bar have same color Hence kept one array for both.

	float street_lamp_middle_bar_position[] = {
										// first triangle
										//1.0f,  1.2f, 0.0f, // right top 
									 //  -1.0f,  1.2f, 0.0f, // left top
									 //  -1.0f, -7.0f, 0.0f, // left bottom

									 //  // second triangle
									 //  -1.0f, -7.0f, 0.0f, // left bottom
										//1.0f, -7.0f, 0.0f, // right bottom
										//1.0f,  1.2f, 0.0f // right top

										1.0f,  1.0f, 0.0f, // right top 
									   -1.0f,  1.0f, 0.0f, // left top
									   -1.0f, -1.0f, 0.0f, // left bottom

									   // second triangle
									   -1.0f, -1.0f, 0.0f, // left bottom
										1.0f, -1.0f, 0.0f, // right bottom
										1.0f,  1.0f, 0.0f // right top
	};

	float street_lamp_horizontal_top_bar_position[] = {
										// first triangle
										1.0f,  1.2f, 0.0f, // right top 
									   -1.0f,  1.2f, 0.0f, // left top
									   -1.0f, -7.0f, 0.0f, // left bottom

									   // second triangle
									   -1.0f, -7.0f, 0.0f, // left bottom
										1.0f, -7.0f, 0.0f, // right bottom
										1.0f,  1.2f, 0.0f // right top
	};

	float street_lamp_middle_bar_color[] = {

								  0.0f, 0.0f, 0.0f,
								  0.0f, 0.0f, 0.0f,
								  0.0f, 0.0f, 0.0f,
								  
								  0.0f, 0.0f, 0.0f,
								  0.0f, 0.0f, 0.0f,
								  0.0f, 0.0f, 0.0f,
	};


	// street_lamp_light_1 and street_lamp_light_2 have same position array and color array

	float street_lamp_light_position[] = {
									// first triangle
									1.0f,  1.0f, 0.0f, // right top 
								   -1.0f,  1.0f, 0.0f, // left top
								   -1.0f, -1.0f, 0.0f, // left bottom

								   // second triangle
								   -1.0f, -1.0f, 0.0f, // left bottom
									1.0f, -1.0f, 0.0f, // right bottom
									1.0f,  1.0f, 0.0f // right top
	};

	float street_lamp_light_color[] = {
								  0.0f, 0.4f, 0.0f,						  
								  0.0f, 0.4f, 0.0f,						  
								  0.0f, 0.4f, 0.0f,						  
								  
								  0.0f, 0.4f, 0.0f,						  
								  0.0f, 0.4f, 0.0f,						  
								  0.0f, 0.4f, 0.0f
	};

	// street_light_middle_bar  Started
	//================================= street_light_middle_bar : VERTEX POSITION BUFFER ====================================================
	
	//Step 04: memset our global vertexData_Position. 
	memset((void *)&vetextData_street_light_middle_bar_position, 0, sizeof(VetextData));

	/*Step 05: declare and memset vkBufferCreateInfo.It has 8 member we will use 5. out of them 2 are very important
				a.usage
				b.size
	*/

	/*typedef struct VkBufferCreateInfo {
		VkStructureType        sType;
		const void* pNext;
		VkBufferCreateFlags    flags;
		VkDeviceSize           size;
		VkBufferUsageFlags     usage;
		VkSharingMode          sharingMode;
		uint32_t               queueFamilyIndexCount;
		const uint32_t* pQueueFamilyIndices;
	} VkBufferCreateInfo;*/

	VkBufferCreateInfo vkBufferCreateInfo;
	memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vkBufferCreateInfo.pNext = NULL;
	vkBufferCreateInfo.flags = 0; // valid flags used in scattered buffer
	vkBufferCreateInfo.size  = sizeof(street_lamp_middle_bar_position);
	vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	//step 06: call vkCreateBuffer() vulkan api.in the "." vkBuffer member of our global struct
	/*VkResult vkCreateBuffer(
		VkDevice                                    device,
		const VkBufferCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkBuffer * pBuffer);*/

	vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vetextData_street_light_middle_bar_position.vkBuffer);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkCreateBuffer() is failed due to error : %d FOR street_light_middle_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkCreateBuffer() FOR street_light_middle_bar \n");
	}

	//Step 07: declare and memset struct VkMemoryRequirements and then call , vkGetBufferMemeoryRequirement() to get the memory requirement.
	/*typedef struct VkMemoryRequirements {
		VkDeviceSize    size;
		VkDeviceSize    alignment;
		uint32_t        memoryTypeBits;
	} VkMemoryRequirements;*/

	VkMemoryRequirements vkMemoryRequirements;
	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

	/*void vkGetBufferMemoryRequirements(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		VkMemoryRequirements * pMemoryRequirements);*/

	vkGetBufferMemoryRequirements(vkDevice, vetextData_street_light_middle_bar_position.vkBuffer, &vkMemoryRequirements);

	/*Step 08:	To actually required memory we need to call vkAllocateMemory().but before that we need to declareand memset stuct VkMemoryAllocateInfo.
				Important members of this structure are = ".memoryTypeIndex" and ".allocationSize".
				for allocation size use the size obtained from memory requirements.
				For memory type index : start a loop with counter as vkPhysicalDeviceMemoryProperties.memoryTypeCount
				b.Inside the loop check vkMemoryRequirement.memoryTypeBits contain 1 or not.If yes
				c.check vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags member contains VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT. if yes
				d.then ith index will be our memeory type index.If found break out of the loop.
				e. if not continue the loop by right shiffting vkMemoryRequirements.memoryTypeBits  by 1, over each iteration.
	*/

	/*typedef struct VkMemoryAllocateInfo {
		VkStructureType    sType;
		const void* pNext;
		VkDeviceSize       allocationSize;
		uint32_t           memoryTypeIndex;
	} VkMemoryAllocateInfo;*/

	VkMemoryAllocateInfo vkMemoryAllocateInfo;
	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType			 = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext			 = NULL;
	vkMemoryAllocateInfo.allocationSize	 = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}

	//Step 09: call vkAllocateMemory() and get the required vulkan memory handle into the ".vkDeviceMemory" member of our global member
	/*VkResult vkAllocateMemory(
		VkDevice                                    device,
		const VkMemoryAllocateInfo * pAllocateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDeviceMemory * pMemory);*/

	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vetextData_street_light_middle_bar_position.vkDeviceMemory);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkAllocateMemory() is failed due to error : %d FOR street_light_middle_bar\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkAllocateMemory() FOR street_light_middle_bar \n");
	}

	/*Step 10: now we have our required device memeory handle as well as our vkBufferHandle.
			   Bind device memeory handle to vulkan memory handle by using vkBindBUfferMemory();*/
	/*VkResult vkBindBufferMemory(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		VkDeviceMemory                              memory,
		VkDeviceSize                                memoryOffset);*/
	
	vkResult = vkBindBufferMemory(vkDevice, vetextData_street_light_middle_bar_position.vkBuffer, vetextData_street_light_middle_bar_position.vkDeviceMemory, 0);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkBindBufferMemory() is failed due to error : %d FOR street_light_middle_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkBindBufferMemory() FOR street_light_middle_bar \n");
	}

	//Step 11: declare local void* buffer say "data" and call vkMapMemory to map our device memory object handle to this void* data.This will allow us to do memory mapped IO
	//	means when we wirte on void* data it will get automatically return / copied onto the device memory represented by device memory handle

	void* data = NULL;
	/*VkResult vkMapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory,
		VkDeviceSize                                offset,
		VkDeviceSize                                size,
		VkMemoryMapFlags                            flags,
		void** ppData);*/

	vkResult = vkMapMemory(vkDevice, vetextData_street_light_middle_bar_position.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkMapMemory() is failed due to error : %d FOR street_light_middle_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkMapMemory() FOR street_light_middle_bar \n");
	}

	//Step 12: now to do actually memeory mapped IO call memcpy()
	memcpy(data, street_lamp_middle_bar_position, sizeof(street_lamp_middle_bar_position));

	//Step 13: To complete this memory mapped IO finally call vkUnMapMemory()
	/*void vkUnmapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory);*/

	vkUnmapMemory(vkDevice, vetextData_street_light_middle_bar_position.vkDeviceMemory);

	// street_light_middle_bar  Started
	//================================= street_light_middle_bar : VERTEX COLOR BUFFER ====================================================
	
	//Step 04: memset our global vertexData_Position. 
	memset((void*)&vetextData_street_light_middle_bar_color, 0, sizeof(VetextData));

	/*Step 05: declare and memset vkBufferCreateInfo.It has 8 member we will use 5. out of them 2 are very important
				a.usage
				b.size
	*/

	/*typedef struct VkBufferCreateInfo {
		VkStructureType        sType;
		const void* pNext;
		VkBufferCreateFlags    flags;
		VkDeviceSize           size;
		VkBufferUsageFlags     usage;
		VkSharingMode          sharingMode;
		uint32_t               queueFamilyIndexCount;
		const uint32_t* pQueueFamilyIndices;
	} VkBufferCreateInfo;*/

	memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vkBufferCreateInfo.pNext = NULL;
	vkBufferCreateInfo.flags = 0; // valid flags used in scattered buffer
	vkBufferCreateInfo.size = sizeof(street_lamp_middle_bar_position);
	vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	//step 06: call vkCreateBuffer() vulkan api.in the "." vkBuffer member of our global struct
	/*VkResult vkCreateBuffer(
		VkDevice                                    device,
		const VkBufferCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkBuffer * pBuffer);*/

	vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vetextData_street_light_middle_bar_color.vkBuffer);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkCreateBuffer() is failed for vetextData_color_street_light_lamp.vkBuffer due to error : %d FOR street_light_middle_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkCreateBuffer() for vetextData_color_street_light_lamp FOR street_light_middle_bar \n");
	}

	//Step 07: declare and memset struct VkMemoryRequirements and then call , vkGetBufferMemeoryRequirement() to get the memory requirement.
	/*typedef struct VkMemoryRequirements {
		VkDeviceSize    size;
		VkDeviceSize    alignment;
		uint32_t        memoryTypeBits;
	} VkMemoryRequirements;*/

	//VkMemoryRequirements vkMemoryRequirements;
	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

	/*void vkGetBufferMemoryRequirements(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		VkMemoryRequirements * pMemoryRequirements);*/

	vkGetBufferMemoryRequirements(vkDevice, vetextData_street_light_middle_bar_color.vkBuffer, &vkMemoryRequirements);

	/*Step 08:	To actually required memory we need to call vkAllocateMemory().but before that we need to declareand memset stuct VkMemoryAllocateInfo.
				Important members of this structure are = ".memoryTypeIndex" and ".allocationSize".
				for allocation size use the size obtained from memory requirements.
				For memory type index : start a loop with counter as vkPhysicalDeviceMemoryProperties.memoryTypeCount
				b.Inside the loop check vkMemoryRequirement.memoryTypeBits contain 1 or not.If yes
				c.check vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags member contains VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT. if yes
				d.then ith index will be our memeory type index.If found break out of the loop.
				e. if not continue the loop by right shiffting vkMemoryRequirements.memoryTypeBits  by 1, over each iteration.
	*/

	/*typedef struct VkMemoryAllocateInfo {
		VkStructureType    sType;
		const void* pNext;
		VkDeviceSize       allocationSize;
		uint32_t           memoryTypeIndex;
	} VkMemoryAllocateInfo;*/

	//VkMemoryAllocateInfo vkMemoryAllocateInfo;
	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext = NULL;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}

	//Step 09: call vkAllocateMemory() and get the required vulkan memory handle into the ".vkDeviceMemory" member of our global member
	/*VkResult vkAllocateMemory(
		VkDevice                                    device,
		const VkMemoryAllocateInfo * pAllocateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDeviceMemory * pMemory);*/

	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vetextData_street_light_middle_bar_color.vkDeviceMemory);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkAllocateMemory() is failed for vetextData_street_light_middle_bar_color.vkDeviceMemory due to error : %d FOR street_light_middle_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkAllocateMemory() for vetextData_street_light_middle_bar_color FOR street_light_middle_bar \n");
	}

	/*Step 10: now we have our required device memeory handle as well as our vkBufferHandle.
			   Bind device memeory handle to vulkan memory handle by using vkBindBUfferMemory();*/
			   /*VkResult vkBindBufferMemory(
				   VkDevice                                    device,
				   VkBuffer                                    buffer,
				   VkDeviceMemory                              memory,
				   VkDeviceSize                                memoryOffset);*/

	vkResult = vkBindBufferMemory(vkDevice, vetextData_street_light_middle_bar_color.vkBuffer, vetextData_street_light_middle_bar_color.vkDeviceMemory, 0);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkBindBufferMemory() is failed for vetextData_street_light_middle_bar_color due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkBindBufferMemory() for  vetextData_street_light_middle_bar_color\n");
	}

	//Step 11: declare local void* buffer say "data" and call vkMapMemory to map our device memory object handle to this void* data.This will allow us to do memory mapped IO
	//	means when we wirte on void* data it will get automatically return / copied onto the device memory represented by device memory handle

	data = NULL;
	/*VkResult vkMapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory,
		VkDeviceSize                                offset,
		VkDeviceSize                                size,
		VkMemoryMapFlags                            flags,
		void** ppData);*/

	vkResult = vkMapMemory(vkDevice, vetextData_street_light_middle_bar_color.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkMapMemory() is failed for vetextData_street_light_middle_bar_color due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkMapMemory() for vetextData_street_light_middle_bar_color \n");
	}

	//Step 12: now to do actually memeory mapped IO call memcpy()
	memcpy(data, street_lamp_middle_bar_color, sizeof(street_lamp_middle_bar_color));

	//Step 13: To complete this memory mapped IO finally call vkUnMapMemory()
	/*void vkUnmapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory);*/

	vkUnmapMemory(vkDevice, vetextData_street_light_middle_bar_color.vkDeviceMemory);

	// street_light_middle_bar  Ended Vertex Position and Color Buffer
	
	// street_light_lamp : Started
	//================================= street_light_lamp :  VERTEX POSITION BUFFER ====================================================

	//Step 04: memset our global vertexData_Position. 
	memset((void*)&vetextData_position_street_light_lamp, 0, sizeof(VetextData));

	memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vkBufferCreateInfo.pNext = NULL;
	vkBufferCreateInfo.flags = 0; // valid flags used in scattered buffer
	vkBufferCreateInfo.size = sizeof(street_lamp_light_position);
	vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


	vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vetextData_position_street_light_lamp.vkBuffer);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkCreateBuffer() is failed due to error : %d FOR street_light_lamp \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkCreateBuffer()  FOR street_light_lamp  \n");
	}

	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

	vkGetBufferMemoryRequirements(vkDevice, vetextData_position_street_light_lamp.vkBuffer, &vkMemoryRequirements);

	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext = NULL;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}

	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vetextData_position_street_light_lamp.vkDeviceMemory);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkAllocateMemory() is failed due to error : %d  FOR street_light_lamp  \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkAllocateMemory()  FOR street_light_lamp  \n");
	}

	vkResult = vkBindBufferMemory(vkDevice, vetextData_position_street_light_lamp.vkBuffer, vetextData_position_street_light_lamp.vkDeviceMemory, 0);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkBindBufferMemory() is failed due to error : %d  FOR street_light_lamp  \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkBindBufferMemory()  FOR street_light_lamp  \n");
	}

	//Step 11: declare local void* buffer say "data" and call vkMapMemory to map our device memory object handle to this void* data.This will allow us to do memory mapped IO
	//	means when we wirte on void* data it will get automatically return / copied onto the device memory represented by device memory handle

	data = NULL;
	/*VkResult vkMapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory,
		VkDeviceSize                                offset,
		VkDeviceSize                                size,
		VkMemoryMapFlags                            flags,
		void** ppData);*/

	vkResult = vkMapMemory(vkDevice, vetextData_position_street_light_lamp.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkMapMemory() is failed due to error : %d  FOR street_light_lamp  \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkMapMemory()  FOR street_light_lamp \n");
	}

	//Step 12: now to do actually memeory mapped IO call memcpy()
	memcpy(data, street_lamp_light_position, sizeof(street_lamp_light_position));

	//Step 13: To complete this memory mapped IO finally call vkUnMapMemory()
	/*void vkUnmapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory);*/

	vkUnmapMemory(vkDevice, vetextData_position_street_light_lamp.vkDeviceMemory);

	//================================= street_light_lamp : VERTEX COLOR BUFFER====================================================
	
	//Step 04: memset our global vertexData_Position. 
	memset((void*)&vetextData_color_street_light_lamp, 0, sizeof(VetextData));

	/*Step 05: declare and memset vkBufferCreateInfo.It has 8 member we will use 5. out of them 2 are very important
				a.usage
				b.size
	*/

	/*typedef struct VkBufferCreateInfo {
		VkStructureType        sType;
		const void* pNext;
		VkBufferCreateFlags    flags;
		VkDeviceSize           size;
		VkBufferUsageFlags     usage;
		VkSharingMode          sharingMode;
		uint32_t               queueFamilyIndexCount;
		const uint32_t* pQueueFamilyIndices;
	} VkBufferCreateInfo;*/

	memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vkBufferCreateInfo.pNext = NULL;
	vkBufferCreateInfo.flags = 0; // valid flags used in scattered buffer
	vkBufferCreateInfo.size = sizeof(street_lamp_light_color);
	vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	//step 06: call vkCreateBuffer() vulkan api.in the "." vkBuffer member of our global struct
	/*VkResult vkCreateBuffer(
		VkDevice                                    device,
		const VkBufferCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkBuffer * pBuffer);*/

	vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &vetextData_color_street_light_lamp.vkBuffer);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkCreateBuffer() is failed for vetextData_color_street_light_lamp.vkBuffer due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkCreateBuffer() for vetextData_color_street_light_lamp\n");
	}

	//Step 07: declare and memset struct VkMemoryRequirements and then call , vkGetBufferMemeoryRequirement() to get the memory requirement.
	/*typedef struct VkMemoryRequirements {
		VkDeviceSize    size;
		VkDeviceSize    alignment;
		uint32_t        memoryTypeBits;
	} VkMemoryRequirements;*/

	//VkMemoryRequirements vkMemoryRequirements;
	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

	/*void vkGetBufferMemoryRequirements(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		VkMemoryRequirements * pMemoryRequirements);*/

	vkGetBufferMemoryRequirements(vkDevice, vetextData_color_street_light_lamp.vkBuffer, &vkMemoryRequirements);

	/*Step 08:	To actually required memory we need to call vkAllocateMemory().but before that we need to declareand memset stuct VkMemoryAllocateInfo.
				Important members of this structure are = ".memoryTypeIndex" and ".allocationSize".
				for allocation size use the size obtained from memory requirements.
				For memory type index : start a loop with counter as vkPhysicalDeviceMemoryProperties.memoryTypeCount
				b.Inside the loop check vkMemoryRequirement.memoryTypeBits contain 1 or not.If yes
				c.check vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags member contains VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT. if yes
				d.then ith index will be our memeory type index.If found break out of the loop.
				e. if not continue the loop by right shiffting vkMemoryRequirements.memoryTypeBits  by 1, over each iteration.
	*/

	/*typedef struct VkMemoryAllocateInfo {
		VkStructureType    sType;
		const void* pNext;
		VkDeviceSize       allocationSize;
		uint32_t           memoryTypeIndex;
	} VkMemoryAllocateInfo;*/

	//VkMemoryAllocateInfo vkMemoryAllocateInfo;
	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext = NULL;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}

	//Step 09: call vkAllocateMemory() and get the required vulkan memory handle into the ".vkDeviceMemory" member of our global member
	/*VkResult vkAllocateMemory(
		VkDevice                                    device,
		const VkMemoryAllocateInfo * pAllocateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDeviceMemory * pMemory);*/

	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &vetextData_color_street_light_lamp.vkDeviceMemory);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkAllocateMemory() is failed for vetextData_color_street_light_lamp.vkDeviceMemory due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkAllocateMemory() for vetextData_color_street_light_lamp\n");
	}

	/*Step 10: now we have our required device memeory handle as well as our vkBufferHandle.
			   Bind device memeory handle to vulkan memory handle by using vkBindBUfferMemory();*/
			   /*VkResult vkBindBufferMemory(
				   VkDevice                                    device,
				   VkBuffer                                    buffer,
				   VkDeviceMemory                              memory,
				   VkDeviceSize                                memoryOffset);*/

	vkResult = vkBindBufferMemory(vkDevice, vetextData_color_street_light_lamp.vkBuffer, vetextData_color_street_light_lamp.vkDeviceMemory, 0);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkBindBufferMemory() is failed for vetextData_color_street_light_lamp due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkBindBufferMemory() for  vetextData_color_street_light_lamp\n");
	}

	//Step 11: declare local void* buffer say "data" and call vkMapMemory to map our device memory object handle to this void* data.This will allow us to do memory mapped IO
	//	means when we wirte on void* data it will get automatically return / copied onto the device memory represented by device memory handle

	data = NULL;
	/*VkResult vkMapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory,
		VkDeviceSize                                offset,
		VkDeviceSize                                size,
		VkMemoryMapFlags                            flags,
		void** ppData);*/

	vkResult = vkMapMemory(vkDevice, vetextData_color_street_light_lamp.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVertexBuffer() -> vkMapMemory() is failed for vetextData_color_street_light_lamp due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createVertexBuffer() -> vkMapMemory() for vetextData_color_street_light_lamp \n");
	}

	//Step 12: now to do actually memeory mapped IO call memcpy()
	memcpy(data, street_lamp_light_color, sizeof(street_lamp_light_color));

	//Step 13: To complete this memory mapped IO finally call vkUnMapMemory()
	/*void vkUnmapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory);*/

	vkUnmapMemory(vkDevice, vetextData_color_street_light_lamp.vkDeviceMemory);


	fprintf(gpFile, "\n=======================================================================================\n");
	return vkResult;
}

VkResult createUniformBuffer(void) 
{
	// function declaration
	VkResult updateUniformBuffer(void);

	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code

	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createUniformBuffer() : 25th June 2025\n\n");

	VkBufferCreateInfo vkBufferCreateInfo;
	memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vkBufferCreateInfo.pNext = NULL;
	vkBufferCreateInfo.flags = 0; // valid flags used in scattered buffer
	vkBufferCreateInfo.size = sizeof(struct MyUniformData);
	vkBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	//=================================street_light_middle_bar===================================================================
	// memset of unformm data
	memset((void*)&uniformData_street_light_middle_bar, 0, sizeof(UniformData));

	vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &uniformData_street_light_middle_bar.vkBuffer);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkCreateBuffer() is failed due to error : %d FOR street_light_middle_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkCreateBuffer()  FOR street_light_middle_bar\n");
	}

	//Step 07: declare and memset struct VkMemoryRequirements and then call , vkGetBufferMemeoryRequirement() to get the memory requirement.
	/*typedef struct VkMemoryRequirements {
		VkDeviceSize    size;
		VkDeviceSize    alignment;
		uint32_t        memoryTypeBits;
	} VkMemoryRequirements;*/

	VkMemoryRequirements vkMemoryRequirements;
	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

	/*void vkGetBufferMemoryRequirements(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		VkMemoryRequirements * pMemoryRequirements);*/

	vkGetBufferMemoryRequirements(vkDevice, uniformData_street_light_middle_bar.vkBuffer, &vkMemoryRequirements);

	/*Step 08:	To actually required memory we need to call vkAllocateMemory().but before that we need to declareand memset stuct VkMemoryAllocateInfo.
				Important members of this structure are = ".memoryTypeIndex" and ".allocationSize".
				for allocation size use the size obtained from memory requirements.
				For memory type index : start a loop with counter as vkPhysicalDeviceMemoryProperties.memoryTypeCount
				b.Inside the loop check vkMemoryRequirement.memoryTypeBits contain 1 or not.If yes
				c.check vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags member contains VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT. if yes
				d.then ith index will be our memeory type index.If found break out of the loop.
				e. if not continue the loop by right shiffting vkMemoryRequirements.memoryTypeBits  by 1, over each iteration.
	*/

	/*typedef struct VkMemoryAllocateInfo {
		VkStructureType    sType;
		const void* pNext;
		VkDeviceSize       allocationSize;
		uint32_t           memoryTypeIndex;
	} VkMemoryAllocateInfo;*/

	VkMemoryAllocateInfo vkMemoryAllocateInfo;
	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext = NULL;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}

	//Step 09: call vkAllocateMemory() and get the required vulkan memory handle into the ".vkDeviceMemory" member of our global member
	/*VkResult vkAllocateMemory(
		VkDevice                                    device,
		const VkMemoryAllocateInfo * pAllocateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDeviceMemory * pMemory);*/

	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData_street_light_middle_bar.vkDeviceMemory);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkAllocateMemory() is failed due to error : %d  FOR street_light_middle_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkAllocateMemory()  FOR street_light_middle_bar \n");
	}

	/*Step 10: now we have our required device memeory handle as well as our vkBufferHandle.
			   Bind device memeory handle to vulkan memory handle by using vkBindBUfferMemory();*/
			   /*VkResult vkBindBufferMemory(
				   VkDevice                                    device,
				   VkBuffer                                    buffer,
				   VkDeviceMemory                              memory,
				   VkDeviceSize                                memoryOffset);*/

	vkResult = vkBindBufferMemory(vkDevice, uniformData_street_light_middle_bar.vkBuffer, uniformData_street_light_middle_bar.vkDeviceMemory, 0);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkBindBufferMemory() is failed due to error : %d  FOR street_light_middle_bar\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkBindBufferMemory()  FOR street_light_middle_bar\n");
	}


	//===============================street_light_lamp================================================

	// For 1st 
	// memset of unformm data
	memset((void*)&uniformData_street_light_lamp_1, 0, sizeof(UniformData));

	vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &uniformData_street_light_lamp_1.vkBuffer);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkCreateBuffer() is failed due to error : %d FOR street_light_lamp \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkCreateBuffer() FOR street_light_lamp  \n");
	}

	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

	vkGetBufferMemoryRequirements(vkDevice, uniformData_street_light_lamp_1.vkBuffer, &vkMemoryRequirements);

	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext = NULL;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}

	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData_street_light_lamp_1.vkDeviceMemory);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkAllocateMemory() is failed due to error : %d FOR street_light_lamp \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkAllocateMemory() FOR street_light_lamp \n");
	}

	vkResult = vkBindBufferMemory(vkDevice, uniformData_street_light_lamp_1.vkBuffer, uniformData_street_light_lamp_1.vkDeviceMemory, 0);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkBindBufferMemory() is failed due to error : %d FOR street_light_lamp  \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkBindBufferMemory() FOR street_light_lamp  \n");
	}

	// for 2nd rectangle
	// memset of unformm data
	memset((void*)&uniformData_street_light_lamp_2, 0, sizeof(UniformData));

	vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &uniformData_street_light_lamp_2.vkBuffer);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkCreateBuffer() is failed due to error : %d FOR street_light_lamp 2 \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkCreateBuffer() FOR street_light_lamp  2\n");
	}

	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));

	vkGetBufferMemoryRequirements(vkDevice, uniformData_street_light_lamp_2.vkBuffer, &vkMemoryRequirements);

	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext = NULL;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}

	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData_street_light_lamp_2.vkDeviceMemory);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkAllocateMemory() is failed due to error : %d FOR street_light_lamp 2 \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkAllocateMemory() FOR street_light_lamp 2\n");
	}

	vkResult = vkBindBufferMemory(vkDevice, uniformData_street_light_lamp_2.vkBuffer, uniformData_street_light_lamp_2.vkDeviceMemory, 0);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkBindBufferMemory() is failed due to error : %d FOR street_light_lamp 2 \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkBindBufferMemory() FOR street_light_lamp 2 \n");
	}

	//============================================== uniformData_street_light_horizontal_top_bar  =============================================================================
	

	memset((void*)&uniformData_street_light_horizontal_top_bar, 0, sizeof(UniformData));

	vkResult = vkCreateBuffer(vkDevice, &vkBufferCreateInfo, NULL, &uniformData_street_light_horizontal_top_bar.vkBuffer);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkCreateBuffer() is failed due to error : %d FOR uniformData_street_light_horizontal_top_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkCreateBuffer()  FOR uniformData_street_light_horizontal_top_bar\n");
	}

	memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
	vkGetBufferMemoryRequirements(vkDevice, uniformData_street_light_horizontal_top_bar.vkBuffer, &vkMemoryRequirements);

	memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.pNext = NULL;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = 0; // initialise value before entering in the loop

	for (uint32_t i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
		{
			if (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				vkMemoryAllocateInfo.memoryTypeIndex = i;
				break;
			}
		}
		else
		{
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}
	}
	
	vkResult = vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData_street_light_horizontal_top_bar.vkDeviceMemory);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkAllocateMemory() is failed due to error : %d  FOR uniformData_street_light_horizontal_top_bar \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkAllocateMemory()  FOR uniformData_street_light_horizontal_top_bar \n");
	}

	vkResult = vkBindBufferMemory(vkDevice, uniformData_street_light_horizontal_top_bar.vkBuffer, uniformData_street_light_horizontal_top_bar.vkDeviceMemory, 0);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> vkBindBufferMemory() is failed due to error : %d  FOR uniformData_street_light_horizontal_top_bar\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> vkBindBufferMemory()  FOR uniformData_street_light_horizontal_top_bar\n");
	}



	//=========================================================================================================================================================================
	// call updateUnformBuffer
	vkResult = updateUniformBuffer();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createUniformBuffer() -> updateUniformBuffer() is failed due to error : %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createUniformBuffer() -> updateUniformBuffer() \n");
	}

	fprintf(gpFile, "\n=======================================================================================\n");

	return vkResult;
}

VkResult updateUniformBuffer(void)
{
	// variable declaration

	VkResult vkResult = VK_SUCCESS;
	
	// code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "updateUniformBuffer() : 25th June 2025\n\n");

	//=================================street_light_middle_bar========================================================
	MyUniformData myUniformData;
	memset((void*)&myUniformData, 0, sizeof(MyUniformData));

	//update matrices
	myUniformData.modelMatrix	   = glm::mat4(1.0); // identity matrix

	glm::mat4 translationMatrix    = glm::mat4(1.0);
	translationMatrix			   = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -12.0f));

	glm::mat4 scaleMatrix	       = glm::mat4(1.0);
	scaleMatrix				       = glm::scale(glm::mat4(1.0), glm::vec3(0.35, 3.0, 0.0));

	myUniformData.modelMatrix	   = translationMatrix * scaleMatrix;

	myUniformData.viewMatrix	   = glm::mat4(1.0); 
	//myUniformData.projectionMatrix = glm::mat4(1.0); 
	glm::mat4 perspectiveProjectionMatrix = glm::mat4(1.0);
	
	//********************			IMP 
	//glm follows coloum major matrix just like opengl BUT unlike opengl its matrix array is 2d array is 4*4 not one d array of 16 elements

	//perspective la call
	perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth/(float)winHeight, 0.1f, 100.0f );
	perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f); // 2n/t-b

	myUniformData.projectionMatrix = perspectiveProjectionMatrix;
	
	// map uniform buffer
	void* data = NULL;
	vkResult = vkMapMemory(vkDevice, uniformData_street_light_middle_bar.vkDeviceMemory, 0, sizeof(MyUniformData), 0, &data);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "updateUniformBuffer() -> vkMapMemory() is failed due to error : %d FOR street_light_middle_bar \n", vkResult);
		return (vkResult);
	}

	// copy the data to the mapped buffer
	memcpy(data, &myUniformData, sizeof(MyUniformData));

	// unmap memory
	vkUnmapMemory(vkDevice, uniformData_street_light_middle_bar.vkDeviceMemory);


	//================================street_light_lamp =======================================================

	memset((void*)&myUniformData, 0, sizeof(MyUniformData));

	//update matrices
	myUniformData.modelMatrix = glm::mat4(1.0); // identity matrix

	translationMatrix		  = glm::mat4(1.0);
	translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-3.0f, 3.0f, -12.0f));

	scaleMatrix = glm::mat4(1.0);
	scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.8, 0.0));

	myUniformData.modelMatrix = translationMatrix * scaleMatrix;

	myUniformData.viewMatrix = glm::mat4(1.0);
	//myUniformData.projectionMatrix = glm::mat4(1.0); 
	perspectiveProjectionMatrix = glm::mat4(1.0);

	//********************			IMP 
	//glm follows coloum major matrix just like opengl BUT unlike opengl its matrix array is 2d array is 4*4 not one d array of 16 elements

	//perspective la call
	perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);

	perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f); // 2n/t-b

	myUniformData.projectionMatrix = perspectiveProjectionMatrix;

	// map uniform buffer
	data = NULL;
	vkResult = vkMapMemory(vkDevice, uniformData_street_light_lamp_1.vkDeviceMemory, 0, sizeof(MyUniformData), 0, &data);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "updateUniformBuffer() -> vkMapMemory() is failed due to error : %d FOR street_light_lamp\n", vkResult);
		return (vkResult);
	}

	// copy the data to the mapped buffer
	memcpy(data, &myUniformData, sizeof(MyUniformData));

	// unmap memory
	vkUnmapMemory(vkDevice, uniformData_street_light_lamp_1.vkDeviceMemory);


	// Another Shape
	//================================street_light_lamp =======================================================

	memset((void*)&myUniformData, 0, sizeof(MyUniformData));

	//update matrices
	myUniformData.modelMatrix = glm::mat4(1.0); // identity matrix
	

	translationMatrix = glm::mat4(1.0);
	translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(3.0f, 3.0f, -12.0f));

	scaleMatrix = glm::mat4(1.0);
	scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.8, 0.0));

	myUniformData.modelMatrix = translationMatrix * scaleMatrix;

	myUniformData.viewMatrix = glm::mat4(1.0);
	//myUniformData.projectionMatrix = glm::mat4(1.0); 
	perspectiveProjectionMatrix = glm::mat4(1.0);

	//********************			IMP 
	//glm follows coloum major matrix just like opengl BUT unlike opengl its matrix array is 2d array is 4*4 not one d array of 16 elements

	//perspective la call
	perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);

	perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f); // 2n/t-b

	myUniformData.projectionMatrix = perspectiveProjectionMatrix;

	// map uniform buffer
	data = NULL;
	vkResult = vkMapMemory(vkDevice, uniformData_street_light_lamp_2.vkDeviceMemory, 0, sizeof(MyUniformData), 0, &data);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "updateUniformBuffer() -> vkMapMemory() is failed due to error : %d FOR street_light_lamp 2\n", vkResult);
		return (vkResult);
	}

	// copy the data to the mapped buffer
	memcpy(data, &myUniformData, sizeof(MyUniformData));

	// unmap memory
	vkUnmapMemory(vkDevice, uniformData_street_light_lamp_2.vkDeviceMemory);


	//=================================street_light_horizontal_bar========================================================
	
	memset((void*)&myUniformData, 0, sizeof(MyUniformData));

	//update matrices
	myUniformData.modelMatrix = glm::mat4(1.0); // identity matrix

	translationMatrix = glm::mat4(1.0);
	translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 3.0f, -12.0f));

	scaleMatrix = glm::mat4(1.0);
	scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(2.5, 0.2, 0.0));

	myUniformData.modelMatrix = translationMatrix * scaleMatrix;

	myUniformData.viewMatrix = glm::mat4(1.0);
	//myUniformData.projectionMatrix = glm::mat4(1.0); 
	perspectiveProjectionMatrix = glm::mat4(1.0);

	//********************			IMP 
	//glm follows coloum major matrix just like opengl BUT unlike opengl its matrix array is 2d array is 4*4 not one d array of 16 elements

	//perspective la call
	perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
	perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f); // 2n/t-b

	myUniformData.projectionMatrix = perspectiveProjectionMatrix;

	// map uniform buffer
	data = NULL;
	vkResult = vkMapMemory(vkDevice, uniformData_street_light_horizontal_top_bar.vkDeviceMemory, 0, sizeof(MyUniformData), 0, &data);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "updateUniformBuffer() -> vkMapMemory() is failed due to error : %d FOR street_light_horizontal_bar  \n", vkResult);
		return (vkResult);
	}

	// copy the data to the mapped buffer
	memcpy(data, &myUniformData, sizeof(MyUniformData));

	// unmap memory
	vkUnmapMemory(vkDevice, uniformData_street_light_horizontal_top_bar.vkDeviceMemory);


	fprintf(gpFile, "\n=======================================================================================\n");

	return vkResult;
}

VkResult createShaders(void) 
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createShaders() : 23rd April 2025\n\n");

	// For vertex shader
	//Step 06: Inside our function first open the shader file, set the file pointer at the end of file, find the bytesize of the shader file data, reset the file pointer at the
	//		   begining of the file, allocate A character buffer of file sizeand read shader file data into itand finally close the file.DO all this things using conventional file IO

	const char* szFileName = "./shaders/Shader.vert.spv";
	FILE* fp = NULL;
	size_t size;

	fp = fopen(szFileName, "rb"); // rb :read sathi open kara binary read karyla

	if (fp == NULL)
	{
		fprintf(gpFile, "createShaders() -> failed to open vertex.shader.spv file \n");
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createShaders() -> scuccesed to open vertex.shader.spv file \n" );
	}

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);

	if (size == 0)
	{
		fprintf(gpFile, "createShaders() -> size of vertex.shader.spv file size zero \n" );
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}

	fseek(fp, 0L, SEEK_SET); // prt reset kr starting

	char* shaderData = (char*) malloc (sizeof(char) * size);
	size_t retVal = fread(shaderData, size, 1, fp);

	if (retVal != 1)
	{
		fprintf(gpFile, "createShaders() -> failed to read vertex.shader.spv file \n");
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createShaders() -> scuccesed to read vertex.shader.spv file \n");
	}

	fclose(fp);

	//Step 07: Declare and memset struct VkShaderModuleCreateInfo and specify above file size and buffer while initialising it

	/*typedef struct VkShaderModuleCreateInfo {
		VkStructureType              sType;
		const void* pNext;
		VkShaderModuleCreateFlags    flags;
		size_t                       codeSize;
		const uint32_t* pCode;
	} VkShaderModuleCreateInfo;*/

	VkShaderModuleCreateInfo vkShaderModuleCreateInfo;
	memset((void *) &vkShaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));

	vkShaderModuleCreateInfo.sType		 = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vkShaderModuleCreateInfo.pNext		 = NULL;
	vkShaderModuleCreateInfo.flags		 = 0; // reserved for future use hence 0
	vkShaderModuleCreateInfo.codeSize	 = size;
	vkShaderModuleCreateInfo.pCode		 = (uint32_t*) shaderData;

	//Step 08: Call vkCreateShaderModule() vulkan API, pass above struct pointer to it as parameter and obtain shader module object in global variable that we declare in step 2

	/*VkResult vkCreateShaderModule(
		VkDevice                                    device,
		const VkShaderModuleCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkShaderModule * pShaderModule);*/

	vkResult = vkCreateShaderModule(vkDevice, &vkShaderModuleCreateInfo, NULL, &vkShaderModule_vertex_shader);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createShaders() -> vkCreateShaderModule() is failed : %d\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createShaders() -> vkCreateShaderModule() successed \n");
	}

	//Step 09: Free the shader code buffer which we allocated in step 06
	if (shaderData)
	{
		free(shaderData);
		shaderData = NULL;
	}

	fprintf(gpFile, "createShaders() -> VERTEX SHADER MODULE SUCCESSFULLY CREATED \n");

	//======================================================================================
	//							For fragment shader
	
	szFileName = "./shaders/Shader.frag.spv";
	size = 0;
	fp = NULL;

	fp = fopen(szFileName, "rb"); // rb :read sathi open kara binary read karyla

	if (fp == NULL)
	{
		fprintf(gpFile, "createShaders() -> failed to open fragment.shader.spv file \n" );
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createShaders() -> scuccesed to open fragment.shader.spv file \n");
	}

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);

	fseek(fp, 0L, SEEK_SET); // prt reset kr starting

	if (size == 0)
	{
		fprintf(gpFile, "createShaders() -> size of fragment.shader.spv file size zero \n" );
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}

	shaderData = (char*)malloc(sizeof(char) * size);
	retVal = fread(shaderData, size, 1, fp);

	if (retVal != 1)
	{
		fprintf(gpFile, "createShaders() -> failed to read fragment.shader.spv file \n" );
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createShaders() -> scuccesed to read fragment.shader.spv file \n");
	}

	fclose(fp);

	//Step 07: Declare and memset struct VkShaderModuleCreateInfo and specify above file size and buffer while initialising it

	memset((void*)&vkShaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));

	vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vkShaderModuleCreateInfo.pNext = NULL;
	vkShaderModuleCreateInfo.flags = 0; // reserved for future use hence 0
	vkShaderModuleCreateInfo.codeSize = size;
	vkShaderModuleCreateInfo.pCode = (uint32_t*)shaderData;

	//Step 08: Call vkCreateShaderModule() vulkan API, pass above struct pointer to it as parameter and obtain shader module object in global variable that we declare in step 2

	vkResult = vkCreateShaderModule(vkDevice, &vkShaderModuleCreateInfo, NULL, &vkShaderModule_fragment_shader);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createShaders() -> vkCreateShaderModule() is failed for fragment shader: %d\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createShaders() -> vkCreateShaderModule() successed for fragment shader \n");
	}

	//Step 09: Free the shader code buffer which we allocated in step 06
	if (shaderData)
	{
		free(shaderData);
		shaderData = NULL;
	}

	fprintf(gpFile, "createShaders() -> FRAGMENT SHADER MODULE SUCCESSFULLY CREATED \n");



	fprintf(gpFile, "\n=======================================================================================\n");
	return vkResult;
}

VkResult createDecriptorSetLayout(void) // rikami plate
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createDecriptorSetLayout() : 30th April 2025\n\n");

	// initialise descriptor set binding
	/*typedef struct VkDescriptorSetLayoutBinding {
		uint32_t              binding;
		VkDescriptorType      descriptorType;
		uint32_t              descriptorCount;
		VkShaderStageFlags    stageFlags;
		const VkSampler* pImmutableSamplers;
	} VkDescriptorSetLayoutBinding;*/

	VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding;
	memset((void*)&vkDescriptorSetLayoutBinding, 0, sizeof(VkDescriptorSetLayoutBinding));

	vkDescriptorSetLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vkDescriptorSetLayoutBinding.binding		    = 0; // this 0 is related with vertex shader in binding = 0
	vkDescriptorSetLayoutBinding.descriptorCount    = 1;
	vkDescriptorSetLayoutBinding.stageFlags		    = VK_SHADER_STAGE_VERTEX_BIT; // stages = shader ch nav(vertex, fragment)
	vkDescriptorSetLayoutBinding.pImmutableSamplers = NULL;

	//Step 03: while writing this UDF declare, memset and initialise struct VkDescriptorLayoutCreateInfo particulare 2 members 1. bindingCount. 2. pBindings array
	/*typedef struct VkDescriptorSetLayoutCreateInfo {
		VkStructureType                        sType;
		const void* pNext;
		VkDescriptorSetLayoutCreateFlags       flags;
		uint32_t                               bindingCount;
		const VkDescriptorSetLayoutBinding* pBindings;
	} VkDescriptorSetLayoutCreateInfo;*/

	VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo;
	memset((void *)&vkDescriptorSetLayoutCreateInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));

	vkDescriptorSetLayoutCreateInfo.sType		 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	vkDescriptorSetLayoutCreateInfo.pNext		 = NULL;
	vkDescriptorSetLayoutCreateInfo.flags		 = 0;
	vkDescriptorSetLayoutCreateInfo.bindingCount = 1;
	vkDescriptorSetLayoutCreateInfo.pBindings	 = &vkDescriptorSetLayoutBinding;

	//uint32_t binding;
	//VkDescriptorType descriptorType;

	//Step 04: then call vkCreateDescriptorSetLayout() vulkan api with address of above initialised struct and get the required global vulkan object VkDescriptorLayout  in its last parameter
	
	/*VkResult vkCreateDescriptorSetLayout(
		VkDevice                                    device,
		const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDescriptorSetLayout * pSetLayout);*/

	vkResult = vkCreateDescriptorSetLayout(vkDevice, &vkDescriptorSetLayoutCreateInfo, NULL, &vkDescriptorSetLayout);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createDecriptorSetLayout() -> createDecriptorSetLayout() is failed :  %d\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createDecriptorSetLayout() -> createDecriptorSetLayout() successed\n");
	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return vkResult;
}

VkResult createPipelineLayout(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createDecriptorSetLayout() : 30th April 2025\n\n");

	/*typedef struct VkPipelineLayoutCreateInfo {
		VkStructureType                 sType;
		const void* pNext;
		VkPipelineLayoutCreateFlags     flags;
		uint32_t                        setLayoutCount;
		const VkDescriptorSetLayout* pSetLayouts;
		uint32_t                        pushConstantRangeCount;
		const VkPushConstantRange* pPushConstantRanges;
	} VkPipelineLayoutCreateInfo;*/

	VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo;
	memset((void*)&vkPipelineLayoutCreateInfo, 0, sizeof(VkPipelineLayoutCreateInfo));

	vkPipelineLayoutCreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	vkPipelineLayoutCreateInfo.pNext					= NULL;
	vkPipelineLayoutCreateInfo.flags					= 0;
	vkPipelineLayoutCreateInfo.setLayoutCount			= 1;
	vkPipelineLayoutCreateInfo.pSetLayouts				= &vkDescriptorSetLayout;
	vkPipelineLayoutCreateInfo.pushConstantRangeCount	= 0;
	vkPipelineLayoutCreateInfo.pPushConstantRanges		= NULL;

	//step 04: then call vkCreatePipeLineLayout() vulkan api with address of above initialised struct and get the required global vulkan object VkPipeLineLayout in its last parameter

	vkResult = vkCreatePipelineLayout(vkDevice, &vkPipelineLayoutCreateInfo, NULL, &vkPipelineLayout); //vkPipeLineCreateInfo
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createPipelineLayout() -> vkCreatePipeLineLayout() is failed :  %d\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createPipelineLayout() -> vkCreatePipeLineLayout() successed\n");
	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return vkResult;
}

VkResult createDescriptorPool(void)
{
	VkResult vkResult = VK_SUCCESS;

	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createDescriptorPool() : 25th June 2025\n\n");

	// code

	// before creating actual descriptor pool, vulkan expects descriptor pool size
	/*typedef struct VkDescriptorPoolSize {
		VkDescriptorType    type;
		uint32_t            descriptorCount;
	} VkDescriptorPoolSize;*/

	VkDescriptorPoolSize vkDescriptorPoolSize;
	memset((void*)&vkDescriptorPoolSize, 0, sizeof(VkDescriptorPoolSize));

	vkDescriptorPoolSize.type			 = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vkDescriptorPoolSize.descriptorCount = 1;

	// create pool
	/*typedef struct VkDescriptorPoolCreateInfo {
		VkStructureType                sType;
		const void* pNext;
		VkDescriptorPoolCreateFlags    flags;
		uint32_t                       maxSets;
		uint32_t                       poolSizeCount;
		const VkDescriptorPoolSize* pPoolSizes;
	} VkDescriptorPoolCreateInfo;*/

	VkDescriptorPoolCreateInfo vkDescriptorPoolCreateInfo;
	memset((void*)&vkDescriptorPoolCreateInfo, 0, sizeof(VkDescriptorPoolCreateInfo));

	vkDescriptorPoolCreateInfo.sType		 = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	vkDescriptorPoolCreateInfo.pNext		 = NULL;
	vkDescriptorPoolCreateInfo.flags	 	 = 0;
	vkDescriptorPoolCreateInfo.poolSizeCount = 1; // array pn deu shakto
	vkDescriptorPoolCreateInfo.pPoolSizes	 = &vkDescriptorPoolSize;
	vkDescriptorPoolCreateInfo.maxSets		 = 4;

	/*VkResult vkCreateDescriptorPool(
		VkDevice                                    device,
		const VkDescriptorPoolCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDescriptorPool * pDescriptorPool);*/

	vkResult = vkCreateDescriptorPool(vkDevice, &vkDescriptorPoolCreateInfo, NULL, &vkDescriptorPool);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createDescriptorPool() -> vkCreateDescriptorPool() is failed %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createDescriptorPool() -> vkCreateDescriptorPool() successed\n");
	}

	fprintf(gpFile, "\n=======================================================================================\n");

	return vkResult;
}

VkResult createDescriptorSet(void)
{
	VkResult vkResult = VK_SUCCESS;

	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createDescriptorSet() : 25th June 2025\n\n");

	// initialise descriptor set allocation info
	/*typedef struct VkDescriptorSetAllocateInfo {
		VkStructureType                 sType;
		const void* pNext;
		VkDescriptorPool                descriptorPool;
		uint32_t                        descriptorSetCount;
		const VkDescriptorSetLayout* pSetLayouts;
	} VkDescriptorSetAllocateInfo;*/

	//=============================street_light_middle_bar=================================================

	VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo;
	memset((void*)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));

	vkDescriptorSetAllocateInfo.sType				 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	vkDescriptorSetAllocateInfo.pNext				 = NULL;
	vkDescriptorSetAllocateInfo.descriptorPool		 = vkDescriptorPool;
	vkDescriptorSetAllocateInfo.descriptorSetCount	 = 1;
	vkDescriptorSetAllocateInfo.pSetLayouts			 = &vkDescriptorSetLayout;

	/*VkResult vkAllocateDescriptorSets(
		VkDevice                                    device,
		const VkDescriptorSetAllocateInfo * pAllocateInfo,
		VkDescriptorSet * pDescriptorSets);*/
	vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet_street_light_middle_bar);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createDescriptorSet() -> vkAllocateDescriptorSets() is failed %d for triangle \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createDescriptorSet() -> vkAllocateDescriptorSets() successed for triangle \n");
	}

	// describe whether we want buffer as uniform or image as uniform. 

	/*typedef struct VkDescriptorBufferInfo {
		VkBuffer        buffer;
		VkDeviceSize    offset;
		VkDeviceSize    range;
	} VkDescriptorBufferInfo;*/

	VkDescriptorBufferInfo vkDescriptorBufferInfo;
	memset((void*)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));

	vkDescriptorBufferInfo.buffer = uniformData_street_light_middle_bar.vkBuffer;
	vkDescriptorBufferInfo.offset = 0;
	vkDescriptorBufferInfo.range  = sizeof(MyUniformData);

	// now update the above descriptor directly to the shader
	// there are two ways to update. 1. Writtingdirectly to the shader or copying from one shader to another shader
	// we will perfer directly writting to the shader. this required initialisation of following structure

	/*typedef struct VkWriteDescriptorSet {
		VkStructureType                  sType;
		const void* pNext;
		VkDescriptorSet                  dstSet;
		uint32_t                         dstBinding;
		uint32_t                         dstArrayElement;
		uint32_t                         descriptorCount;
		VkDescriptorType                 descriptorType;
		const VkDescriptorImageInfo* pImageInfo;
		const VkDescriptorBufferInfo* pBufferInfo;
		const VkBufferView* pTexelBufferView;
	} VkWriteDescriptorSet;*/

	VkWriteDescriptorSet vkWriteDescriptorSet;
	memset((void*)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

	vkWriteDescriptorSet.sType			  = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vkWriteDescriptorSet.pNext			  = NULL;
	vkWriteDescriptorSet.dstSet			  = vkDescriptorSet_street_light_middle_bar; //ISSUE
	vkWriteDescriptorSet.dstBinding		  = 0; // bcouz our uniform binding at 0th indexin shader
	vkWriteDescriptorSet.dstArrayElement  = 0;
	vkWriteDescriptorSet.descriptorCount  = 1;
	vkWriteDescriptorSet.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vkWriteDescriptorSet.pImageInfo		  = NULL;
	vkWriteDescriptorSet.pBufferInfo	  = &vkDescriptorBufferInfo;
	vkWriteDescriptorSet.pTexelBufferView = NULL; // texture chya veli lagel

	/*void vkUpdateDescriptorSets(
		VkDevice                                    device,
		uint32_t                                    descriptorWriteCount,
		const VkWriteDescriptorSet * pDescriptorWrites,
		uint32_t                                    descriptorCopyCount,
		const VkCopyDescriptorSet * pDescriptorCopies);*/

	vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);

	//================================================street_light_lamp============================
	// 1

	memset((void*)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));

	vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	vkDescriptorSetAllocateInfo.pNext = NULL;
	vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool;
	vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
	vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout;

	/*VkResult vkAllocateDescriptorSets(
		VkDevice                                    device,
		const VkDescriptorSetAllocateInfo * pAllocateInfo,
		VkDescriptorSet * pDescriptorSets);*/
	vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet_street_light_lamp_1);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createDescriptorSet() -> vkAllocateDescriptorSets() is failed %d FOR street_light_lamp \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createDescriptorSet() -> vkAllocateDescriptorSets() successed FOR street_light_lamp \n");
	}

	// describe whether we want buffer as uniform or image as uniform. 

	/*typedef struct VkDescriptorBufferInfo {
		VkBuffer        buffer;
		VkDeviceSize    offset;
		VkDeviceSize    range;
	} VkDescriptorBufferInfo;*/

	memset((void*)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));

	vkDescriptorBufferInfo.buffer = uniformData_street_light_lamp_1.vkBuffer;
	vkDescriptorBufferInfo.offset = 0;
	vkDescriptorBufferInfo.range = sizeof(MyUniformData);

	// now update the above descriptor directly to the shader
	// there are two ways to update. 1. Writtingdirectly to the shader or copying from one shader to another shader
	// we will perfer directly writting to the shader. this required initialisation of following structure

	/*typedef struct VkWriteDescriptorSet {
		VkStructureType                  sType;
		const void* pNext;
		VkDescriptorSet                  dstSet;
		uint32_t                         dstBinding;
		uint32_t                         dstArrayElement;
		uint32_t                         descriptorCount;
		VkDescriptorType                 descriptorType;
		const VkDescriptorImageInfo* pImageInfo;
		const VkDescriptorBufferInfo* pBufferInfo;
		const VkBufferView* pTexelBufferView;
	} VkWriteDescriptorSet;*/

	
	memset((void*)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

	vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vkWriteDescriptorSet.pNext = NULL;
	vkWriteDescriptorSet.dstSet = vkDescriptorSet_street_light_lamp_1; //ISSUE
	vkWriteDescriptorSet.dstBinding = 0; // bcouz our uniform binding at 0th indexin shader
	vkWriteDescriptorSet.dstArrayElement = 0;
	vkWriteDescriptorSet.descriptorCount = 1;
	vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vkWriteDescriptorSet.pImageInfo = NULL;
	vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
	vkWriteDescriptorSet.pTexelBufferView = NULL; // texture chya veli lagel

	/*void vkUpdateDescriptorSets(
		VkDevice                                    device,
		uint32_t                                    descriptorWriteCount,
		const VkWriteDescriptorSet * pDescriptorWrites,
		uint32_t                                    descriptorCopyCount,
		const VkCopyDescriptorSet * pDescriptorCopies);*/

	vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);

	// 2 Rectangle

	memset((void*)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));

	vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	vkDescriptorSetAllocateInfo.pNext = NULL;
	vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool;
	vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
	vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout;

	vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet_street_light_lamp_2);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createDescriptorSet() -> vkAllocateDescriptorSets() is failed %d FOR street_light_lamp 2\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createDescriptorSet() -> vkAllocateDescriptorSets() successed FOR street_light_lamp 2\n");
	}

	// describe whether we want buffer as uniform or image as uniform. 
	 
	memset((void*)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));

	vkDescriptorBufferInfo.buffer = uniformData_street_light_lamp_2.vkBuffer;
	vkDescriptorBufferInfo.offset = 0;
	vkDescriptorBufferInfo.range = sizeof(MyUniformData);

	
	memset((void*)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

	vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vkWriteDescriptorSet.pNext = NULL;
	vkWriteDescriptorSet.dstSet = vkDescriptorSet_street_light_lamp_2; //ISSUE
	vkWriteDescriptorSet.dstBinding = 0; // bcouz our uniform binding at 0th indexin shader
	vkWriteDescriptorSet.dstArrayElement = 0;
	vkWriteDescriptorSet.descriptorCount = 1;
	vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vkWriteDescriptorSet.pImageInfo = NULL;
	vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
	vkWriteDescriptorSet.pTexelBufferView = NULL; // texture chya veli lagel

	/*void vkUpdateDescriptorSets(
		VkDevice                                    device,
		uint32_t                                    descriptorWriteCount,
		const VkWriteDescriptorSet * pDescriptorWrites,
		uint32_t                                    descriptorCopyCount,
		const VkCopyDescriptorSet * pDescriptorCopies);*/

	vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);


	//=============================street_light_horizontal_top_bar=================================================

	
	memset((void*)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));

	vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	vkDescriptorSetAllocateInfo.pNext = NULL;
	vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool;
	vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
	vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout;

	vkResult = vkAllocateDescriptorSets(vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet_street_light_horizontal_top_bar);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createDescriptorSet() -> vkAllocateDescriptorSets() is failed %d for vkDescriptorSet_street_light_horizontal_top_bar  \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createDescriptorSet() -> vkAllocateDescriptorSets() successed for vkDescriptorSet_street_light_horizontal_top_bar  \n");
	}

	// describe whether we want buffer as uniform or image as uniform. 
	memset((void*)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));

	vkDescriptorBufferInfo.buffer = uniformData_street_light_horizontal_top_bar.vkBuffer;
	vkDescriptorBufferInfo.offset = 0;
	vkDescriptorBufferInfo.range = sizeof(MyUniformData);

	memset((void*)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

	vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vkWriteDescriptorSet.pNext = NULL;
	vkWriteDescriptorSet.dstSet = vkDescriptorSet_street_light_horizontal_top_bar; //ISSUE
	vkWriteDescriptorSet.dstBinding = 0; // bcouz our uniform binding at 0th indexin shader
	vkWriteDescriptorSet.dstArrayElement = 0;
	vkWriteDescriptorSet.descriptorCount = 1;
	vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vkWriteDescriptorSet.pImageInfo = NULL;
	vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
	vkWriteDescriptorSet.pTexelBufferView = NULL; // texture chya veli lagel

	/*void vkUpdateDescriptorSets(
		VkDevice                                    device,
		uint32_t                                    descriptorWriteCount,
		const VkWriteDescriptorSet * pDescriptorWrites,
		uint32_t                                    descriptorCopyCount,
		const VkCopyDescriptorSet * pDescriptorCopies);*/

	vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);

	fprintf(gpFile, "\n=======================================================================================\n");

	return vkResult;
}

VkResult createRenderPass(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "RenderPass() : 12th March 2025\n\n");

	//Step 01: declare and initialise VkAttachmentDesciption structure array.No.of element in array depends on no.of attachmet
	//		   although we have only one attachemnet color attachmenet for this example we will consider this as array

	/*typedef struct VkAttachmentDescription {
		VkAttachmentDescriptionFlags    flags;
		VkFormat                        format;
		VkSampleCountFlagBits           samples;
		VkAttachmentLoadOp              loadOp;
		VkAttachmentStoreOp             storeOp;
		VkAttachmentLoadOp              stencilLoadOp;
		VkAttachmentStoreOp             stencilStoreOp;
		VkImageLayout                   initialLayout;
		VkImageLayout                   finalLayout;
	} VkAttachmentDescription;*/

	VkAttachmentDescription vkAttachmentDescription_array[2];
	memset((void*)vkAttachmentDescription_array, 0, sizeof(VkAttachmentDescription) * _ARRAYSIZE(vkAttachmentDescription_array));

	// FOR COLOR
	vkAttachmentDescription_array[0].flags				= 0;
	vkAttachmentDescription_array[0].format				= vkFormat_color;
	vkAttachmentDescription_array[0].samples			= VK_SAMPLE_COUNT_1_BIT;
	vkAttachmentDescription_array[0].loadOp				= VK_ATTACHMENT_LOAD_OP_CLEAR;
	vkAttachmentDescription_array[0].storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
	vkAttachmentDescription_array[0].stencilLoadOp		= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	vkAttachmentDescription_array[0].stencilStoreOp		= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vkAttachmentDescription_array[0].initialLayout		= VK_IMAGE_LAYOUT_UNDEFINED;
	vkAttachmentDescription_array[0].finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// FOR DEPTH
	vkAttachmentDescription_array[1].flags = 0;
	vkAttachmentDescription_array[1].format = vkFormat_depth;
	vkAttachmentDescription_array[1].samples = VK_SAMPLE_COUNT_1_BIT;
	vkAttachmentDescription_array[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	vkAttachmentDescription_array[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	vkAttachmentDescription_array[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	vkAttachmentDescription_array[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vkAttachmentDescription_array[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vkAttachmentDescription_array[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//Step 02: declare and initialise VkAttachmentReference structure which will have information about the attchment we described above
	/*typedef struct VkAttachmentReference {
		uint32_t         attachment;
		VkImageLayout    layout;
	} VkAttachmentReference;*/

	// FOr Color Attachment
	VkAttachmentReference vkAttachmentReference_color;
	memset((void*)&vkAttachmentReference_color, 0, sizeof(VkAttachmentReference));

	vkAttachmentReference_color.attachment = 0; // 0 means oth index no. in above array. i.e color attachment
	vkAttachmentReference_color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// For depth Attachment
	VkAttachmentReference vkAttachmentReference_depth;
	memset((void*)&vkAttachmentReference_depth, 0, sizeof(VkAttachmentReference));

	vkAttachmentReference_depth.attachment = 1; // 1 means 1st index no. in above array. i.e depth attachment
	vkAttachmentReference_depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//Step 03: declare and initialise VkSubpassDescribtion structure and keep referebce about VkAttachmentReference structure

	/*typedef struct VkSubpassDescription {
		VkSubpassDescriptionFlags       flags;
		VkPipelineBindPoint             pipelineBindPoint;
		uint32_t                        inputAttachmentCount;
		const VkAttachmentReference* pInputAttachments;
		uint32_t                        colorAttachmentCount;
		const VkAttachmentReference* pColorAttachments;
		const VkAttachmentReference* pResolveAttachments;
		const VkAttachmentReference* pDepthStencilAttachment;
		uint32_t                        preserveAttachmentCount;
		const uint32_t* pPreserveAttachments;
	} VkSubpassDescription;*/


	VkSubpassDescription vkSubpassDescription;
	memset((void*)&vkSubpassDescription, 0, sizeof(VkSubpassDescription));

	vkSubpassDescription.flags					 = 0;
	vkSubpassDescription.pipelineBindPoint		 = VK_PIPELINE_BIND_POINT_GRAPHICS;
	vkSubpassDescription.inputAttachmentCount	 = 0;
	vkSubpassDescription.pInputAttachments		 = NULL;
	vkSubpassDescription.colorAttachmentCount	 = _ARRAYSIZE(vkAttachmentDescription_array);
	vkSubpassDescription.pColorAttachments		 = &vkAttachmentReference_color;
	vkSubpassDescription.pResolveAttachments	 = NULL;
	vkSubpassDescription.pDepthStencilAttachment = &vkAttachmentReference_depth;
	vkSubpassDescription.preserveAttachmentCount = 0;
	vkSubpassDescription.pPreserveAttachments	 = NULL;

	/*Step 04: declare and initialise VkRenderPassCreateInfo structure and refer VkAttachmentDesciption and VkSubpassDescribtion into it.
			   Remember here also we need attachment information in the form of image views.Which will be used by framebuffer later.We alos need to
			   specify inter dependance between subpass if needed*/

	/*typedef struct VkRenderPassCreateInfo {
		VkStructureType                   sType;
		const void* pNext;
		VkRenderPassCreateFlags           flags;
		uint32_t                          attachmentCount;
		const VkAttachmentDescription* pAttachments;
		uint32_t                          subpassCount;
		const VkSubpassDescription* pSubpasses;
		uint32_t                          dependencyCount;
		const VkSubpassDependency* pDependencies;
	} VkRenderPassCreateInfo;*/

	VkRenderPassCreateInfo vkRenderPassCreateInfo;
	memset((void*)&vkRenderPassCreateInfo, 0, sizeof(VkRenderPassCreateInfo));

	vkRenderPassCreateInfo.sType			 = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	vkRenderPassCreateInfo.pNext			 = NULL;
	vkRenderPassCreateInfo.flags			 = 0;
	vkRenderPassCreateInfo.attachmentCount	 = _ARRAYSIZE(vkAttachmentDescription_array);
	vkRenderPassCreateInfo.pAttachments		 = vkAttachmentDescription_array;
	vkRenderPassCreateInfo.subpassCount		 = 1;
	vkRenderPassCreateInfo.pSubpasses		 = &vkSubpassDescription;
	vkRenderPassCreateInfo.dependencyCount	 = 0;
	vkRenderPassCreateInfo.pDependencies	 = NULL;

	//Step 05: Now call vkCreateRenderPass() api to create the actual render pass.
	/*VkResult vkCreateRenderPass(
		VkDevice                                    device,
		const VkRenderPassCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkRenderPass* pRenderPass);*/

	vkResult = vkCreateRenderPass(vkDevice, &vkRenderPassCreateInfo, NULL, &vkRenderPass);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createRenderPass() -> vkCreateRenderPass() is failed %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createRenderPass() -> vkCreateRenderPass() successed\n");

	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return (vkResult);
}

VkResult createPipeline(void)
{
	// function declaration

	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n===========================================================================================================================\n");
	fprintf(gpFile, "createPipeline() : 07th May 2025\n\n");

	/*typedef struct VkVertexInputBindingDescription {
											uint32_t             binding;
											uint32_t             stride;
											VkVertexInputRate    inputRate;
	} VkVertexInputBindingDescription;*/

	VkVertexInputBindingDescription vkVertexInputBindingDescription_array[2];
	memset((void *)vkVertexInputBindingDescription_array, 0, sizeof(VkVertexInputBindingDescription) * _ARRAYSIZE(vkVertexInputBindingDescription_array));

	// position
	vkVertexInputBindingDescription_array[0].binding		 = 0;  //corresponding to layout(location 0) in vertex shader
	vkVertexInputBindingDescription_array[0].stride			 = sizeof(float) * 3;
	vkVertexInputBindingDescription_array[0].inputRate		 = VK_VERTEX_INPUT_RATE_VERTEX;

	// color
	vkVertexInputBindingDescription_array[1].binding		= 1;  //corresponding to layout(location 1) in vertex shader
	vkVertexInputBindingDescription_array[1].stride			= sizeof(float) * 3;
	vkVertexInputBindingDescription_array[1].inputRate		= VK_VERTEX_INPUT_RATE_VERTEX;

	/*typedef struct VkVertexInputAttributeDescription {
															uint32_t    location;
															uint32_t    binding;
															VkFormat    format;
															uint32_t    offset;
	} VkVertexInputAttributeDescription;*/

	VkVertexInputAttributeDescription vkVertexInputAttributeDescription_array[2];
	memset((void*)vkVertexInputAttributeDescription_array, 0, sizeof(VkVertexInputAttributeDescription) * _ARRAYSIZE(vkVertexInputAttributeDescription_array));

	// For Position
	vkVertexInputAttributeDescription_array[0].binding	 = 0;
	vkVertexInputAttributeDescription_array[0].location  = 0;
	vkVertexInputAttributeDescription_array[0].format	 = VK_FORMAT_R32G32B32_SFLOAT;
	vkVertexInputAttributeDescription_array[0].offset    = 0;

	// For Color
	vkVertexInputAttributeDescription_array[1].binding	 = 1;
	vkVertexInputAttributeDescription_array[1].location  = 1;
	vkVertexInputAttributeDescription_array[1].format	 = VK_FORMAT_R32G32B32_SFLOAT;
	vkVertexInputAttributeDescription_array[1].offset	 = 0;

	//PSO
	/*typedef struct VkPipelineVertexInputStateCreateInfo {
																VkStructureType                             sType;
																const void* pNext;
																VkPipelineVertexInputStateCreateFlags       flags;
																uint32_t                                    vertexBindingDescriptionCount;
																const VkVertexInputBindingDescription* pVertexBindingDescriptions;
																uint32_t                                    vertexAttributeDescriptionCount;
																const VkVertexInputAttributeDescription* pVertexAttributeDescriptions;
	} VkPipelineVertexInputStateCreateInfo;*/

	VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo;
	memset((void*)&vkPipelineVertexInputStateCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

	vkPipelineVertexInputStateCreateInfo.sType							 = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vkPipelineVertexInputStateCreateInfo.pNext							 = NULL;
	vkPipelineVertexInputStateCreateInfo.flags							 = 0;
	vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount	 = _ARRAYSIZE(vkVertexInputBindingDescription_array);
	vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions		 = vkVertexInputBindingDescription_array;
	vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = _ARRAYSIZE(vkVertexInputAttributeDescription_array);
	vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions	 = vkVertexInputAttributeDescription_array;

	// Input Assembly State
	/*typedef struct VkPipelineInputAssemblyStateCreateInfo {
		VkStructureType                            sType;
		const void* pNext;
		VkPipelineInputAssemblyStateCreateFlags    flags;
		VkPrimitiveTopology                        topology;
		VkBool32                                   primitiveRestartEnable;
	} VkPipelineInputAssemblyStateCreateInfo;*/

	VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo;
	memset((void*)&vkPipelineInputAssemblyStateCreateInfo, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));

	vkPipelineInputAssemblyStateCreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	vkPipelineInputAssemblyStateCreateInfo.pNext					= NULL;
	vkPipelineInputAssemblyStateCreateInfo.flags					= 0;
	vkPipelineInputAssemblyStateCreateInfo.topology					= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //
	
	// Rasterizer state
	/*typedef struct VkPipelineRasterizationStateCreateInfo {
																VkStructureType                            sType;
																const void*								   pNext;
																VkPipelineRasterizationStateCreateFlags    flags;
																VkBool32                                   depthClampEnable;
																VkBool32                                   rasterizerDiscardEnable;
																VkPolygonMode                              polygonMode;
																VkCullModeFlags                            cullMode;
																VkFrontFace                                frontFace;
																VkBool32                                   depthBiasEnable;
																float                                      depthBiasConstantFactor;
																float                                      depthBiasClamp;
																float                                      depthBiasSlopeFactor;
																float                                      lineWidth;
	} VkPipelineRasterizationStateCreateInfo;*/

	VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo;
	memset((void*)&vkPipelineRasterizationStateCreateInfo, 0, sizeof(VkPipelineRasterizationStateCreateInfo));

	vkPipelineRasterizationStateCreateInfo.sType					 = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	vkPipelineRasterizationStateCreateInfo.pNext					 = NULL;
	vkPipelineRasterizationStateCreateInfo.flags					 = 0;
	vkPipelineRasterizationStateCreateInfo.polygonMode				 = VK_POLYGON_MODE_FILL;
	vkPipelineRasterizationStateCreateInfo.cullMode					 = VK_CULL_MODE_BACK_BIT;
	vkPipelineRasterizationStateCreateInfo.frontFace				 = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	vkPipelineRasterizationStateCreateInfo.lineWidth				 = 1.0;

	// color blend state
	/*typedef struct VkPipelineColorBlendAttachmentState {
															VkBool32                 blendEnable;
															VkBlendFactor            srcColorBlendFactor;
															VkBlendFactor            dstColorBlendFactor;
															VkBlendOp                colorBlendOp;
															VkBlendFactor            srcAlphaBlendFactor;
															VkBlendFactor            dstAlphaBlendFactor;
															VkBlendOp                alphaBlendOp;
															VkColorComponentFlags    colorWriteMask;
	} VkPipelineColorBlendAttachmentState;*/

	VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState_array[1];
	memset((void*)vkPipelineColorBlendAttachmentState_array, 0, sizeof(VkPipelineColorBlendAttachmentState) * _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array));

	vkPipelineColorBlendAttachmentState_array[0].colorWriteMask			 = 0xf;
	vkPipelineColorBlendAttachmentState_array[0].blendEnable			 = VK_FALSE;
	
	/*typedef struct VkPipelineColorBlendStateCreateInfo {
		VkStructureType                               sType;
		const void* pNext;
		VkPipelineColorBlendStateCreateFlags          flags;
		VkBool32                                      logicOpEnable;
		VkLogicOp                                     logicOp;
		uint32_t                                      attachmentCount;
		const VkPipelineColorBlendAttachmentState* pAttachments;
		float                                         blendConstants[4];
	} VkPipelineColorBlendStateCreateInfo;*/
	
	VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo;
	memset((void*)&vkPipelineColorBlendStateCreateInfo, 0, sizeof(VkPipelineColorBlendStateCreateInfo));

	vkPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	vkPipelineColorBlendStateCreateInfo.pNext			 = NULL;
	vkPipelineColorBlendStateCreateInfo.flags			 = 0;
	vkPipelineColorBlendStateCreateInfo.attachmentCount  = _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array);
	vkPipelineColorBlendStateCreateInfo.pAttachments	 = vkPipelineColorBlendAttachmentState_array;
	
	// viewport sicssor state
	/*typedef struct VkPipelineViewportStateCreateInfo {
															VkStructureType                       sType;
															const void* pNext;
															VkPipelineViewportStateCreateFlags    flags;
															uint32_t                              viewportCount;
															const VkViewport* pViewports;
															uint32_t                              scissorCount;
															const VkRect2D* pScissors;
	} VkPipelineViewportStateCreateInfo;*/

	memset((void*)&vkViweport, 0, sizeof(VkViewport));
	vkViweport.x = 0;
	vkViweport.y = 0;
	vkViweport.width = (float)vkExtent2D_swapchain.width;
	vkViweport.height = (float)vkExtent2D_swapchain.height;
	vkViweport.minDepth = 0.0f;
	vkViweport.maxDepth = 1.0f;

	memset((void*)&vkRect2D_scissor, 0, sizeof(VkRect2D));
	vkRect2D_scissor.offset.x	 	 = 0;
	vkRect2D_scissor.offset.y		 = 0;
	vkRect2D_scissor.extent.width	 = vkExtent2D_swapchain.width;
	vkRect2D_scissor.extent.height	 = vkExtent2D_swapchain.height;


	VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo;
	memset((void*)&vkPipelineViewportStateCreateInfo, 0, sizeof(VkPipelineViewportStateCreateInfo));

	vkPipelineViewportStateCreateInfo.sType			 = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vkPipelineViewportStateCreateInfo.pNext			 = NULL;
	vkPipelineViewportStateCreateInfo.flags			 = 0;
	vkPipelineViewportStateCreateInfo.viewportCount  = 1; 
	vkPipelineViewportStateCreateInfo.pViewports	 = &vkViweport;
	vkPipelineViewportStateCreateInfo.scissorCount	 = 1;
	vkPipelineViewportStateCreateInfo.pScissors		 = &vkRect2D_scissor;

	// depth stencil state
	//as we don't have depth yet we can ommit this state
	VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateCreateInfo;
	/*typedef struct VkPipelineDepthStencilStateCreateInfo {
		VkStructureType                           sType;
		const void* pNext;
		VkPipelineDepthStencilStateCreateFlags    flags;
		VkBool32                                  depthTestEnable;
		VkBool32                                  depthWriteEnable;
		VkCompareOp                               depthCompareOp;
		VkBool32                                  depthBoundsTestEnable;
		VkBool32                                  stencilTestEnable;
		VkStencilOpState                          front;
		VkStencilOpState                          back;
		float                                     minDepthBounds;
		float                                     maxDepthBounds;
	} VkPipelineDepthStencilStateCreateInfo;*/

	memset((void*)&vkPipelineDepthStencilStateCreateInfo, 0, sizeof(VkPipelineDepthStencilStateCreateInfo));

	vkPipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	//vkPipelineDepthStencilStateCreateInfo.pNext = ;
	//vkPipelineDepthStencilStateCreateInfo.flags = ;
	vkPipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
	vkPipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
	vkPipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	vkPipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
	vkPipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;

	vkPipelineDepthStencilStateCreateInfo.front = vkPipelineDepthStencilStateCreateInfo.back;

	vkPipelineDepthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
	vkPipelineDepthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
	vkPipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;

	/*vkPipelineDepthStencilStateCreateInfo.minDepthBounds	 = ;
	vkPipelineDepthStencilStateCreateInfo.maxDepthBounds	 = ;*/


	//dynamic state
	// we don't have any dynamic state

	// multi sampling state
	/*typedef struct VkPipelineMultisampleStateCreateInfo {
																VkStructureType                          sType;
																const void* pNext;
																VkPipelineMultisampleStateCreateFlags    flags;
																VkSampleCountFlagBits                    rasterizationSamples;
																VkBool32                                 sampleShadingEnable;
																float                                    minSampleShading;
																const VkSampleMask* pSampleMask;
																VkBool32                                 alphaToCoverageEnable;
																VkBool32                                 alphaToOneEnable;
	} VkPipelineMultisampleStateCreateInfo; */
	
	VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo;
	memset((void*)&vkPipelineMultisampleStateCreateInfo, 0, sizeof(VkPipelineMultisampleStateCreateInfo));

	vkPipelineMultisampleStateCreateInfo.sType					 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	vkPipelineMultisampleStateCreateInfo.pNext					 = NULL;
	vkPipelineMultisampleStateCreateInfo.flags					 = 0;
	vkPipelineMultisampleStateCreateInfo.rasterizationSamples	 = VK_SAMPLE_COUNT_1_BIT;
	
	// shader state
	/*typedef struct VkPipelineShaderStageCreateInfo {
												VkStructureType                     sType;
												const void* pNext;
												VkPipelineShaderStageCreateFlags    flags;
												VkShaderStageFlagBits               stage;
												VkShaderModule                      module;
												const char* pName;
												const VkSpecializationInfo* pSpecializationInfo;
	} VkPipelineShaderStageCreateInfo;*/
	
	VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfo_array[2];
	memset((void*)vkPipelineShaderStageCreateInfo_array, 0, sizeof(VkPipelineShaderStageCreateInfo) * _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array));

	// vertex shader

	vkPipelineShaderStageCreateInfo_array[0].sType					 = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vkPipelineShaderStageCreateInfo_array[0].pNext					 = NULL;
	vkPipelineShaderStageCreateInfo_array[0].flags					 = 0;
	vkPipelineShaderStageCreateInfo_array[0].stage					 = VK_SHADER_STAGE_VERTEX_BIT;
	vkPipelineShaderStageCreateInfo_array[0].module					 = vkShaderModule_vertex_shader;
	vkPipelineShaderStageCreateInfo_array[0].pName					 = "main";
	vkPipelineShaderStageCreateInfo_array[0].pSpecializationInfo	 = NULL;

	// fragment shader
	vkPipelineShaderStageCreateInfo_array[1].sType					 = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vkPipelineShaderStageCreateInfo_array[1].pNext					 = NULL;
	vkPipelineShaderStageCreateInfo_array[1].flags					 = 0;
	vkPipelineShaderStageCreateInfo_array[1].stage					 = VK_SHADER_STAGE_FRAGMENT_BIT;
	vkPipelineShaderStageCreateInfo_array[1].module					 = vkShaderModule_fragment_shader;
	vkPipelineShaderStageCreateInfo_array[1].pName					 = "main";
	vkPipelineShaderStageCreateInfo_array[1].pSpecializationInfo	 = NULL;

	// TESSELLATION state
	// we don't have it now we can ommit it now

	// as pipeline are created for pipeline cahce we are creating it
	/*typedef struct VkPipelineCacheCreateInfo {
													VkStructureType               sType;
													const void* pNext;
													VkPipelineCacheCreateFlags    flags;
													size_t                        initialDataSize;
													const void* pInitialData;
	} VkPipelineCacheCreateInfo; */
	VkPipelineCacheCreateInfo vkPipelineCacheCreateInfo;
	memset((void*)&vkPipelineCacheCreateInfo, 0, sizeof(VkPipelineCacheCreateInfo));

	vkPipelineCacheCreateInfo.sType				 = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	vkPipelineCacheCreateInfo.pNext				 = NULL;
	vkPipelineCacheCreateInfo.flags				 = 0;

	// create vkpipe line

	VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;
	/*VkResult vkCreatePipelineCache(
									VkDevice                                    device,
									const VkPipelineCacheCreateInfo* pCreateInfo,
									const VkAllocationCallbacks* pAllocator,
									VkPipelineCache* pPipelineCache);*/

	vkResult = vkCreatePipelineCache(vkDevice, &vkPipelineCacheCreateInfo, NULL, &vkPipelineCache);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createPipeline() -> vkCreatePipelineCache() is failed %d \n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createPipeline() -> vkCreatePipelineCache() successed\n");

	}

	// create the actual graphics pipeline
	/*typedef struct VkGraphicsPipelineCreateInfo {
		VkStructureType                                  sType;
		const void* pNext;
		VkPipelineCreateFlags                            flags;
		uint32_t                                         stageCount;
		const VkPipelineShaderStageCreateInfo*			 pStages;
		const VkPipelineVertexInputStateCreateInfo*		 pVertexInputState;
		const VkPipelineInputAssemblyStateCreateInfo*	 pInputAssemblyState;
		const VkPipelineTessellationStateCreateInfo*	 pTessellationState;
		const VkPipelineViewportStateCreateInfo*		 pViewportState;
		const VkPipelineRasterizationStateCreateInfo*	 pRasterizationState;
		const VkPipelineMultisampleStateCreateInfo*		 pMultisampleState;
		const VkPipelineDepthStencilStateCreateInfo*	 pDepthStencilState;
		const VkPipelineColorBlendStateCreateInfo*		 pColorBlendState;
		const VkPipelineDynamicStateCreateInfo*			 pDynamicState;
		VkPipelineLayout                                 layout;
		VkRenderPass                                     renderPass;
		uint32_t                                         subpass;
		VkPipeline                                       basePipelineHandle;
		int32_t                                          basePipelineIndex;
	} VkGraphicsPipelineCreateInfo;*/


	VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo;
	memset((void*)&vkGraphicsPipelineCreateInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));

	vkGraphicsPipelineCreateInfo.sType					 = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	vkGraphicsPipelineCreateInfo.pNext					 = NULL;
	vkGraphicsPipelineCreateInfo.flags					 = 0;
	vkGraphicsPipelineCreateInfo.pVertexInputState		 = &vkPipelineVertexInputStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pInputAssemblyState	 = &vkPipelineInputAssemblyStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pRasterizationState	 = &vkPipelineRasterizationStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pColorBlendState	 	 = &vkPipelineColorBlendStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pViewportState			 = &vkPipelineViewportStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pDepthStencilState		 = &vkPipelineDepthStencilStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pDynamicState			 = NULL;
	vkGraphicsPipelineCreateInfo.pMultisampleState		 = &vkPipelineMultisampleStateCreateInfo;
	vkGraphicsPipelineCreateInfo.stageCount				 = _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array);
	vkGraphicsPipelineCreateInfo.pStages				 = vkPipelineShaderStageCreateInfo_array;
	vkGraphicsPipelineCreateInfo.pTessellationState		 = NULL;
	vkGraphicsPipelineCreateInfo.layout					 = vkPipelineLayout;
	vkGraphicsPipelineCreateInfo.renderPass				 = vkRenderPass;
	vkGraphicsPipelineCreateInfo.subpass				 = 0;
	vkGraphicsPipelineCreateInfo.basePipelineHandle		 = VK_NULL_HANDLE;
	vkGraphicsPipelineCreateInfo.basePipelineIndex		 = 0;

	// now create the pipe line
	/*VkResult vkCreateGraphicsPipelines(
										VkDevice                                    device,
										VkPipelineCache                             pipelineCache,
										uint32_t                                    createInfoCount,
										const VkGraphicsPipelineCreateInfo* pCreateInfos,
										const VkAllocationCallbacks* pAllocator,
										VkPipeline* pPipelines);*/

	vkResult = vkCreateGraphicsPipelines(vkDevice, vkPipelineCache, 1, &vkGraphicsPipelineCreateInfo, NULL, &vkPipeline);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createPipeline() -> vkCreateGraphicsPipelines() is failed %d \n", vkResult);
		vkDestroyPipelineCache(vkDevice, vkPipelineCache, NULL);
		vkPipelineCache = VK_NULL_HANDLE;
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createPipeline() -> vkCreateGraphicsPipelines() successed\n");
		
	}

	// we are done with pipeline cahce

	vkDestroyPipelineCache(vkDevice, vkPipelineCache, NULL);
	vkPipelineCache = VK_NULL_HANDLE;

	fprintf(gpFile, "\n===========================================================================================================================\n");
	return vkResult;
}

VkResult createFramebuffers(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createFramebuffers() : 12th March 2025\n\n");

	//Step 03: allocate the frame buffer array by malloc equal to the size of swapchain image count

	vkFramebuffer_array = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * swapchainImageCount);
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		//------------------------------------------------------------------------------------------------------------------
		// This is recommanded change no. 2 for depth

		VkImageView vkimageView_attachments_array[2];
		memset((void*)vkimageView_attachments_array, 0, sizeof(VkImageView) * _ARRAYSIZE(vkimageView_attachments_array));

		//Step 02: declare and initialise VkFramebufferCreateInfo structure

		/*typedef struct VkFramebufferCreateInfo {
			VkStructureType             sType;
			const void* pNext;
			VkFramebufferCreateFlags    flags;
			VkRenderPass                renderPass;
			uint32_t                    attachmentCount;
			const VkImageView* pAttachments;
			uint32_t                    width;
			uint32_t                    height;
			uint32_t                    layers;
		} VkFramebufferCreateInfo;*/

		VkFramebufferCreateInfo vkFramebufferCreateInfo;
		memset((void*)&vkFramebufferCreateInfo, 0, sizeof(VkFramebufferCreateInfo));

		vkFramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		vkFramebufferCreateInfo.pNext = NULL;
		vkFramebufferCreateInfo.flags = 0;
		vkFramebufferCreateInfo.renderPass = vkRenderPass;
		vkFramebufferCreateInfo.attachmentCount = _ARRAYSIZE(vkimageView_attachments_array); //
		vkFramebufferCreateInfo.pAttachments = vkimageView_attachments_array; // VkImageView
		vkFramebufferCreateInfo.width = vkExtent2D_swapchain.width; //FBO width
		vkFramebufferCreateInfo.height = vkExtent2D_swapchain.height;
		vkFramebufferCreateInfo.layers = 1;

		//--------------------------------------------------------------------------------------------------------------

		/*VkResult vkCreateFramebuffer(
			VkDevice                                    device,
			const VkFramebufferCreateInfo * pCreateInfo,
			const VkAllocationCallbacks * pAllocator,
			VkFramebuffer * pFramebuffer);*/

		vkimageView_attachments_array[0] = swapchainimageView_array[i];
		vkimageView_attachments_array[1] = vkImageView_depth;

		vkResult = vkCreateFramebuffer(vkDevice, &vkFramebufferCreateInfo, NULL, &vkFramebuffer_array[i]);

		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "createFramebuffers() -> vkCreateFramebuffer() is failed %d at ith : %d\n", vkResult, i);
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "createFramebuffers() -> vkCreateFramebuffer() successed at ith : %d\n", i);

		}
	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return(vkResult);
}

VkResult createSemaphores(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createSemaphores() : 21st March 2025\n\n");

	//Step 02: In createSemaphore() user define function declare, memset and initialise VkSemaphoreCreateInfo structure
	
	/*typedef struct VkSemaphoreCreateInfo {
		VkStructureType           sType;
		const void* pNext;
		VkSemaphoreCreateFlags    flags;
	} VkSemaphoreCreateInfo;*/
	VkSemaphoreCreateInfo vkSemaphoreCreateInfo;
	memset((void*) &vkSemaphoreCreateInfo, 0, sizeof(VkSemaphoreCreateInfo));

	vkSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkSemaphoreCreateInfo.pNext = NULL;
	vkSemaphoreCreateInfo.flags = 0; // must be zero reserved


	//Step 03: Now call vkCreateSemaphore() api two times to create our two semaphore objects.
    //			Remamber both will use same VkSemaphoreCreateInfo  struture

	/*VkResult vkCreateSemaphore(
		VkDevice                                    device,
		const VkSemaphoreCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSemaphore * pSemaphore);*/

	vkResult = vkCreateSemaphore(vkDevice, &vkSemaphoreCreateInfo, NULL, &vkSemaphore_backBuffer);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createSemaphores() -> vkCreateSemaphore() is failed %d for vkSemaphore_backBuffer\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createSemaphores() -> vkCreateSemaphore() successed for vkSemaphore_backBuffer\n");

	}

	vkResult = vkCreateSemaphore(vkDevice, &vkSemaphoreCreateInfo, NULL, &vkSemaphore_renderComplete);

	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createSemaphores() -> vkCreateSemaphore() is failed %d for vkSemaphore_renderComplete\n", vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "createSemaphores() -> vkCreateSemaphore() successed for vkSemaphore_renderComplete\n");

	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return vkResult;
}

VkResult createFences(void)
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "createFences() : 21st March 2025\n\n");

	//18 : Step 04: in createFences() user define function declare, memsetand initialise VkFenceCreateInfo structure
	
	/*typedef struct VkFenceCreateInfo {
		VkStructureType       sType;
		const void* pNext;
		VkFenceCreateFlags    flags;
	} VkFenceCreateInfo;*/

	VkFenceCreateInfo vkFenceCreateInfo;
	memset((void*)&vkFenceCreateInfo, 0, sizeof(VkFenceCreateInfo));

	vkFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vkFenceCreateInfo.pNext = NULL;
	vkFenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	//Step 05: In this function allocate our global fence array to the size of swapchain image count using malloc
	
	vkFence_array = (VkFence*) malloc (sizeof(VkFence) * swapchainImageCount);
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		//Step 06: Now in a loop call vkCreateFence() api  to initalise aour global fences array
		/*VkResult vkCreateFence(
			VkDevice                                    device,
			const VkFenceCreateInfo * pCreateInfo,
			const VkAllocationCallbacks * pAllocator,
			VkFence * pFence);*/

		vkResult = vkCreateFence(vkDevice, &vkFenceCreateInfo, NULL, &vkFence_array[i]);
		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "createFences() -> vkCreateFence() is failed %d at index : %d\n", vkResult, i);
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "createFences() -> vkCreateFence() successed for ith index %d\n", i);

		}
	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return vkResult;
}

VkResult buildCommandBuffer(void) 
{
	// variable declaration
	VkResult vkResult = VK_SUCCESS;

	// Code
	fprintf(gpFile, "\n=======================================================================================\n");
	fprintf(gpFile, "buildCommandBuffer() : 21st March 2025\n\n");

	// loop per swapchain image
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		// step 02 reset command buffer

		/*VkResult vkResetCommandBuffer(
			VkCommandBuffer                             commandBuffer,
			VkCommandBufferResetFlags                   flags);*/

		vkResult = vkResetCommandBuffer(vkCommandBuffer_array[i], 0);

		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "buildCommandBuffer() -> vkResetCommandBuffer() is failed to reset %d at index : %d\n", vkResult, i);
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "buildCommandBuffer() -> vkResetCommandBuffer() successed to reset for ith index %d\n", i);

		}


		//step 03 Then declare, memset, initialise vkCommandBufferBeginInfo structure
		
		/*typedef struct VkCommandBufferBeginInfo {
			VkStructureType                          sType;
			const void* pNext;
			VkCommandBufferUsageFlags                flags;
			const VkCommandBufferInheritanceInfo* pInheritanceInfo;
		} VkCommandBufferBeginInfo;*/

		VkCommandBufferBeginInfo vkCommandBufferBeginInfo;
		memset((void*)&vkCommandBufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));

		vkCommandBufferBeginInfo.sType				 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkCommandBufferBeginInfo.pNext				 = NULL;
		vkCommandBufferBeginInfo.flags				 = 0;
		///vkCommandBufferBeginInfo.pInheritanceInfo	 = ;

		//Step 04: Now call vkBeginCommandBuffer() api to record vulkan different drawing releated commands.Do error checking
		/*VkResult vkBeginCommandBuffer(
			VkCommandBuffer                             commandBuffer,
			const VkCommandBufferBeginInfo * pBeginInfo);*/
		
		vkResult = vkBeginCommandBuffer(vkCommandBuffer_array[i], &vkCommandBufferBeginInfo);
		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "buildCommandBuffer() -> vkBeginCommandBuffer() is failed %d at index : %d\n", vkResult, i);
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "buildCommandBuffer() -> vkBeginCommandBuffer() successed at ith index %d\n", i);

		}

		// step 05: set clear value
		/*typedef union VkClearValue {
			VkClearColorValue           color;
			VkClearDepthStencilValue    depthStencil;
		} VkClearValue;*/

		VkClearValue vkClearValue_array[2]; // till now it was 1 but now making 2 as depth is added
		memset((void*)vkClearValue_array, 0, sizeof(VkClearValue) * _ARRAYSIZE(vkClearValue_array));

		vkClearValue_array[0].color = vkClearColorValue;
		vkClearValue_array[1].depthStencil = vkClearDepthStensilValue;
		
		//Step 06: then declare memset and initialse VkRenderPassBeginInfo struture
		/*typedef struct VkRenderPassBeginInfo {
			VkStructureType        sType;
			const void* pNext;
			VkRenderPass           renderPass;
			VkFramebuffer          framebuffer;
			VkRect2D               renderArea;
			uint32_t               clearValueCount;
			const VkClearValue* pClearValues;
		} VkRenderPassBeginInfo;*/

		VkRenderPassBeginInfo vkRenderPassBeginInfo;
		memset((void*)&vkRenderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));

		vkRenderPassBeginInfo.sType						 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		vkRenderPassBeginInfo.pNext						 = NULL;
		vkRenderPassBeginInfo.renderPass				 = vkRenderPass;
		vkRenderPassBeginInfo.renderArea.offset.x		 = 0;
		vkRenderPassBeginInfo.renderArea.offset.y		 = 0;
		vkRenderPassBeginInfo.renderArea.extent.width	 = vkExtent2D_swapchain.width;
		vkRenderPassBeginInfo.renderArea.extent.height	 = vkExtent2D_swapchain.height;
		vkRenderPassBeginInfo.clearValueCount			 = _ARRAYSIZE(vkClearValue_array);
		vkRenderPassBeginInfo.pClearValues				 =	vkClearValue_array;
		vkRenderPassBeginInfo.framebuffer				 = vkFramebuffer_array[i];

		//Step 07: begin render pass by vkCmdBeginRenderPass() api
		/*void vkCmdBeginRenderPass(
			VkCommandBuffer                             commandBuffer,
			const VkRenderPassBeginInfo * pRenderPassBegin,
			VkSubpassContents                           contents);*/

		vkCmdBeginRenderPass(vkCommandBuffer_array[i], &vkRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		// bind with the pipeline
		/*void vkCmdBindPipeline(
			VkCommandBuffer                             commandBuffer,
			VkPipelineBindPoint                         pipelineBindPoint,
			VkPipeline                                  pipeline);*/

		vkCmdBindPipeline(vkCommandBuffer_array[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

		// bind our descriptor to the pipeline
		
		//=============================================================================================================================================
		//										street_light_lamp
		/*void vkCmdBindDescriptorSets(
			VkCommandBuffer                             commandBuffer,
			VkPipelineBindPoint                         pipelineBindPoint,
			VkPipelineLayout                            layout,
			uint32_t                                    firstSet,
			uint32_t                                    descriptorSetCount,
			const VkDescriptorSet * pDescriptorSets,
			uint32_t                                    dynamicOffsetCount,
			const uint32_t * pDynamicOffsets);*/


		//=============================================================================================================================================
		//										street_light_horizontal_top_bar
		vkCmdBindDescriptorSets(vkCommandBuffer_array[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet_street_light_horizontal_top_bar, 0, NULL);
		VkDeviceSize vkDeviceSize_offset_position[1];

		memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));

		vkCmdBindVertexBuffers(vkCommandBuffer_array[i], 0, 1, &vetextData_street_light_middle_bar_position.vkBuffer, vkDeviceSize_offset_position);
		VkDeviceSize vkDeviceSize_offset_color[1];
		memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));

		vkCmdBindVertexBuffers(vkCommandBuffer_array[i], 1, 1, &vetextData_street_light_middle_bar_color.vkBuffer, vkDeviceSize_offset_color);
		vkCmdDraw(vkCommandBuffer_array[i], 6, 1, 0, 0);

		//===================================================================================================================================================


		vkCmdBindDescriptorSets(vkCommandBuffer_array[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet_street_light_lamp_1, 0, NULL);

		// bind with vertex position buffer
		//VkDeviceSize vkDeviceSize_offset_position[1];
		memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));
		vkCmdBindVertexBuffers(vkCommandBuffer_array[i], 0, 1, &vetextData_position_street_light_lamp.vkBuffer, vkDeviceSize_offset_position);

		/*void vkCmdBindVertexBuffers(
			VkCommandBuffer                             commandBuffer,
			uint32_t                                    firstBinding,
			uint32_t                                    bindingCount,
			const VkBuffer* pBuffers,
			const VkDeviceSize* pOffsets);*/

		// bind with vertex color buffer
		//VkDeviceSize vkDeviceSize_offset_color[1];
		memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));
		vkCmdBindVertexBuffers(vkCommandBuffer_array[i], 1, 1, &vetextData_color_street_light_lamp.vkBuffer, vkDeviceSize_offset_color);

		// here we should call drawing function
		vkCmdDraw(vkCommandBuffer_array[i], 6, 1, 0, 0);
		
		//=============================================================================================================================================

	
		
		
		
		
		//=============================================================================================================================================

		//=============================================================================================================================================
		//										street_light_middle_bar
		vkCmdBindDescriptorSets(vkCommandBuffer_array[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet_street_light_middle_bar, 0, NULL);
		memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));

		vkCmdBindVertexBuffers(vkCommandBuffer_array[i], 0, 1, &vetextData_street_light_middle_bar_position.vkBuffer, vkDeviceSize_offset_position);

		memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));

		vkCmdBindVertexBuffers(vkCommandBuffer_array[i], 1, 1, &vetextData_street_light_middle_bar_color.vkBuffer, vkDeviceSize_offset_color);
		vkCmdDraw(vkCommandBuffer_array[i], 6, 1, 0, 0);
		
		//=============================================================================================================================================

		vkCmdBindDescriptorSets(vkCommandBuffer_array[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet_street_light_lamp_2, 0, NULL);
		memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize)* _ARRAYSIZE(vkDeviceSize_offset_position));
		vkCmdBindVertexBuffers(vkCommandBuffer_array[i], 0, 1, &vetextData_position_street_light_lamp.vkBuffer, vkDeviceSize_offset_position);

		memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize)* _ARRAYSIZE(vkDeviceSize_offset_color));
		vkCmdBindVertexBuffers(vkCommandBuffer_array[i], 1, 1, &vetextData_color_street_light_lamp.vkBuffer, vkDeviceSize_offset_color);

		// here we should call drawing function
		vkCmdDraw(vkCommandBuffer_array[i], 6, 1, 0, 0);


		

		//=============================================================================================================================================

		//----------------------------------------------------------------------------------------------------------------------------------------------
		/*void vkCmdEndRenderPass(
			VkCommandBuffer                             commandBuffer);*/
		vkCmdEndRenderPass(vkCommandBuffer_array[i]);

		//Step 09: end the recording of command buffer by calling vkEndCommandBuffer()
		/*VkResult vkEndCommandBuffer(
			VkCommandBuffer                             commandBuffer);*/

		vkResult = vkEndCommandBuffer(vkCommandBuffer_array[i]);
		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "buildCommandBuffer() -> vkEndCommandBuffer() is failed %d at index : %d\n", vkResult, i);
			return (vkResult);
		}
		else
		{
			fprintf(gpFile, "buildCommandBuffer() -> vkEndCommandBuffer() successed at ith index %d\n", i);

		}
	}

	fprintf(gpFile, "\n=======================================================================================\n");
	return vkResult;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallBack(
								VkDebugReportFlagsEXT		vkDebugReportFlagsEXT, 
								VkDebugReportObjectTypeEXT  vkDebugReportObjectTypeEXT,
								uint64_t		object, 
								size_t			location, 
								int32_t			messageCode, 
								const char*		pLayerPrefix, 
								const char*		pMessage, 
								void*			pUserData)
{
	// code 
	fprintf(gpFile, "\nSSK_VALIDATION : debugReportCallBack() -> %s (%d) = %s \n", pLayerPrefix, messageCode, pMessage);

	return VK_FALSE;
}


//=================================================================================================================

