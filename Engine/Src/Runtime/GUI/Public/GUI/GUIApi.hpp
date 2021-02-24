#pragma once

#ifdef FADE_GUI_EXPORT
#define FADE_GUI_API __declspec(dllexport)
#else
#define FADE_GUI_API __declspec(dllimport)
#endif