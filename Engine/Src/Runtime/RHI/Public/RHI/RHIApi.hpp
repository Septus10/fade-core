#pragma once

#ifdef FADE_RHI_EXPORT
#define FADE_RHI_API __declspec(dllexport)
#else
#define FADE_RHI_API __declspec(dllimport)
#endif