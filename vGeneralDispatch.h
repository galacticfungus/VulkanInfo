#pragma once
#include "stdafx.h"

class vGeneralDispatch {
public:
	
	//A list of instance methods that are called via the dispatch table
	VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
		VkInstance                                  instance,
		const char* pName);
	VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceVersion(
		uint32_t*                                   pApiVersion);
	VkResult vkEnumerateInstanceExtensionProperties(
		const char*                                 pLayerName,
		uint32_t*                                   pPropertyCount,
		VkExtensionProperties*                      pProperties);
	VkResult vkEnumerateInstanceLayerProperties(
		uint32_t*                                   pPropertyCount,
		VkLayerProperties*                          pProperties);
	VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(
		const VkInstanceCreateInfo*                 pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkInstance*                                 pInstance);
	//The initializer method since creating the class can fail it uses a factory method
	static vGeneralDispatch* Init();
	
	
private:
	enum VK_GENERAL_DISPATCH {
		ID_vkGetInstanceProcAddr,
		ID_vkCreateInstance,
		ID_vkEnumerateInstanceVersion,
		ID_vkEnumerateInstanceExtensionProperties,
		ID_vkEnumerateInstanceLayerProperties,
	};
	//Windows Vulkan library
	LPCWSTR vulkanName = L"vulkan-1.dll";
	//Handle to Vulkan library
	HMODULE vLibModule;
	//The dispatch table	
	std::map<VK_GENERAL_DISPATCH, PFN_vkVoidFunction> dispatchTable;
	//The initial function pointer to the vkGetInstanceFunction
	vGeneralDispatch();
	~vGeneralDispatch();

	
};