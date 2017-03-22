using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Fade
{
    namespace BuildTool
    {
        class Dependency
        {
            public Dependency()
            {
                External = false;
            }

            public string Name { get; set; }
            public bool External { get; set; }
            public bool InterfaceOnly { get; set; }
            public RequiredVersion RequiredVersion { get; set; }
        }

        class ExternalDependency : Dependency
        {
            public ExternalDependency()
            {
                External = true;
            }

            public string IncludeFolder { get; set; }
            public string LibraryFolder { get; set; }
            public string LibraryName { get; set; }
            public List<string> RequiredLibraries { get; set; }
        }

        class Module
        {
            Module()
            {
                ImplementsMain = false;
                DynamicallyLoaded = false;
            }

            public string Name { get; set; }
            public string Version { get; set; }
            public List<Dependency> Dependencies { get; set; }
            public List<ExternalDependency> ExternalDependencies { get; set; }
            public List<string> Implementations { get; set; }
            public bool ImplementsMain { get; set; }
            public bool DynamicallyLoaded { get; set; }
            public string ActiveImplementation { get; set; }
            public bool InterfaceOnly { get; set; }
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
