#pragma once

#ifdef FADE_EXPORT
#define FADE_APPLICATION_API __declspec(dllexport)
#else
#define FADE_APPLICATION_API __declspec(dllimport)
#endif