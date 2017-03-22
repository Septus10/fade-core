#include <editor.hpp>

#include <rendering/window.hpp>
#include <core/bootstrapping/bootstrapper.hpp>

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
	importer_hub_ = resources::get_resource_importer_hub();
	wnd_ = rendering::get_window();
}

void editor_application::initialize()
{	
	wnd_->create("Test", FADE_WINDOW_WINDOWED | FADE_WINDOW_FOCUSED | FADE_WINDOW_DECORATED, 720, 480, FADE_WINDOW_DEFAULT);
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

