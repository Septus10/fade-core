#include <graphics/renderer.hpp>
#include <graphics/window.hpp>

#include "vk_helpers.hpp"

#include <vulkan/vulkan.hpp>
#include <definitions.hpp>
#include <iostream>
#include <unordered_map>
#include <glfw3.h>
#include <map>
#include <set>

namespace fade 
{ 
    
namespace graphics 
{

//VK_DEBUG_REPORT_INFORMATION_BIT_EXT			= 0x00000001,
//VK_DEBUG_REPORT_WARNING_BIT_EXT			    = 0x00000002,
//VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT	= 0x00000004,
//VK_DEBUG_REPORT_ERROR_BIT_EXT				    = 0x00000008,
//VK_DEBUG_REPORT_DEBUG_BIT_EXT			        = 0x00000010,
static std::unordered_map<u32, const char*> debug_flag_map =
{
	{ 0x00000001, "INFO" },
	{ 0x00000002, "WARNING" },
	{ 0x00000004, "PERFORMANCE" },
	{ 0x00000008, "ERROR" },
	{ 0x00000010, "DEBUG" }
};

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData) 
{
	std::cout << "VULKAN[" << debug_flag_map[u32(flags)] << "][" << layerPrefix << "]: " << msg << std::endl;
	return VK_FALSE;
}

VK_DECLARE_EXTERNAL_CREATE_FUNCTION(DebugReportCallback)
VK_DECLARE_EXTERNAL_DELETE_FUNCTION(DebugReportCallback)

class renderer::impl
{

public:
	//========================//
	//		 FUNCTIONS		  //
	//========================//
	impl();
	~impl();

	bool vk_create_instance();
    bool vk_create_surface();
	bool vk_setup_debug_callback();
	bool vk_get_physical_device();
    bool vk_create_logical_device();
    bool vk_create_swapchain();
    bool vk_create_image_views();
    bool vk_create_graphics_pipeline();

	//========================//
	//		 VARIABLES		  //
	//========================//
	vulkan_object<VkInstance>				vk_instance_{ vkDestroyInstance };
	vulkan_object<VkDebugReportCallbackEXT> vk_debug_report_callback_{ vk_instance_, DestroyDebugReportCallbackEXT };
	
    // devices
	VkPhysicalDevice			vk_physical_device_ { nullptr };
    vulkan_object<VkDevice>     vk_device_ { vkDestroyDevice };
    
    // queues
    VkQueue     vk_graphics_queue_;
    VkQueue     vk_present_queue_;

    // window surface
    vulkan_object<VkSurfaceKHR> vk_surface_ { vk_instance_, vkDestroySurfaceKHR };

    // swapchain
    vulkan_object<VkSwapchainKHR> vk_swapchain_ { vk_device_, vkDestroySwapchainKHR };

    // swapchain information
    std::vector<VkImage>                    vk_swapchain_images_;
    VkFormat                                vk_swapchain_image_format_;
    VkExtent2D                              vk_swapchain_extent_;
    std::vector<vulkan_object<VkImageView>> vk_swapchain_image_views_;

	std::vector<const char*>	vk_validation_layers_;
	std::vector<const char*>	vk_extensions_;

    std::vector<const char*>    vk_device_extensions_;

    window*     wnd_;

private:
	bool                            vk_check_validation_layer_support();
	u32                             vk_calculate_device_score(VkPhysicalDevice device);
    bool                            vk_is_device_suitable(VkPhysicalDevice device);
    vk_queue_family_indices         vk_find_queue_families(VkPhysicalDevice device);
    bool                            vk_check_device_extension_support(VkPhysicalDevice device);
    vk_swapchain_support_details    vk_query_swapchain_support(VkPhysicalDevice device);
    VkSurfaceFormatKHR              vk_select_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
    VkPresentModeKHR                vk_select_swap_present_mode(const std::vector<VkPresentModeKHR>& available_modes);
    VkExtent2D                      vk_select_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);
};

#define FADE_APPLICATION_VERSION_MAJOR		0
#define FADE_APPLICATION_VERSION_MINOR		0
#define FADE_APPLICATION_VERSION_REVISION	1
#define FADE_ENGINE_VERSION_MAJOR			0
#define FADE_ENGINE_VERSION_MINOR			0
#define FADE_ENGINE_VERSION_REVISION		1
#define FADE_APPLICATION_NAME				"FadeEngine - Editor"

renderer::impl::impl()
{
	vk_extensions_ =
	{
#ifdef _DEBUG
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#endif
	};

	vk_validation_layers_ =
	{
#ifdef _DEBUG
		"VK_LAYER_LUNARG_standard_validation"
#endif
	};

    vk_device_extensions_ = 
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}

renderer::impl::~impl()
{
	
}

bool renderer::impl::vk_create_instance()
{
	// Application info
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = FADE_APPLICATION_NAME;
	app_info.applicationVersion = VK_MAKE_VERSION(FADE_APPLICATION_VERSION_MAJOR, FADE_APPLICATION_VERSION_MINOR, FADE_APPLICATION_VERSION_REVISION);
	app_info.pEngineName = "Fade Engine";
	app_info.engineVersion = VK_MAKE_VERSION(FADE_ENGINE_VERSION_MAJOR, FADE_ENGINE_VERSION_MINOR, FADE_ENGINE_VERSION_REVISION);
	app_info.apiVersion = VK_API_VERSION_1_0;

	// we need to know the extensions glfw requires
	u32 glfw_extension_count = 0;
	const char** glfw_extension_names;

	glfw_extension_names = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	for (u32 i = 0; i <glfw_extension_count; i++)
	{
		vk_extensions_.push_back(glfw_extension_names[i]);
	}

	// Instance creation information
	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = u32(vk_extensions_.size());
	create_info.ppEnabledExtensionNames = vk_extensions_.data();

	// check if the validation layers are supported
	if (!vk_check_validation_layer_support())
	{
		throw std::runtime_error("One or more validation layers aren't supported");
	}

	create_info.enabledLayerCount = u32(vk_validation_layers_.size());
	create_info.ppEnabledLayerNames = vk_validation_layers_.data();

	VkResult res = vkCreateInstance(&create_info, nullptr, vk_instance_.replace());

	if (res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

bool renderer::impl::vk_create_surface()
{
    GLFWwindow* glfw_window = static_cast<GLFWwindow*>(wnd_->get_window_ptr());
    VkResult res = glfwCreateWindowSurface(vk_instance_, glfw_window, nullptr, vk_surface_.replace());
    if (res != VK_SUCCESS)
    {
        return false;
    }
    return true;
}

bool renderer::impl::vk_setup_debug_callback()
{
	VkDebugReportCallbackCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	create_info.flags = 
#ifdef _DEBUG
	VK_DEBUG_REPORT_DEBUG_BIT_EXT |
#endif
	VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT
	;
	create_info.pfnCallback = vk_debug_callback;

	VkResult res = CreateDebugReportCallbackEXT(vk_instance_, &create_info, nullptr, vk_debug_report_callback_.replace());
	if (res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

bool renderer::impl::vk_get_physical_device()
{
	u32 device_count = 0;
	vkEnumeratePhysicalDevices(vk_instance_, &device_count, nullptr);
	if (device_count == 0)
	{
		return false;
	}

	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(vk_instance_, &device_count, devices.data());

	std::multimap<u32, VkPhysicalDevice> device_map;
	for (const auto& device: devices)
	{
		u32 score = vk_calculate_device_score(device);
		device_map.insert(std::make_pair(score, device));
	}

	u32 score = 0;
	for (auto it: device_map)
	{
		if (it.first > score)
		{
			score = it.first;
			vk_physical_device_ = it.second;
		}
	}

	if (vk_physical_device_ == nullptr)
	{
		return false;
	}

	return true;
}

bool renderer::impl::vk_create_logical_device()
{
    vk_queue_family_indices indices = vk_find_queue_families(vk_physical_device_);
    

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<int> unique_queue_families = {indices.graphics_family_, indices.present_family_};

    float queue_priority = 1.0f;
    for (int queue_family: unique_queue_families)
    {
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }    

    VkPhysicalDeviceFeatures device_features = {};

    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.queueCreateInfoCount = u32(queue_create_infos.size());
    create_info.pEnabledFeatures = &device_features;

    create_info.enabledExtensionCount = u32(vk_device_extensions_.size());
    create_info.ppEnabledExtensionNames = vk_device_extensions_.data();

    create_info.enabledLayerCount = u32(vk_validation_layers_.size());
    create_info.ppEnabledLayerNames = vk_validation_layers_.data();

    VkResult res = vkCreateDevice(vk_physical_device_, &create_info, nullptr, vk_device_.replace());
    if (res != VK_SUCCESS)
    {
        return false;
    }

    vkGetDeviceQueue(vk_device_, indices.graphics_family_, 0, &vk_graphics_queue_);
    vkGetDeviceQueue(vk_device_, indices.present_family_, 0, &vk_present_queue_);

    return true;
}

bool renderer::impl::vk_create_swapchain()
{
    vk_swapchain_support_details swapchain_support = vk_query_swapchain_support(vk_physical_device_);

    VkSurfaceFormatKHR surface_format = vk_select_swap_surface_format(swapchain_support.formats_);
    VkPresentModeKHR present_mode = vk_select_swap_present_mode(swapchain_support.present_modes_);
    VkExtent2D extent = vk_select_swap_extent(swapchain_support.capabilities_);

    u32 image_count = swapchain_support.capabilities_.minImageCount + 1;
    if (swapchain_support.capabilities_.maxImageCount > 0 && image_count > swapchain_support.capabilities_.maxImageCount)
    {
        image_count = swapchain_support.capabilities_.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = vk_surface_;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // for things like post processing use VK_IMAGE_USAGE_TRANSFER_DST_BIT

    vk_queue_family_indices indices = vk_find_queue_families(vk_physical_device_);
    u32 queue_family_indices[] = {u32(indices.graphics_family_), u32(indices.present_family_)};

    if (indices.graphics_family_ != indices.present_family_)
    {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0; // Optional
        create_info.pQueueFamilyIndices = nullptr; // Optional
    }

    create_info.preTransform = swapchain_support.capabilities_.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = true;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    VkResult res = vkCreateSwapchainKHR(vk_device_, &create_info, nullptr, vk_swapchain_.replace());
    if (res != VK_SUCCESS)
    {
        return false;
    }

    vkGetSwapchainImagesKHR(vk_device_, vk_swapchain_, &image_count, nullptr);
    vk_swapchain_images_.resize(image_count);
    vkGetSwapchainImagesKHR(vk_device_, vk_swapchain_, &image_count, vk_swapchain_images_.data());

    vk_swapchain_image_format_ = surface_format.format;
    vk_swapchain_extent_ = extent;

    return true;

}

bool renderer::impl::vk_create_image_views()
{
    vk_swapchain_image_views_.resize(vk_swapchain_images_.size(), vulkan_object<VkImageView>{vk_device_, vkDestroyImageView});

    for (u32 i = 0; i < vk_swapchain_images_.size(); i++)
    {
        VkImageViewCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = vk_swapchain_images_[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = vk_swapchain_image_format_;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        VkResult res = vkCreateImageView(vk_device_, &create_info, nullptr, vk_swapchain_image_views_[i].replace());
        if (res != VK_SUCCESS)
        {
            return false;
        }
    }

    return true;
}

bool renderer::impl::vk_create_graphics_pipeline()
{
    return true;
}

bool renderer::impl::vk_check_validation_layer_support()
{
	// get layer count
	u32 layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
	// setup vector with layer count
	std::vector<VkLayerProperties> available_layers(layer_count);
	// get data
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

	for (std::string layer : vk_validation_layers_)
	{
		bool found = false;

		for (const auto& properties : available_layers)
		{
			if (layer.compare(properties.layerName) == 0)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			return false;
		}
	}

	return true;
}

u32 renderer::impl::vk_calculate_device_score(VkPhysicalDevice device)
{
	// physical device information structures
	VkPhysicalDeviceProperties device_properties;

	// get the information for said structures
	vkGetPhysicalDeviceProperties(device, &device_properties);


	u32 score = 0;
	// Discrete GPU's are the best, we'll just add 10000 to the score
	if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 10000;
	}

	score += device_properties.limits.maxImageDimension2D;
	score += device_properties.limits.maxViewports;

	return score;
}

bool renderer::impl::vk_is_device_suitable(VkPhysicalDevice device)
{
    // device features
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    if (!device_features.multiViewport)
    {
        return false;
    }

    // queue families
    vk_queue_family_indices indices = vk_find_queue_families(device);

    bool extensions_supported = vk_check_device_extension_support(device);
    bool swapchain_adequate = false;
    if (extensions_supported)
    {
        vk_swapchain_support_details swapchain_support = vk_query_swapchain_support(device);
        swapchain_adequate = !swapchain_support.formats_.empty() && !swapchain_support.present_modes_.empty();
    }

    return indices.is_complete() && extensions_supported && swapchain_adequate;
}

vk_queue_family_indices renderer::impl::vk_find_queue_families(VkPhysicalDevice device)
{
    vk_queue_family_indices index;
    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    for (int i = 0; i < queue_families.size(); i++)
    {
        if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            index.graphics_family_ = i;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vk_surface_, &present_support);

        if (queue_families[i].queueCount > 0 && present_support)
        {
            index.present_family_ = i;
        }

        if (index.is_complete())
        {
            break;
        }
    }

    return index;
}

bool renderer::impl::vk_check_device_extension_support(VkPhysicalDevice device)
{
    u32 extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device ,nullptr, &extension_count, available_extensions.data());

    std::set<std::string> required_extensions(vk_device_extensions_.begin(), vk_device_extensions_.end());
    
    for (const auto& extension: available_extensions)
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

vk_swapchain_support_details renderer::impl::vk_query_swapchain_support(VkPhysicalDevice device)
{
    vk_swapchain_support_details details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vk_surface_, &details.capabilities_);

    u32 format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface_, &format_count, nullptr);

    if (format_count != 0)
    {
        details.formats_.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface_, &format_count, details.formats_.data());
    }

    u32 present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface_, &present_mode_count, nullptr);

    if (present_mode_count != 0)
    {
        details.present_modes_.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface_, &present_mode_count, details.present_modes_.data());
    }

    return details;
}

VkSurfaceFormatKHR renderer::impl::vk_select_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats)
{
    if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED)
    {
        return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& format : available_formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR renderer::impl::vk_select_swap_present_mode(const std::vector<VkPresentModeKHR>& available_modes)
{
    VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto& mode: available_modes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return mode;
        }
        else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            best_mode = mode;
        }
    }

    return best_mode;
}

VkExtent2D renderer::impl::vk_select_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        VkExtent2D actual_extent = { wnd_->get_width(), wnd_->get_height() };

        actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
        actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actual_extent.height));

        return actual_extent;
    }
}

renderer::renderer(window* wnd) :
    FADE_INIT_PIMPL(renderer)
{
    impl_->wnd_ = wnd;
}

renderer::~renderer()
{
}

bool renderer::initialize()
{
	if (!impl_->vk_create_instance())
	{
		return false;
	}

    if (!impl_->vk_create_surface())
    {
        return false;
    }

	if (!impl_->vk_setup_debug_callback())
	{
		return false;
	}

	if (!impl_->vk_get_physical_device())
	{
		return false;
	}

    if (!impl_->vk_create_logical_device())
    {
        return false;
    }

    if (!impl_->vk_create_swapchain())
    {
        return false;
    }

    //if (!impl_->vk_create_image_views())
    //{
    //    return false;
    //}
    //
    //if (!impl_->vk_create_graphics_pipeline())
    //{
    //    return false;
    //}

	return true;
}

void* renderer::get_device_ptr()
{
    return static_cast<void*>(impl_->vk_device_);
}

} 

}

/**
============================
Requirements
============================
- scene graph
Using the scene graph we can get a list of objects we need to render
- data gathering (implemented in the scene graph)
With data gathering algorithms we can calculate which objects we need to render
(BSPs, Frustum Culling)
- data ordering
Order the data in a way that makes the GPU happy
(Visible geometry should be rendered front to back, transparent geometry should be rendered back to front)
- resource manager
Loads/Saves, handles and tracks our resources
**/