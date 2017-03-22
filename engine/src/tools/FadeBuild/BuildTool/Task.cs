using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Collections.Generic;
using Newtonsoft.Json;
using RazorEngine;
using RazorEngine.Templating;

namespace Fade
{
    namespace BuildTool
    {
        enum ETask
        {
            Null,
            Generate,
            Build
        }

        struct SourceFile
        {
            public string FileName { get; set; }
            public string Directory { get; set; }
        }

        class BuildTask
        {
            public BuildTask(string filePath, ETask task, string config, string path)
            {
                ProjectFilePath = filePath;
                Task = task;
                Config = config;
                Proj = null;
                ProjectPath = path;
            }

            private string FileToString(string Path)
            {
                if (!File.Exists(Path))
                {
                    LastError = $"File not found ({Path})";
                    return "";
                }

                StreamReader file = new StreamReader(Path);
                string contents = file.ReadToEnd();
                return contents;
            }

            public bool ExecuteProject()
            {
                //================================================================
                // Get project file contents
                //================================================================
                string json = FileToString(ProjectFilePath);
                if (json == "")
                {
                    return false;
                }

                //================================================================
                // Deserialize project
                //================================================================
                Proj = JsonConvert.DeserializeObject<Project>(json);
                if (Proj == null)
                {
                    LastError = "Unable to deserialize project file";
                    return false;
                }

                ProjectFilePath = ProjectFilePath.Remove(ProjectFilePath.LastIndexOf('\\') + 1);
                return true;
            }

            public bool ExecuteConfig()
            {
                //================================================================
                // Get config file contents
                //================================================================
                string configPath = $"{ProjectFilePath}config\\{Proj.Configuration}.fcfg";
                string json = FileToString(configPath);
                if (json == "")
                {
                    return false;
                }

                //================================================================
                // Deserialize config
                //================================================================
                Cfg = JsonConvert.DeserializeObject<Config>(json);
                if (Cfg == null || Cfg.Modules == null)
                {
                    LastError = "Unable to deserialize config file";
                    return false;
                }

                return true;
            }

            private bool parseVersion(string version, out int[] ints)
            {
                ints = new int[3];

                string[] versions = version.Split('.');
                if (!Int32.TryParse(versions[0], out ints[0]) || !Int32.TryParse(versions[1], out ints[1]) || !Int32.TryParse(versions[2], out ints[2]))
                {
                    LastError = "Error parsing version";
                    return false;
                }
                return true;
            }

            private bool deserializeModule(string path, string name, string implementation)
            {
                //================================================================
                // Get module configuration
                //================================================================
                string moduleFile = $"{path}{name}.fmodule";
                string moduleJson = FileToString(moduleFile);
                Module module = JsonConvert.DeserializeObject<Module>(moduleJson);
                if (module == null || module.Name == null)
                {
                    LastError = $"Unable to deserialize module file: {moduleFile}";
                    return false;
                }

                //================================================================
                // Set seperate variables
                //================================================================
                module.GUID = Guid.NewGuid();
                module.Path = moduleFile.Substring(0, moduleFile.LastIndexOf('\\') + 1);
                module.ActiveImplementation = implementation;

                //================================================================
                // Check if module has already been added
                //================================================================
                bool exists = false;
                if ((exists = ModuleMap.ContainsKey(module.Name)))
                {
                    if (ModuleMap[module.Name].ActiveImplementation == module.ActiveImplementation)
                    {
                        LastError = $"Module {module.Name} with implementation {module.ActiveImplementation} already exists\n\tPath: {moduleFile}";
                        return false;
                    }                    
                }

                //================================================================
                // Check if the implementation we've chosen exists
                //================================================================
                bool implementationFound = false;
                foreach (string impl in module.Implementations)
                {
                    if (impl == module.ActiveImplementation)
                    {
                        implementationFound = true;
                        break;
                    }
                }

                if (!implementationFound)
                {
                    LastError = $"Couldn't find implementation {module.ActiveImplementation} in Module {module.Name}";
                    return false;
                }

                //================================================================
                // Check if there is are implementation specific dependencies
                //================================================================
                string implementationFile = $"{path}\\implementations\\{module.ActiveImplementation}.fimpl";
                string implementationJson = FileToString(implementationFile);
                if (implementationJson != "")
                {
                    Implementation impl = JsonConvert.DeserializeObject<Implementation>(implementationJson);
                    foreach (var dep in impl.Dependencies)
                    {
                        module.Dependencies.Add(dep);
                    }
                }

                //================================================================
                // Some mandatory null checks
                //================================================================                
                if (module.Dependencies == null)
                {
                    module.Dependencies = new List<Dependency>();
                }

                if (module.ExternalDependencies == null)
                {
                    module.ExternalDependencies = new List<ExternalDependency>();
                }

                //================================================================
                // Finally we save the module
                //================================================================
                if (module.ImplementsMain)
                {
                    Modules.Insert(0, module);
                }
                else
                {
                    Modules.Add(module);
                }

                if (!exists)
                {
                    ModuleMap.Add(module.Name, module);
                }

                return true;
            }

            public bool ExecuteModules()
            {
                //================================================================
                // Get application module
                //================================================================
                if (!deserializeModule($"{ProjectFilePath}src\\application\\", "application", Cfg.Application))
                {
                    return false;
                }

                foreach (ModuleInfo module in Cfg.Modules)
                {
                    if (!deserializeModule($"{ProjectFilePath}src\\{module.Type}\\{module.Name}\\", $"{module.Name}", module.Implementation))
                    {
                        return false;
                    }
                }

                //================================================================
                // Check whether dependencies exist in our current config
                // Check whether versions are correct
                //================================================================
                for (int i = 0; i < Modules.Count; i++)
                {
                    Module mod = Modules[i];
                    for (int j = 0; j < mod.Dependencies.Count; j++)
                    {
                        Dependency dep = mod.Dependencies[j];
                        if (ModuleMap.ContainsKey(dep.Name))
                        {
                            Module temp = ModuleMap[dep.Name];
                            int[] vers;
                            int[] min;
                            int[] max;

                            if (!parseVersion(temp.Version, out vers) || !parseVersion(dep.RequiredVersion.Min, out min) || !parseVersion(dep.RequiredVersion.Max, out max))
                            {
                                return false;
                            }

                            //================================================================
                            // Check version
                            //================================================================
                            if (vers[0] > max[0] || vers[0] < min[0] ||
                                vers[1] > max[1] || vers[1] < min[1] ||
                                vers[2] > max[2] || vers[2] < min[2])
                            {
                                LastError = $"Dependency version {temp.Version} exceeds our module's required version (Min {dep.RequiredVersion.Min} & Max {dep.RequiredVersion.Max})";
                                return false;
                            }

                            continue;
                        }

                        //================================================================
                        // Dependency doesn't seem to exist
                        //================================================================
                        if (dep.External) // if it's external, we load the library
                        {
                            string externalFolder = $"{ProjectFilePath}src\\external\\";
                            var folders = Directory.GetDirectories(externalFolder);
                            foreach (var folder in folders)
                            {
                                string folderName = folder.Substring(folder.LastIndexOf('\\') + 1);
                                if (folderName == dep.Name)
                                {
                                    string externalFile = FileToString($"{folder}\\{dep.Name}.fmodule");
                                    ExternalDependency exdep = JsonConvert.DeserializeObject<ExternalDependency>(externalFile);
                                    exdep.RequiredVersion = dep.RequiredVersion;
                                    exdep.External = dep.External;
                                    exdep.IncludeFolder = $"{folder}\\include";
                                    exdep.LibraryFolder = $"{folder}\\lib";
                                    if (exdep.LibraryName == null)
                                    {
                                        exdep.LibraryName = dep.Name;
                                    }

                                    if (exdep.RequiredLibraries == null)
                                    {
                                        exdep.RequiredLibraries = new List<string>();
                                    }
                                    mod.ExternalDependencies.Add(exdep);
                                    break;
                                }
                            }
                        }
                        else // if not, we're missing a dependency
                        {
                            LastError = $"Dependency \"{dep.Name}\" in \"{mod.Name}\" not found in module list";
                            return false;
                        }
                    }
                }

                return true;
            }

            private void GetSourceFiles(string Path, string Denom, ref List<SourceFile> HeaderFiles, ref List<SourceFile> SourceFiles, ref List<string> Filters)
            {
                var files = Directory.GetFiles(Path);

                //================================================================
                // Iterate through current directory
                //================================================================
                foreach (var file in files)
                {
                    SourceFile temp = new SourceFile();
                    int start = Path.IndexOf(Denom) + 1;
                    if (start == -1)
                    {
                        start = 0;
                    }
                    string newPath = Path.Substring(start);
                    if (!Filters.Contains(newPath))
                    {
                        Filters.Add(newPath);
                    }

                    temp.Directory = newPath;
                    temp.FileName = file;

                    string ext = file.Substring(file.LastIndexOf('.'));
                    if (ext == ".h" || ext == ".hpp")
                    {
                        //================================
                        // Add header file
                        //================================
                        HeaderFiles.Add(temp);
                    }
                    else if (ext == ".cpp")
                    {
                        //================================
                        // Add source file
                        //================================
                        SourceFiles.Add(temp);
                    }
                }

                //================================================================
                // Recursively get source files in sub directories
                //================================================================
                var dirs = Directory.GetDirectories(Path);
                foreach (var dir in dirs)
                {
                    GetSourceFiles(dir, Denom, ref HeaderFiles, ref SourceFiles, ref Filters);
                }
                
            }

            public bool GenerateProjectFiles()
            {
                //================================================================
                // Generate solution
                //================================================================
                string solutionTemplate = FileToString("solution_template.txt");
                if (solutionTemplate == "")
                {
                    LastError = "Unable to find file: \".\\solution_template.txt\"";
                    return false;
                }

                string solutionResult = Engine.Razor.RunCompile(solutionTemplate, "solution", null, new { ProjectGuid = Guid.NewGuid(), Modules = this.Modules});

                string solutionPath = $"{ProjectPath}\\{Proj.Name}.sln";

                if (File.Exists(solutionPath))
                {
                    File.Delete(solutionPath);
                }

                File.WriteAllText(solutionPath, solutionResult);

                string projectTemplate = FileToString("project_template.txt");
                if (projectTemplate == "")
                {
                    LastError = "Unable to find file: \".\\project_template.txt\"";
                    return false;
                }

                string filterTemplate = FileToString("filter_template.txt");
                if (filterTemplate == "")
                {
                    LastError = "Unable to find file: \".\\filter_template.txt\"";
                    return false;
                }

                //==============================================================
                // Generate project files
                //==============================================================
                foreach (Module mod in Modules)
                {
                    List<SourceFile> headerFiles = new List<SourceFile>();
                    List<SourceFile> sourceFiles = new List<SourceFile>();

                    List<Module> dependencies = new List<Module>();
                    List<string> filters = new List<string>();

                    foreach (var dep in mod.Dependencies)
                    {
                        if (dep.External)
                        {
                            continue;
                        }

                        Module temp = Modules.First(item => item.Name == dep.Name);
                        temp.InterfaceOnly = dep.InterfaceOnly;
                        dependencies.Add(temp);
                    }

                    GetSourceFiles(mod.Path + "\\interfaces", "\\interfaces",ref headerFiles, ref sourceFiles, ref filters);
                    GetSourceFiles(mod.Path + "\\implementations\\" + mod.ActiveImplementation, "\\" + mod.ActiveImplementation, ref headerFiles, ref sourceFiles, ref filters);
                    GetSourceFiles(mod.Path + "\\includes", "\\includes", ref headerFiles, ref sourceFiles, ref filters);

                    string projectResult = Engine.Razor.RunCompile(projectTemplate, "project", null, new { ProjectName = Cfg.Application, Module = mod, Dependencies = dependencies, HeaderFiles = headerFiles, SourceFiles = sourceFiles });
                    string projectPath;
                    if (mod.DynamicallyLoaded)
                    {
                        projectPath = $"{ProjectPath}\\intermediate\\{mod.Name}_{mod.ActiveImplementation}.vcxproj";
                    }
                    else
                    {
                        projectPath = $"{ProjectPath}\\intermediate\\{mod.Name}.vcxproj";
                    }
    
                    string filterResult = Engine.Razor.RunCompile(filterTemplate, "filter", null, new { ModuleName = mod.Name, Filters = filters, HeaderFiles = headerFiles, SourceFiles = sourceFiles });
                    string filterPath;
                    if (mod.DynamicallyLoaded)
                    {
                        filterPath = $"{ProjectPath}\\intermediate\\{mod.Name}_{mod.ActiveImplementation}.vcxproj.filters";
                    }
                    else
                    {
                        filterPath = $"{ProjectPath}\\intermediate\\{mod.Name}.vcxproj.filters";
                    }

                    if (File.Exists(projectPath))
                    {
                        File.Delete(projectPath);
                    }

                    if (File.Exists(filterPath))
                    {
                        File.Delete(filterPath);
                    }

                    File.WriteAllText(projectPath, projectResult);
                    File.WriteAllText(filterPath, filterResult);
                }

                return true;
            }

            private string ProjectFilePath;
            public ETask Task;
            private string Config;
            private string ProjectPath;

            public string LastError;

            private Project Proj;
            private Config Cfg;
            private List<Module> Modules = new List<Module>();
            private Dictionary<string, Module> ModuleMap = new Dictionary<string, Module>();
        }
    }
}