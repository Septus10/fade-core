#include <rendering/renderer.hpp>

namespace fade { namespace rendering {



} }

/**
============================
Requirements
============================
- scene graph
Using the scene graph we can get a list of objects we need to render
- data gathering
With data gathering algorithms we can calculate which objects we need to render
(BSPs, Frustum Culling)
- data ordering
Order the data in a way that makes the GPU happy
(Visible geometry should be rendered front to back, transparent geometry should be rendered back to front)
- resource manager
Loads/Saves, handles and tracks our resources
**/