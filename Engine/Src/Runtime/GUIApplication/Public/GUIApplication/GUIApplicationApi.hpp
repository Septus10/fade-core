#pragma once

#ifdef FADE_GUIAPPLICATION_EXPORT
#define FADE_GUIAPPLICATION_API __declspec(dllexport)
#else
#define FADE_GUIAPPLICATION_API __declspec(dllimport)
#endif