#pragma once
#include "vGeneralDispatch.h"
class vInstanceDispatch
{
public:
	
	~vInstanceDispatch();
	static vInstanceDispatch* Init(VkInstance instance, vGeneralDispatch *loader);
	VkResult vkEnumeratePhysicalDevices(
		VkInstance                                  instance,
		uint32_t*                                   pPhysicalDeviceCount,
		VkPhysicalDevice*                           pPhysicalDevices);
	VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(
		VkInstance                                  instance,
		const VkAllocationCallbacks*                pAllocator);
	VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups(
		VkInstance                                  instance,
		uint32_t*                                   pPhysicalDeviceGroupCount,
		VkPhysicalDeviceGroupProperties*            pPhysicalDeviceGroupProperties);
	VKAPI_ATTR void VKAPI_CALL vkDestroySurfaceKHR(
		VkInstance                                  instance,
		VkSurfaceKHR                                surface,
		const VkAllocationCallbacks*                pAllocator);
	VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(
		VkInstance                                  instance,
		const VkDisplaySurfaceCreateInfoKHR*        pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkSurfaceKHR*                               pSurface);
	VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR(
		VkInstance                                  instance,
		uint32_t*                                   pPhysicalDeviceGroupCount,
		VkPhysicalDeviceGroupProperties*            pPhysicalDeviceGroupProperties);
	VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
		VkInstance                                  instance,
		const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkDebugReportCallbackEXT*                   pCallback);

	VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
		VkInstance                                  instance,
		VkDebugReportCallbackEXT                    callback,
		const VkAllocationCallbacks*                pAllocator);

	VKAPI_ATTR void VKAPI_CALL vkDebugReportMessageEXT(
		VkInstance                                  instance,
		VkDebugReportFlagsEXT                       flags,
		VkDebugReportObjectTypeEXT                  objectType,
		uint64_t                                    object,
		size_t                                      location,
		int32_t                                     messageCode,
		const char*                                 pLayerPrefix,
		const char*                                 pMessage);

	VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
		VkInstance                                  instance,
		const VkDebugUtilsMessengerCreateInfoEXT*   pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkDebugUtilsMessengerEXT*                   pMessenger);

	VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
		VkInstance                                  instance,
		VkDebugUtilsMessengerEXT                    messenger,
		const VkAllocationCallbacks*                pAllocator);

	VKAPI_ATTR void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(
		VkInstance                                  instance,
		VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);

	void vkGetPhysicalDeviceFeatures(
		VkPhysicalDevice                            physicalDevice,
		VkPhysicalDeviceFeatures*                   pFeatures);

	void vkGetPhysicalDeviceFeatures2(
		VkPhysicalDevice                            physicalDevice,
		VkPhysicalDeviceFeatures2*                  pFeatures);

	VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties(
		VkPhysicalDevice                            physicalDevice,
		VkPhysicalDeviceProperties*                 pProperties);

	VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(
		VkPhysicalDevice                            physicalDevice,
		uint32_t*                                   pQueueFamilyPropertyCount,
		VkQueueFamilyProperties*                    pQueueFamilyProperties);
private:
	enum VK_INSTANCE_DISPATCH {
		ID_vkEnumeratePhysicalDevices,
		ID_vkDestroyInstance,
		ID_vkEnumeratePhysicalDeviceGroups,
		ID_vkDestroySurfaceKHR,
		ID_vkCreateDisplayPlaneSurfaceKHR,
		ID_vkEnumeratePhysicalDeviceGroupsKHR,
		ID_vkCreateDebugReportCallbackEXT,
		ID_vkDestroyDebugReportCallbackEXT,
		ID_vkDebugReportMessageEXT,
		ID_vkCreateDebugUtilsMessengerEXT,
		ID_vkDestroyDebugUtilsMessengerEXT,
		ID_vkSubmitDebugUtilsMessageEXT,
		ID_vkGetPhysicalDeviceFeatures,
		ID_vkGetPhysicalDeviceFeatures2,
		ID_vkGetPhysicalDeviceProperties,
		ID_vkGetPhysicalDeviceQueueFamilyProperties,
	};
	//Dispatch table for all instance functions
	//VkInstance instance = VK_NULL_HANDLE;
	//vGeneralDispatch *generalDispatch;
	vInstanceDispatch();
	std::map<VK_INSTANCE_DISPATCH, PFN_vkVoidFunction> dispatchTable = std::map<VK_INSTANCE_DISPATCH, PFN_vkVoidFunction>();
	
};

