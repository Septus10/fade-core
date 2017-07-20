#pragma once

#include <core/core_api.hpp>
#include <core/definitions.hpp>
#include <core/pimpl.hpp>

#ifdef FADE_PLATFORM_WINDOWS
#include <windows.h>
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif

namespace fade
{
	
/**
 * Default value for certain options
 */
#define FADE_WINDOW_DEFAULT	0

/**
 * Fade fullscreen flag
 */
#define	FADE_WINDOW_FULLSCREEN	0x1

/**
 * Fade windowed flag
 */
#define	FADE_WINDOW_WINDOWED	0x2

/**
 * Fade borderless (fullscreen windowed) flag
 */
#define	FADE_WINDOW_BORDERLESS	0x4

/**
 * Enable window decorations (side bars, close widget)
 */
#define	FADE_WINDOW_DECORATED	0x8

/**
 * Set window as main input focus on creation
 */
#define	FADE_WINDOW_FOCUSED	0x10

/**
 * Enables the window to be always on top of all other windows
 */
#define	FADE_WINDOW_FLOATING	0x20

/**
 * Start window maximized (ignored if fullscreen)
 */
#define	FADE_WINDOW_MAXIMIZED	0x40

struct FADE_API window_settings
{
    // window rect settings
    u32 pos_x_;
    u32 pos_y_;
    u32 width_;
    u32 height_;

    // window specific settings
    u32 flags;
    u32 refresh_rate;
};

/**
 * Fade window class, used to create windows and viewports and update these
 */
class FADE_API window
{
public:
    /**
     * CTor
     */
    window();

	/**
	 * DTor
	 */
	~window();

    /**
    * Creates window
    * @param title window title
    * @param flags window flags
    * @param width window width
    * @param height window height
    * @param refresh_rate window refresh rate
    */
    bool create(const char* title, window_settings& settings);

    /**
     * Shows the window
     */
    void show();

    /**
     * Focuses the window
     */
    void focus();

    /**
     * Handles window messages
     */
    void handle_messages();

    void handle_resize(i32 new_width, i32 new_height);

    /**
     * Check if the window was requested to close
     * @returns true if exit was clicked, false otherwise
     */
    static bool should_close();

    window_settings get_window_settings() const;

#ifdef FADE_PLATFORM_WINDOWS
    HWND get_window_handle() const;
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif
       

    static void should_close(bool should_close);
private:
    FADE_MAKE_PIMPL
};

}
