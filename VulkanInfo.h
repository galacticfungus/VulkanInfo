#pragma once
class VulkanInfo
{
public:
	void DisplayVersion();
	bool CreateInstance(std::string engineName, std::string applicationName, uint32_t engineVersion, uint32_t applicationVersion);
	VulkanInfo();
	bool LoadVulkanLibrary();
	~VulkanInfo();
	void DisplayLayerProperties();
	void DisplayExtensionProperties();
	void DisplayLayerExtensions(const VkLayerProperties *layer);
	void DisplayDevices();

private:
	struct InstanceLayer {
		VkLayerProperties *layerProperties;
		std::vector<VkExtensionProperties> extensions;
	};
	
	vGeneralDispatch * globalDispatch;
	uint32_t instanceVersion = 0;
	std::vector<InstanceLayer> *layerProperties;//TODO: Use this if we need to keep track of supports
	vInstanceDispatch *instanceDispatch;
	std::string _engineName;
	std::string _applicationName;
	VkInstance _instance;
	std::string ConvertDeviceTypeToString(VkPhysicalDeviceType deviceType);
	std::unique_ptr<std::string> ConvertPackedVersionToString(uint32_t packedVersion);
	void DisplayLimits(VkPhysicalDeviceLimits *limits);
	void DisplaySparseProperties(VkPhysicalDeviceSparseProperties *sparse);
	std::unique_ptr<std::string> ConvertSampleFlagToString(VkSampleCountFlags flags);
	void PrintOneColumn(std::string label, std::string data);
	void PrintTwoColumn(std::string labelOne, std::string dataOne, std::string labelTwo, std::string dataTwo);
	void PrintThreeColumn(std::string labelOne, std::string dataOne, std::string labelTwo, std::string dataTwo, std::string labelThree, std::string dataThree);
	void DisplayFeature(std::string featureName, VkBool32 isActive);
	void DisplayFeatures(VkPhysicalDeviceFeatures *features);
	std::string CreateRangeLimitString(std::string minValue, std::string maxValue);
	std::string CreateXYZString(std::string xValue, std::string yValue, std::string zValue);
	void DisplayDeviceQueues(std::vector<VkQueueFamilyProperties> *queues);
};
