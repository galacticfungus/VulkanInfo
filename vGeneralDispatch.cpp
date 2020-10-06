#include "stdafx.h"
#include "vGeneralDispatch.h"

//The initializer method since creating the class can fail it uses a factory method
vGeneralDispatch* vGeneralDispatch::Init() {
	//TODO: Return NULL if initialization fails for any reason
	//Create an object instance
	auto loader = new vGeneralDispatch();
	//Load the library
	loader->vLibModule = LoadLibraryW(loader->vulkanName);
	if (loader->vLibModule == nullptr) {
		return nullptr;
	}
	//Setup the Vulkan function loader
	const FARPROC vkGetProc = GetProcAddress(loader->vLibModule, "vkGetInstanceProcAddr");
	//TODO: Check that the platform function was loaded correctly
	//Create the dispatch table and add global instance functions
	loader->dispatchTable[VK_GENERAL_DISPATCH::ID_vkGetInstanceProcAddr] = reinterpret_cast<PFN_vkVoidFunction> (vkGetProc);
	// Add PFN_vkGetInstanceProcAddr to it and use it to initialize the rest of the global dispatch table
	loader->dispatchTable[VK_GENERAL_DISPATCH::ID_vkCreateInstance] = loader->vkGetInstanceProcAddr(nullptr, "vkCreateInstance");
	loader->dispatchTable[VK_GENERAL_DISPATCH::ID_vkEnumerateInstanceVersion] = loader->vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion");
	loader->dispatchTable[VK_GENERAL_DISPATCH::ID_vkEnumerateInstanceExtensionProperties] = loader->vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceExtensionProperties");
	loader->dispatchTable[VK_GENERAL_DISPATCH::ID_vkEnumerateInstanceLayerProperties] = loader->vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceLayerProperties");
	//TODO: Check that the dispatch table was,loaded correctly
	for (auto dispatch : loader->dispatchTable) {
		if (dispatch.second == nullptr)
		{
			std::cerr << "General Dispatch failed to load function " << dispatch.first << std::endl;
			return nullptr;
		}
	}
	return loader;
}





VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vGeneralDispatch::vkGetInstanceProcAddr(
	VkInstance                                  instance,
	const char* pName) {
	const auto getProc =  (PFN_vkGetInstanceProcAddr) dispatchTable[VK_GENERAL_DISPATCH::ID_vkGetInstanceProcAddr];
	return getProc(instance, pName);
}

VKAPI_ATTR VkResult VKAPI_CALL vGeneralDispatch::vkEnumerateInstanceVersion(
	uint32_t*                                   pApiVersion) {
	const auto getProc = reinterpret_cast<PFN_vkEnumerateInstanceVersion>(dispatchTable[VK_GENERAL_DISPATCH::ID_vkEnumerateInstanceVersion]);
	return getProc(pApiVersion);
}

VkResult vGeneralDispatch::vkEnumerateInstanceExtensionProperties(
	const char*                                 pLayerName,
	uint32_t*                                   pPropertyCount,
	VkExtensionProperties*                      pProperties) {
	const auto getProc = (PFN_vkEnumerateInstanceExtensionProperties) dispatchTable[VK_GENERAL_DISPATCH::ID_vkEnumerateInstanceExtensionProperties];
	return getProc(pLayerName, pPropertyCount, pProperties);
}

VkResult vGeneralDispatch::vkEnumerateInstanceLayerProperties(
	uint32_t*                                   pPropertyCount,
	VkLayerProperties*                          pProperties) {
	const auto getProc = (PFN_vkEnumerateInstanceLayerProperties) dispatchTable[VK_GENERAL_DISPATCH::ID_vkEnumerateInstanceLayerProperties];
	return getProc(pPropertyCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL vGeneralDispatch::vkCreateInstance(const VkInstanceCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkInstance * pInstance)
{
	const auto getProc = (PFN_vkCreateInstance)dispatchTable[VK_GENERAL_DISPATCH::ID_vkCreateInstance];
	return getProc(pCreateInfo, pAllocator, pInstance);
}

//Private constructor
vGeneralDispatch::vGeneralDispatch() {
	//Initialise some defaults
	vLibModule = nullptr;
	dispatchTable = std::map<VK_GENERAL_DISPATCH, PFN_vkVoidFunction>();
}

vGeneralDispatch::~vGeneralDispatch()
{
	FreeLibrary(vLibModule);
	//delete dispatchTable;//TODO: Use smart pointer instead
}