#pragma once

#ifdef FADE_EXPORT
#define FADE_PLATFORMCORE_API __declspec(dllexport)
#else
#define FADE_PLATFORMCORE_API __declspec(dllimport)
#endif