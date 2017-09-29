#include <editor.hpp>

#include <core/window/window.hpp>
#include <core/bootstrapping/bootstrapper.hpp>
#include <graphics/renderer.hpp>
#include <graphics/renderables/renderable.hpp>
#include <graphics/types/vertex.hpp>
#include <graphics/types/render_batch.hpp>
#include <resource_importer_hub/resource_importer_hub.hpp>

#include <iostream>

namespace fade {
namespace editor {

int editor_application::tick(double delta_time)
{
    if (window::should_close())
    {
        return FADE_REQUEST_STOP;
    }

    wnd_->handle_messages();

    renderer_->begin_frame();

    // the following comments will be implemented in the scene graph frame tick function
    // get the scenegraph to do occlusion and frustum culling
    // get the scenegraph to make batches according to material
    // get the scenegraph to call render_frame on the renderer with a finalized list of objects

    renderer_->render_frame(batches_);

    renderer_->end_frame();

    return FADE_CONTINUE;
}

void editor_application::fixed_tick(double fixed_delta_time)
{

}

void editor_application::pre_initialize()
{
	importer_hub_   = resources::get_resource_importer_hub();
	wnd_            = std::make_unique<window>();
	renderer_       = std::make_unique<graphics::renderer>();

}

void editor_application::initialize()
{	
    window_settings settings {};
    settings.flags = FADE_WINDOW_WINDOWED | FADE_WINDOW_FOCUSED | FADE_WINDOW_DECORATED;
    settings.width_ = 720;
    settings.height_ = 480;
    settings.pos_x_ = 0;
    settings.pos_y_ = 0;
    settings.refresh_rate = FADE_WINDOW_DEFAULT;

    if(!wnd_->create("Test", settings))
    {
        window::should_close(true);
    }

    if(!renderer_->set_context(wnd_.get()))
    {
        window::should_close(true);
    }

    if(!renderer_->initialize())
    {
        window::should_close(true);
    }

	cur_time_ = 0;
}

void editor_application::post_initialize()
{

}

void editor_application::deinitialize()
{
    batches_.clear();
}

} }

/** Needs to be defined out of the namespace */
std::unique_ptr<fade::application> fade::get_application()
{
	return std::make_unique<editor::editor_application>();
}

