// common header files
#include <Windows.h>	// win32 API
#include <stdio.h>		// file IO
#include <stdlib.h>		// for exit()

// MACROS 
#define WIN_WIDTH	800
#define WIN_HEIGHT	600 

// userdefine header file
#include "VK.h"


// link with library
#pragma comment	(lib,"vulkan-1.lib")	// link with Vulkan library

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variables declarations
HWND DRSJ_ghwnd = NULL;
DWORD DRSJ_dwStyle = 0;
BOOL DRSJ_gbActive = FALSE;
FILE* DRSJ_gpLogFile = NULL;
BOOL DRSJ_gbFullscreen = FALSE;
BOOL DRSJ_bWindowMinimised = FALSE;
WINDOWPLACEMENT DRSJ_wpPrev;

const char* DRSJ_gpAppName = "ARTR-002";

//
// Vulkan related global variables
//

// Instance extension related variables
uint32_t DRSJ_enabledInstanceExtensionCount = 0;

// VK_KHR_SURFACE_EXTENSION_NAME and VK_KHR_WIN32_SURFACE_EXTENSION_NAME and // VK_EXT_DEBUG_REPORT_EXTENSION_NAME
const char* DRSJ_enabledInstanceExtensionNames_array[3];

// Vulkan Instance 
VkInstance DRSJ_vkInstance = VK_NULL_HANDLE;

// Vulkan Presentaion surface
VkSurfaceKHR DRSJ_vkSurfaceKHR = VK_NULL_HANDLE;

// Vulkan physical device related 
VkPhysicalDevice DRSJ_vkPhysicalDevice_selected = VK_NULL_HANDLE;
uint32_t DRSJ_graphicsQueueFamilyIndex_selected = UINT32_MAX;
VkPhysicalDeviceMemoryProperties DRSJ_vkPhysicalDeviceMemoryProperties;

uint32_t DRSJ_physicalDeviceCount = 0;
VkPhysicalDevice* DRSJ_vkPhysicalDevice_array = NULL;

// Device extension related variables
uint32_t DRSJ_enabledDeviceExtensionCount = 0;

// VK_KHR_SWAPCHAIN_EXTENSION_NAME
const char* DRSJ_enabledDeviceExtensionNames_array[1];

VkDevice DRSJ_vkDevice = VK_NULL_HANDLE;
VkQueue DRSJ_vkQueue;

// Color Format and Color Space
VkFormat DRSJ_vkFormat_color = VK_FORMAT_UNDEFINED;
VkColorSpaceKHR DRSJ_vkColorSpaceKHR = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

// Presentation Mode
VkPresentModeKHR DRSJ_vkPresentModeKHR = VK_PRESENT_MODE_FIFO_KHR;


// Swapchain related variables
int winWidth = WIN_WIDTH;
int winHeight = WIN_HEIGHT;
VkSwapchainKHR DRSJ_vkSwapchainKHR = VK_NULL_HANDLE;
VkExtent2D DRSJ_vkExtent2D_swapchain;


// Swapchain Image And Image Views related variables
uint32_t DRSJ_swapchainImageCount = UINT32_MAX;
VkImage* DRSJ_vkSwapchainImage_array = NULL;
VkImageView* DRSJ_vkSwapchainImageView_array = NULL;

// Command Pool related variables
VkCommandPool DRSJ_vkCommandPool;

// Command Buffers related variables
VkCommandBuffer* DRSJ_vkCommandBuffer_array = NULL;

// Renderpass related variables
VkRenderPass vkRenderPass = VK_NULL_HANDLE;

// Framebuffer related variables
VkFramebuffer* DRSJ_vkFramebuffer_array = NULL;

// Fence and Semaphore related variables
VkSemaphore DRSJ_vkSemaphore_backBuffer = VK_NULL_HANDLE;
VkSemaphore DRSJ_vkSemaphore_renderComplete = VK_NULL_HANDLE;
VkFence* DRSJ_vkFence_array = NULL;

// 
VkClearColorValue DRSJ_vkClearColorValue;

BOOL DRSJ_bInitialized = FALSE;
uint32_t DRSJ_currentImageIndex = UINT_MAX;


BOOL bValidation = TRUE;
uint32_t enabledValidationLayerCount = 0;
const char* enabledValidationLayerName_array[1]; //For VK_LAYER_KHRONOS_validation
VkDebugReportCallbackEXT vkDebugReportCallbackEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT_fnptr = NULL;



// Position
VertexData vertexData_position_road_border;
VertexData vertexData_position_main_road;
VertexData vertexData_position_zebra_crossing;

// Color
VertexData vertexData_color_road_border;
VertexData vertexData_color_main_road;
VertexData vertexData_color_zebra_crossing;

UniformData uniformData_road_border;
UniformData uniformData_main_road;
UniformData uniformData_zebra_crossing;

PushConstant pushConstant_model_uniformData[4];

// Shader related variables
VkShaderModule vkShaderModule_vertex_shader = VK_NULL_HANDLE;
VkShaderModule vkShaderModule_fragment_shader = VK_NULL_HANDLE;

// Descriptor Set Layout related variables
VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

// Pipeline Layout related variables
VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

// Dscriptor pool
VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;
VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;

// Pipeline related variables
VkViewport vkViewport;
VkRect2D vkRect2D_scissor;
VkPipeline vkPipeline = VK_NULL_HANDLE;

//////////////// START SHAPES  ////////////////
FILE* gpFile= NULL;

VertexData vertexData_position_2;
VertexData vertexData_color_2;
UniformData uniformData_2;

// entry point function
int WINAPI  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function declarations
	VkResult initialize(void);
	void uninitialize(void);
	VkResult  display(void);
	void update(void);

	// local variable declarations
	WNDCLASSEX DRSJ_wndClass;
	HWND DRSJ_hwnd;
	MSG DRSJ_msg;
	TCHAR DRSJ_szAppName[255];
	TCHAR DRSJ_szWindowTitle[255] = { "DRSJ : Vulkan - Road Module" };

	int DRSJ_iResult = 0;
	BOOL DRSJ_bDone = FALSE;

	VkResult DRSJ_vkResult = VK_SUCCESS;

	wsprintf(DRSJ_szAppName, TEXT("%s"), DRSJ_gpAppName);

	// code
	DRSJ_gpLogFile = fopen("LogFile.txt", "w");
	if (NULL == DRSJ_gpLogFile)
	{
		MessageBox(NULL, TEXT("Log File Opening/Creation FAILED"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : Log File opened Sucessfully !!!\n", __func__, __LINE__);
	}

	// WNDCLASSEX initialization : 12 members
	DRSJ_wndClass.cbSize = sizeof(WNDCLASSEX);
	DRSJ_wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	DRSJ_wndClass.cbWndExtra = 0;
	DRSJ_wndClass.cbClsExtra = 0;
	DRSJ_wndClass.lpfnWndProc = WndProc;
	DRSJ_wndClass.hInstance = hInstance;
	DRSJ_wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	DRSJ_wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(VOID_DERIVATIVES_ICON));
	DRSJ_wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	DRSJ_wndClass.lpszClassName = DRSJ_szAppName;
	DRSJ_wndClass.lpszMenuName = NULL;
	DRSJ_wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(VOID_DERIVATIVES_ICON));

	// Register WNDCLASSEX 
	RegisterClassEx(&DRSJ_wndClass);

	int DRSJ_screen_width = GetSystemMetrics(SM_CXSCREEN);
	int DRSJ_screen_height = GetSystemMetrics(SM_CYSCREEN);

	// Create Window : 12 parameters

	DRSJ_hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		DRSJ_szAppName,
		DRSJ_szWindowTitle,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		(DRSJ_screen_width / 2) - (WIN_WIDTH / 2),
		(DRSJ_screen_height / 2) - (WIN_HEIGHT / 2),
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	DRSJ_ghwnd = DRSJ_hwnd;

	// Initialization 
	DRSJ_vkResult = initialize();
	if (VK_SUCCESS != DRSJ_vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : initialize() FAILED [%d]\n", __func__, __LINE__, DRSJ_vkResult);
		MessageBox(DRSJ_hwnd, TEXT("Initialization FAILED"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		DestroyWindow(DRSJ_hwnd);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : initialize() SUCCESSED\n", __func__, __LINE__);
	}

	// Show the window
	ShowWindow(DRSJ_hwnd, iCmdShow);

	SetForegroundWindow(DRSJ_hwnd);
	SetFocus(DRSJ_hwnd);

	// Game Loop
	while (FALSE == DRSJ_bDone)
	{
		if (PeekMessage(&DRSJ_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == DRSJ_msg.message)
				DRSJ_bDone = TRUE;
			else
			{
				TranslateMessage(&DRSJ_msg);
				DispatchMessage(&DRSJ_msg);
			}
		}
		else
		{
			if (TRUE == DRSJ_gbActive && !DRSJ_bWindowMinimised)
			{

				// Render
				DRSJ_vkResult = display();
				if (VK_FALSE != DRSJ_vkResult && 
					VK_SUCCESS != DRSJ_vkResult && 
					VK_ERROR_OUT_OF_DATE_KHR != DRSJ_vkResult && 
					VK_SUBOPTIMAL_KHR != DRSJ_vkResult
				   )
				{
					fprintf(DRSJ_gpLogFile, "%s %d : Call To display() FAILED\n", __func__, __LINE__);
					DRSJ_bDone = TRUE;
				}

				// Update
				update();
			}
		}
	}

	// Unitialization
	uninitialize();

	return ((int)DRSJ_msg.wParam);
}

// callback function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void ToggleFullscreen(void);
	VkResult resize(int, int);
	void uninitialize(void);

	// code
	switch (iMsg)
	{
	case WM_CREATE:
	{
		memset(&DRSJ_wpPrev, 0, sizeof(WINDOWPLACEMENT));
		DRSJ_wpPrev.length = sizeof(WINDOWPLACEMENT);
		break;
	}

	case WM_SETFOCUS:
	{
		DRSJ_gbActive = TRUE;
		break;
	}

	case WM_KILLFOCUS:
	{
		DRSJ_gbActive = FALSE;
		break;
	}

	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
		{
			DRSJ_bWindowMinimised = TRUE;
		}
		else
		{	
			resize(LOWORD(lParam), HIWORD(lParam));
			DRSJ_bWindowMinimised = FALSE;
		}
		break;
	}

	case WM_ERASEBKGND:
	{
		//return (0);
		break;
	}

	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;
	}

	case WM_CHAR:
	{
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			if (FALSE == DRSJ_gbFullscreen)
			{
				ToggleFullscreen();
				DRSJ_gbFullscreen = TRUE;
			}
			else
			{
				ToggleFullscreen();
				DRSJ_gbFullscreen = FALSE;
			}
			break;
		}
		break;
	}

	case WM_CLOSE:
	{
		uninitialize();
		DestroyWindow(hwnd);
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	// local variable declarations
	MONITORINFO DRSJ_mi = { sizeof(MONITORINFO) };	// zerout object

	// code

	if (FALSE == DRSJ_gbFullscreen)
	{
		DRSJ_dwStyle = GetWindowLong(DRSJ_ghwnd, GWL_STYLE);

		if (WS_OVERLAPPEDWINDOW & DRSJ_dwStyle)
		{
			if (GetWindowPlacement(DRSJ_ghwnd, &DRSJ_wpPrev) && GetMonitorInfo(MonitorFromWindow(DRSJ_ghwnd, MONITORINFOF_PRIMARY), &DRSJ_mi))
			{
				SetWindowLong(DRSJ_ghwnd, GWL_STYLE, DRSJ_dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(DRSJ_ghwnd,
					HWND_TOP,
					DRSJ_mi.rcMonitor.left,
					DRSJ_mi.rcMonitor.top,
					DRSJ_mi.rcMonitor.right - DRSJ_mi.rcMonitor.left,
					DRSJ_mi.rcMonitor.bottom - DRSJ_mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED
				);
			}
		} // if (WS_OVERLAPPEDWINDOW & dwStyle)	

		ShowCursor(FALSE);

	}	// if (FALSE == gbFullscreen)
	else
	{
		SetWindowPlacement(DRSJ_ghwnd, &DRSJ_wpPrev);
		SetWindowLong(DRSJ_ghwnd, GWL_STYLE, DRSJ_dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(DRSJ_ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

VkResult initialize(void)
{
	// function declarations
	VkResult createVulkanInstance(void);
	VkResult getSupportedSurface(void);
	VkResult getPhysicalDevice(void);
	VkResult printVkInfo(void);
	VkResult createVulkanDevice(void);
	void getDeviceQueue(void);
	VkResult createSwapchain(VkBool32 vsync);
	VkResult createImagesAndImageViews(void);
	VkResult createCommandPool(void);
	VkResult createCommandBuffer(void);
	VkResult createVertexBuffer(void);	
	VkResult createUniformBuffer(void);
	VkResult createShaders(void);
	VkResult createDescriptorSetLayout(void);
	VkResult createPipelineLayout(void);	
	VkResult createDescriptorPool(void);
	VkResult createDescriptorSet(void);
	VkResult createRenderPass(void);
	VkResult createPipeline(void);
	VkResult createFramebuffer(void);
	VkResult createSemaphores(void);
	VkResult createFences(void);
	VkResult buildCommandBuffers(void);


	// local variable declarations
	VkResult vkResult = VK_SUCCESS;

	// code
	vkResult = createVulkanInstance();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createVulkanInstance() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createVulkanInstance() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Vulkan Presentation Surface
	vkResult = getSupportedSurface();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : getSupportedSurface() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : getSupportedSurface() SUCCESSED\n", __func__, __LINE__);
	}

	// Enumaret and Select required physical device
	vkResult = getPhysicalDevice();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : getPhysicalDevice() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : getPhysicalDevice() SUCCESSED\n", __func__, __LINE__);
	}

	// Print Vulkan Information
	vkResult = printVkInfo();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : printVkInfo() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : printVkInfo() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Vulkan Device	
	vkResult = createVulkanDevice();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createVulkanDevice() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createVulkanDevice() SUCCESSED\n", __func__, __LINE__);
	}


	// Get Device Queue
	getDeviceQueue();

	
	// Create Swapchain
	vkResult = createSwapchain(VK_FALSE);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createSwapchain() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createSwapchain() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Vulkan Images and Image View
	vkResult = createImagesAndImageViews();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createImagesAndImageViews() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createImagesAndImageViews() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Vulkan Command Pool
	vkResult = createCommandPool();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createCommandPool() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createCommandPool() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Vulkan Command Buffers
	vkResult = createCommandBuffer();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createCommandBuffer() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createCommandBuffer() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Vertex Buffers
	vkResult = createVertexBuffer();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createVertexBuffer() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createVertexBuffer() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Uniform Buffers
	vkResult = createUniformBuffer();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createUniformBuffer() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createUniformBuffer() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Shaders
	vkResult = createShaders();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createShaders() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createShaders() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Descriptor Set Layout
	vkResult = createDescriptorSetLayout();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createDescriptorSetLayout() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createDescriptorSetLayout() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Pipeline Layout
	vkResult = createPipelineLayout();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createPipelineLayout() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createPipelineLayout() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Descriptor Pool
	vkResult = createDescriptorPool();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createDescriptorPool() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createDescriptorPool() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Descriptor Set
	vkResult = createDescriptorSet();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createDescriptorSet() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createDescriptorSet() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Renderpass
	vkResult = createRenderPass();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createRenderPass() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createRenderPass() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Pipeline
	vkResult = createPipeline();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createPipeline() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createPipeline() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Framebuffer
	vkResult = createFramebuffer();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createFramebuffer() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createFramebuffer() SUCCESSED\n", __func__, __LINE__);
	}


	// Create Semaphores
	vkResult = createSemaphores();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createSemaphores() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createSemaphores() SUCCESSED\n", __func__, __LINE__);
	}

	// Create Fences
	vkResult = createFences();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createFences() FAILED [%d]\n", __func__, __LINE__,vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createFences() SUCCESSED\n", __func__, __LINE__);
	}


	// Initialize clear color values
	// this is same as glClearColor(0.0f,0.0f,1.0f,1.0f);
	memset((void*)&DRSJ_vkClearColorValue, 0, sizeof(VkClearColorValue));
	DRSJ_vkClearColorValue.float32[0] = 0.0f;
	DRSJ_vkClearColorValue.float32[1] = 0.5f;
	DRSJ_vkClearColorValue.float32[2] = 0.0f;
	DRSJ_vkClearColorValue.float32[3] = 1.0f;

	// Build Command Buffers
	vkResult = buildCommandBuffers();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : buildCommandBuffers() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : buildCommandBuffers() SUCCESSED\n", __func__, __LINE__);
	}


	// Initialization is completed
	DRSJ_bInitialized = TRUE;
	fprintf(DRSJ_gpLogFile, "%s %d : Initialization Is Completed SUCCESSFULLY\n", __func__, __LINE__);

	return (vkResult);
}

VkResult resize(int width, int height)
{
	// Function Declaration
	VkResult createSwapchain(VkBool32 vsync);
	VkResult createImagesAndImageViews(void);
	VkResult createCommandBuffer(void);
	VkResult createPipelineLayout(void);
	VkResult createRenderPass(void);
	VkResult createPipeline(void);	
	VkResult createFramebuffer(void);
	VkResult buildCommandBuffers(void);


	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	

	// code
	
	// Checked the bInitialized variable
	if (FALSE == DRSJ_bInitialized)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		fprintf(DRSJ_gpLogFile, "%s %d : Initialization Yet NOT Completed / FAILED\n", __func__, __LINE__);
		return vkResult;
	}

	// As recreation of swapchain, we are going to repeate initlialize steps again. Hence we are set FALSE into DRSJ_bInitialized
	DRSJ_bInitialized = FALSE;

	if (height <= 0)
		height = 1;

	// Set winWidth and winHeight global variables
	winWidth = width;
	winHeight = height;


	// Check present of swapchain
	if (DRSJ_vkSwapchainKHR == VK_NULL_HANDLE)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : Swapchian is already NULL cam mot procceed\n", __func__, __LINE__);
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return (vkResult);
	}


	if (DRSJ_vkDevice)
	{
		// Wait for device to complete in hand 
		vkDeviceWaitIdle(DRSJ_vkDevice);

		// Destroy Framebuffer
		for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		{
			vkDestroyFramebuffer(DRSJ_vkDevice, DRSJ_vkFramebuffer_array[i], NULL);
			//fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyFramebuffer() SUCCESSED for index:[%d]\n", __func__, __LINE__, i);
		}

		if (DRSJ_vkFramebuffer_array)
		{
			free(DRSJ_vkFramebuffer_array);
			DRSJ_vkFramebuffer_array = NULL;
		}

		// Destory Renderpass
		vkDestroyRenderPass(DRSJ_vkDevice, vkRenderPass, NULL);
		//fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyRenderPass() is SUCCESSED\n", __func__, __LINE__);

		// Destroy Pipeline
		if (vkPipeline)
		{
			vkDestroyPipeline(DRSJ_vkDevice, vkPipeline, NULL);
			vkPipeline = VK_NULL_HANDLE;
			//fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyPipeline() SUCCESSED\n", __func__, __LINE__);
		}

		if (vkPipelineLayout)
		{
			vkDestroyPipelineLayout(DRSJ_vkDevice, vkPipelineLayout, NULL);
			vkPipelineLayout = VK_NULL_HANDLE;
			//fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyPipelineLayout() SUCCESSED\n", __func__, __LINE__);
		}


		// Destroy Command buffers
		for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		{
			vkFreeCommandBuffers(DRSJ_vkDevice, DRSJ_vkCommandPool, 1, &(DRSJ_vkCommandBuffer_array[i]));
			//fprintf(DRSJ_gpLogFile, "%s %d : vkFreeCommandBuffers() SUCCESSED for index:[%d]\n", __func__, __LINE__, i);
		}

		if (DRSJ_vkCommandBuffer_array)
		{
			free(DRSJ_vkCommandBuffer_array);
			DRSJ_vkCommandBuffer_array = NULL;
			//fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkCommandBuffer_array Free\n", __func__, __LINE__);
		}

		// Free swapchain images views
		for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		{
			vkDestroyImageView(DRSJ_vkDevice, DRSJ_vkSwapchainImageView_array[i], NULL);
			//fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkSwapchainImageView_array[%d] Freed\n", __func__, __LINE__, i);
		}

		if (DRSJ_vkSwapchainImageView_array)
		{
			free(DRSJ_vkSwapchainImageView_array);
			DRSJ_vkSwapchainImageView_array = NULL;
			//fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkSwapchainImageView_array Free\n", __func__, __LINE__);
		}

		// Free swapchain images 
		
		//for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		//{
		//	vkDestroyImage(DRSJ_vkDevice, DRSJ_vkSwapchainImage_array[i], NULL);
		//	fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkSwapchainImage_array[%d] Freed\n", __func__, __LINE__,i);
		//}

		if (DRSJ_vkSwapchainImage_array)
		{
			free(DRSJ_vkSwapchainImage_array);
			DRSJ_vkSwapchainImage_array = NULL;
			//fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkSwapchainImage_array Free\n", __func__, __LINE__);
		}

		// Destroy Swapchain
		if (DRSJ_vkSwapchainKHR)
		{
			vkDestroySwapchainKHR(DRSJ_vkDevice, DRSJ_vkSwapchainKHR, NULL);
			DRSJ_vkSwapchainKHR = VK_NULL_HANDLE;
			//fprintf(DRSJ_gpLogFile, "%s %d : vkDestroySwapchainKHR() SUCCESSED\n", __func__, __LINE__);
		}
	}


	//----------------------------------------------------
	//
	// - - - - - - - RECREATE FOR RESIZE() - - - - - - - -
	//
	//----------------------------------------------------



	// Create Swapchain
	vkResult = createSwapchain(VK_FALSE);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createSwapchain() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}

	// Create Vulkan Images and Image View
	vkResult = createImagesAndImageViews();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createImagesAndImageViews() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}

	// Create Vulkan Command Buffers
	vkResult = createCommandBuffer();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createCommandBuffer() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}


	// Create Descriptor Set Layout
	vkResult = createPipelineLayout();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createPipelineLayout() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	
	// Create Renderpass
	vkResult = createRenderPass();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createRenderPass() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	
	// Create Pipeline
	vkResult = createPipeline();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createPipeline() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	
	// Create Framebuffer
	vkResult = createFramebuffer();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createFramebuffer() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	

	// Build Command Buffers
	vkResult = buildCommandBuffers();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : buildCommandBuffers() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}

	fprintf(DRSJ_gpLogFile, "%s %d : RECREATE FOR RESIZE() SUCCESSED\n", __func__, __LINE__);

	DRSJ_bInitialized = TRUE;
	return (vkResult);
}

VkResult display(void)
{
	VkResult resize(int width, int height);
	VkResult updateUniformBuffer(void);

	// Local Variables
	VkResult vkResult = VK_SUCCESS;
	BOOL bLogger = FALSE;
	// code

	// If control comes here before initialization is completed then return FALSE.

	if (FALSE == DRSJ_bInitialized)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : Initialzation Yet not completed\n", __func__, __LINE__);
		return (VkResult)VK_FALSE;
	}

	// Aquire index of next swapchain image
	vkResult = vkAcquireNextImageKHR(DRSJ_vkDevice, DRSJ_vkSwapchainKHR, UINT_MAX, DRSJ_vkSemaphore_backBuffer, VK_NULL_HANDLE, &DRSJ_currentImageIndex);
				//3rd para is in nano seconds
	if (VK_SUCCESS != vkResult)
	{
		if (VK_ERROR_OUT_OF_DATE_KHR == vkResult || VK_SUBOPTIMAL_KHR == vkResult)
		{
			resize(winWidth, winHeight);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAcquireNextImageKHR() FAILED [%d]\n", __func__, __LINE__, vkResult);
			return (vkResult);
		}
	}
	else if(bLogger)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkAcquireNextImageKHR() SUCCESSED\n", __func__, __LINE__);
	}

	// Use fence to allow to host to wait for completetion of execution of previous command buffer.

	vkResult = vkWaitForFences(DRSJ_vkDevice, 1, &(DRSJ_vkFence_array[DRSJ_currentImageIndex]), VK_TRUE, UINT64_MAX);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkWaitForFences() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else if (bLogger)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkWaitForFences() SUCCESSED\n", __func__, __LINE__);
	}

	//
	// vkWaitForFences 
	// VK_TRUE -> wait for all fences of siglaed, VK_FALSE-> kuthla ek jari free zala tari ha return yeil.
	// UINT64_MAX -> timeout, kitivela sathi 
	//

	// Now ready the fences for execution of next command buffer
	vkResult = vkResetFences(DRSJ_vkDevice, 1, &(DRSJ_vkFence_array[DRSJ_currentImageIndex]));
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkResetFences() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else if (bLogger)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkResetFences() SUCCESSED\n", __func__, __LINE__);
	}

	// One of the member of VkSubmitInfo structure requires of array of pipeline stages, we have only one
	// of completion of color attachment output still we need one member array.
	const VkPipelineStageFlags waitDestStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	
	//declare and memset initialize VkSubmitInfo structure

	VkSubmitInfo vkSubmitInfo;
	memset((void*)&vkSubmitInfo, 0, sizeof(VkSubmitInfo));

	vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.pNext = NULL;
	vkSubmitInfo.pWaitDstStageMask = &waitDestStageMask;
	vkSubmitInfo.waitSemaphoreCount = 1;
	vkSubmitInfo.pWaitSemaphores = &DRSJ_vkSemaphore_backBuffer;
	vkSubmitInfo.commandBufferCount = 1;
	vkSubmitInfo.pCommandBuffers = &(DRSJ_vkCommandBuffer_array[DRSJ_currentImageIndex]);
	vkSubmitInfo.signalSemaphoreCount = 1;
	vkSubmitInfo.pSignalSemaphores = &DRSJ_vkSemaphore_renderComplete;

	// now submit above work
	vkResult = vkQueueSubmit(DRSJ_vkQueue, 1, &vkSubmitInfo, DRSJ_vkFence_array[DRSJ_currentImageIndex]);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkQueueSubmit() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else if (bLogger)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkQueueSubmit() SUCCESSED\n", __func__, __LINE__);
	}

	// we are going to present render image after declaring and initializing VkPresentInfoKHR structure
	VkPresentInfoKHR vkPresentInfoKHR;
	memset((void*)&vkPresentInfoKHR, 0, sizeof(VkPresentInfoKHR));

	vkPresentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	vkPresentInfoKHR.pNext = NULL;
	vkPresentInfoKHR.swapchainCount = 1;
	vkPresentInfoKHR.pSwapchains = &DRSJ_vkSwapchainKHR;
	vkPresentInfoKHR.pImageIndices = &DRSJ_currentImageIndex;
	vkPresentInfoKHR.waitSemaphoreCount = 1;
	vkPresentInfoKHR.pWaitSemaphores = &DRSJ_vkSemaphore_renderComplete;
	vkPresentInfoKHR.pResults = &vkResult;
	
	// Now present the queue 
	vkResult = vkQueuePresentKHR(DRSJ_vkQueue, &vkPresentInfoKHR);
	if (VK_SUCCESS != vkResult)
	{
		if (VK_ERROR_OUT_OF_DATE_KHR == vkResult || VK_SUBOPTIMAL_KHR == vkResult)
		{
			resize(winWidth, winHeight);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkQueuePresentKHR() FAILED [%d]\n", __func__, __LINE__, vkResult);
			return (vkResult);
		}
	}
	else if (bLogger)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkQueuePresentKHR() SUCCESSED\n", __func__, __LINE__);
	}

	bLogger = FALSE;


	vkResult = updateUniformBuffer();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : updateUniformBuffer() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else if (bLogger)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : updateUniformBuffer() SUCCESSED\n", __func__, __LINE__);
	}
	vkDeviceWaitIdle(DRSJ_vkDevice);
	return (vkResult);
}

void update(void)
{
	// code

}

void uninitialize(void)
{
	// function declarations
	void ToggleFullscreen(void);

	// code

	// Destroy Swapchain
	if (DRSJ_vkSwapchainKHR)
	{
		vkDestroySwapchainKHR(DRSJ_vkDevice, DRSJ_vkSwapchainKHR, NULL);
		DRSJ_vkSwapchainKHR = VK_NULL_HANDLE;
		fprintf(DRSJ_gpLogFile, "%s %d : vkDestroySwapchainKHR() SUCCESSED\n", __func__, __LINE__);
	}

	// Destroy Vulkan device
	if (DRSJ_vkDevice)
	{
		vkDeviceWaitIdle(DRSJ_vkDevice);
		fprintf(DRSJ_gpLogFile, "%s %d : vkDeviceWaitIdle() is DONE\n", __func__, __LINE__);
		
		for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		{
			vkDestroyFence(DRSJ_vkDevice, DRSJ_vkFence_array[i], NULL);
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyFence() SUCCESSED for index:[%d]\n", __func__, __LINE__, i);
		}

		if (DRSJ_vkFence_array)
		{
			free(DRSJ_vkFence_array);
			DRSJ_vkFence_array = NULL;
			fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkFence_array Free\n", __func__, __LINE__);
		}

		if(DRSJ_vkSemaphore_renderComplete)
		{
			vkDestroySemaphore(DRSJ_vkDevice, DRSJ_vkSemaphore_renderComplete, NULL);
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroySemaphore() SUCCESSED for DRSJ_vkSemaphore_renderComplete\n", __func__, __LINE__);
			DRSJ_vkSemaphore_renderComplete = VK_NULL_HANDLE;
		}

		if(DRSJ_vkSemaphore_backBuffer)
		{
			vkDestroySemaphore(DRSJ_vkDevice, DRSJ_vkSemaphore_backBuffer, NULL);
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroySemaphore() SUCCESSED for DRSJ_vkSemaphore_backBuffer\n", __func__, __LINE__);
			DRSJ_vkSemaphore_backBuffer = VK_NULL_HANDLE;
		}
		
		// Free swapchain images views
		for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		{
			vkDestroyImageView(DRSJ_vkDevice, DRSJ_vkSwapchainImageView_array[i], NULL);
			fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkSwapchainImageView_array[%d] Freed\n", __func__, __LINE__, i);
		}

		free(DRSJ_vkSwapchainImageView_array);
		DRSJ_vkSwapchainImageView_array = NULL;
		fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkSwapchainImageView_array Free\n", __func__, __LINE__);

		// Free swapchain images 
		//for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		//{
		//	vkDestroyImage(DRSJ_vkDevice, DRSJ_vkSwapchainImage_array[i], NULL);
		//	fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkSwapchainImage_array[%d] Freed\n", __func__, __LINE__,i);
		//}

		for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		{
			vkDestroyFramebuffer(DRSJ_vkDevice, DRSJ_vkFramebuffer_array[i], NULL);
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyFramebuffer() SUCCESSED for index:[%d]\n", __func__, __LINE__, i);
		}

		if (DRSJ_vkFramebuffer_array)
		{
			free(DRSJ_vkFramebuffer_array);
			DRSJ_vkFramebuffer_array = NULL;
		}


		if (vkDescriptorSetLayout)
		{
			vkDestroyDescriptorSetLayout(DRSJ_vkDevice, vkDescriptorSetLayout, NULL);
			vkDescriptorSetLayout = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyDescriptorSetLayout() SUCCESSED\n", __func__, __LINE__);
		}

		if (vkDescriptorPool)	// when descriptor pool is destory, descriptor set destroyed automatically
		{
			vkDestroyDescriptorPool(DRSJ_vkDevice, vkDescriptorPool, NULL);
			vkDescriptorPool = VK_NULL_HANDLE;
			vkDescriptorSet = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyDescriptorPool() SUCCESSED\n", __func__, __LINE__);
		}

		if (vkPipelineLayout)
		{
			vkDestroyPipelineLayout(DRSJ_vkDevice, vkPipelineLayout, NULL);
			vkPipelineLayout = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyPipelineLayout() SUCCESSED\n", __func__, __LINE__);
		}

		if (vkPipeline)
		{
			vkDestroyPipeline(DRSJ_vkDevice, vkPipeline, NULL);
			vkPipeline = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyPipeline() SUCCESSED\n", __func__, __LINE__);
		}

		vkDestroyRenderPass(DRSJ_vkDevice, vkRenderPass, NULL);
		fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyRenderPass() is SUCCESSED\n", __func__, __LINE__);

		// Destroy Shader modules
		if (vkShaderModule_fragment_shader)
		{
			vkDestroyShaderModule(DRSJ_vkDevice, vkShaderModule_fragment_shader, NULL);
			vkShaderModule_fragment_shader = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyShaderModule() is SUCCESSED for Fragment Shader \n", __func__, __LINE__);
		}

		if (vkShaderModule_vertex_shader)
		{
			vkDestroyShaderModule(DRSJ_vkDevice, vkShaderModule_vertex_shader, NULL);
			vkShaderModule_vertex_shader = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyShaderModule() is SUCCESSED for Vertex Shader \n", __func__, __LINE__);
		}

		for (uint32_t i = 0; i < DRSJ_swapchainImageCount; i++)
		{
			vkFreeCommandBuffers(DRSJ_vkDevice, DRSJ_vkCommandPool, 1, &(DRSJ_vkCommandBuffer_array[i]));
			fprintf(DRSJ_gpLogFile, "%s %d : vkFreeCommandBuffers() SUCCESSED for index:[%d]\n", __func__, __LINE__,i);
		}


		
		
		// Destory uniform buffer for road_border
		if (uniformData_road_border.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, uniformData_road_border.vkBuffer, NULL);
			uniformData_road_border.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : uniformData_road_border.vkBuffer Free\n", __func__, __LINE__);
		}

		if (uniformData_road_border.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, uniformData_road_border.vkDeviceMemory, NULL);
			uniformData_road_border.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : uniformData_road_border.vkDeviceMemory Free\n", __func__, __LINE__);
		}
		
		// Destory color vertex buffer for STAR
		if (vertexData_color_2.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, vertexData_color_2.vkDeviceMemory, NULL);
			vertexData_color_2.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_color_2.vkDeviceMemory Free\n", __func__, __LINE__);
		}
		if (vertexData_color_2.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, vertexData_color_2.vkBuffer, NULL);
			vertexData_color_2.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_color_2.vkBuffer Free\n", __func__, __LINE__);
		}
		
		// Destory position vertex buffer for STAR
		if (vertexData_position_2.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, vertexData_position_2.vkDeviceMemory, NULL);
			vertexData_position_2.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_position_2.vkDeviceMemory Free\n", __func__, __LINE__);
		}
		if (vertexData_position_2.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, vertexData_position_2.vkBuffer, NULL);
			vertexData_position_2.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_position_2.vkBuffer Free\n", __func__, __LINE__);
		}

		// Destory position vertex buffer for rectangle
		if (vertexData_position_zebra_crossing.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, vertexData_position_zebra_crossing.vkDeviceMemory, NULL);
			vertexData_position_zebra_crossing.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_position_zebra_crossing.vkDeviceMemory Free\n", __func__, __LINE__);
		}

		if (vertexData_position_zebra_crossing.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, vertexData_position_zebra_crossing.vkBuffer, NULL);
			vertexData_position_zebra_crossing.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_position_zebra_crossing.vkBuffer Free\n", __func__, __LINE__);
		}

		// Destory position vertex buffer for rectangle
		if (vertexData_position_main_road.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, vertexData_position_main_road.vkDeviceMemory, NULL);
			vertexData_position_main_road.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_position_main_road.vkDeviceMemory Free\n", __func__, __LINE__);
		}

		if (vertexData_position_main_road.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, vertexData_position_main_road.vkBuffer, NULL);
			vertexData_position_main_road.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_position_main_road.vkBuffer Free\n", __func__, __LINE__);
		}

		// Destory position vertex buffer for triangle
		if (vertexData_position_road_border.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, vertexData_position_road_border.vkDeviceMemory, NULL);
			vertexData_position_road_border.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_position_road_border.vkDeviceMemory Free\n", __func__, __LINE__);
		}

		if (vertexData_position_road_border.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, vertexData_position_road_border.vkBuffer, NULL);
			vertexData_position_road_border.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_position_road_border.vkBuffer Free\n", __func__, __LINE__);
		}

		// Destory color vertex buffer for rectangle
		if (vertexData_color_zebra_crossing.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, vertexData_color_zebra_crossing.vkDeviceMemory, NULL);
			vertexData_color_zebra_crossing.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_color_zebra_crossing.vkDeviceMemory Free\n", __func__, __LINE__);
		}

		if (vertexData_color_zebra_crossing.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, vertexData_color_zebra_crossing.vkBuffer, NULL);
			vertexData_color_zebra_crossing.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_color_zebra_crossing.vkBuffer Free\n", __func__, __LINE__);
		}

		// Destory color vertex buffer for rectangle
		if (vertexData_color_main_road.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, vertexData_color_main_road.vkDeviceMemory, NULL);
			vertexData_color_main_road.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_color_main_road.vkDeviceMemory Free\n", __func__, __LINE__);
		}

		if (vertexData_color_main_road.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, vertexData_color_main_road.vkBuffer, NULL);
			vertexData_color_main_road.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_color_main_road.vkBuffer Free\n", __func__, __LINE__);
		}

		// Destory color vertex buffer for triangle
		if (vertexData_color_road_border.vkDeviceMemory)
		{
			vkFreeMemory(DRSJ_vkDevice, vertexData_color_road_border.vkDeviceMemory, NULL);
			vertexData_color_road_border.vkDeviceMemory = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_color_road_border.vkDeviceMemory Free\n", __func__, __LINE__);
		}

		if (vertexData_color_road_border.vkBuffer)
		{
			vkDestroyBuffer(DRSJ_vkDevice, vertexData_color_road_border.vkBuffer, NULL);
			vertexData_color_road_border.vkBuffer = VK_NULL_HANDLE;
			fprintf(DRSJ_gpLogFile, "%s %d : vertexData_color_road_border.vkBuffer Free\n", __func__, __LINE__);
		}



		if(DRSJ_vkCommandBuffer_array)
		{
			free(DRSJ_vkCommandBuffer_array);
			DRSJ_vkCommandBuffer_array = NULL;
			fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkCommandBuffer_array Free\n", __func__, __LINE__);
		}

		if (DRSJ_vkCommandPool)
		{
			vkDestroyCommandPool(DRSJ_vkDevice, DRSJ_vkCommandPool, NULL);
			fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyCommandPool() SUCCESSED\n", __func__, __LINE__);
		}

		vkDestroyDevice(DRSJ_vkDevice, NULL);
		DRSJ_vkDevice = VK_NULL_HANDLE;
		fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyDevice() SUCCESSED\n", __func__, __LINE__);
	}

	free(DRSJ_vkSwapchainImage_array);
	DRSJ_vkSwapchainImage_array = NULL;
	fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkSwapchainImage_array Free\n", __func__, __LINE__);

	// No need to destroy selected physical device

	if (DRSJ_vkSurfaceKHR)
	{
		vkDestroySurfaceKHR(DRSJ_vkInstance, DRSJ_vkSurfaceKHR, NULL);
		DRSJ_vkSurfaceKHR = VK_NULL_HANDLE;
		fprintf(DRSJ_gpLogFile, "%s %d : vkDestroySurfaceKHR() SUCCESSED\n", __func__, __LINE__);
	}

	if (vkDebugReportCallbackEXT && vkDestroyDebugReportCallbackEXT_fnptr)
	{
		vkDestroyDebugReportCallbackEXT_fnptr(DRSJ_vkInstance, vkDebugReportCallbackEXT, NULL);
		vkDebugReportCallbackEXT = VK_NULL_HANDLE;
		fprintf(DRSJ_gpLogFile, "%s %d : vkDebugReportCallbackEXT destroy SUCCESSED\n", __func__, __LINE__);
		vkDestroyDebugReportCallbackEXT_fnptr = NULL;
	}

	if (DRSJ_vkInstance)
	{
		vkDestroyInstance(DRSJ_vkInstance, NULL);
		DRSJ_vkInstance = VK_NULL_HANDLE;
		DRSJ_vkSurfaceKHR = VK_NULL_HANDLE;
		fprintf(DRSJ_gpLogFile, "%s %d : vkDestroyInstance() SUCCESSED\n", __func__, __LINE__);
	}

	// if application is exiting in full screen then this code will execute
	if (TRUE == DRSJ_gbFullscreen)
	{
		ToggleFullscreen();
		DRSJ_gbFullscreen = FALSE;
	}

	// close the log file
	if (DRSJ_gpLogFile)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : Program is Succesfully Ended!!!\n", __func__, __LINE__);
		fclose(DRSJ_gpLogFile);
		DRSJ_gpLogFile = NULL;
	}

}

/******************************************************
//
// Vulkan Related Userdefine Functions
//
******************************************************/

/******************************************************
* Name : createVulkanInstance
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*	
*   
*
******************************************************/
VkResult createVulkanInstance(void)
{
	// function declarations
	VkResult fillInstanceExtensionNames(void);
	VkResult fillValidationLayerName(void);
	VkResult createValidationCallbackFunction(void);


	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	//Step 1 : As explain before filled and initialize required extension names and count in global variables.
	vkResult = fillInstanceExtensionNames();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : fillInstanceExtensionNames() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : fillInstanceExtensionNames() SUCCESSED\n", __func__, __LINE__);
	}

	if (TRUE == bValidation)
	{
		// filled validation layers
		vkResult = fillValidationLayerName();
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s, %d, : fillValidationLayerName() FAILED\n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s, %d, : fillValidationLayerName() SUCCESSED\n", __func__, __LINE__);
		}

	}

	//Step 2:Initialize struct VkApplicationInfo.

	VkApplicationInfo vkApplicationInfo;
	memset((void*)&vkApplicationInfo, 0, sizeof(VkApplicationInfo));

	vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkApplicationInfo.pNext = NULL;
	vkApplicationInfo.pApplicationName = DRSJ_gpAppName;
	vkApplicationInfo.applicationVersion = 1;
	vkApplicationInfo.pEngineName = DRSJ_gpAppName;
	vkApplicationInfo.engineVersion = 1;
	vkApplicationInfo.apiVersion = VK_API_VERSION_1_3;



	//Step 3:Initialize struct VkInstanceCreateInfo by using information step 1 and step 2.

	VkInstanceCreateInfo vkInstanceCreateInfo;
	memset((void*)&vkInstanceCreateInfo, 0, sizeof(VkInstanceCreateInfo));

	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pNext = NULL;
	vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;
	vkInstanceCreateInfo.enabledExtensionCount = DRSJ_enabledInstanceExtensionCount;
	vkInstanceCreateInfo.ppEnabledExtensionNames = DRSJ_enabledInstanceExtensionNames_array;

	if (TRUE == bValidation)
	{
		vkInstanceCreateInfo.enabledLayerCount = enabledValidationLayerCount;
		vkInstanceCreateInfo.ppEnabledLayerNames = enabledValidationLayerName_array;
	}
	else
	{
		vkInstanceCreateInfo.enabledLayerCount = 0;
		vkInstanceCreateInfo.ppEnabledLayerNames = NULL;
	}

	//Step 4 :Call vkCreateInstance() to get VkInstnace in global variable and do error checking.
	vkResult = vkCreateInstance(&vkInstanceCreateInfo, NULL, &DRSJ_vkInstance);
	if (VK_ERROR_INCOMPATIBLE_DRIVER == vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkCreateInstance() FAILED due to incompatible driver. (%d)\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else if (VK_ERROR_EXTENSION_NOT_PRESENT == vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkCreateInstance() FAILED due to required extension not present. (%d)\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else if (VK_ERROR_LAYER_NOT_PRESENT == vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkCreateInstance() FAILED due to required error layer not present. (%d)\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}	
	else if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkCreateInstance() FAILED due to unkown reason. (%d)\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkCreateInstance() SUCCESSED\n", __func__, __LINE__);
	}

	// Doing Validation Callback
	if (TRUE == bValidation)
	{
		vkResult = createValidationCallbackFunction();
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s, %d, : createValidationCallbackFunction() FAILED\n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s, %d, : createValidationCallbackFunction() SUCCESSED\n", __func__, __LINE__);
		}
	}
	return (vkResult);
}

/******************************************************
* Name : fillInstanceExtensionNames
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*	We fetch all supported extensions.
*   And filled needed extensions name from all extesions.
*
******************************************************/
VkResult fillInstanceExtensionNames(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;
	uint32_t instanceExtenstionCount = 0;

	//Step 1: Find how many instance extensions are supported by the Vulkan driver of this version.
	vkResult = vkEnumerateInstanceExtensionProperties(NULL, &instanceExtenstionCount, NULL);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceExtensionProperties() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceExtensionProperties() SUCCESSED\n", __func__, __LINE__);
	}


	//Step 2:Allocate and fill the struct VkExtensionProperties array coresponding to the above count.

	VkExtensionProperties* vkExntesionPropertiesArr = NULL;

	vkExntesionPropertiesArr = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * instanceExtenstionCount);
	if (!vkExntesionPropertiesArr)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : malloc() FAILED\n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	vkResult = vkEnumerateInstanceExtensionProperties(NULL, &instanceExtenstionCount, vkExntesionPropertiesArr);

	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceExtensionProperties() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceExtensionProperties() SUCCESSED\n", __func__, __LINE__);
	}

	//Step 3:Fill and display a local string array of extension names obtained from VkExtensionProperties Array.
	char** instanceExtensionNamesArr = NULL;

	instanceExtensionNamesArr = (char**)calloc(instanceExtenstionCount, sizeof(char*));
	if (!instanceExtensionNamesArr)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : calloc() FAILED\n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	for (uint32_t iCounter = 0; iCounter < instanceExtenstionCount; iCounter++)
	{
		uint32_t strLength = strlen(vkExntesionPropertiesArr[iCounter].extensionName) + 1;

		instanceExtensionNamesArr[iCounter] = (char*)calloc(strLength, sizeof(char));

		memcpy_s(instanceExtensionNamesArr[iCounter], strLength, vkExntesionPropertiesArr[iCounter].extensionName, strLength);

		fprintf(DRSJ_gpLogFile, "%s %d :  Vulkan Instance Extension Names [%d] = %s\n", __func__, __LINE__, iCounter, instanceExtensionNamesArr[iCounter]);
	}

	//Step 4:As not required here onwards, free VkExtensionProperties array.
	free(vkExntesionPropertiesArr);
	vkExntesionPropertiesArr = NULL;

	/*
	* Step 5:
		Find whether the above extension names contain our required 2 extensions.
		(1) VK_KHR_SURFACE_EXTENSION_NAME (VK_KHR_surface : actual name)
		(2) VK_KHR_WIN32_SURFACE_EXTENSION_NAME (VK_KHR_win32 : actual name)

		Accordingly set 2 global variables,
		(a) required extension count.
		(b) required extension names array.

	*/

	VkBool32 vulkanSurfaceExtensionFound = VK_FALSE;
	VkBool32 win32SurfaceExtensionFound = VK_FALSE;
	VkBool32 debugReportExtensionFound = VK_FALSE;

	for (uint32_t iCounter = 0; iCounter < instanceExtenstionCount; iCounter++)
	{
		if (!strcmp(instanceExtensionNamesArr[iCounter], VK_KHR_SURFACE_EXTENSION_NAME))
		{
			vulkanSurfaceExtensionFound = VK_TRUE;
			DRSJ_enabledInstanceExtensionNames_array[DRSJ_enabledInstanceExtensionCount++] = VK_KHR_SURFACE_EXTENSION_NAME;
		}

		if (!strcmp(instanceExtensionNamesArr[iCounter], VK_KHR_WIN32_SURFACE_EXTENSION_NAME))
		{
			win32SurfaceExtensionFound = VK_TRUE;
			DRSJ_enabledInstanceExtensionNames_array[DRSJ_enabledInstanceExtensionCount++] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
		}

		if (!strcmp(instanceExtensionNamesArr[iCounter], VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
		{	
			if (bValidation)
			{
				debugReportExtensionFound = VK_TRUE;
				DRSJ_enabledInstanceExtensionNames_array[DRSJ_enabledInstanceExtensionCount++] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_enabledInstanceExtensionNames_array not contain entry of VK_EXT_DEBUG_REPORT_EXTENSION_NAME\n", __func__, __LINE__);
			}
		}
		

	}


	//Step 6:As not needed, hence for free the local string array.
	for (uint32_t iCounter = 0; iCounter < instanceExtenstionCount; iCounter++)
	{
		free(instanceExtensionNamesArr[iCounter]);
	}

	free(instanceExtensionNamesArr);
	instanceExtensionNamesArr = NULL;


	//Step 7:Print whether our Vulkan driver supports our required extensions or not.
	if (VK_FALSE == vulkanSurfaceExtensionFound)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
		fprintf(DRSJ_gpLogFile, "%s %d : VK_KHR_SURFACE_EXTENSION_NAME NOT-FOUND\n", __func__, __LINE__);
		return (vkResult);
	}
	else
		fprintf(DRSJ_gpLogFile, "%s %d : VK_KHR_SURFACE_EXTENSION_NAME FOUND\n", __func__, __LINE__);


	if (VK_FALSE == win32SurfaceExtensionFound)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
		fprintf(DRSJ_gpLogFile, "%s %d : VK_KHR_WIN32_SURFACE_EXTENSION_NAME NOT-FOUND\n", __func__, __LINE__);
		return (vkResult);
	}
	else
		fprintf(DRSJ_gpLogFile, "%s %d : VK_KHR_WIN32_SURFACE_EXTENSION_NAME FOUND\n", __func__, __LINE__);

	if (VK_FALSE == debugReportExtensionFound)
	{
		if (bValidation)
		{
			vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
			fprintf(DRSJ_gpLogFile, "%s %d : Validation is On But Required VK_EXT_DEBUG_REPORT_EXTENSION_NAME NOT-FOUND\n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : Validation OFF and VK_EXT_DEBUG_REPORT_EXTENSION_NAME NOT-FOUND\n", __func__, __LINE__);
		}
	}
	else
	{
		if (bValidation)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : Validation is On and VK_EXT_DEBUG_REPORT_EXTENSION_NAME FOUND\n", __func__, __LINE__);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : Validation OFF and VK_EXT_DEBUG_REPORT_EXTENSION_NAME FOUND\n", __func__, __LINE__);
		}
	}
		
	//Steps 8: Print only supported extension names.
	for (uint32_t iCounter = 0; iCounter < DRSJ_enabledInstanceExtensionCount; iCounter++)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : Enabled Vulkan Instance Extension Name = %s\n", __func__, __LINE__, DRSJ_enabledInstanceExtensionNames_array[iCounter]);
	}

	return (vkResult);
}

/******************************************************
* Name : fillValidationLayerName
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This functions fill validation layer names into
*		global validation layer name array
* 
******************************************************/
VkResult fillValidationLayerName(void)
{
	// local variable declaration
	VkResult vkResult = VK_SUCCESS;
	uint32_t validationLayerCount = 0;
	

	// code
	vkResult = vkEnumerateInstanceLayerProperties(&validationLayerCount, NULL);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceLayerProperties() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else if (0 == validationLayerCount)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceLayerProperties() SUCCESSED But no Instance Layer Found\n", __func__, __LINE__);
		return (VK_ERROR_INITIALIZATION_FAILED);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceLayerProperties() SUCCESSED : validationLayerCount= %d\n", __func__, __LINE__, validationLayerCount);
	}

	VkLayerProperties* vkLayerProperties_array = NULL;

	vkLayerProperties_array = (VkLayerProperties*)calloc(validationLayerCount, sizeof(VkLayerProperties));
	if (!vkLayerProperties_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : memory allocation FAILED\n", __func__, __LINE__);
		return (VK_ERROR_OUT_OF_HOST_MEMORY);
	}

	vkResult = vkEnumerateInstanceLayerProperties(&validationLayerCount, vkLayerProperties_array);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceLayerProperties() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateInstanceLayerProperties() SUCCESSED : validationLayerCount= %d\n", __func__, __LINE__, validationLayerCount);
	}

	char** validationLayerNames_array = NULL;
	validationLayerNames_array = (char**)calloc(validationLayerCount, sizeof(char*));
	if (!validationLayerNames_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : memory allocation FAILED\n", __func__, __LINE__);
		return (VK_ERROR_OUT_OF_HOST_MEMORY);
	}

	for (uint32_t iCounter = 0; iCounter < validationLayerCount; iCounter++)
	{
		uint32_t strLength = strlen(vkLayerProperties_array[iCounter].layerName) + 1;
		validationLayerNames_array[iCounter] = (char*)calloc(strLength, sizeof(char));
		if (!validationLayerNames_array[iCounter])
		{
			fprintf(DRSJ_gpLogFile, "%s %d : memory allocation FAILED\n", __func__, __LINE__);
			return (VK_ERROR_OUT_OF_HOST_MEMORY);
		}

		memcpy_s(validationLayerNames_array[iCounter], strLength, vkLayerProperties_array[iCounter].layerName, strLength);

		fprintf(DRSJ_gpLogFile, "%s %d : Vulkan Instance Layer Names [%d] = %s\n", __func__, __LINE__, iCounter, validationLayerNames_array[iCounter]);
	}

	//As not required here onwards, free vkLayerProperties_array array.
	free(vkLayerProperties_array);
	vkLayerProperties_array = NULL;
	
	// For required one layer
	VkBool32 validationLayerFound = VK_FALSE;

	for (uint32_t iCounter = 0; iCounter < validationLayerCount; iCounter++)
	{
		if (!strcmp(validationLayerNames_array[iCounter], "VK_LAYER_KHRONOS_validation"))
		{
			validationLayerFound = VK_TRUE;
			enabledValidationLayerName_array[enabledValidationLayerCount++] = "VK_LAYER_KHRONOS_validation";
		}
	}

	for (uint32_t iCounter = 0; iCounter < validationLayerCount; iCounter++)
	{
		free(validationLayerNames_array[iCounter]);
	}

	free(validationLayerNames_array);
	validationLayerNames_array = NULL;

	if (VK_FALSE == validationLayerFound)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
		fprintf(DRSJ_gpLogFile, "%s %d : VK_LAYER_KHRONOS_validation NOT-FOUND\n", __func__, __LINE__);
		return (vkResult);
	}
	else
		fprintf(DRSJ_gpLogFile, "%s %d : VK_LAYER_KHRONOS_validation FOUND\n", __func__, __LINE__);


	// Print only supported Layer names.
	for (uint32_t iCounter = 0; iCounter < enabledValidationLayerCount; iCounter++)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : Enabled Vulkan Validation Layer Name = %s\n", __func__, __LINE__, enabledValidationLayerName_array[iCounter]);
	}

	return (vkResult);
}

/******************************************************
* Name : createValidationCallbackFunction
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*	
*   
*
******************************************************/
VkResult createValidationCallbackFunction(void)
{
	// Local function declaration
	VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char*, void*);

	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT_fnptr = NULL;

	// local variable declaration
	VkResult vkResult = VK_SUCCESS;


	// code
	
	// get the required functions pointers
	
	vkCreateDebugReportCallbackEXT_fnptr = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(DRSJ_vkInstance, "vkCreateDebugReportCallbackEXT");
	if (NULL == vkCreateDebugReportCallbackEXT_fnptr)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetInstanceProcAddr FAILED for vkCreateDebugReportCallbackEXT() \n", __func__, __LINE__);
		return (vkResult);
	}
	else
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetInstanceProcAddr SUCCESS for vkCreateDebugReportCallbackEXT() \n", __func__, __LINE__);

	vkDestroyDebugReportCallbackEXT_fnptr = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(DRSJ_vkInstance, "vkDestroyDebugReportCallbackEXT");
	if (NULL == vkDestroyDebugReportCallbackEXT_fnptr)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetInstanceProcAddr FAILED for vkDestroyDebugReportCallbackEXT() \n", __func__, __LINE__);
		return (vkResult);
	}
	else
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetInstanceProcAddr SUCCESS for vkDestroyDebugReportCallbackEXT() \n", __func__, __LINE__);

	// Get the vulkan debug report callback object	
	
	VkDebugReportCallbackCreateInfoEXT vkDebugReportCallbackCreateInfoEXT;
	memset((void*)&vkDebugReportCallbackCreateInfoEXT, 0, sizeof(VkDebugReportCallbackCreateInfoEXT));

	vkDebugReportCallbackCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	vkDebugReportCallbackCreateInfoEXT.pNext = NULL;
	vkDebugReportCallbackCreateInfoEXT.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	vkDebugReportCallbackCreateInfoEXT.pfnCallback = debugReportCallback;
	vkDebugReportCallbackCreateInfoEXT.pUserData = NULL;

	vkResult = vkCreateDebugReportCallbackEXT_fnptr(DRSJ_vkInstance, &vkDebugReportCallbackCreateInfoEXT, NULL, &vkDebugReportCallbackEXT);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateDebugReportCallbackEXT_fnptr() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateDebugReportCallbackEXT_fnptr() SUCCESSED\n", __func__, __LINE__);
	}

	return (vkResult);
}

/******************************************************
* Name : getSupportedSurface
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*	
*   
*
******************************************************/
VkResult getSupportedSurface(void)
{
	// local variable declaration
	VkResult vkResult = VK_SUCCESS;

	// code
	VkWin32SurfaceCreateInfoKHR vkWin32SurfaceCreateInfoKHR;

	memset((void*) & vkWin32SurfaceCreateInfoKHR, 0, sizeof(VkWin32SurfaceCreateInfoKHR));

	vkWin32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	vkWin32SurfaceCreateInfoKHR.pNext = NULL;
	vkWin32SurfaceCreateInfoKHR.flags = 0;
	vkWin32SurfaceCreateInfoKHR.hinstance = (HINSTANCE)GetWindowLongPtr(DRSJ_ghwnd, GWLP_HINSTANCE); // we can initialize by using GetModule()
	vkWin32SurfaceCreateInfoKHR.hwnd = DRSJ_ghwnd;
	
	vkResult = vkCreateWin32SurfaceKHR(DRSJ_vkInstance, &vkWin32SurfaceCreateInfoKHR, NULL, &DRSJ_vkSurfaceKHR);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateWin32SurfaceKHR() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateWin32SurfaceKHR() SUCCESSED\n", __func__, __LINE__);
	}

	return (vkResult);
}

/******************************************************
* Name : getPhysicalDevice
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function get physical devices
*
*
******************************************************/
VkResult getPhysicalDevice(void)
{
	// local variable declaration
	VkResult vkResult = VK_SUCCESS;
	

	// code
	vkResult = vkEnumeratePhysicalDevices(DRSJ_vkInstance, &DRSJ_physicalDeviceCount, NULL);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumeratePhysicalDevices() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else if (0 == DRSJ_physicalDeviceCount)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumeratePhysicalDevices() SUCCESSED But no device Found\n", __func__, __LINE__);
		return (VK_ERROR_INITIALIZATION_FAILED);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumeratePhysicalDevices() SUCCESSED\n", __func__, __LINE__);
	}

	
	DRSJ_vkPhysicalDevice_array = (VkPhysicalDevice*)calloc(DRSJ_physicalDeviceCount, sizeof(VkPhysicalDevice));
	if (!DRSJ_vkPhysicalDevice_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : memory allocation FAILED\n", __func__, __LINE__);
		return (VK_ERROR_OUT_OF_HOST_MEMORY);
	}


	vkResult = vkEnumeratePhysicalDevices(DRSJ_vkInstance, &DRSJ_physicalDeviceCount, DRSJ_vkPhysicalDevice_array);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumeratePhysicalDevices() FAILED\n", __func__, __LINE__);
		free(DRSJ_vkPhysicalDevice_array);
		DRSJ_vkPhysicalDevice_array = NULL;
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumeratePhysicalDevices() SUCCESSED\n", __func__, __LINE__);
	}

	VkBool32 bFound = VK_FALSE;

	for (uint32_t iCounter1 = 0; iCounter1 < DRSJ_physicalDeviceCount; iCounter1++)
	{
		uint32_t queueCount = UINT32_MAX;

		vkGetPhysicalDeviceQueueFamilyProperties(DRSJ_vkPhysicalDevice_array[iCounter1], &queueCount, NULL);
		// If physical device present then it must have at least 1 queue family

		VkQueueFamilyProperties* vkQueueFamilyProperties_array = NULL;
		vkQueueFamilyProperties_array = (VkQueueFamilyProperties*)calloc(queueCount, sizeof(VkQueueFamilyProperties));
		if (!vkQueueFamilyProperties_array)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : memory allocation FAILED\n", __func__, __LINE__);
			return (VK_ERROR_OUT_OF_HOST_MEMORY);
		}

		vkGetPhysicalDeviceQueueFamilyProperties(DRSJ_vkPhysicalDevice_array[iCounter1], &queueCount, vkQueueFamilyProperties_array);

		VkBool32* isQueueSurfaceSupported_array = NULL;
		isQueueSurfaceSupported_array = (VkBool32*)calloc(queueCount, sizeof(VkBool32));
		if (!isQueueSurfaceSupported_array)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : memory allocation FAILED\n", __func__, __LINE__);
			return (VK_ERROR_OUT_OF_HOST_MEMORY);
		}


		for (uint32_t iCounter2 = 0; iCounter2 < queueCount; iCounter2++)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(DRSJ_vkPhysicalDevice_array[iCounter1], iCounter2, DRSJ_vkSurfaceKHR, &isQueueSurfaceSupported_array[iCounter2]);
		}

		for (uint32_t iCounter2 = 0; iCounter2 < queueCount; iCounter2++)
		{
			if (vkQueueFamilyProperties_array[iCounter2].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				if (VK_TRUE == isQueueSurfaceSupported_array[iCounter2])
				{
					DRSJ_vkPhysicalDevice_selected = DRSJ_vkPhysicalDevice_array[iCounter1];
					DRSJ_graphicsQueueFamilyIndex_selected = iCounter2;
					bFound = VK_TRUE;
					break;
				}
			}
		}

		if (isQueueSurfaceSupported_array)
		{
			free(isQueueSurfaceSupported_array);
			isQueueSurfaceSupported_array = NULL;
			fprintf(DRSJ_gpLogFile, "%s %d : isQueueSurfaceSupported_array memory is free \n", __func__, __LINE__);
		}

		if (vkQueueFamilyProperties_array)
		{
			free(vkQueueFamilyProperties_array);
			vkQueueFamilyProperties_array = NULL;
			fprintf(DRSJ_gpLogFile, "%s %d : vkQueueFamilyProperties_array memory is free \n", __func__, __LINE__);
		}

		if (VK_TRUE == bFound)
		{
			break;
		}
	}

	

	if (VK_TRUE == bFound)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : get physical devices with graphics enable SUCCESSED \n", __func__, __LINE__);
	}
	else
	{

		fprintf(DRSJ_gpLogFile, "%s %d : Not getting any physical devices with graphics enable FAILED \n", __func__, __LINE__);

		if (DRSJ_vkPhysicalDevice_array)
		{
			free(DRSJ_vkPhysicalDevice_array);
			DRSJ_vkPhysicalDevice_array = NULL;
			fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkPhysicalDevice_array memory is free \n", __func__, __LINE__);
		}

		return (VK_ERROR_INITIALIZATION_FAILED);
	}
	
	memset((void*)&DRSJ_vkPhysicalDeviceMemoryProperties, 0, sizeof(VkPhysicalDeviceMemoryProperties));

	vkGetPhysicalDeviceMemoryProperties(DRSJ_vkPhysicalDevice_selected, &DRSJ_vkPhysicalDeviceMemoryProperties);

	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
	memset((void*)&vkPhysicalDeviceFeatures, 0, sizeof(VkPhysicalDeviceFeatures));

	vkGetPhysicalDeviceFeatures(DRSJ_vkPhysicalDevice_selected, &vkPhysicalDeviceFeatures);
	
	if (VK_TRUE == vkPhysicalDeviceFeatures.tessellationShader)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : selected physical device supports 'tessellationShader' \n", __func__, __LINE__);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : selected physical device NOT supports 'tessellationShader' \n", __func__, __LINE__);
	}

	if (VK_TRUE == vkPhysicalDeviceFeatures.geometryShader)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : selected physical device supports 'geometryShader' \n", __func__, __LINE__);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : selected physical device NOT supports 'geometryShader' \n", __func__, __LINE__);
	}

	return (vkResult);
}

/******************************************************
* Name : printVkInfo
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will print the information 
*		about Vulkan.
*
******************************************************/
VkResult printVkInfo(void)
{
	// local variable declaration
	VkResult vkResult = VK_SUCCESS;

	// code

	fprintf(DRSJ_gpLogFile, "\n\t - - - - - - - - - - - - - - - - - - - - - -\n");
	fprintf(DRSJ_gpLogFile, "\t * * * * * Start Vulkan Information * * * * *\n");

	for (uint32_t iCounter1 = 0; iCounter1 < DRSJ_physicalDeviceCount; iCounter1++)
	{

		fprintf(DRSJ_gpLogFile, "\n");

		VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
		memset((void*) & vkPhysicalDeviceProperties, 0, sizeof(VkPhysicalDeviceProperties));

		vkGetPhysicalDeviceProperties(DRSJ_vkPhysicalDevice_array[iCounter1], &vkPhysicalDeviceProperties);

		fprintf(DRSJ_gpLogFile, "\t Device Name : %s\n",vkPhysicalDeviceProperties.deviceName);

		uint32_t majorVersion = VK_API_VERSION_MAJOR(vkPhysicalDeviceProperties.apiVersion);
		uint32_t minorVersion = VK_API_VERSION_MINOR(vkPhysicalDeviceProperties.apiVersion);
		uint32_t patchVersion = VK_API_VERSION_PATCH(vkPhysicalDeviceProperties.apiVersion);

		fprintf(DRSJ_gpLogFile, "\t Version     : %d.%d.%d\n",majorVersion, minorVersion, patchVersion);
		
		switch (vkPhysicalDeviceProperties.deviceType)
		{
		case VK_PHYSICAL_DEVICE_TYPE_OTHER :
			fprintf(DRSJ_gpLogFile, "\t Device Type : Other \n");
			break;

		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU :
			fprintf(DRSJ_gpLogFile, "\t Device Type : Integrated GPU (IGPU)\n");
			break;

		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU :
			fprintf(DRSJ_gpLogFile, "\t Device Type : Discrete GPU (DGPU)\n");
			break;

		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU :
			fprintf(DRSJ_gpLogFile, "\t Device Type : Virtual GPU (VGPU)\n");
			break;

		case VK_PHYSICAL_DEVICE_TYPE_CPU :
			fprintf(DRSJ_gpLogFile, "\t Device Type : CPU \n");	// Vulkan is use for Compute via CPU
			break;

		default:
			break;
		}

		fprintf(DRSJ_gpLogFile, "\t Vender Id : 0x%04x\n", vkPhysicalDeviceProperties.vendorID);
		fprintf(DRSJ_gpLogFile, "\t Device Id : 0x%04x\n", vkPhysicalDeviceProperties.deviceID);
	}

	fprintf(DRSJ_gpLogFile, "\n\t * * * * * End Vulkan Information * * * * *\n");
	fprintf(DRSJ_gpLogFile, "\t - - - - - - - - - - - - - - - - - - - - - -\n\n");

	if (DRSJ_vkPhysicalDevice_array)
	{
		free(DRSJ_vkPhysicalDevice_array);
		DRSJ_vkPhysicalDevice_array = NULL;
		fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkPhysicalDevice_array memory is free \n", __func__, __LINE__);
	}

	return (vkResult);
}

/******************************************************
* Name : fillDeviceExtensionNames
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will get device extensions name
*
******************************************************/
VkResult fillDeviceExtensionNames(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;
	uint32_t deviceExtensionCount = 0;

	//Step 1: Find how many device extensions are supported by the Vulkan driver of this version.
	vkResult = vkEnumerateDeviceExtensionProperties(DRSJ_vkPhysicalDevice_selected,NULL, &deviceExtensionCount, NULL);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateDeviceExtensionProperties() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateDeviceExtensionProperties() SUCCESSED\n", __func__, __LINE__);
	}


	//Step 2:Allocate and fill the struct VkExtensionProperties array coresponding to the above count.

	VkExtensionProperties* vkExntesionPropertiesArr = NULL;

	vkExntesionPropertiesArr = (VkExtensionProperties*)calloc(deviceExtensionCount,sizeof(VkExtensionProperties));
	if (!vkExntesionPropertiesArr)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : calloc() FAILED\n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	vkResult = vkEnumerateDeviceExtensionProperties(DRSJ_vkPhysicalDevice_selected,NULL, &deviceExtensionCount, vkExntesionPropertiesArr);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateDeviceExtensionProperties() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkEnumerateDeviceExtensionProperties() SUCCESSED\n", __func__, __LINE__);
	}

	//Step 3:Fill and display a local string array of extension names obtained from VkExtensionProperties Array.
	char** deviceExtensionNamesArr = NULL;

	deviceExtensionNamesArr = (char**)calloc(deviceExtensionCount, sizeof(char*));
	if (!deviceExtensionNamesArr)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : calloc() FAILED\n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	for (uint32_t iCounter = 0; iCounter < deviceExtensionCount; iCounter++)
	{
		uint32_t strLength = strlen(vkExntesionPropertiesArr[iCounter].extensionName) + 1;

		deviceExtensionNamesArr[iCounter] = (char*)calloc(strLength, sizeof(char));

		memcpy_s(deviceExtensionNamesArr[iCounter], strLength, vkExntesionPropertiesArr[iCounter].extensionName, strLength);

		fprintf(DRSJ_gpLogFile, "%s %d : Vulkan Device Extension Names [%d] = %s\n", __func__, __LINE__, iCounter, deviceExtensionNamesArr[iCounter]);
	}

	//Step 4:As not required here onwards, free VkExtensionProperties array.
	free(vkExntesionPropertiesArr);
	vkExntesionPropertiesArr = NULL;

	/*
	Step 5:
		Find whether the above extension names contain our required 1 extension.
		(1) VK_KHR_SWAPCHAIN_EXTENSION_NAME (VK_KHR_swapchain : actual name)

		Accordingly set 2 global variables,
		(a) required device extension count.
		(b) required device extension names array.

	*/

	VkBool32 vulkanSwapchainExtensionFound = VK_FALSE;

	for (uint32_t iCounter = 0; iCounter < deviceExtensionCount; iCounter++)
	{
		if (!strcmp(deviceExtensionNamesArr[iCounter], VK_KHR_SWAPCHAIN_EXTENSION_NAME))
		{
			vulkanSwapchainExtensionFound = VK_TRUE;
			DRSJ_enabledDeviceExtensionNames_array[DRSJ_enabledDeviceExtensionCount++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		}
	}

	//Step 6:As not needed, hence for free the local string array.
	for (uint32_t iCounter = 0; iCounter < deviceExtensionCount; iCounter++)
	{
		free(deviceExtensionNamesArr[iCounter]);
	}

	free(deviceExtensionNamesArr);
	deviceExtensionNamesArr = NULL;

	//Step 7:Print whether our Vulkan driver supports our required device extensions or not.
	if (VK_FALSE == vulkanSwapchainExtensionFound)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
		fprintf(DRSJ_gpLogFile, "%s %d : VK_KHR_SWAPCHAIN_EXTENSION_NAME NOT-FOUND\n", __func__, __LINE__);
		return (vkResult);
	}
	else
		fprintf(DRSJ_gpLogFile, "%s %d : VK_KHR_SWAPCHAIN_EXTENSION_NAME FOUND\n", __func__, __LINE__);

	//Steps 8: Print only supported extension names.
	for (uint32_t iCounter = 0; iCounter < DRSJ_enabledDeviceExtensionCount; iCounter++)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : Enabled Vulkan Device Extension Name = %s\n", __func__, __LINE__, DRSJ_enabledDeviceExtensionNames_array[iCounter]);
	}

	return (vkResult);
}

/******************************************************
* Name : createVulkanDevice
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will get device extensions name
*
******************************************************/
VkResult createVulkanDevice(void)
{
	// Local Function Declaration
	VkResult fillDeviceExtensionNames(void);

	// Local Variables
	VkResult vkResult = VK_SUCCESS;


	vkResult = fillDeviceExtensionNames();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : fillDeviceExtensionNames() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : fillDeviceExtensionNames() SUCCESSED\n", __func__, __LINE__);
	}

	// Newly added code:
	float queuePriorities[1] = { 0.0f };
	VkDeviceQueueCreateInfo vkDeviceQueueCreateInfo;
	memset((void*)&vkDeviceQueueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));

	vkDeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vkDeviceQueueCreateInfo.pNext = NULL;
	vkDeviceQueueCreateInfo.flags = 0;
	vkDeviceQueueCreateInfo.queueFamilyIndex = DRSJ_graphicsQueueFamilyIndex_selected;
	vkDeviceQueueCreateInfo.queueCount = 1;
	vkDeviceQueueCreateInfo.pQueuePriorities = queuePriorities;

	VkDeviceCreateInfo vkDeviceCreateInfo;
	memset((void*) & vkDeviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));

	vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vkDeviceCreateInfo.pNext = NULL;
	vkDeviceCreateInfo.flags = 0;
	vkDeviceCreateInfo.enabledExtensionCount = DRSJ_enabledDeviceExtensionCount;
	vkDeviceCreateInfo.ppEnabledExtensionNames = DRSJ_enabledDeviceExtensionNames_array;
	vkDeviceCreateInfo.enabledLayerCount = 0;
	vkDeviceCreateInfo.ppEnabledLayerNames = NULL;
	vkDeviceCreateInfo.pEnabledFeatures = NULL;

	vkDeviceCreateInfo.queueCreateInfoCount = 1;
	vkDeviceCreateInfo.pQueueCreateInfos = &vkDeviceQueueCreateInfo;

	vkResult = vkCreateDevice(DRSJ_vkPhysicalDevice_selected, &vkDeviceCreateInfo, NULL, &DRSJ_vkDevice);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkCreateDevice() FAILED [%d]\n", __func__, __LINE__, vkResult);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkCreateDevice() SUCCESSED\n", __func__, __LINE__);
	}

	return (vkResult);

}

void getDeviceQueue(void)
{
	// code
	vkGetDeviceQueue(DRSJ_vkDevice, DRSJ_graphicsQueueFamilyIndex_selected, 0, &DRSJ_vkQueue);

	if (DRSJ_vkQueue == VK_NULL_HANDLE)
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkGetDeviceQueue() FAILED \n", __func__, __LINE__);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s, %d, : vkGetDeviceQueue() SUCCESSED \n", __func__, __LINE__);
	}
}

/******************************************************
* Name : getPhysicalDeviceSurfaceFormatAndColorSpace
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will get Physical Device Surface Format And Color Space
*
******************************************************/
VkResult getPhysicalDeviceSurfaceFormatAndColorSpace(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// Code
	//Get the count of supported Color Formats

	uint32_t formatCount = 0;
	vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(DRSJ_vkPhysicalDevice_selected, DRSJ_vkSurfaceKHR, &formatCount,NULL);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfaceFormatsKHR() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else if (0 == formatCount)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfaceFormatsKHR() SUCCESSED But no Surface Format Found\n", __func__, __LINE__);
		return (VK_ERROR_INITIALIZATION_FAILED);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfaceFormatsKHR() SUCCESSED\n", __func__, __LINE__);
	}

	//
	VkSurfaceFormatKHR* vkSurfaceFormatKHR_array = NULL;

	vkSurfaceFormatKHR_array = (VkSurfaceFormatKHR*)calloc(formatCount, sizeof(VkSurfaceFormatKHR));
	if (!vkSurfaceFormatKHR_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : memory allocation FAILED\n", __func__, __LINE__);
		return (VK_ERROR_OUT_OF_HOST_MEMORY);
	}

	// Filling the array
	vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(DRSJ_vkPhysicalDevice_selected, DRSJ_vkSurfaceKHR, &formatCount, vkSurfaceFormatKHR_array);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfaceFormatsKHR() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfaceFormatsKHR() SUCCESSED\n", __func__, __LINE__);
	}

	// Decide the surface color format first
	if (1 == formatCount && VK_FORMAT_UNDEFINED == vkSurfaceFormatKHR_array[0].format)
	{
		DRSJ_vkFormat_color = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else
	{
		DRSJ_vkFormat_color = vkSurfaceFormatKHR_array[0].format;
	}

	// Decide the surface color space
	
	DRSJ_vkColorSpaceKHR = vkSurfaceFormatKHR_array[0].colorSpace;


	fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkColorSpaceKHR = [%d].\n", __func__, __LINE__, DRSJ_vkColorSpaceKHR);
	fprintf(DRSJ_gpLogFile, "%s %d : DRSJ_vkFormat_color = [%d].\n", __func__, __LINE__, DRSJ_vkFormat_color);

	if (vkSurfaceFormatKHR_array)
	{
		free(vkSurfaceFormatKHR_array);
		vkSurfaceFormatKHR_array = NULL;
		fprintf(DRSJ_gpLogFile, "%s %d : vkSurfaceFormatKHR_array free SUCCESSFULLY\n", __func__, __LINE__);

	}

	return vkResult;
}

/******************************************************
* Name : getPhysicalDevicePresentMode
*
* Parameter : void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will get Physical Device Present Mode
*
******************************************************/
VkResult getPhysicalDevicePresentMode(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;
	uint32_t presentCount = 0;

	vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(DRSJ_vkPhysicalDevice_selected, DRSJ_vkSurfaceKHR, &presentCount, NULL);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfacePresentModesKHR() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else if (0 == presentCount)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfacePresentModesKHR() SUCCESSED But no Surface Format Found\n", __func__, __LINE__);
		return (VK_ERROR_INITIALIZATION_FAILED);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfacePresentModesKHR() SUCCESSED\n", __func__, __LINE__);
	}


	VkPresentModeKHR* vkPresentModeKHR_array = NULL;

	vkPresentModeKHR_array = (VkPresentModeKHR*)calloc(presentCount, sizeof(VkPresentModeKHR));
	if (!vkPresentModeKHR_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : memory allocation FAILED\n", __func__, __LINE__);
		return (VK_ERROR_OUT_OF_HOST_MEMORY);
	}

	// Filling the array
	vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(DRSJ_vkPhysicalDevice_selected, DRSJ_vkSurfaceKHR, &presentCount, vkPresentModeKHR_array);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfacePresentModesKHR() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfacePresentModesKHR() SUCCESSED\n", __func__, __LINE__);
	}


	for (uint32_t iCounter = 0; iCounter < presentCount; iCounter++)
	{
		if (vkPresentModeKHR_array[iCounter] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			DRSJ_vkPresentModeKHR = VK_PRESENT_MODE_MAILBOX_KHR;
			fprintf(DRSJ_gpLogFile, "%s %d : VK_PRESENT_MODE_MAILBOX_KHR FOUND at [%d]\n", __func__, __LINE__,iCounter);

			break;
		}
	}

	if (DRSJ_vkPresentModeKHR != VK_PRESENT_MODE_MAILBOX_KHR)
	{
		DRSJ_vkPresentModeKHR = VK_PRESENT_MODE_FIFO_KHR;
		fprintf(DRSJ_gpLogFile, "%s %d : Assign minimum VK_PRESENT_MODE_FIFO_KHR .\n", __func__, __LINE__);
	}

	if (vkPresentModeKHR_array)
	{
		free(vkPresentModeKHR_array);
		vkPresentModeKHR_array = NULL;
		fprintf(DRSJ_gpLogFile, "%s %d : vkPresentModeKHR_array free SUCCESSFULLY\n", __func__, __LINE__);

	}
	return vkResult;
}

/******************************************************
* Name : createSwapchain
*
* Parameter : VkBool32 vsync
*				verticle sync : verticle sync sobat map karaychay ki nahi
*				TRUE : 
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates swapchain
*
******************************************************/
VkResult createSwapchain(VkBool32 vsync)
{
	// local function declaration
	VkResult getPhysicalDeviceSurfaceFormatAndColorSpace(void);
	VkResult getPhysicalDevicePresentMode(void);

	// Local Variables
	VkResult vkResult = VK_SUCCESS;


	// code
	
	// ColorFormat and ColorSpace [STEP_1]
	vkResult = getPhysicalDeviceSurfaceFormatAndColorSpace();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : getPhysicalDeviceSurfaceFormatAndColorSpace() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : getPhysicalDeviceSurfaceFormatAndColorSpace() SUCCESSED\n", __func__, __LINE__);
	}

	// Get physical device surface capabilities [STEP_2]
	VkSurfaceCapabilitiesKHR vkSurfaceCapabilitiesKHR;
	memset((void*) &vkSurfaceCapabilitiesKHR, 0, sizeof(VkSurfaceCapabilitiesKHR));

	vkResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(DRSJ_vkPhysicalDevice_selected, DRSJ_vkSurfaceKHR, &vkSurfaceCapabilitiesKHR);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfaceCapabilitiesKHR() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetPhysicalDeviceSurfaceCapabilitiesKHR() SUCCESSED\n", __func__, __LINE__);
	}

	fprintf(DRSJ_gpLogFile, "%s %d : minImageCount = %d, maxImageCount = %d\n", __func__, __LINE__, vkSurfaceCapabilitiesKHR.minImageCount, vkSurfaceCapabilitiesKHR.maxImageCount);


	// Find out desired swapchain image count
	uint32_t testingNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.minImageCount + 1;
	uint32_t desiredNumberOfSwapchainImages = 0;

	if (vkSurfaceCapabilitiesKHR.maxImageCount > 0 && vkSurfaceCapabilitiesKHR.maxImageCount < testingNumberOfSwapchainImages)
	{
		desiredNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.maxImageCount;
	}
	else
	{
		desiredNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.minImageCount;
	}

	// Choose size of swapchain image
	memset((void*)&DRSJ_vkExtent2D_swapchain, 0, sizeof(VkExtent2D));
	if (vkSurfaceCapabilitiesKHR.currentExtent.width != UINT32_MAX)
	{
		DRSJ_vkExtent2D_swapchain.width = vkSurfaceCapabilitiesKHR.currentExtent.width;
		DRSJ_vkExtent2D_swapchain.height = vkSurfaceCapabilitiesKHR.currentExtent.height;
		fprintf(DRSJ_gpLogFile, "%s %d : swapchain image width X height = %d X %d\n", __func__, __LINE__, DRSJ_vkExtent2D_swapchain.width, DRSJ_vkExtent2D_swapchain.height);
	}
	else
	{
		// if surface size is already defined then swapchain image size must match with it
		VkExtent2D vkExtent2D;
		memset((void*)&vkExtent2D, 0, sizeof(VkExtent2D));
		vkExtent2D.width = (uint32_t)winWidth;
		vkExtent2D.height = (uint32_t)winHeight;

		DRSJ_vkExtent2D_swapchain.width = glm::max(vkSurfaceCapabilitiesKHR.minImageExtent.width, glm::min(vkSurfaceCapabilitiesKHR.maxImageExtent.width, vkExtent2D.width));
		DRSJ_vkExtent2D_swapchain.height = glm::max(vkSurfaceCapabilitiesKHR.minImageExtent.height, glm::min(vkSurfaceCapabilitiesKHR.maxImageExtent.height, vkExtent2D.height));
		fprintf(DRSJ_gpLogFile, "%s %d : swapchain image width X height = %d X %d\n", __func__, __LINE__, DRSJ_vkExtent2D_swapchain.width, DRSJ_vkExtent2D_swapchain.height);
	}
	
	// Set swapchain image usage flag
	VkImageUsageFlags vkImageUsageFlag = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	//Whether to consider pre-transform or not
	
	VkSurfaceTransformFlagBitsKHR vkSurfaceTransformFlagBitsKHR;
	if (vkSurfaceCapabilitiesKHR.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		vkSurfaceTransformFlagBitsKHR = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		vkSurfaceTransformFlagBitsKHR = vkSurfaceCapabilitiesKHR.currentTransform;
	}

	// Presentation mode [STEP_7]
	vkResult = getPhysicalDevicePresentMode();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : getPhysicalDevicePresentMode() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : getPhysicalDevicePresentMode() SUCCESSED\n", __func__, __LINE__);
	}


	// initialize VkCreateSwapchainInfo structure
	VkSwapchainCreateInfoKHR vkSwapchainCreateInfoKHR;
	memset((void*)&vkSwapchainCreateInfoKHR, 0, sizeof(VkSwapchainCreateInfoKHR));
	vkSwapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	vkSwapchainCreateInfoKHR.flags = 0;
	vkSwapchainCreateInfoKHR.pNext = NULL;
	vkSwapchainCreateInfoKHR.surface = DRSJ_vkSurfaceKHR;
	vkSwapchainCreateInfoKHR.minImageCount = desiredNumberOfSwapchainImages;
	vkSwapchainCreateInfoKHR.imageFormat = DRSJ_vkFormat_color;
	vkSwapchainCreateInfoKHR.imageColorSpace = DRSJ_vkColorSpaceKHR;
	vkSwapchainCreateInfoKHR.imageExtent.width = DRSJ_vkExtent2D_swapchain.width;
	vkSwapchainCreateInfoKHR.imageExtent.height = DRSJ_vkExtent2D_swapchain.height;
	vkSwapchainCreateInfoKHR.imageUsage = vkImageUsageFlag;
	vkSwapchainCreateInfoKHR.preTransform = vkSurfaceTransformFlagBitsKHR;
	vkSwapchainCreateInfoKHR.imageArrayLayers = 1;
	vkSwapchainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkSwapchainCreateInfoKHR.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	vkSwapchainCreateInfoKHR.presentMode = DRSJ_vkPresentModeKHR;
	vkSwapchainCreateInfoKHR.clipped = VK_TRUE;

	vkResult = vkCreateSwapchainKHR(DRSJ_vkDevice, &vkSwapchainCreateInfoKHR, NULL, &DRSJ_vkSwapchainKHR);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateSwapchainKHR() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateSwapchainKHR() SUCCESSED\n", __func__, __LINE__);
	}



	return (vkResult);
}

/******************************************************
* Name : createImagesAndImageViews
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates vulkan images and image views
*
******************************************************/
VkResult createImagesAndImageViews(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	// Get swapchgain image count
	vkResult = vkGetSwapchainImagesKHR(DRSJ_vkDevice, DRSJ_vkSwapchainKHR, &DRSJ_swapchainImageCount, NULL);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetSwapchainImagesKHR() FAILED \n", __func__, __LINE__);
		return (vkResult);
	}
	else if (0 == DRSJ_swapchainImageCount)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetSwapchainImagesKHR() SUCCESSED But No Swapchain Images Found \n", __func__, __LINE__);
		return (VK_ERROR_INITIALIZATION_FAILED);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetSwapchainImagesKHR() SUCCESSED with swapchain image count : [%d] \n", __func__, __LINE__, DRSJ_swapchainImageCount);
	}

	DRSJ_vkSwapchainImage_array = (VkImage*)calloc(DRSJ_swapchainImageCount, sizeof(VkImage));
	if (!DRSJ_vkSwapchainImage_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : calloc() FAILED For DRSJ_vkSwapchainImage_array \n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	// Fill this array by swapchain images
	vkResult = vkGetSwapchainImagesKHR(DRSJ_vkDevice, DRSJ_vkSwapchainKHR, &DRSJ_swapchainImageCount, DRSJ_vkSwapchainImage_array);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetSwapchainImagesKHR() FAILED \n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkGetSwapchainImagesKHR() SUCCESSED\n", __func__, __LINE__);
	}

	// allocate swapchain image view
	DRSJ_vkSwapchainImageView_array = (VkImageView*)calloc(DRSJ_swapchainImageCount, sizeof(VkImageView));
	if (!DRSJ_vkSwapchainImageView_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : calloc() FAILED For DRSJ_vkSwapchainImageView_array \n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	VkImageViewCreateInfo vkImageViewCreateInfo;
	memset(&vkImageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

	vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vkImageViewCreateInfo.pNext = NULL;
	vkImageViewCreateInfo.flags = 0;
	vkImageViewCreateInfo.format = DRSJ_vkFormat_color;
	vkImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	vkImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;	
	vkImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	vkImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;	
	vkImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	vkImageViewCreateInfo.subresourceRange.levelCount = 1;
	vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	vkImageViewCreateInfo.subresourceRange.layerCount = 1;
	vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

	for (uint32_t iCounter = 0; iCounter < DRSJ_swapchainImageCount; iCounter++)
	{
		vkImageViewCreateInfo.image = DRSJ_vkSwapchainImage_array[iCounter];
		vkResult = vkCreateImageView(DRSJ_vkDevice, &vkImageViewCreateInfo, NULL, &DRSJ_vkSwapchainImageView_array[iCounter]);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateImageView() FAILED for iteration : %d. With Error[%d]\n", __func__, __LINE__, iCounter,GetLastError());
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateImageView() SUCCESSED for iteration : %d\n", __func__, __LINE__, iCounter);
		}
	}
	
	return vkResult;
}

/******************************************************
* Name : createCommandPool
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Command Pool
*
******************************************************/
VkResult createCommandPool(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	VkCommandPoolCreateInfo vkCommandPoolCreateInfo;
	memset((void*)&vkCommandPoolCreateInfo, 0, sizeof(VkCommandPoolCreateInfo));

	vkCommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vkCommandPoolCreateInfo.pNext = NULL;
	vkCommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	vkCommandPoolCreateInfo.queueFamilyIndex = DRSJ_graphicsQueueFamilyIndex_selected;

	vkResult = vkCreateCommandPool(DRSJ_vkDevice, &vkCommandPoolCreateInfo, NULL, &DRSJ_vkCommandPool);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateCommandPool() FAILED \n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateCommandPool() SUCCESSED \n", __func__, __LINE__, DRSJ_swapchainImageCount);
	}

	return vkResult;
}

/******************************************************
* Name : createCommandBuffer
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Command Buffers
*
******************************************************/
VkResult createCommandBuffer(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo;
	memset((void*)&vkCommandBufferAllocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));
	
	vkCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	vkCommandBufferAllocateInfo.pNext = NULL;
	vkCommandBufferAllocateInfo.commandPool = DRSJ_vkCommandPool;
	vkCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	vkCommandBufferAllocateInfo.commandBufferCount = 1;


	DRSJ_vkCommandBuffer_array = (VkCommandBuffer*)calloc(DRSJ_swapchainImageCount, sizeof(VkCommandBuffer));
	if (!DRSJ_vkCommandBuffer_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : calloc() FAILED For DRSJ_vkCommandBuffer_array \n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	for (uint32_t iCounter = 0; iCounter < DRSJ_swapchainImageCount; iCounter++)
	{
		vkResult = vkAllocateCommandBuffers(DRSJ_vkDevice, &vkCommandBufferAllocateInfo, &(DRSJ_vkCommandBuffer_array[iCounter]));
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateCommandBuffers() FAILED at index : [%d]\n", __func__, __LINE__,iCounter);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateCommandBuffers() SUCCESSED at index : [%d]\n", __func__, __LINE__, iCounter);
		}
	}


	return vkResult;
}

/******************************************************
* Name : createVertexBuffer
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Vertex Buffers
*
******************************************************/
VkResult createVertexBuffer(void)
{
	// code:

	VkResult vkResult_DRSJ= VK_SUCCESS;

	vkResult_DRSJ = createVertexBuffer_by_darshan();
	if (VK_SUCCESS != vkResult_DRSJ)
	{
		fprintf(DRSJ_gpLogFile, " Failed inside %s() After  createVertexBuffer_by_darshan()   - %d \n", __func__, __LINE__);
		return (vkResult_DRSJ);
	}

	vkResult_DRSJ = createVertexBuffer_2();
	if (VK_SUCCESS != vkResult_DRSJ)
	{
		fprintf(DRSJ_gpLogFile, " Failed inside %s() After  createVertexBuffer_2()   - %d \n", __func__, __LINE__);
		return (vkResult_DRSJ);
	}

	return (vkResult_DRSJ);
}



/******************************************************
* Name : createUniformBuffer
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Uniform Buffers
*
******************************************************/
VkResult createUniformBuffer(void)
{
	// Functions declaraions
	VkResult updateUniformBuffer(void);

	// variable declarations
	VkResult vkResult = VK_SUCCESS;

	// code
	vkResult = createUniformBuffer_by_darshan();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createUniformBuffer_by_darshan() FAILED \n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : createUniformBuffer_by_darshan() SUCCESSED\n", __func__, __LINE__);
	}

	
	//vkResult = createUniformBuffer_2();
	//if (VK_SUCCESS != vkResult)
	//{
	//	fprintf(DRSJ_gpLogFile, "%s %d : createUniformBuffer_2() FAILED \n", __func__, __LINE__);
	//	return (vkResult);
	//}
	//else
	//{
	//	fprintf(DRSJ_gpLogFile, "%s %d : createUniformBuffer_2() SUCCESSED\n", __func__, __LINE__);
	//}


	// Call Update Uinform buffer
	vkResult = updateUniformBuffer();
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : updateUniformBuffer() FAILED \n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : updateUniformBuffer() SUCCESSED\n", __func__, __LINE__);
	}



	return vkResult;
}


/******************************************************
* Name : updateUniformBuffer
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will update Uniform Buffers
*
******************************************************/
VkResult updateUniformBuffer(void)
{
	VkResult vkResult_DRSJ= VK_SUCCESS;

	// code:

	vkResult_DRSJ = updateUniformBuffer_by_darshan();
	if (VK_SUCCESS != vkResult_DRSJ)
	{
		fprintf(DRSJ_gpLogFile, " Failed inside %s() After  updateUniformBuffer_by_darshan()   - %d \n", __func__, __LINE__);
		return (vkResult_DRSJ);
	}

	vkResult_DRSJ = updateUniformBuffer_2();
	if (VK_SUCCESS != vkResult_DRSJ)
	{
		fprintf(DRSJ_gpLogFile, " Failed inside %s() After  updateUniformBuffer_2()   - %d \n", __func__, __LINE__);
		return (vkResult_DRSJ);
	}

	return (vkResult_DRSJ);
}

/******************************************************
* Name : createShaders
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Shaders
*
******************************************************/
VkResult createShaders(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code
	FILE* fpShaderFile = NULL;
	size_t size = 0;
	VkShaderModuleCreateInfo vkShaderModuleCreateInfo;

	// For Vertex Shader
	{
		fprintf(DRSJ_gpLogFile, "\n%s %d : --- Vertex Shader Module Creation Start ---\n", __func__, __LINE__);

		const char* szFileName = "shader.vert.spv";
		fopen_s(&fpShaderFile, szFileName, "rb");
		if (!fpShaderFile)
		{
			vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
			fprintf(DRSJ_gpLogFile, "%s %d : fopen_s() for file: [%s] FAILED \n", __func__, __LINE__, szFileName);
			return vkResult;
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : fopen_s() for file: [%s] SUCCESSED\n", __func__, __LINE__, szFileName);
		}

		fseek(fpShaderFile, 0L, SEEK_END);

		size = ftell(fpShaderFile);
		if (0 == size)
		{
			vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
			fprintf(DRSJ_gpLogFile, "%s %d : ftell() for file: [%s] FAILED \n", __func__, __LINE__, szFileName);
			return vkResult;
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : size-[%d] of file-[%s] SUCCESSED\n", __func__, __LINE__, (int)size, szFileName);
		}

		fseek(fpShaderFile, 0L, SEEK_SET);

		char* shaderData = (char*)calloc(size, sizeof(char));
		size_t retVal = fread(shaderData, size, 1, fpShaderFile);
		if (retVal != 1)
		{
			vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
			fprintf(DRSJ_gpLogFile, "%s %d : fread() for file: [%s] FAILED \n", __func__, __LINE__, szFileName);
			return vkResult;
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : fread() for file: [%s] SUCCESSED\n", __func__, __LINE__,szFileName);
		}

		fclose(fpShaderFile);

		memset((void*)&vkShaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));
		vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vkShaderModuleCreateInfo.pNext = NULL;
		vkShaderModuleCreateInfo.flags = 0; // reserved hence must be 0
		vkShaderModuleCreateInfo.codeSize = size;
		vkShaderModuleCreateInfo.pCode = (uint32_t*)shaderData;

		vkResult = vkCreateShaderModule(DRSJ_vkDevice, &vkShaderModuleCreateInfo, NULL, &vkShaderModule_vertex_shader);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateShaderModule() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateShaderModule() SUCCESSED\n", __func__, __LINE__);
		}

		if (shaderData)
		{
			free(shaderData);
			shaderData = NULL;
		}

		fprintf(DRSJ_gpLogFile, "%s %d : --- Vertex Shader Module Succesfully Created --- \n\n", __func__, __LINE__);
	}

	// For Fragment Shader
	{
		fprintf(DRSJ_gpLogFile, "\n%s %d : --- Fragment Shader Module Creation Start ---\n", __func__, __LINE__);
		const char* szFileName = "shader.frag.spv";
		fopen_s(&fpShaderFile, szFileName, "rb");
		if (!fpShaderFile)
		{
			vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
			fprintf(DRSJ_gpLogFile, "%s %d : fopen_s() for file: [%s] FAILED \n", __func__, __LINE__, szFileName);
			return vkResult;
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : fopen_s() for file: [%s] SUCCESSED\n", __func__, __LINE__, szFileName);
		}

		fseek(fpShaderFile, 0L, SEEK_END);

		size = ftell(fpShaderFile);
		if (0 == size)
		{
			vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
			fprintf(DRSJ_gpLogFile, "%s %d : ftell() for file: [%s] FAILED \n", __func__, __LINE__, szFileName);
			return vkResult;
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : size-[%d] of file-[%s] SUCCESSED\n", __func__, __LINE__, (int)size, szFileName);
		}

		fseek(fpShaderFile, 0L, SEEK_SET);

		char* shaderData = (char*)calloc(size, sizeof(char));
		size_t retVal = fread(shaderData, size, 1, fpShaderFile);
		if (retVal != 1)
		{
			vkResult = VK_ERROR_INITIALIZATION_FAILED;	// Return Hard-Coded Failure
			fprintf(DRSJ_gpLogFile, "%s %d : fread() for file: [%s] FAILED \n", __func__, __LINE__, szFileName);
			return vkResult;
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : fread() for file: [%s] SUCCESSED\n", __func__, __LINE__, szFileName);
		}

		fclose(fpShaderFile);

		
		memset((void*)&vkShaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));
		vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vkShaderModuleCreateInfo.pNext = NULL;
		vkShaderModuleCreateInfo.flags = 0; // reserved hence must be 0
		vkShaderModuleCreateInfo.codeSize = size;
		vkShaderModuleCreateInfo.pCode = (uint32_t*)shaderData;

		vkResult = vkCreateShaderModule(DRSJ_vkDevice, &vkShaderModuleCreateInfo, NULL, &vkShaderModule_fragment_shader);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateShaderModule() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateShaderModule() SUCCESSED\n", __func__, __LINE__);
		}

		if (shaderData)
		{
			free(shaderData);
			shaderData = NULL;
		}

		fprintf(DRSJ_gpLogFile, "%s %d : --- Fragment Shader Module Succesfully Created --- \n\n", __func__, __LINE__);

	}
	return vkResult;
}

/******************************************************
* Name : createDescriptorSetLayout
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Descriptor Set Layout
*
******************************************************/
VkResult createDescriptorSetLayout(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	// Initialize desciptor set bindings
	VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding;
	memset((void*)&vkDescriptorSetLayoutBinding, 0, sizeof(VkDescriptorSetLayoutBinding));
	vkDescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vkDescriptorSetLayoutBinding.binding = 0; // this 0 related with binding 0 in vertex shader
	vkDescriptorSetLayoutBinding.descriptorCount = 1;
	vkDescriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	vkDescriptorSetLayoutBinding.pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo;
	memset((void*)&vkDescriptorSetLayoutCreateInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
	vkDescriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	vkDescriptorSetLayoutCreateInfo.flags = 0;
	vkDescriptorSetLayoutCreateInfo.pNext = NULL;
	
	//vkDescriptorSetLayoutCreateInfo.bindingCount = 0;	// we dont have descriptor
	//vkDescriptorSetLayoutCreateInfo.pBindings = NULL;	// array of VkDescriptorSetLayout of binding	explained 30th April Lec at 1.38

	// In the shaders up till now there were no uniforms hence bindingCount and pBindings members of VkDescriptorSetLayoutCreateInfo were not used.
	// But in this application our shader at least one uniform means at least one descriptor set hence above two members used with new type VkDescriptorSetLayoutBindings.
	vkDescriptorSetLayoutCreateInfo.bindingCount = 1;	
	vkDescriptorSetLayoutCreateInfo.pBindings = &vkDescriptorSetLayoutBinding;

	memset((void*)&vkDescriptorSetLayout, 0, sizeof(VkDescriptorSetLayout));

	vkResult = vkCreateDescriptorSetLayout(DRSJ_vkDevice, &vkDescriptorSetLayoutCreateInfo, NULL, &vkDescriptorSetLayout);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateDescriptorSetLayout() FAILED \n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateDescriptorSetLayout() SUCCESSED\n", __func__, __LINE__);
	}

	return vkResult;
}

/******************************************************
* Name : createPipelineLayout
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Descriptor Set Layout
*
******************************************************/
VkResult createPipelineLayout(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	VkPushConstantRange vkPushConstantRange;
	memset((void*)&vkPushConstantRange, 0, sizeof(VkPushConstantRange));
	vkPushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	vkPushConstantRange.offset = 0;
	vkPushConstantRange.size = sizeof(glm::mat4);

	VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo;
	
	memset((void*)&vkPipelineLayoutCreateInfo, 0, sizeof(VkPipelineLayoutCreateInfo));

	vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	vkPipelineLayoutCreateInfo.flags = 0;
	vkPipelineLayoutCreateInfo.pNext = NULL;
	vkPipelineLayoutCreateInfo.setLayoutCount = 1;
	vkPipelineLayoutCreateInfo.pSetLayouts = &vkDescriptorSetLayout;
	vkPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	vkPipelineLayoutCreateInfo.pPushConstantRanges = &vkPushConstantRange;

	vkResult = vkCreatePipelineLayout(DRSJ_vkDevice, &vkPipelineLayoutCreateInfo, NULL, &vkPipelineLayout);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreatePipelineLayout() FAILED \n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreatePipelineLayout() SUCCESSED\n", __func__, __LINE__);
	}

	return vkResult;
}


/******************************************************
* Name : createRenderPass
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Renderpass
*
******************************************************/
VkResult createRenderPass(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	// declate and initi
	VkAttachmentDescription vkAttachmentDescription_array[1];
	memset((void*)vkAttachmentDescription_array, 0, sizeof(VkAttachmentDescription) * _ARRAYSIZE(vkAttachmentDescription_array));

	vkAttachmentDescription_array[0].flags = 0;
	vkAttachmentDescription_array[0].format = DRSJ_vkFormat_color;
	vkAttachmentDescription_array[0].samples = VK_SAMPLE_COUNT_1_BIT;
	vkAttachmentDescription_array[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	vkAttachmentDescription_array[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	vkAttachmentDescription_array[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	vkAttachmentDescription_array[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vkAttachmentDescription_array[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vkAttachmentDescription_array[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference vkAttachmentReference;
	memset((void*)&vkAttachmentReference, 0, sizeof(VkAttachmentReference));
	vkAttachmentReference.attachment = 0;	// index of VkAttachmentDescription array
	vkAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription vkSubpassDescription;
	memset((void*)&vkSubpassDescription, 0, sizeof(VkSubpassDescription));

	vkSubpassDescription.flags = 0;
	vkSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	vkSubpassDescription.inputAttachmentCount = 0;
	vkSubpassDescription.pInputAttachments = NULL;
	vkSubpassDescription.colorAttachmentCount = _ARRAYSIZE(vkAttachmentDescription_array);
	vkSubpassDescription.pColorAttachments = &vkAttachmentReference;
	vkSubpassDescription.pResolveAttachments = NULL;
	vkSubpassDescription.pDepthStencilAttachment = NULL;
	vkSubpassDescription.preserveAttachmentCount = 0;
	vkSubpassDescription.pPreserveAttachments = NULL;

	// Step4 declare createInfostruct
	VkRenderPassCreateInfo vkRenderPassCreateInfo;
	memset((void*)&vkRenderPassCreateInfo, 0, sizeof(VkRenderPassCreateInfo));
	vkRenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	vkRenderPassCreateInfo.pNext = NULL;
	vkRenderPassCreateInfo.flags = 0;
	vkRenderPassCreateInfo.attachmentCount = _ARRAYSIZE(vkAttachmentDescription_array);
	vkRenderPassCreateInfo.pAttachments = vkAttachmentDescription_array;
	vkRenderPassCreateInfo.subpassCount = 1;
	vkRenderPassCreateInfo.pSubpasses = &vkSubpassDescription;
	vkRenderPassCreateInfo.dependencyCount = 0;
	vkRenderPassCreateInfo.pDependencies = NULL;

	// Create render pass
	
	vkResult = vkCreateRenderPass(DRSJ_vkDevice, &vkRenderPassCreateInfo, NULL, &vkRenderPass);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateRenderPass() FAILED \n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateRenderPass() SUCCESSED\n", __func__, __LINE__);
	}


	return vkResult;
}

/******************************************************
* Name : createPipeline
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Pipeline
*
******************************************************/
VkResult createPipeline(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code
	//[PSO : Pipeline State Object]
	// Vertex input state [PSO]
	VkVertexInputBindingDescription vkVertexInputBindingDescription_array[2];
	memset((void*)vkVertexInputBindingDescription_array, 0, sizeof(VkVertexInputBindingDescription) * _ARRAYSIZE(vkVertexInputBindingDescription_array));
	
	vkVertexInputBindingDescription_array[0].binding = 0;
	vkVertexInputBindingDescription_array[0].stride = sizeof(float) * 3;
	vkVertexInputBindingDescription_array[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vkVertexInputBindingDescription_array[1].binding = 1;
	vkVertexInputBindingDescription_array[1].stride = sizeof(float) * 3;
	vkVertexInputBindingDescription_array[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription vkVertexInputAttributeDescription_array[2];
	memset((void*)vkVertexInputAttributeDescription_array, 0, sizeof(VkVertexInputAttributeDescription) * _ARRAYSIZE(vkVertexInputAttributeDescription_array));

	// For position
	vkVertexInputAttributeDescription_array[0].binding = 0;
	vkVertexInputAttributeDescription_array[0].location = 0;
	vkVertexInputAttributeDescription_array[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vkVertexInputAttributeDescription_array[0].offset = 0;	
	
	// For color
	vkVertexInputAttributeDescription_array[1].binding = 1;
	vkVertexInputAttributeDescription_array[1].location = 1;
	vkVertexInputAttributeDescription_array[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	vkVertexInputAttributeDescription_array[1].offset = 0;

	VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo;
	memset((void*)&vkPipelineVertexInputStateCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

	vkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vkPipelineVertexInputStateCreateInfo.pNext = NULL;
	vkPipelineVertexInputStateCreateInfo.flags = 0;
	vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = _ARRAYSIZE(vkVertexInputBindingDescription_array);
	vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vkVertexInputBindingDescription_array;

	vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = _ARRAYSIZE(vkVertexInputAttributeDescription_array);
	vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vkVertexInputAttributeDescription_array;

	// Input assembly state
	VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo;
	memset((void*)&vkPipelineInputAssemblyStateCreateInfo, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
	vkPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	vkPipelineInputAssemblyStateCreateInfo.pNext = NULL;
	vkPipelineInputAssemblyStateCreateInfo.flags = 0;
	vkPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	//vkPipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_TRUE;

	// Rasterization State
	VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo;
	memset((void*)&vkPipelineRasterizationStateCreateInfo, 0, sizeof(VkPipelineRasterizationStateCreateInfo));
	vkPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	vkPipelineRasterizationStateCreateInfo.pNext = NULL;
	vkPipelineRasterizationStateCreateInfo.flags = 0;
	vkPipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	vkPipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	vkPipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	vkPipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

	// Color Blend State
	VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState_array[1];
	memset((void*)vkPipelineColorBlendAttachmentState_array, 0, sizeof(VkPipelineColorBlendAttachmentState)*_ARRAYSIZE(vkPipelineColorBlendAttachmentState_array));
	vkPipelineColorBlendAttachmentState_array[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	vkPipelineColorBlendAttachmentState_array[0].blendEnable = VK_FALSE;
	
	
	VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo;
	memset((void*)&vkPipelineColorBlendStateCreateInfo, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
	vkPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	vkPipelineColorBlendStateCreateInfo.pNext = NULL;
	vkPipelineColorBlendStateCreateInfo.flags = 0;
	vkPipelineColorBlendStateCreateInfo.attachmentCount = _ARRAYSIZE(vkPipelineColorBlendAttachmentState_array);
	vkPipelineColorBlendStateCreateInfo.pAttachments = vkPipelineColorBlendAttachmentState_array;
	
	// Viewport Scissor State
	memset((void*)&vkViewport, 0, sizeof(VkViewport));
	vkViewport.x = 0;
	vkViewport.y = 0;
	vkViewport.width = (float)DRSJ_vkExtent2D_swapchain.width;
	vkViewport.height = (float)DRSJ_vkExtent2D_swapchain.height;
	vkViewport.minDepth = 0.0f;
	vkViewport.maxDepth = 1.0f;

	memset((void*)&vkRect2D_scissor, 0, sizeof(VkRect2D));
	vkRect2D_scissor.offset.x = 0;
	vkRect2D_scissor.offset.y = 0;
	//vkRect2D_scissor.extent = DRSJ_vkExtent2D_swapchain;
	vkRect2D_scissor.extent.width = DRSJ_vkExtent2D_swapchain.width;
	vkRect2D_scissor.extent.height = DRSJ_vkExtent2D_swapchain.height;

	VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo;
	memset((void*)&vkPipelineViewportStateCreateInfo, 0, sizeof(VkPipelineViewportStateCreateInfo));
	vkPipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vkPipelineViewportStateCreateInfo.pNext = NULL;
	vkPipelineViewportStateCreateInfo.flags = 0;
	vkPipelineViewportStateCreateInfo.viewportCount = 1;	// That means we can create multiple viewport
	vkPipelineViewportStateCreateInfo.pViewports = &vkViewport;
	vkPipelineViewportStateCreateInfo.scissorCount = 1;
	vkPipelineViewportStateCreateInfo.pScissors = &vkRect2D_scissor;
	
	// viewportCount and scissorCount must be same, samor jaun vkCreateGraphicsPipelines use honare in loop

	// Depth Stencil State
	// As we don't have depth then we can omit this state

	// Dynamic State
	// As we don't have any dynamic state then we can omit this state

	// Multi Sample State
	VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo;
	memset((void*)&vkPipelineMultisampleStateCreateInfo, 0, sizeof(VkPipelineMultisampleStateCreateInfo));
	vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	vkPipelineMultisampleStateCreateInfo.pNext = NULL;
	vkPipelineMultisampleStateCreateInfo.flags = 0;
	vkPipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Shader State
	VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfo_array[2];
	memset((void*)vkPipelineShaderStageCreateInfo_array, 0, sizeof(VkPipelineShaderStageCreateInfo) * _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array));
	
	// For Vertex Shader
	vkPipelineShaderStageCreateInfo_array[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vkPipelineShaderStageCreateInfo_array[0].pNext = NULL;
	vkPipelineShaderStageCreateInfo_array[0].flags = 0;
	vkPipelineShaderStageCreateInfo_array[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vkPipelineShaderStageCreateInfo_array[0].module = vkShaderModule_vertex_shader;
	vkPipelineShaderStageCreateInfo_array[0].pName = "main";
	vkPipelineShaderStageCreateInfo_array[0].pSpecializationInfo = NULL;
	
	// For Fragment Shader
	vkPipelineShaderStageCreateInfo_array[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vkPipelineShaderStageCreateInfo_array[1].pNext = NULL;
	vkPipelineShaderStageCreateInfo_array[1].flags = 0;
	vkPipelineShaderStageCreateInfo_array[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vkPipelineShaderStageCreateInfo_array[1].module = vkShaderModule_fragment_shader;
	vkPipelineShaderStageCreateInfo_array[1].pName = "main";
	vkPipelineShaderStageCreateInfo_array[1].pSpecializationInfo = NULL;

	// Tessellation State
	// We don't have tessellation shader so we can omit this state

	// As pipeline create as pipeline cache we create pipeline cache
	VkPipelineCacheCreateInfo vkPipelineCacheCreateInfo;
	memset((void*)&vkPipelineCacheCreateInfo, 0, sizeof(VkPipelineCacheCreateInfo));
	vkPipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	vkPipelineCacheCreateInfo.pNext = NULL;
	vkPipelineCacheCreateInfo.flags = 0;

	VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;

	vkResult = vkCreatePipelineCache(DRSJ_vkDevice,&vkPipelineCacheCreateInfo,NULL,&vkPipelineCache);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreatePipelineCache() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreatePipelineCache() SUCCESSED\n", __func__, __LINE__);
	}

	// Create the actual pipeline
	VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo;
	memset((void*)&vkGraphicsPipelineCreateInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));
	vkGraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	vkGraphicsPipelineCreateInfo.pNext = NULL;
	vkGraphicsPipelineCreateInfo.flags = 0;

	vkGraphicsPipelineCreateInfo.pVertexInputState = &vkPipelineVertexInputStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pInputAssemblyState = &vkPipelineInputAssemblyStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pRasterizationState = &vkPipelineRasterizationStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pColorBlendState = &vkPipelineColorBlendStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pViewportState = &vkPipelineViewportStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pDepthStencilState = NULL;
	vkGraphicsPipelineCreateInfo.pDynamicState = NULL;
	vkGraphicsPipelineCreateInfo.pMultisampleState = &vkPipelineMultisampleStateCreateInfo;
	vkGraphicsPipelineCreateInfo.stageCount = _ARRAYSIZE(vkPipelineShaderStageCreateInfo_array);
	vkGraphicsPipelineCreateInfo.pStages = vkPipelineShaderStageCreateInfo_array;
	vkGraphicsPipelineCreateInfo.pTessellationState = NULL;

	vkGraphicsPipelineCreateInfo.layout = vkPipelineLayout;
	vkGraphicsPipelineCreateInfo.renderPass = vkRenderPass;
	vkGraphicsPipelineCreateInfo.subpass = 0;
	vkGraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	vkGraphicsPipelineCreateInfo.basePipelineIndex = 0;
	
	// Now create the pipeline
	vkResult = vkCreateGraphicsPipelines(DRSJ_vkDevice, vkPipelineCache, 1, &vkGraphicsPipelineCreateInfo, NULL, &vkPipeline);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateGraphicsPipelines() FAILED\n", __func__, __LINE__);
		vkDestroyPipelineCache(DRSJ_vkDevice, vkPipelineCache, NULL);
		vkPipelineCache = VK_NULL_HANDLE;
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateGraphicsPipelines() SUCCESSED\n", __func__, __LINE__);
	}


	// we have done with pipelineCache destroy it
	vkDestroyPipelineCache(DRSJ_vkDevice, vkPipelineCache, NULL);
	vkPipelineCache = VK_NULL_HANDLE;

	return vkResult;
}


VkResult createDescriptorPool(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// Before cretaing actual pool, Vulkan expect pool size
	VkDescriptorPoolSize vkDescriptorPoolSize;
	memset((void*)&vkDescriptorPoolSize, 0, sizeof(VkDescriptorPoolSize));
	
	vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vkDescriptorPoolSize.descriptorCount = 1;

	// create the pool
	VkDescriptorPoolCreateInfo vkDescriptorPoolCreateInfo;
	memset((void*)&vkDescriptorPoolCreateInfo, 0, sizeof(VkDescriptorPoolCreateInfo));
	vkDescriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	vkDescriptorPoolCreateInfo.pNext = NULL;
	vkDescriptorPoolCreateInfo.flags = 0;
	vkDescriptorPoolCreateInfo.poolSizeCount = 1;
	vkDescriptorPoolCreateInfo.pPoolSizes = &vkDescriptorPoolSize;
	vkDescriptorPoolCreateInfo.maxSets = 3;


	vkResult = vkCreateDescriptorPool(DRSJ_vkDevice, &vkDescriptorPoolCreateInfo, NULL, &vkDescriptorPool);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateDescriptorPool() FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateDescriptorPool() SUCCESSED\n", __func__, __LINE__);
	}


	return vkResult;
}

VkResult createDescriptorSet(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code
	// Initalize descriptor Set Allocate Info
	VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo;
	memset((void*)&vkDescriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));
	vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	vkDescriptorSetAllocateInfo.pNext = NULL;
	vkDescriptorSetAllocateInfo.descriptorPool = vkDescriptorPool;
	vkDescriptorSetAllocateInfo.descriptorSetCount = 1;
	vkDescriptorSetAllocateInfo.pSetLayouts = &vkDescriptorSetLayout;

	fprintf(DRSJ_gpLogFile, "%s %d : - - - - - For road border - - - - -\n", __func__, __LINE__);
	{
		vkResult = vkAllocateDescriptorSets(DRSJ_vkDevice, &vkDescriptorSetAllocateInfo, &vkDescriptorSet);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateDescriptorSets() FAILED\n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateDescriptorSets() SUCCESSED\n", __func__, __LINE__);
		}

		// Describe whether buffer as uniform 
		VkDescriptorBufferInfo vkDescriptorBufferInfo;
		memset((void*)&vkDescriptorBufferInfo, 0, sizeof(VkDescriptorBufferInfo));
		vkDescriptorBufferInfo.buffer = uniformData_road_border.vkBuffer;
		vkDescriptorBufferInfo.offset = 0;
		vkDescriptorBufferInfo.range = sizeof(MyUniformData);

		// Now update the above descriptor set directly to the shader.
		// there are two  ways to update writing directly to shader or writing from one shader to another shader
		// we will prefare direct writing to shaders, this req following struct
		VkWriteDescriptorSet vkWriteDescriptorSet;
		memset((void*)&vkWriteDescriptorSet, 0, sizeof(VkWriteDescriptorSet));

		vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vkWriteDescriptorSet.dstSet = vkDescriptorSet;
		vkWriteDescriptorSet.dstArrayElement = 0;
		vkWriteDescriptorSet.descriptorCount = 1;
		vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vkWriteDescriptorSet.pBufferInfo = &vkDescriptorBufferInfo;
		vkWriteDescriptorSet.pTexelBufferView = NULL;
		vkWriteDescriptorSet.dstBinding = 0;	// bcz our uniform binding 0 index

		vkUpdateDescriptorSets(DRSJ_vkDevice, 1, &vkWriteDescriptorSet, 0, NULL);
	}


	return vkResult;
}


/******************************************************
* Name : createFramebuffer
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Renderpass
*
******************************************************/
VkResult createFramebuffer(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code
	VkImageView vkImageView_attachment_array[1];
	memset((void*)vkImageView_attachment_array, 0, sizeof(VkImageView) * _ARRAYSIZE(vkImageView_attachment_array));

	VkFramebufferCreateInfo vkFramebufferCreateInfo;
	memset((void*)&vkFramebufferCreateInfo, 0, sizeof(VkFramebufferCreateInfo));

	vkFramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	vkFramebufferCreateInfo.pNext = NULL;
	vkFramebufferCreateInfo.flags = 0;
	vkFramebufferCreateInfo.renderPass = vkRenderPass;
	vkFramebufferCreateInfo.attachmentCount = _ARRAYSIZE(vkImageView_attachment_array);
	vkFramebufferCreateInfo.pAttachments = vkImageView_attachment_array;
	vkFramebufferCreateInfo.width = DRSJ_vkExtent2D_swapchain.width;
	vkFramebufferCreateInfo.height = DRSJ_vkExtent2D_swapchain.height;
	vkFramebufferCreateInfo.layers = 1;


	DRSJ_vkFramebuffer_array = (VkFramebuffer*)calloc(DRSJ_swapchainImageCount, sizeof(VkFramebuffer));
	if (!DRSJ_vkFramebuffer_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : calloc() FAILED For DRSJ_vkFramebuffer_array \n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}


	for (uint32_t iCounter = 0; iCounter < DRSJ_swapchainImageCount; iCounter++)
	{
		vkImageView_attachment_array[0] = DRSJ_vkSwapchainImageView_array[iCounter];
		vkResult = vkCreateFramebuffer(DRSJ_vkDevice, &vkFramebufferCreateInfo, NULL, &(DRSJ_vkFramebuffer_array[iCounter]));
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateFramebuffer() FAILED at index : [%d]\n", __func__, __LINE__, iCounter);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateFramebuffer() SUCCESSED at index : [%d]\n", __func__, __LINE__, iCounter);
		}
	}

	return vkResult;
}

/******************************************************
* Name : createSemaphores
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Semaphores
*
******************************************************/
VkResult createSemaphores(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	VkSemaphoreCreateInfo vkSemaphoreCreateInfo;
	memset((void*)&vkSemaphoreCreateInfo, 0, sizeof(VkSemaphoreCreateInfo));

	vkSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkSemaphoreCreateInfo.flags = 0;	// reserved must be 0
	vkSemaphoreCreateInfo.pNext = NULL;

	vkResult = vkCreateSemaphore(DRSJ_vkDevice, &vkSemaphoreCreateInfo, NULL, &DRSJ_vkSemaphore_backBuffer);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateSemaphore() for vkSemaphore_backBuffer FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateSemaphore() for vkSemaphore_backBuffer SUCCESSED\n", __func__, __LINE__);
	}

	vkResult = vkCreateSemaphore(DRSJ_vkDevice, &vkSemaphoreCreateInfo, NULL, &DRSJ_vkSemaphore_renderComplete);
	if (VK_SUCCESS != vkResult)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateSemaphore() for DRSJ_vkSemaphore_renderComplete FAILED\n", __func__, __LINE__);
		return (vkResult);
	}
	else
	{
		fprintf(DRSJ_gpLogFile, "%s %d : vkCreateSemaphore() for DRSJ_vkSemaphore_renderComplete SUCCESSED\n", __func__, __LINE__);
	}

	return vkResult;
}

/******************************************************
* Name : createFences
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates Fences
*
******************************************************/
VkResult createFences(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code
	VkFenceCreateInfo vkFenceCreateInfo;
	memset((void*)&vkFenceCreateInfo, 0, sizeof(VkFenceCreateInfo));

	vkFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vkFenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	vkFenceCreateInfo.pNext = NULL;

	DRSJ_vkFence_array = (VkFence*)calloc(DRSJ_swapchainImageCount, sizeof(VkFence));
	if (!DRSJ_vkFence_array)
	{
		fprintf(DRSJ_gpLogFile, "%s %d : calloc() FAILED For DRSJ_vkFence_array \n", __func__, __LINE__);
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	for (uint32_t iCounter = 0; iCounter < DRSJ_swapchainImageCount; iCounter++)
	{
		vkResult = vkCreateFence(DRSJ_vkDevice, &vkFenceCreateInfo, NULL, &(DRSJ_vkFence_array[iCounter]));
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateFence() FAILED at index[%d]\n", __func__, __LINE__,iCounter);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateFence() SUCCESSED at index[%d]\n", __func__, __LINE__, iCounter);
		}
	}

	return vkResult;
}

/******************************************************
* Name : buildCommandBuffers
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will build command buffers
*
******************************************************/
VkResult buildCommandBuffers(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	// Loop per swapchain image count

	for (uint32_t iCounter = 0; iCounter < DRSJ_swapchainImageCount; iCounter++)
	{

		// Rset command buffers

		vkResult = vkResetCommandBuffer(DRSJ_vkCommandBuffer_array[iCounter], 0);	
									// 0 -> don't release the resources by command pool by these command buffers.
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkResetCommandBuffer() FAILED at index[%d]\n", __func__, __LINE__, iCounter);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkResetCommandBuffer() SUCCESSED at index[%d]\n", __func__, __LINE__, iCounter);
		}

		VkCommandBufferBeginInfo vkCommandBufferBeginInfo;
		memset((void*)&vkCommandBufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
		vkCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkCommandBufferBeginInfo.flags = 0;
		vkCommandBufferBeginInfo.pNext = NULL;
		
		//vkCommandBufferBeginInfo.pInheritanceInfo = NULL;

		vkResult = vkBeginCommandBuffer(DRSJ_vkCommandBuffer_array[iCounter], &vkCommandBufferBeginInfo);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBeginCommandBuffer() FAILED at index[%d]\n", __func__, __LINE__, iCounter);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBeginCommandBuffer() SUCCESSED at index[%d]\n", __func__, __LINE__, iCounter);
		}

		

		// Set clear values
		VkClearValue vkClearValue_array[1];
		memset((void*)vkClearValue_array, 0, sizeof(VkClearValue) * _ARRAYSIZE(vkClearValue_array));
		
		vkClearValue_array[0].color = DRSJ_vkClearColorValue;

		
		//Render pass begin info
		VkRenderPassBeginInfo vkRenderPassBeginInfo;
		memset((void*)&vkRenderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
		vkRenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		vkRenderPassBeginInfo.pNext = NULL;
		vkRenderPassBeginInfo.renderPass = vkRenderPass;
		vkRenderPassBeginInfo.renderArea.offset.x = 0;
		vkRenderPassBeginInfo.renderArea.offset.y = 0;
		vkRenderPassBeginInfo.renderArea.extent.width = DRSJ_vkExtent2D_swapchain.width;
		vkRenderPassBeginInfo.renderArea.extent.height = DRSJ_vkExtent2D_swapchain.height;
		vkRenderPassBeginInfo.clearValueCount = _ARRAYSIZE(vkClearValue_array);
		vkRenderPassBeginInfo.pClearValues = vkClearValue_array;
		vkRenderPassBeginInfo.framebuffer = DRSJ_vkFramebuffer_array[iCounter];


		// Begin the render pass
		vkCmdBeginRenderPass(DRSJ_vkCommandBuffer_array[iCounter], &vkRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		
		// Bind Pipeline
		vkCmdBindPipeline(DRSJ_vkCommandBuffer_array[iCounter], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

		// Bind our descriptor set with pipeline
		vkCmdBindDescriptorSets(DRSJ_vkCommandBuffer_array[iCounter], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet, 0, NULL);


		buildCommandBuffers_by_darshan(iCounter);

		buildCommandBuffers_2(iCounter);
		
		//End Render pass
		vkCmdEndRenderPass(DRSJ_vkCommandBuffer_array[iCounter]);
	
		// End command buffer recording
		vkResult = vkEndCommandBuffer(DRSJ_vkCommandBuffer_array[iCounter]);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkEndCommandBuffer() FAILED at index[%d]\n", __func__, __LINE__, iCounter);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkEndCommandBuffer() SUCCESSED at index[%d]\n", __func__, __LINE__, iCounter);
		}
		
	}

	return vkResult;
}


/******************************************************
* Name : createDummy
*
* Parameter :void
*
* Return Value : VkResult
*				If all good returns VK_SUCCESS
*
* Explanation :
*		This function will creates ---
*
******************************************************/
VkResult createDummy(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code

	return vkResult;
}


VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(
					VkDebugReportFlagsEXT vkDebugReportFlagsEXT, 
					VkDebugReportObjectTypeEXT vkDebugReportObjectTypeEXT, 
					uint64_t object, 
					size_t location, 
					int32_t messageCode, 
					const char* pLayerPrefix, 
					const char* pMessage, 
					void* pUserData)
{
	FILE* logFile = fopen("ValidationLogFile.txt", "w");
	if (NULL == logFile)
	{
		MessageBox(NULL, TEXT("Log File Opening/Creation FAILED"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(logFile, "%s %d : Log File opened Sucessfully !!!\n", __func__, __LINE__);
	}

	fprintf(logFile, "%s %d : DRSJ_Validation : debugReportCallback() : %s (%d) = %s\n", __func__, __LINE__, pLayerPrefix,messageCode, pMessage);

	return (FALSE);
}

