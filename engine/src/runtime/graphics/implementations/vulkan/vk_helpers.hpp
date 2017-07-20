#pragma once

#include <vulkan/vulkan.hpp>
#include <functional>

//==============================================//
//					MACROS						//
//==============================================//
#define VK_DECLARE_EXTERNAL_CREATE_FUNCTION(fn_name)																																	\
VkResult Create##fn_name##EXT(VkInstance instance, const Vk##fn_name##CreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)		\
{																																													\
	auto func = PFN_vkCreate##fn_name##EXT(vkGetInstanceProcAddr(instance, "vkCreate" #fn_name "EXT"));																				\
	if (func != nullptr) {																																							\
		return func(instance, pCreateInfo, pAllocator, pCallback);																													\
	}																																												\
	else {																																											\
		return VK_ERROR_EXTENSION_NOT_PRESENT;																																		\
	}																																												\
}

#define VK_DECLARE_EXTERNAL_DELETE_FUNCTION(fn_name)																	\
void Destroy##fn_name##EXT(VkInstance instance, Vk##fn_name##EXT callback, const VkAllocationCallbacks* pAllocator)		\
{																														\
	auto func = PFN_vkDestroy##fn_name##EXT(vkGetInstanceProcAddr(instance, "vkDestroy" #fn_name "EXT"));				\
	if (func != nullptr) {																								\
		func(instance, callback, pAllocator);																			\
	}																													\
}

//==============================================//
//				VULKAN OBJECT					//
//==============================================//
/**
 * Wrapper for vulkan objects, handles destruction and creation of said objects
 */
template <typename T>
class vulkan_object
{
public:
	//=========================================//
	//				  C & D-Tors    		   //
	//=========================================//
	/**
	 * Default constructor
	 */
	vulkan_object() :
		vulkan_object([](T, VkAllocationCallbacks*) {})
	{
		object_ = VK_NULL_HANDLE;
	}

	/**
	 * Constructor with delete callback
	 */
	vulkan_object(std::function<void(T, VkAllocationCallbacks*)> delete_cb)
	{
		delete_cb_ = [=](T obj) { delete_cb(obj, nullptr); };
	}

	/**
	 * Constructor with instance vulkan object reference and delete callback
	 */
	vulkan_object(const vulkan_object<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> delete_cb)
	{
		delete_cb_ = [&instance, delete_cb](T obj) { delete_cb(instance, obj, nullptr); };
	}

	/**
	 * Constructor onstructor with device vulkan object reference and delete callback
	 */
	vulkan_object(const vulkan_object<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> delete_cb)
	{
		delete_cb_ = [&device, delete_cb](T obj) { delete_cb(device, obj, nullptr); };
	}

	/**
	 * Default destructor
	 */
	~vulkan_object()
	{
		cleanup();
	}

	//=========================================//
	//			      OPERATORS	    		   //
	//=========================================//
	/**
	 * Reference operator
	 */
	const T* operator&() const
	{
		return &object_;
	}

	/**
	 * Function call operator
	 */
	operator T() const
	{
		return object_;
	}

	/**
	 * Assignment operator
	 */
	void operator=(T rhs)
	{
		if (rhs != object_)
		{
			cleanup();
			object_ = rhs;
		}
	}

	/**
	 * Equals operator
	 */
	template <typename U>
	bool operator==(U rhs)
	{
		return object_ == T(rhs);
	}
	
	//=========================================//
	//			      MISC FNS	    		   //
	//=========================================//
	/**
	 * Cleans the current object
	 * @returns a pointer to the cleaned (empty) object
	 */
	T* replace()
	{
		cleanup();
		return &object_;
	}

private:
	//=========================================//
	//			  PRIVATE VARIABLES	    	   //
	//=========================================//
	T						object_;
	std::function<void(T)>	delete_cb_;

	//=========================================//
	//			  PRIVATE FUNCTIONS	    	   //
	//=========================================//
	void cleanup()
	{
		if (object_ != VK_NULL_HANDLE)
		{
			delete_cb_(object_);
		}
		object_ = VK_NULL_HANDLE;
	}

};
//==============================================//

//==============================================//
//             QUEUE FAMILY INDEX               //
//==============================================//
struct vk_queue_family_indices
{
    int graphics_family_ = -1;
    int present_family_ = -1;

    bool is_complete() const
    {
        return graphics_family_ >= 0 && present_family_ >= 0;
    }
};

//==============================================//
//             SWAPCHAIN SUPPORT                //
//==============================================//
struct vk_swapchain_support_details
{
    VkSurfaceCapabilitiesKHR        capabilities_;
    std::vector<VkSurfaceFormatKHR> formats_;
    std::vector<VkPresentModeKHR>   present_modes_;
};