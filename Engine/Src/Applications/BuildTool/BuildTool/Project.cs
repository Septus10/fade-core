using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Fade
{
    namespace BuildTool
    {
        struct ConfigPath
        {
            public string Name { get; set; }

            public string Path { get; set; }
        }

        class Project
        {
            public string Name { get; set; }

            public string Version { get; set; }

            public List<ConfigPath> Configs { get; set; }
        }
    }
}
