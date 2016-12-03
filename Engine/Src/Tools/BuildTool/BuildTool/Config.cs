using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Fade
{
    namespace BuildTool
    {
        struct ModuleInfo
        {
            public string Name { get; set; }
            public string Type { get; set; }
            public string Implementation { get; set; }
        }

        class Config
        {
            public string Application { get; set; }
            public List<ModuleInfo> Modules { get; set; }
        }
    }
}
