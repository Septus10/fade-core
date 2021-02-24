#include <ApplicationBase/Application.hpp>

namespace Fade { namespace ApplicationBase {

FADE_APPLICATIONBASE_API TUniquePtr<PlatformCore::CPlatformContext> IApplication::sm_PlatformContext = nullptr;

} }