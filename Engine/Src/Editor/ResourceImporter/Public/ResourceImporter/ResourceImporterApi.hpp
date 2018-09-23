#pragma once

#ifdef FADE_EXPORT
#define FADE_RESOURCEIMPORTER_API __declspec(dllexport)
#else
#define FADE_RESOURCEIMPORTER_API __declspec(dllimport)
#endif