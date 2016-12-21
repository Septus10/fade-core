#pragma once

#include <window.hpp>

struct GLFWwindow;

namespace fade
{
	
namespace rendering
{

class window_impl : public window
{
public:
	window_impl();
	~window_impl() = default;

	bool create(const char* title, i32 flags, u32 width, u32 height, i32 refresh_rate) override;
	
	void swap_buffer() override;

	void poll_events() override;
private:
	GLFWwindow* window_;
};

}

}