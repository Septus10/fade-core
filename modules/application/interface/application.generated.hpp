#define DECLARE_MODULE

#if FADE_PLATFORM_WIN32
    #if FADE_APPLICATION_EXPORT
    #define FADE_APPLICATION_API
    #else
    #define FADE_APPLICATION_API
    #endif
#elif FADE_PLATFORM_UNIX
    #define FADE_APPLICATION_API __attribute__((visibility("default")))
#endif