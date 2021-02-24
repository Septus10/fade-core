#pragma once

#include <Application/ApplicationApi.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <ApplicationBase/Application.hpp> 

namespace Fade
{

/**
 * Application factory function
 */
FADE_APPLICATION_API Fade::TUniquePtr<IApplication> GetApplication();

}
