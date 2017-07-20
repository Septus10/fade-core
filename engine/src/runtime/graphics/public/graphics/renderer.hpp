#pragma once

#include <graphics/graphics_api.hpp>

#include <core/definitions.hpp>
#include <core/pimpl.hpp>

#include <vector>

namespace fade {
class window; 

namespace graphics {

class FADE_API renderer
{
public:
    renderer();
	~renderer();

	/**
	 * Renderer initialization
	 * @returns true or false, depending on whether initialization has succeeded
	 */
	bool initialize();

    /**
     * Set window context
     */
    bool set_context(window* wnd);

    /**
     * Initializes the current frame
     */
    void begin_frame();

    /**
     * Renders a list of renderables onto the frame
     * @param render_batches reference to the render batch object created and maintained in the scene graph
     */
    void render_frame(const std::vector<struct render_batch>& render_batches);

    /**
     * Ends the frame, draws it on the screen and swaps the buffer
     */
    void end_frame();

    /**
     * Handles resizing of the window
     * @param width new width
     * @param height new height
     */
    void resize(u32 width, u32 height);

private:
	FADE_MAKE_PIMPL

};

} }
