using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Fade
{
    namespace BuildTool
    {
        struct Dependency
        {
            public string Name { get; set; }
            public RequiredVersion RequiredVersion { get; set; }
        }

        class Module
        {
            Module()
            {
                ImplementsMain = false;
            }

            public string Name { get; set; }
            public string Version { get; set; }
            public List<Dependency> Dependencies { get; set; }
            public List<string> Implementations { get; set; }
            public bool ImplementsMain { get; set; }
            public string ActiveImplementation { get; set; }
            public Guid GUID { get; set; }
            public string Path { get; set; }
        }

        class Implementation
        {
            public string Name { get; set; }
            public string Version { get; set; }
            public List<Dependency> Dependencies { get; set; }
        }
    }
}
