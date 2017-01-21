#pragma once

#include <rendering/rendering_api.hpp>
#include <definitions.hpp>

namespace fade
{
	
namespace rendering
{

/**
 * Default value for certain options
 */
#define FADE_DEFAULT	0

/**
 * Fade fullscreen flag
 */
#define	FADE_FULLSCREEN	0x1

/**
 * Fade windowed flag
 */
#define	FADE_WINDOWED	0x2

/**
 * Fade borderless (fullscreen windowed) flag
 */
#define	FADE_BORDERLESS	0x4

/**
 * Enable window decorations (side bars, close widget)
 */
#define	FADE_DECORATED	0x8

/**
 * Set window as main input focus on creation
 */
#define	FADE_FOCUSED	0x10

/**
 * Enables the window to be always on top of all other windows
 */
#define	FADE_FLOATING	0x20

/**
 * Start window maximized (ignored if fullscreen)
 */
#define	FADE_MAXIMIZED	0x40

/**
 * Fade window class, used to create windows and viewports and update these
 */
class FADE_API window abstract
{
public:
	/**
	 * Virtual DTOr
	 */
	virtual ~window() = default;

	/**
	 * Creates window
	 * @param title window title
	 * @param flags window flags
	 * @param width window width
	 * @param height window height
	 * @param refresh_rate window refresh rate
	 */
	virtual bool create(const char* title, i32 flags, u32 width, u32 height, i32 refresh_rate) = 0;

	/**
	 * Swaps the current buffer and clears the screen
	 */
	virtual void swap_buffer() = 0;

	/**
	 * Polls input events
	 */
	virtual void poll_events() = 0;
};

/**
 * Default factory
 */
std::unique_ptr<window> FADE_API get_window();
}

}