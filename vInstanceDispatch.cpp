#include "stdafx.h"
#include "vInstanceDispatch.h"


vInstanceDispatch::vInstanceDispatch()
{
	
	//TODO: Create dispatch table dynamically or at least add an initial capacity to store all the functions required
}


vInstanceDispatch::~vInstanceDispatch()
{
}

VKAPI_ATTR void VKAPI_CALL vInstanceDispatch::vkDestroyInstance(
	VkInstance                                  instance,
	const VkAllocationCallbacks*                pAllocator) {
	auto getProc = (PFN_vkDestroyInstance) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDestroyInstance];
	return getProc(instance, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vInstanceDispatch::vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t * pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties)
{
	auto getProc = (PFN_vkEnumeratePhysicalDeviceGroups) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkEnumeratePhysicalDeviceGroups];
	return getProc(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

VkResult vInstanceDispatch::vkEnumeratePhysicalDevices(
	VkInstance                                  instance,
	uint32_t*                                   pPhysicalDeviceCount,
	VkPhysicalDevice*                           pPhysicalDevices) {
	auto getProc = (PFN_vkEnumeratePhysicalDevices) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkEnumeratePhysicalDevices];
	return getProc(instance, pPhysicalDeviceCount, pPhysicalDevices);
}

VKAPI_ATTR void VKAPI_CALL vInstanceDispatch::vkDestroySurfaceKHR(
	VkInstance                                  instance,
	VkSurfaceKHR                                surface,
	const VkAllocationCallbacks*                pAllocator) {
	auto getProc = (PFN_vkDestroySurfaceKHR) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDestroySurfaceKHR];
	return getProc(instance, surface, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vInstanceDispatch::vkCreateDisplayPlaneSurfaceKHR(
	VkInstance                                  instance,
	const VkDisplaySurfaceCreateInfoKHR*        pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkSurfaceKHR*                               pSurface) {
	auto getProc = (PFN_vkCreateDisplayPlaneSurfaceKHR) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkCreateDisplayPlaneSurfaceKHR];
	return getProc(instance, pCreateInfo, pAllocator,pSurface);
}

VKAPI_ATTR VkResult VKAPI_CALL vInstanceDispatch::vkEnumeratePhysicalDeviceGroupsKHR(
	VkInstance                                  instance,
	uint32_t*                                   pPhysicalDeviceGroupCount,
	VkPhysicalDeviceGroupProperties*            pPhysicalDeviceGroupProperties) {
	auto getProc = (PFN_vkEnumeratePhysicalDeviceGroupsKHR) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkEnumeratePhysicalDeviceGroupsKHR];
	return getProc(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}
VKAPI_ATTR VkResult VKAPI_CALL vInstanceDispatch::vkCreateDebugReportCallbackEXT(
	VkInstance                                  instance,
	const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkDebugReportCallbackEXT*                   pCallback) {
	auto getProc = (PFN_vkCreateDebugReportCallbackEXT) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkCreateDebugReportCallbackEXT];
	return getProc(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vInstanceDispatch::vkDestroyDebugReportCallbackEXT(
	VkInstance                                  instance,
	VkDebugReportCallbackEXT                    callback,
	const VkAllocationCallbacks*                pAllocator) {
	auto getProc = (PFN_vkDestroyDebugReportCallbackEXT) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDestroyDebugReportCallbackEXT];
	return getProc(instance, callback, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL vInstanceDispatch::vkDebugReportMessageEXT(
	VkInstance                                  instance,
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMessage) {
	auto getProc = (PFN_vkDebugReportMessageEXT) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDebugReportMessageEXT];
	return getProc(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

VKAPI_ATTR VkResult VKAPI_CALL vInstanceDispatch::vkCreateDebugUtilsMessengerEXT(
	VkInstance                                  instance,
	const VkDebugUtilsMessengerCreateInfoEXT*   pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkDebugUtilsMessengerEXT*                   pMessenger) {
	auto getProc = (PFN_vkCreateDebugUtilsMessengerEXT) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkCreateDebugUtilsMessengerEXT];
	return getProc(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vInstanceDispatch::vkDestroyDebugUtilsMessengerEXT(
	VkInstance                                  instance,
	VkDebugUtilsMessengerEXT                    messenger,
	const VkAllocationCallbacks*                pAllocator) {
	auto getProc = (PFN_vkDestroyDebugUtilsMessengerEXT) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDestroyDebugUtilsMessengerEXT];
	return getProc(instance, messenger, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL vInstanceDispatch::vkSubmitDebugUtilsMessageEXT(
	VkInstance                                  instance,
	VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
	auto getProc = (PFN_vkSubmitDebugUtilsMessageEXT) dispatchTable[VK_INSTANCE_DISPATCH::ID_vkSubmitDebugUtilsMessageEXT];
	return getProc(instance, messageSeverity, messageTypes, pCallbackData);
}

void vInstanceDispatch::vkGetPhysicalDeviceFeatures(
	VkPhysicalDevice                            physicalDevice,
	VkPhysicalDeviceFeatures*                   pFeatures) {
	auto getProc = (PFN_vkGetPhysicalDeviceFeatures)dispatchTable[VK_INSTANCE_DISPATCH::ID_vkGetPhysicalDeviceFeatures];
	return getProc(physicalDevice, pFeatures);
}

void vInstanceDispatch::vkGetPhysicalDeviceFeatures2(
	VkPhysicalDevice                            physicalDevice,
	VkPhysicalDeviceFeatures2*                  pFeatures) {
	auto getProc = (PFN_vkGetPhysicalDeviceFeatures2)dispatchTable[VK_INSTANCE_DISPATCH::ID_vkGetPhysicalDeviceFeatures2];
	return getProc(physicalDevice, pFeatures);
}

VKAPI_ATTR void VKAPI_CALL vInstanceDispatch::vkGetPhysicalDeviceProperties(
	VkPhysicalDevice                            physicalDevice,
	VkPhysicalDeviceProperties*                 pProperties) {
	auto getProc = (PFN_vkGetPhysicalDeviceProperties)dispatchTable[VK_INSTANCE_DISPATCH::ID_vkGetPhysicalDeviceProperties];
	return getProc(physicalDevice, pProperties);
}

VKAPI_ATTR void VKAPI_CALL vInstanceDispatch::vkGetPhysicalDeviceQueueFamilyProperties(
	VkPhysicalDevice                            physicalDevice,
	uint32_t*                                   pQueueFamilyPropertyCount,
	VkQueueFamilyProperties*                    pQueueFamilyProperties) {
	auto getProc = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)dispatchTable[VK_INSTANCE_DISPATCH::ID_vkGetPhysicalDeviceQueueFamilyProperties];
	return getProc(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

vInstanceDispatch* vInstanceDispatch::Init(VkInstance instance, vGeneralDispatch *genDispatch)
{
	vInstanceDispatch *instDispatch = new vInstanceDispatch();

	instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDestroyInstance] = genDispatch->vkGetInstanceProcAddr(instance, "vkDestroyInstance");
	instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkEnumeratePhysicalDevices] = genDispatch->vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices");
	instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkEnumeratePhysicalDeviceGroups] = genDispatch->vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDeviceGroups");
	//Load an extension for these to work
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDestroySurfaceKHR] = genDispatch->vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkCreateDisplayPlaneSurfaceKHR] = genDispatch->vkGetInstanceProcAddr(instance, "vkCreateDisplayPlaneSurfaceKHR");
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkEnumeratePhysicalDeviceGroupsKHR] = genDispatch->vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDeviceGroupsKHR");
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkCreateDebugReportCallbackEXT] = genDispatch->vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDestroyDebugReportCallbackEXT] = genDispatch->vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDebugReportMessageEXT] = genDispatch->vkGetInstanceProcAddr(instance, "vkDebugReportMessageEXT");
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkCreateDebugUtilsMessengerEXT] = genDispatch->vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkDestroyDebugUtilsMessengerEXT] = genDispatch->vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	//instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkSubmitDebugUtilsMessageEXT] = genDispatch->vkGetInstanceProcAddr(instance, "vkSubmitDebugUtilsMessageEXT");
	instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkGetPhysicalDeviceFeatures] = genDispatch->vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures");
	instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkGetPhysicalDeviceFeatures2] = genDispatch->vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2");
	instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkGetPhysicalDeviceProperties] = genDispatch->vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
	instDispatch->dispatchTable[VK_INSTANCE_DISPATCH::ID_vkGetPhysicalDeviceQueueFamilyProperties] = genDispatch->vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
	//TODO: Ensure that the dispatch table loaded correctly - ie check for NULL's
	for (auto dispatch : instDispatch->dispatchTable) {
		if (dispatch.second == nullptr)
		{
			std::cerr << "Instance Dispatch failed to load function " << dispatch.first << std::endl;
			return nullptr;
		}
	}

	//Now we use the general dispatcher to load the instance functions
	return instDispatch;
}
