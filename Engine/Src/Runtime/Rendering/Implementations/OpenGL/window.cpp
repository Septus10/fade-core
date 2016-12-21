#include <window_impl.hpp>

#define GLEW_STATIC

#include <GL/glew.h>
#include <glfw3.h>

#include <cassert>

namespace fade { namespace rendering { 

window_impl::window_impl() :
	window_(nullptr)
{

}

bool window_impl::create(const char* title, i32 flags, u32 width, u32 height, i32 refresh_rate)
{
	if (!glfwInit())
	{
		return false;
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	
	if (width == 0 || height == 0)
	{
		width = mode->width;
		height = mode->height;
	}

	glfwWindowHint(GLFW_DECORATED, flags & FADE_DECORATED);
	glfwWindowHint(GLFW_FOCUSED, flags & FADE_FOCUSED);
	glfwWindowHint(GLFW_FLOATING, flags & FADE_FOCUSED);
	glfwWindowHint(GLFW_MAXIMIZED, flags & FADE_MAXIMIZED);
	if (!(refresh_rate & FADE_DEFAULT))
	{
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}
	else
	{
		glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
	}		
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);	


	if (!(flags & FADE_FULLSCREEN))
	{
		monitor = nullptr;
	}

	if (flags & FADE_BORDERLESS)
	{
		glfwWindowHint(GLFW_FLOATING, false);
		glfwWindowHint(GLFW_DECORATED, false);
		glfwWindowHint(GLFW_AUTO_ICONIFY, false);
	}

	window_ = glfwCreateWindow(width, height, title, monitor, nullptr);

	if (window_ == nullptr)
	{
		return false;
	}

	glfwMakeContextCurrent(window_);
	glfwSwapInterval(1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	return true;
}

void window_impl::swap_buffer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window_);
}

void window_impl::poll_events()
{
	glfwPollEvents();
}

std::unique_ptr<window> rendering::get_window()
{
	return std::make_unique<window_impl>();
}

} }




