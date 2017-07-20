#include <core/window/window.hpp>


#ifdef FADE_PLATFORM_WINDOWS
#include <windows.h>
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif

namespace fade 
{
static bool bShouldClose = false;
//==========================================================================//
// WINDOW IMPL CLASS                                                        //
//==========================================================================//
class window::impl
{
public:
#ifdef FADE_PLATFORM_WINDOWS
    static LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool                    create_window_windows(const char* title, window_settings& settings);
    void                    handle_messages_windows();
    
    HWND            window_handle_;    
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif

    window_settings window_settings_;

};
//==========================================================================//

//==========================================================================//
// WINDOW IMPL FUNCTIONS                                                    //
//==========================================================================//
#ifdef FADE_PLATFORM_WINDOWS
                                        //==================================//
                                        // WINDOWS                          //
                                        //==================================//
const TCHAR window_class[] = TEXT("FadeEngineWindowClass");

LRESULT window::impl::window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            bShouldClose = true;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}
//==========================================================================//
bool window::impl::create_window_windows(const char* title, window_settings& settings)
{
    WNDCLASSEX  wc;
    HINSTANCE   instance = GetModuleHandleW(nullptr);
    DWORD		dwExStyle;				        // Window Extended Style
    DWORD		dwStyle;				        // Window Style
    RECT		WindowRect;				        // Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left = long(0);			        // Set Left Value To 0
    WindowRect.right = long(settings.width_);	// Set Right Value To Requested Width
    WindowRect.top = long(0);				    // Set Top Value To 0
    WindowRect.bottom = long(settings.height_);	// Set Bottom Value To Requested Height
    
    // Register window class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = impl::window_procedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = window_class;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, "Window Class Registration Failed!", "Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    if (settings.flags & FADE_WINDOW_FULLSCREEN)
    {
        DEVMODE dmScreenSettings;								// Device Mode
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth = settings.width_;			// Selected Screen Width
        dmScreenSettings.dmPelsHeight = settings.height_;		// Selected Screen Height
        dmScreenSettings.dmBitsPerPel = 32;					    // Selected Bits Per Pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // Pop Up A Message Box Letting User Know The Program Is Closing.
            MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
            return FALSE;									// Return FALSE
        }
    }

    if (settings.flags & FADE_WINDOW_FULLSCREEN)				// Are We Still In Fullscreen Mode?
    {
        dwExStyle = WS_EX_APPWINDOW;							// Window Extended Style
        dwStyle = WS_POPUP;										// Windows Style
        ShowCursor(FALSE);										// Hide Mouse Pointer
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

                                                                    // Create The Window
    if (!(window_handle_ = CreateWindowEx(dwExStyle,				// Extended Style For The Window
                                window_class,						// Class Name
                                title,								// Window Title
                                dwStyle |							// Defined Window Style
                                WS_CLIPSIBLINGS |					// Required Window Style
                                WS_CLIPCHILDREN,					// Required Window Style
                                0, 0,								// Window Position
                                WindowRect.right - WindowRect.left,	// Calculate Window Width
                                WindowRect.bottom - WindowRect.top,	// Calculate Window Height
                                nullptr,							// No Parent Window
                                nullptr,							// No Menu
                                instance,							// Instance
                                nullptr)))							// Dont Pass Anything To WM_CREATE
    {
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    return true;
}
//==========================================================================//
void window::impl::handle_messages_windows()
{
    MSG msg;
    if (GetMessage(&msg, NULL, NULL, NULL) == WM_QUIT)
    {
        bShouldClose = true;
        return;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
#endif
//==========================================================================//
// WINDOW CLASS FUNCTIONS                                                   //
//==========================================================================//
window::window() :
    FADE_INIT_PIMPL(window)
{
    bShouldClose = false;
}
//==========================================================================//
window::~window()
{

}
//==========================================================================//
bool window::create(const char* title, window_settings& settings)
{
    impl_->window_settings_ = settings;
#ifdef FADE_PLATFORM_WINDOWS
    return impl_->create_window_windows(title, settings);
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif
}
//==========================================================================//
void window::show()
{
#ifdef FADE_PLATFORM_WINDOWS
    ShowWindow(impl_->window_handle_, SW_SHOWDEFAULT);
    UpdateWindow(impl_->window_handle_);
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif    
}
//==========================================================================//
void window::focus()
{
#ifdef FADE_PLATFORM_WINDOWS
    SetForegroundWindow(impl_->window_handle_);
    SetFocus(impl_->window_handle_);
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif    
}
//==========================================================================//
void window::handle_messages()
{
#ifdef FADE_PLATFORM_WINDOWS
    impl_->handle_messages_windows();
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif
   
}
//==========================================================================//
void window::handle_resize(i32 new_width, i32 new_height)
{
}
//==========================================================================//
bool window::should_close()
{
    return bShouldClose;
}
//==========================================================================//
window_settings window::get_window_settings() const
{
    return impl_->window_settings_;
}
//==========================================================================//
#ifdef FADE_PLATFORM_WINDOWS
HWND window::get_window_handle() const
{
    return impl_->window_handle_;
}
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif
//==========================================================================//
void window::should_close(bool should_close)
{
    bShouldClose = should_close;
}
//==========================================================================//

} 




