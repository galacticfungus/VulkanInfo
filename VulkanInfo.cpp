// VulkanInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vInstanceDispatch.h"
#include "vGeneralDispatch.h"

#include "VulkanInfo.h"

using std::cout;

int main()
{
	//Initialize and wrap the Vulkan library
	auto info = VulkanInfo();//TODO: Add factory constructor to properly initialize an info object
	auto libraryLoaded = info.LoadVulkanLibrary();
	if (libraryLoaded == false) {
		cout << "ERROR: Vulkan driver could not be loaded." << std::endl;
		return 0;
	}
	info.DisplayVersion();
	info.DisplayLayerProperties();
	info.DisplayExtensionProperties();
	//Now we create an instance and load a new dispatch table that includes all functions that are not device specific
	if (info.CreateInstance("", "vkInfo", VK_MAKE_VERSION(0, 0, 0), VK_MAKE_VERSION(0, 1, 0))) {
		info.DisplayDevices();
	}
	return 0;
}

void VulkanInfo::DisplayExtensionProperties() {
	VkResult result;
	cout << "List of supported Extensions" << std::endl;
	uint32_t extensionCount = 0;
	do {
		result = globalDispatch->vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	} while (result == VK_INCOMPLETE);
	if (result != VK_SUCCESS) {
		cout << "ERROR: Failed to obtain number of supported extensions";
		return;
	}
	auto extensions = std::vector<VkExtensionProperties>(extensionCount);
	do {
		result = globalDispatch->vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	} while (result == VK_INCOMPLETE);
	if (result != VK_SUCCESS) {
		cout << "ERROR: Failed to enumerate the supported extensions";
		return;
	}
	cout << std::left << std::setw(40) << "Extension Name" << std::right << std::setw(20) << "Version" << std::endl;
	for (auto const extension : extensions) {
		cout << std::left << std::setw(40) << extension.extensionName << std::right << std::setw(20) << extension.specVersion << std::endl;
	}
	cout << std::endl;
}

void VulkanInfo::DisplayVersion() {
	if (VK_SUCCESS != globalDispatch->vkEnumerateInstanceVersion(&instanceVersion)) {
		printf("Failed to query instance version");
	}
	auto major = VK_VERSION_MAJOR(instanceVersion);
	auto minor = VK_VERSION_MINOR(instanceVersion);
	auto patch = VK_VERSION_PATCH(instanceVersion);
	//TODO: Replace with cout
	printf("Instance Version: %d.%d.%d \n", major, minor, patch);
	//This doesn't mean that a Vulkan capable device is installed
}

void VulkanInfo::DisplayDevices() {
	uint32_t deviceCount = 0;
	VkResult result;
	do {
		result = instanceDispatch->vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
	} while (result == VK_INCOMPLETE);
	if (result != VK_SUCCESS) {
		cout << "Failed to obtain the number of Vulkan Devices" << std::endl;
		return;
	}
	if (deviceCount == 0) {
		cout << "No Vulkan Devices detected" << std::endl;
		return;
	}
	auto devices = std::vector<VkPhysicalDevice>(deviceCount);
	do {
		result = instanceDispatch->vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());
	} while (result == VK_INCOMPLETE);
	if (result != VK_SUCCESS) {
		cout << "Failed to enumerate the Vulkan Devices" << std::endl;
		return;
	}

	VkPhysicalDeviceProperties properties;

	VkPhysicalDeviceFeatures features;
	cout << "Detected Vulkan Devices" << std::endl << std::endl;
	//TODO: Create heading row
	for (const auto device : devices) {
		instanceDispatch->vkGetPhysicalDeviceProperties(device, &properties);
		std::string deviceType = ConvertDeviceTypeToString(properties.deviceType);

		auto apiVersion = ConvertPackedVersionToString(properties.apiVersion);
		auto driverVersion = ConvertPackedVersionToString(properties.driverVersion);
		//TODO: Parse deviceID to string, low 16 is a PCI device ID
		//TODO: Parse vendorId to String, low 16 is pci vendor id
		//cout << std::left << std::setw(14) << "Device Type:" << std::endl;
		cout << std::left << std::setw(25) << properties.deviceName << std::left << std::setw(15) << deviceType << std::endl;
		cout << std::left << std::setw(14) << "Device ID: " << std::left << std::setw(8) << properties.deviceID << std::left << std::setw(17) << "Vendor ID: " << std::left << std::setw(14) << properties.vendorID << std::endl;

		cout << std::left << std::setw(14) << "API Version: " << std::left << std::setw(8) << *apiVersion << std::left << std::setw(17) << "Driver Version: " << std::left << std::setw(14) << *driverVersion;
		//pipelineCacheUUID is an array of size VK_UUID_SIZE, containing 8-bit values that represent a universally unique identifier for the device
		cout << std::endl << "Unique ID: ";
		for (int i = 0; i < VK_UUID_SIZE; i++) {
			cout << std::hex << std::uppercase << static_cast<int>(properties.pipelineCacheUUID[i]) << " ";
		}
		cout << std::endl;
		//Sparse Properties
		DisplaySparseProperties(&properties.sparseProperties);
		//Limits
		DisplayLimits(&properties.limits);


		instanceDispatch->vkGetPhysicalDeviceFeatures(device, &features);
		//instanceDispatch->vkGetPhysicalDeviceFeatures2(device, &features2);
		//print available features
		DisplayFeatures(&features);


		//Display queue properties of the device
		uint32_t queueCount = 0;


		instanceDispatch->vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);
		auto queues = std::vector<VkQueueFamilyProperties>(queueCount);
		instanceDispatch->vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queues.data());
		DisplayDeviceQueues(&queues);

	}
}

void VulkanInfo::DisplayDeviceQueues(std::vector<VkQueueFamilyProperties> *queues) {
	cout << "Device Queues" << std::endl;
	for (const auto queue : *queues) {
		auto familyDesc = std::string();
		
		if ((queue.queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT) != 0) {
			if (familyDesc.empty()) {
				familyDesc.append("COMPUTE");
			}
			else {
				familyDesc.append(" | COMPUTE");
			}
			
		}
		if ((queue.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) != 0) {
			if (familyDesc.empty()) {
				familyDesc.append("GRAPHICS");
			}
			else {
				familyDesc.append(" | GRAPHICS");
			}
		}
		if (familyDesc.empty()) {
			familyDesc.append("TRANSFER");
		}
		else {
			familyDesc.append(" | TRANSFER");
		}
		if (familyDesc.empty()) {
			familyDesc.append("SPARSE");
		}
		else {
			familyDesc.append(" | SPARSE");
		}
		if (familyDesc.empty()) {
			familyDesc.append("PROTECTED");
		}
		else {
			familyDesc.append(" | PROTECTED");
		}
		cout << familyDesc << std::endl;
		cout << std::setw(35) << "Min Image Transfer Granularity: " << std::setw(35) << queue.minImageTransferGranularity.width << "x" << queue.minImageTransferGranularity.height << "x" << queue.minImageTransferGranularity.depth << std::endl;
		cout << std::setw(35) << "Queue Count: " << std::setw(35) << queue.queueCount << std::endl;
		cout << std::setw(35) << "Timestamp Valid Bits: " << std::setw(35) << queue.timestampValidBits << std::endl << std::endl;
	}

}

void VulkanInfo::DisplayFeatures(VkPhysicalDeviceFeatures *features) {
	cout << "Device Features" << std::endl;
	DisplayFeature("Alpha To One: ", features->alphaToOne);
	DisplayFeature("Depth Bias Clamp: ", features->depthBiasClamp);
	DisplayFeature("Depth Bounds: ", features->depthBounds);
	DisplayFeature("Depth Clamp: ", features->depthClamp);
	DisplayFeature("Draw Indirect First Instance: ", features->drawIndirectFirstInstance);
	DisplayFeature("Dual Source Blend: ", features->dualSrcBlend);
	DisplayFeature("Fill Mode Non Solid: ", features->fillModeNonSolid);
	DisplayFeature("Fragment Stores and Atomics: ", features->fragmentStoresAndAtomics);
	DisplayFeature("Full Draw Index: ", features->fullDrawIndexUint32);
	DisplayFeature("Geometry Shader: ", features->geometryShader);
	DisplayFeature("Image Cube Array: ", features->imageCubeArray);
	DisplayFeature("Independant Blend", features->independentBlend);
	DisplayFeature("Inherited Queries: ", features->inheritedQueries);
	DisplayFeature("Large Points: ", features->largePoints);
	DisplayFeature("Logic Op: ", features->logicOp);
	DisplayFeature("Multi Draw Indirect: ", features->multiDrawIndirect);
	DisplayFeature("Multi Viewport: ", features->multiViewport);
	DisplayFeature("Occlusion Query Precise: ", features->occlusionQueryPrecise);
	DisplayFeature("Pipeline Statistics Query: ", features->pipelineStatisticsQuery);
	DisplayFeature("Robust Buffer Access: ", features->robustBufferAccess);
	DisplayFeature("Sampler Anisotropy: ", features->samplerAnisotropy);
	DisplayFeature("Sample Rate Shading: ", features->sampleRateShading);
	DisplayFeature("Shader Clip Distance: ", features->shaderClipDistance);
	DisplayFeature("Shader Cull Distance: ", features->shaderCullDistance);
	DisplayFeature("Shader Float 64: ", features->shaderFloat64);
	DisplayFeature("Shader Image Gather Extended: ", features->shaderImageGatherExtended);
	DisplayFeature("Shader Int 16: ", features->shaderInt16);
	DisplayFeature("Shader Int 64: ", features->shaderInt64);
	DisplayFeature("Shader Resource Min LOD: ", features->shaderResourceMinLod);
	DisplayFeature("Shader Resource Residency: ", features->shaderResourceResidency);
	DisplayFeature("Shader Sampled Image Array Dynamic Indexing: ", features->shaderSampledImageArrayDynamicIndexing);
	DisplayFeature("Shader Storage Buffer Array Dynamic Indexing: ", features->shaderStorageBufferArrayDynamicIndexing);
	DisplayFeature("Shader Storage Image Array Dynamic Indexing: ", features->shaderStorageImageArrayDynamicIndexing);
	DisplayFeature("Shader Storage Image Extended Formats: ", features->shaderStorageImageExtendedFormats);
	DisplayFeature("Shader Storage Image Multisample: ", features->shaderStorageImageMultisample);
	DisplayFeature("Shader Storage Image Read Without Format: ", features->shaderStorageImageReadWithoutFormat);
	DisplayFeature("Shader Storage Image Write Without Format: ", features->shaderStorageImageWriteWithoutFormat);
	DisplayFeature("Shader Tessellation And Geometry Point Size: ", features->shaderTessellationAndGeometryPointSize);
	DisplayFeature("Shader Uniform Buffer Array Dynamic Indexing: ", features->shaderUniformBufferArrayDynamicIndexing);
	DisplayFeature("Sparse Binding: ", features->sparseBinding);
	DisplayFeature("Sparse Residency 16 Samples: ", features->sparseResidency16Samples);
	DisplayFeature("Sparse Residency 2 Samples: ", features->sparseResidency2Samples);
	DisplayFeature("Sparse Residency 4 Samples: ", features->sparseResidency4Samples);
	DisplayFeature("Sparse Residency 8 Samples: ", features->sparseResidency8Samples);
	DisplayFeature("Sparse Residency Aliased: ", features->sparseResidencyAliased);
	DisplayFeature("Sparse Residency Buffer: ", features->sparseResidencyBuffer);
	DisplayFeature("Sparse Residency Image 2D: ", features->sparseResidencyImage2D);
	DisplayFeature("Sparse Residency Image 3D: ", features->sparseResidencyImage3D);
	DisplayFeature("Tessellation Shader: ", features->tessellationShader);
	DisplayFeature("Texture Compression ASTC_LDR: ", features->textureCompressionASTC_LDR);
	DisplayFeature("Texture Compression BC: ", features->textureCompressionBC);
	DisplayFeature("Texture Compression ETC2: ", features->textureCompressionETC2);
	DisplayFeature("Variable Multisample Rate: ", features->variableMultisampleRate);
	DisplayFeature("Vertex Pipeline Stores And Atomics: ", features->vertexPipelineStoresAndAtomics);
}

void VulkanInfo::DisplayFeature(std::string featureName, VkBool32 isActive) {
	cout << featureName << std::boolalpha << static_cast<bool>(isActive) << std::endl;
}

void VulkanInfo::DisplaySparseProperties(VkPhysicalDeviceSparseProperties *sparse) {
	cout << "Sparse Properties" << std::endl;
	cout << std::setw(25) << std::left << "Aligned MIP Size:" << std::boolalpha << std::right << std::setw(5) << static_cast<bool>(sparse->residencyAlignedMipSize) << '\t' <<
		std::setw(25) << std::left << "Non-Resident Strict:" << std::boolalpha << std::right << std::setw(5) << static_cast<bool>(sparse->residencyNonResidentStrict) << '\t' <<
		std::setw(25) << std::left << "Standard 2D Block Shape:" << std::boolalpha << std::right << std::setw(5) << static_cast<bool>(sparse->residencyStandard2DBlockShape) << std::endl <<
		std::setw(25) << std::left << "Multisample Block Shape:" << std::boolalpha << std::right << std::setw(5) << static_cast<bool>(sparse->residencyStandard2DMultisampleBlockShape) << '\t' <<
		std::setw(25) << std::left << "Standard 3D Block Shape:" << std::boolalpha << std::right << std::setw(5) << static_cast<bool>(sparse->residencyStandard3DBlockShape) << std::endl << std::endl;
}

std::unique_ptr<std::string> VulkanInfo::ConvertSampleFlagToString(VkSampleCountFlags flags) {
	std::string *flagString = new std::string();
	if ((flags & VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT) != 0) {
		flagString->append("1");
	}
	if ((flags & VkSampleCountFlagBits::VK_SAMPLE_COUNT_2_BIT) != 0) {
		if (!flagString->empty()) {
			flagString->append(", 2");
		}
		else {
			flagString->append("2");
		}
	}
	if ((flags & VkSampleCountFlagBits::VK_SAMPLE_COUNT_4_BIT) != 0) {
		if (!flagString->empty()) {
			flagString->append(", 4");
		}
		else {
			flagString->append("4");
		}
	}
	if ((flags & VkSampleCountFlagBits::VK_SAMPLE_COUNT_8_BIT) != 0) {
		if (!flagString->empty()) {
			flagString->append(", 8");
		}
		else {
			flagString->append("8");
		}
	}
	if ((flags & VkSampleCountFlagBits::VK_SAMPLE_COUNT_16_BIT) != 0) {
		if (!flagString->empty()) {
			flagString->append(", 16");
		}
		else {
			flagString->append("16");
		}
	}
	if ((flags & VkSampleCountFlagBits::VK_SAMPLE_COUNT_32_BIT) != 0) {
		if (!flagString->empty()) {
			flagString->append(", 32");
		}
		else {
			flagString->append("32");
		}
	}
	if ((flags & VkSampleCountFlagBits::VK_SAMPLE_COUNT_64_BIT) != 0) {
		if (!flagString->empty()) {
			flagString->append(", 64");
		}
		else {
			flagString->append("64");
		}
	}
	flagString->append(" bits");
	return std::unique_ptr<std::string>(flagString);
}

void VulkanInfo::PrintOneColumn(std::string label, std::string data) {
	//TODO: If the last value is a 0 and contains a . then
	if (data.back() == '0' && data.find('.') != std::string::npos) {
		data.erase(data.find_last_not_of('0') + 1, std::string::npos);//Find the last character that is not a zero and remove all characters after it
		data.erase(data.find_last_not_of('.') + 1, std::string::npos);//Find the last character that is not a . and remove all characters after it
	}
	std::string finalString = label + data;
	cout << std::setw(120) << std::left << finalString << std::endl;
}

std::string VulkanInfo::CreateRangeLimitString(std::string minValue, std::string maxValue) {
	if (minValue.back() == '0' && minValue.find('.') != std::string::npos) {
		minValue.erase(minValue.find_last_not_of('0') + 1, std::string::npos);
		minValue.erase(minValue.find_last_not_of('.') + 1, std::string::npos);
	}
	if (maxValue.back() == '0' && maxValue.find('.') != std::string::npos) {
		maxValue.erase(maxValue.find_last_not_of('0') + 1, std::string::npos);
		maxValue.erase(maxValue.find_last_not_of('.') + 1, std::string::npos);
	}
	return minValue + " to " + maxValue;
}

std::string VulkanInfo::CreateXYZString(std::string xValue, std::string yValue, std::string zValue) {
	if (xValue.back() == '0' && xValue.find('.') != std::string::npos) {
		xValue.erase(xValue.find_last_not_of('0') + 1, std::string::npos);
		xValue.erase(xValue.find_last_not_of('.') + 1, std::string::npos);
	}
	if (yValue.back() == '0' && yValue.find('.') != std::string::npos) {
		yValue.erase(yValue.find_last_not_of('0') + 1, std::string::npos);
		yValue.erase(yValue.find_last_not_of('.') + 1, std::string::npos);
	}
	if (zValue.back() == '0' && zValue.find('.') != std::string::npos) {
		zValue.erase(zValue.find_last_not_of('0') + 1, std::string::npos);
		zValue.erase(zValue.find_last_not_of('.') + 1, std::string::npos);
	}
	return "X: " + xValue + ", Y: " + yValue + ", Z: " + zValue;
}

void VulkanInfo::PrintTwoColumn(std::string labelOne, std::string dataOne, std::string labelTwo, std::string dataTwo) {
	dataOne.erase(dataOne.find_last_not_of('0') + 1, std::string::npos);//Find the last character that is not a zero and remove all characters after it
	dataOne.erase(dataOne.find_last_not_of('.') + 1, std::string::npos);//Find the last character that is not a . and remove all characters after it
	dataTwo.erase(dataTwo.find_last_not_of('0') + 1, std::string::npos);
	dataTwo.erase(dataTwo.find_last_not_of('.') + 1, std::string::npos);
	std::string finalOne = labelOne + dataOne;
	std::string finalTwo = labelTwo + dataTwo;
	cout << std::setw(60) << std::left << finalOne << std::setw(60) << std::left << finalTwo << std::endl;
}

void VulkanInfo::PrintThreeColumn(std::string labelOne, std::string dataOne, std::string labelTwo, std::string dataTwo, std::string labelThree, std::string dataThree) {
	dataOne.erase(dataOne.find_last_not_of('0') + 1, std::string::npos);//Find the last character that is not a zero and remove all characters after it
	dataOne.erase(dataOne.find_last_not_of('.') + 1, std::string::npos);//Find the last character that is not a . and remove all characters after it
	dataTwo.erase(dataTwo.find_last_not_of('0') + 1, std::string::npos);
	dataTwo.erase(dataTwo.find_last_not_of('.') + 1, std::string::npos);
	dataThree.erase(dataThree.find_last_not_of('0') + 1, std::string::npos);
	dataThree.erase(dataThree.find_last_not_of('.') + 1, std::string::npos);
	std::string columnOne = labelOne + dataOne;
	std::string columnTwo = labelTwo + dataTwo;
	std::string columnThree = labelThree + dataThree;
	cout << std::setw(40) << std::left << columnOne << std::setw(40) << std::left << columnTwo << std::setw(40) << std::left << columnThree << std::endl;
}

void VulkanInfo::DisplayLimits(VkPhysicalDeviceLimits *limits) {
	cout << "Device Limits" << std::endl;
	//TODO: Correct formatting for all types
	PrintOneColumn(
		"Buffer Image Granularity: ",
		std::to_string(limits->bufferImageGranularity)
	);
	PrintOneColumn(
		"Discrete Queue Priorities: ",
		std::to_string(limits->discreteQueuePriorities)
	);
	PrintOneColumn(
		"Frame Buffer Color Samples: ",
		*ConvertSampleFlagToString(limits->framebufferColorSampleCounts)
	);
	PrintOneColumn(
		"Frame Buffer Depth Samples: ",
		*ConvertSampleFlagToString(limits->framebufferDepthSampleCounts)
	);
	PrintOneColumn(
		"Frame Buffer No Attachment Sample Count: ",
		*ConvertSampleFlagToString(limits->framebufferNoAttachmentsSampleCounts));
	PrintOneColumn(
		"Frame Buffer Stencil Sample Counts: ",
		*ConvertSampleFlagToString(limits->framebufferStencilSampleCounts));
	PrintOneColumn(
		"Line Width Granularity: ",
		std::to_string(limits->lineWidthGranularity)
	);
	PrintOneColumn("Line Width Range: ",//TODO: Fix formatting on range
		CreateRangeLimitString(std::to_string(limits->lineWidthRange[0]), std::to_string(limits->lineWidthRange[1]))
		//std::to_string(limits->lineWidthRange[0]) + " to " + std::to_string(limits->lineWidthRange[1])
	);
	PrintOneColumn(
		"Max Image Dimension 1D: ",
		std::to_string(limits->maxImageDimension1D)
	);
	PrintOneColumn(
		"Max Image Dimension 2D: ",
		std::to_string(limits->maxImageDimension2D)
	);

	PrintOneColumn(
		"Max Image Dimension 3D: ",
		std::to_string(limits->maxImageDimension3D)
	);
	PrintOneColumn(
		"Max Image Dimension Cube: ",
		std::to_string(limits->maxImageDimensionCube)
	);
	PrintOneColumn(
		"Max Image Array Layers: ",
		std::to_string(limits->maxImageArrayLayers));
	PrintOneColumn(
		"Max Texel Buffer Elements: ",
		std::to_string(limits->maxTexelBufferElements)
	);
	PrintOneColumn(
		"Max Texel Buffer Elements: ",
		std::to_string(limits->maxTexelBufferElements)
	);
	PrintOneColumn(
		"Max Uniform Buffer Range: ",
		std::to_string(limits->maxUniformBufferRange)
	);
	PrintOneColumn(
		"Max Storage Buffer Range: ",
		std::to_string(limits->maxStorageBufferRange)
	);
	PrintOneColumn(
		"Max Push Constant Size: ",
		std::to_string(limits->maxPushConstantsSize)
	);
	PrintOneColumn(
		"Max Memory Allocation Count: ",
		std::to_string(limits->maxMemoryAllocationCount)
	);
	PrintOneColumn(
		"Max Sampler Allocation Count: ",
		std::to_string(limits->maxSamplerAllocationCount)
	);
	PrintOneColumn(
		"Sparse Address Space Size: ",
		std::to_string(limits->sparseAddressSpaceSize)
	);
	PrintOneColumn(
		"Max Per Stage Descriptor Samplers: ",
		std::to_string(limits->maxPerStageDescriptorSamplers)
	);
	PrintOneColumn(
		"Max Per Stage Descriptor Uniform Buffers: ",
		std::to_string(limits->maxPerStageDescriptorUniformBuffers)
	);
	PrintOneColumn(
		"Max Per Stage Descriptor Storage Buffers: ",
		std::to_string(limits->maxPerStageDescriptorStorageBuffers)
	);
	PrintOneColumn(
		"Max Per Stage DescriptorSampled Images: ",
		std::to_string(limits->maxPerStageDescriptorSampledImages)
	);
	PrintOneColumn(
		"Max Per Stage Descriptor Storage Images: ",
		std::to_string(limits->maxPerStageDescriptorStorageImages)
	);
	PrintOneColumn(
		"Max Per Stage Descriptor Input Attachments: ",
		std::to_string(limits->maxPerStageDescriptorInputAttachments));
	PrintOneColumn(
		"Max Per Stage Resources: ",
		std::to_string(limits->maxPerStageResources)
	);
	PrintOneColumn(
		"Max Descriptor Set Samplers: ",
		std::to_string(limits->maxDescriptorSetSamplers));
	PrintOneColumn(
		"Max Descriptor Set Uniform Buffers: ",
		std::to_string(limits->maxDescriptorSetUniformBuffers)
	);
	PrintOneColumn(
		"Max Descriptor Set Uniform Buffers Dymanic: ",
		std::to_string(limits->maxDescriptorSetUniformBuffersDynamic));
	PrintOneColumn(
		"Max DescriptorSetStorageBuffers: ",
		std::to_string(limits->maxDescriptorSetStorageBuffers)
	);
	PrintOneColumn(
		"Max Descriptor Set Storage Buffer Dynamic: ",
		std::to_string(limits->maxDescriptorSetStorageBuffersDynamic)
	);
	PrintOneColumn(
		"Max Descriptor Set Sampled Images: ",
		std::to_string(limits->maxDescriptorSetSampledImages)
	);
	PrintOneColumn(
		"Max Descriptor Set Storage Images: ",
		std::to_string(limits->maxDescriptorSetStorageImages)
	);
	PrintOneColumn(
		"Max Descriptor Set Input Attachments: ",
		std::to_string(limits->maxDescriptorSetInputAttachments)
	);
	PrintOneColumn(
		"Max Bound Descriptor Sets: ",
		std::to_string(limits->maxBoundDescriptorSets)
	);
	PrintOneColumn(
		"Max Vertex Input Attributes: ",
		std::to_string(limits->maxVertexInputAttributes)
	);
	PrintOneColumn(
		"Max Vertex Input Bindings: ",
		std::to_string(limits->maxVertexInputBindings)
	);
	PrintOneColumn(
		"Max Vertex Input Attribute Offset: ",
		std::to_string(limits->maxVertexInputAttributeOffset)
	);
	PrintOneColumn(
		"Max Vertex Input Binding Stride: ",
		std::to_string(limits->maxVertexInputBindingStride)
	);
	PrintOneColumn(
		"Max Vertex Output Components: ",
		std::to_string(limits->maxVertexOutputComponents)
	);
	PrintOneColumn(
		"Max Tessellation Generation Level: ",
		std::to_string(limits->maxTessellationGenerationLevel)
	);
	PrintOneColumn(
		"Max Tessellation Patch Size: ",
		std::to_string(limits->maxTessellationPatchSize)
	);
	PrintOneColumn(
		"Max Tessellation Control Per Vertex Input Components: ",
		std::to_string(limits->maxTessellationControlPerVertexInputComponents)
	);
	PrintOneColumn(
		"Max Tessellation Control Per Vertex Output Components: ",
		std::to_string(limits->maxTessellationControlPerVertexOutputComponents)
	);
	PrintOneColumn(
		"Max Tessellation Control Per Patch Output Components: ",
		std::to_string(limits->maxTessellationControlPerPatchOutputComponents)
	);
	PrintOneColumn(
		"Max Tessellation Control Total Output Components: ",
		std::to_string(limits->maxTessellationControlTotalOutputComponents)
	);
	PrintOneColumn("Max Tessellation Evaluation Input Components: ",
		std::to_string(limits->maxTessellationEvaluationInputComponents)
	);
	PrintOneColumn(
		"Max Tessellation Evaluation Output Components: ",
		std::to_string(limits->maxTessellationEvaluationOutputComponents)
	);
	PrintOneColumn(
		"Max Geometry Shader Invocations: ",
		std::to_string(limits->maxGeometryShaderInvocations)
	);
	PrintOneColumn(
		"Max Geometry Input Components: ",
		std::to_string(limits->maxGeometryInputComponents)
	);
	PrintOneColumn(
		"Max Geometry Output Components: ",
		std::to_string(limits->maxGeometryOutputComponents)
	);
	PrintOneColumn(
		"Max Geometry Output Vertices: ",
		std::to_string(limits->maxGeometryOutputVertices)
	);
	PrintOneColumn(
		"Max Geometry Total Output Components: ",
		std::to_string(limits->maxGeometryTotalOutputComponents)
	);
	PrintOneColumn(
		"Max Fragment Input Components: ",
		std::to_string(limits->maxFragmentInputComponents)
	);
	PrintOneColumn(
		"Max Fragment Output Attachments: ",
		std::to_string(limits->maxFragmentOutputAttachments)
	);
	PrintOneColumn(
		"Max Fragment Dual Src Attachments: ",
		std::to_string(limits->maxFragmentDualSrcAttachments)
	);
	PrintOneColumn(
		"Max Fragment Combined Output Resources: ",
		std::to_string(limits->maxFragmentCombinedOutputResources)
	);
	PrintOneColumn(
		"Max Compute Shared Memory Size: ",
		std::to_string(limits->maxComputeSharedMemorySize)
	);
	PrintOneColumn(
		"Max Compute Work Group Count: ",
		CreateXYZString(
			std::to_string(limits->maxComputeWorkGroupCount[0]),
			std::to_string(limits->maxComputeWorkGroupCount[1]),
			std::to_string(limits->maxComputeWorkGroupCount[2]))
	);
	PrintOneColumn(
		"Max Compute Work Group Invocations: ",
		std::to_string(limits->maxComputeWorkGroupInvocations)
	);
	PrintOneColumn(
		"Max Compute Work Group Size: ",
		CreateXYZString(
			std::to_string(limits->maxComputeWorkGroupSize[0]),
			std::to_string(limits->maxComputeWorkGroupSize[1]),
			std::to_string(limits->maxComputeWorkGroupSize[2]))
	);
	PrintOneColumn(
		"Sub Pixel Precision Bits: ",
		std::to_string(limits->subPixelPrecisionBits)
	);
	PrintOneColumn(
		"Sub Texel Precision Bits: ",
		std::to_string(limits->subTexelPrecisionBits)
	);
	PrintOneColumn(
		"Mipmap Precision Bits: ",
		std::to_string(limits->mipmapPrecisionBits)
	);
	PrintOneColumn(
		"Max Draw Indexed Index Value: ",
		std::to_string(limits->maxDrawIndexedIndexValue)
	);
	PrintOneColumn(
		"Max Draw Indirect Count: ",
		std::to_string(limits->maxDrawIndirectCount)
	);
	PrintOneColumn(
		"Max Sampler Lod Bias: ",
		std::to_string(limits->maxSamplerLodBias)
	);
	PrintOneColumn(
		"Max Sampler Anisotropy: ",
		std::to_string(limits->maxSamplerAnisotropy)
	);
	PrintOneColumn(
		"Max Viewports: ",
		std::to_string(limits->maxViewports)
	);
	PrintOneColumn(
		"Max Viewport Dimensions: ",
		CreateRangeLimitString(
			std::to_string(limits->maxViewportDimensions[0]),
			std::to_string(limits->maxViewportDimensions[1])
		)
	);
	PrintOneColumn(
		"Viewport Bounds Range: ",
		CreateRangeLimitString(
			std::to_string(limits->viewportBoundsRange[0]),
			std::to_string(limits->viewportBoundsRange[1])
		)
	);
	PrintOneColumn(
		"Viewport Sub Pixel Bits: ",
		std::to_string(limits->viewportSubPixelBits)
	);
	PrintOneColumn(
		"Min Memory Map Alignment: ",
		std::to_string(limits->minMemoryMapAlignment)
	);
	PrintOneColumn(
		"Min Texel Buffer Offset Alignment: ",
		std::to_string(limits->minTexelBufferOffsetAlignment)
	);
	PrintOneColumn(
		"Min Uniform Buffer Offset Alignment: ",
		std::to_string(limits->minUniformBufferOffsetAlignment)
	);
	PrintOneColumn(
		"Min Storage Buffer Offset Alignment: ",
		std::to_string(limits->minStorageBufferOffsetAlignment)
	);
	PrintOneColumn(
		"Min Texel Offset: ",
		std::to_string(limits->minTexelOffset)
	);
	PrintOneColumn(
		"Max Texel Offset: ",
		std::to_string(limits->maxTexelOffset)
	);
	PrintOneColumn(
		"Min Texel Gather Offset: ",
		std::to_string(limits->minTexelGatherOffset)
	);
	PrintOneColumn(
		"Max Texel Gather Offset: ",
		std::to_string(limits->maxTexelGatherOffset)
	);
	PrintOneColumn(
		"Min Interpolation Offset: ",
		std::to_string(limits->minInterpolationOffset)
	);
	PrintOneColumn(
		"Max Interpolation Offset: ",
		std::to_string(limits->maxInterpolationOffset)
	);
	PrintOneColumn(
		"Sub Pixel Interpolation Offset Bits: ",
		std::to_string(limits->subPixelInterpolationOffsetBits)
	);
	PrintOneColumn(
		"Max Framebuffer Width: ",
		std::to_string(limits->maxFramebufferWidth)
	);
	PrintOneColumn(
		"Max Framebuffer Height: ",
		std::to_string(limits->maxFramebufferHeight)
	);
	PrintOneColumn(
		"Max Framebuffer Layers: ",
		std::to_string(limits->maxFramebufferLayers)
	);
	PrintOneColumn(
		"Max Color Attachments: ",
		std::to_string(limits->maxColorAttachments)
	);
	PrintOneColumn(
		"Sampled Image Color Sample Counts: ",
		std::to_string(limits->sampledImageColorSampleCounts)
	);//TODO: Flag
	PrintOneColumn(
		"Sampled Image Integer Sample Counts: ",
		std::to_string(limits->sampledImageIntegerSampleCounts)
	);//TODO: Flag
	PrintOneColumn(
		"Sampled Image Depth Sample Counts: ",
		std::to_string(limits->sampledImageDepthSampleCounts)
	);//TODO: Flag
	PrintOneColumn(
		"Sampled Image Stencil Sample Counts: ",
		std::to_string(limits->sampledImageStencilSampleCounts)
	);//TODO: Flag
	PrintOneColumn(
		"Storage Image Sample Counts: ",
		std::to_string(limits->storageImageSampleCounts)
	);//TODO: Flag
	PrintOneColumn(
		"Max Sample Mask Words: ",
		std::to_string(limits->maxSampleMaskWords)
	);
	PrintOneColumn(
		"Timestamp Compute And Graphics: ",
		std::to_string(limits->timestampComputeAndGraphics)
	);//TODO: bool
	PrintOneColumn(
		"Timestamp Period: ",
		std::to_string(limits->timestampPeriod)
	);
	PrintOneColumn(
		"Max Clip Distances: ",
		std::to_string(limits->maxClipDistances)
	);
	PrintOneColumn(
		"Max Cull Distances: ",
		std::to_string(limits->maxCullDistances)
	);
	PrintOneColumn(
		"Max Combined Clip And Cull Distances: ",
		std::to_string(limits->maxCombinedClipAndCullDistances)
	);
	PrintOneColumn(
		"Discrete Queue Priorities: ",
		std::to_string(limits->discreteQueuePriorities)
	);
	PrintOneColumn("PointSizeRange: ",
		CreateRangeLimitString(
			std::to_string(limits->pointSizeRange[0]),
			std::to_string(limits->pointSizeRange[1])
		)
	);
	PrintOneColumn(
		"Point Size Granularity: ",
		std::to_string(limits->pointSizeGranularity)
	);
	PrintOneColumn(
		"Strict Lines: ",
		std::to_string(limits->strictLines)//TODO: Bool
	);//TODO: bool
	PrintOneColumn(
		"Standard Sample Locations: ",
		std::to_string(limits->standardSampleLocations)//TODO: Bool
	);
	PrintOneColumn(
		"Optimal Buffer Copy Offset Alignment: ",
		std::to_string(limits->optimalBufferCopyOffsetAlignment)
	);
	PrintOneColumn(
		"Optimal Buffer Copy Row Pitch Alignment: ",
		std::to_string(limits->optimalBufferCopyRowPitchAlignment)
	);
	PrintOneColumn(
		"Non Coherent Atom Size: ",
		std::to_string(limits->nonCoherentAtomSize)
	);
}

std::unique_ptr<std::string> VulkanInfo::ConvertPackedVersionToString(uint32_t packedVersion) {
	auto major = VK_VERSION_MAJOR(packedVersion);
	auto minor = VK_VERSION_MINOR(packedVersion);
	auto patch = VK_VERSION_PATCH(packedVersion);
	//TODO: Replace with cout
	std::unique_ptr<std::string> versionString(new std::string());
	versionString->assign(std::to_string(major));
	versionString->append("." + std::to_string(minor));
	versionString->append("." + std::to_string(patch));
	return versionString;
}

std::string VulkanInfo::ConvertDeviceTypeToString(VkPhysicalDeviceType deviceType) {
	switch (deviceType) {
	case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
		return "CPU";
	case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		return "DISCRETE GPU";
	case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		return "INTEGRATED GPU";
	case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		return "VIRTUAL GPU";
	default:
		return "UNKNOWN";
	}
}

bool VulkanInfo::CreateInstance(std::string engineName, std::string applicationName, uint32_t engineVersion, uint32_t applicationVersion) {
	this->_engineName = engineName;
	this->_applicationName = applicationName;
	VkApplicationInfo appInfo;
	appInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pEngineName = _engineName.data();
	appInfo.pApplicationName = _applicationName.data();
	appInfo.applicationVersion = applicationVersion;
	appInfo.engineVersion = engineVersion;
	VkInstanceCreateInfo createInfo;
	createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = 0;
	createInfo.enabledLayerCount = 0;
	createInfo.flags = 0;
	createInfo.ppEnabledExtensionNames = nullptr;
	createInfo.ppEnabledLayerNames = nullptr;
	//TODO: Deal with incomplete result as well as errors
	globalDispatch->vkCreateInstance(&createInfo, nullptr, &_instance);
	instanceDispatch = vInstanceDispatch::Init(_instance, globalDispatch);
	if (instanceDispatch == nullptr) {
		cout << "Failed to create an instance of the Vulkan Driver" << std::endl;
		return false;
	}
	return true;
}

VulkanInfo::VulkanInfo()
{
	
	globalDispatch = nullptr;
	layerProperties = nullptr;
	instanceDispatch = nullptr;
	_instance = VK_NULL_HANDLE;
}

bool VulkanInfo::LoadVulkanLibrary() {
	globalDispatch = vGeneralDispatch::Init();
	if (globalDispatch == nullptr) {
		return false;
	}
	return true;
}

VulkanInfo::~VulkanInfo()
{
}

void VulkanInfo::DisplayLayerExtensions(const VkLayerProperties *layer) {
	uint32_t extensionCount = 0;
	VkResult result;
	do {
		result = globalDispatch->vkEnumerateInstanceExtensionProperties(layer->layerName, &extensionCount, nullptr);
	} while (result == VK_INCOMPLETE);
	if (result != VK_SUCCESS) {
		cout << "Failed to obtain the number of extensions to the layer " << layer->layerName << std::endl;
	}
	else {

		auto extensions = std::vector<VkExtensionProperties>(extensionCount);
		do {
			result = globalDispatch->vkEnumerateInstanceExtensionProperties(layer->layerName, &extensionCount, extensions.data());
		} while (result == VK_INCOMPLETE);
		if (result != VK_SUCCESS) {
			cout << "Failed to enumerate all the extensions for the layer " << layer->layerName << std::endl;
		}
		else {
			if (extensionCount > 0) {
				cout << '\t' << "Extensions: [";
				//(auto it = myVector.begin(); it != myVector.end(); ++it
				for (auto extension = extensions.begin(); extension != extensions.end(); ++extension) {
					if (std::next(extension) == extensions.end()) // last element
					{
						cout << extension->extensionName << " v" << extension->specVersion;
					}
					else {
						cout << extension->extensionName << " v" << extension->specVersion << ", ";
					}
				}
				cout << ']' << std::endl;
			}
		}
	}
}

void VulkanInfo::DisplayLayerProperties()
{
	cout << "Listing of Supported Layers" << std::endl;
	uint32_t layerCount = 0;
	VkResult result;
	do {
		result = globalDispatch->vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	} while (result == VK_INCOMPLETE);

	if (result != VK_SUCCESS)
	{
		cout << "ERROR: Failed to obtain the number of layers available" << std::endl;
		return;
	}

	auto layers = new std::vector<VkLayerProperties>(layerCount);
	do {
		result = globalDispatch->vkEnumerateInstanceLayerProperties(&layerCount, layers->data());
	} while (result == VK_INCOMPLETE);
	if (result != VK_SUCCESS) {
		cout << "ERROR: Failed to enumerate the available layers" << std::endl;
		return;
	}


	cout << std::setw(45) << std::left << "Layer Name" << std::setw(13) << std::right << "Spec Version" << std::setw(13) << std::right << "Impl Version" << std::setw(40) << std::right << "Description" << std::endl;
	for (auto const& layer : *layers) {
		uint32_t specVersion = layer.specVersion;
		auto specMajor = VK_VERSION_MAJOR(specVersion);
		auto specMinor = VK_VERSION_MINOR(specVersion);
		auto specPatch = VK_VERSION_PATCH(specVersion);
		std::string specString = std::to_string(specMajor) + "." + std::to_string(specMinor) + "." + std::to_string(specPatch);
		uint32_t implementationVersion = layer.implementationVersion;
		auto impMajor = VK_VERSION_MAJOR(implementationVersion);
		auto impMinor = VK_VERSION_MINOR(implementationVersion);
		auto impPatch = VK_VERSION_PATCH(implementationVersion);
		std::string impString = std::to_string(impMajor) + "." + std::to_string(impMinor) + "." + std::to_string(impPatch);

		cout << std::setw(45) << std::left << layer.layerName << std::setw(13) << std::right << specString << std::setw(13) << std::right << impString << std::setw(40) << std::right << layer.description << std::endl;
		DisplayLayerExtensions(&layer);
	}

	cout << std::endl;
}
