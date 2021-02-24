#pragma once

#ifdef FADE_APPLICATIONBASE_EXPORT
#define FADE_APPLICATIONBASE_API __declspec(dllexport)
#else
#define FADE_APPLICATIONBASE_API __declspec(dllimport)
#endif