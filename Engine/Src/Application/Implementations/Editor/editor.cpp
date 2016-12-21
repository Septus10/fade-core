#include <editor.hpp>
#include <window.hpp>
#include <window_impl.hpp>
#include <iostream>

namespace fade { namespace editor {

void editor_application::tick(double delta_time)
{
	wnd_->poll_events();
	wnd_->swap_buffer();	
}

void editor_application::fixed_tick(double fixed_delta_time)
{
	
}

void editor_application::pre_initialize()
{

}

void editor_application::initialize()
{
	std::cout << "editor::initialize()\n";
	wnd_ = rendering::get_window();
	wnd_->create("Test", FADE_WINDOWED | FADE_FOCUSED, 720, 480, FADE_DEFAULT);
}

void editor_application::post_initialize()
{

}

void editor_application::deinitialize()
{

}

} }

/** Needs to be defined out of the namespace */
std::unique_ptr<fade::application> fade::get_application()
{
	return std::make_unique<editor::editor_application>();
}

