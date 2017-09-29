#include <graphics/renderer.hpp>

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/gl.h>

#undef APIENTRY

#include <graphics/renderables/renderable.hpp>
#include <graphics/types/render_batch.hpp>
#include <graphics/pipeline/shader_program.hpp>

#include <core/window/window.hpp>
#include <vector>

#ifdef FADE_PLATFORM_WINDOWS
#include <windows.h>
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif

namespace fade { namespace graphics {

class renderer::impl
{
public:
    ~impl()
    {
#ifdef FADE_PLATFORM_WINDOWS
        wglMakeCurrent(device_context, NULL);   // release device context
        wglDeleteContext(rendering_context);    // delete rendering context
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif
    }

#ifdef FADE_PLATFORM_WINDOWS
    // handle to opengl rendering context
    HGLRC   rendering_context;

    // device context handle
    HDC     device_context;
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif
    
};

renderer::renderer() :
    FADE_INIT_PIMPL(renderer)
{
}

renderer::~renderer()
{
}

void renderer::resize(u32 width, u32 height)
{
    if (height == 0)
    {
        height = 1;
    }

    glViewport(0, 0, width, height);

    //glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    //glLoadIdentity();									// Reset The Projection Matrix
    //
    //glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    //glLoadIdentity();									// Reset The Modelview Matrix
}

bool renderer::initialize()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        return false;
    }

    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    return true;
}

bool renderer::set_context(window* wnd)
{
    // Get platform specific window handle
#ifdef FADE_PLATFORM_WINDOWS
    HWND window_handle = wnd->get_window_handle();

    // create pixel format descriptor
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    if (!(impl_->device_context = GetDC(window_handle)))
    {
        return false;
    }

    int pixel_format;
    if (!(pixel_format = ChoosePixelFormat(impl_->device_context, &pfd)))
    {
        return false;
    }

    // set pixel format
    if (!SetPixelFormat(impl_->device_context, pixel_format, &pfd))
    {
        return false;
    }

    // create context
    if (!(impl_->rendering_context = wglCreateContext(impl_->device_context)))
    {
        return false;
    }

    // make it current
    if (!wglMakeCurrent(impl_->device_context, impl_->rendering_context))
    {
        return false;
    }

    window_settings settings = wnd->get_window_settings();

    wnd->show();
    wnd->focus();
    resize(settings.width_, settings.height_);
#elif FADE_PLATFORM_LINUX
#elif FADE_PLATFORM_MAC
#endif

    return true;
}

void renderer::begin_frame()
{
    //=============
    // clear buffer
    //=============
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void renderer::end_frame()
{
    //=============
    // swap buffer
    //=============
    SwapBuffers(impl_->device_context);
}


//////
    ///     Renderables should be derived from a global object class, which contains a transform.
//////

void renderer::render_frame(const std::vector<render_batch>& render_batches)
{
    //=============
    // do rendering
    //=============
    for (auto batch: render_batches)
    {
        // get the material used by this batch
        material* material = batch.material_;
        pipeline::shader_program* program = material->get_shader_program();

        // initialize shader for this batch
        program->use();
        program->setup_uniforms(material);

        // loop through all renderables and draw them
        for (auto& renderable: batch.renderables_)
        {
            renderable->draw();
        }

        program->stop();
    }   
}

} }