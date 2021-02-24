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
                string configPath = $"{ProjectFilePath}Config\\{Proj.Configuration}.fcfg";
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

                Cfg.Application = Proj.Configuration;
                return true;
            }

            private bool parseVersion(string version, out int[] ints)
            {
                string[] versions = version.Split('.');
                ints = new int[versions.Length];
                for (int i = 0; i < versions.Length; i++)
                {
                    if (!Int32.TryParse(versions[i], out ints[i]))
                    {
                        LastError = "Error parsing version";
                        return false;
                    }
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
                module.ModuleFilePath = moduleFile;

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
                // Some mandatory null checks
                //================================================================                
                if (module.Dependencies == null)
                {
                    module.Dependencies = new List<Dependency>();
                }

                if (module.ImplementationDependencies == null)
                {
                    module.ImplementationDependencies = new List<ImplementationDependency>();
                }

                if (module.ExternalDependencies == null)
                {
                    module.ExternalDependencies = new List<ExternalDependency>();
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
                string implementationFile = $"{path}Implementations\\{module.ActiveImplementation}.fimpl";
                string implementationJson = FileToString(implementationFile);
                if (implementationJson != "")
                {
                    Implementation impl = JsonConvert.DeserializeObject<Implementation>(implementationJson);
                    if (impl.Dependencies != null)
                    {
                        foreach (var dep in impl.Dependencies)
                        {
                            module.Dependencies.Add(dep);
                        }
                    }

                    if (impl.ImplementationDependencies != null)
                    {
                        foreach (var dep in impl.ImplementationDependencies)
                        {
                            module.ImplementationDependencies.Add(dep);
                        }
                    }

                    //================================================================
                    // Set seperate variables
                    //================================================================
                    module.ImplementationFilePath = implementationFile;
                }
                if (module.ImplementationFilePath == null)
                {
                    module.ImplementationFilePath = "";
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
                // Deserialize all modules
                //================================================================
                foreach (ModuleInfo module in Cfg.Modules)
                {
                    if (!deserializeModule($"{ProjectFilePath}Src\\{module.Type}\\{module.Name}\\", $"{module.Name}", module.Implementation))
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
                            if (vers.Length != min.Length || vers.Length != max.Length)
                            {
                                LastError = "Module version is not the same format as specified required version";
                                return false;
                            }

                            for(int k = 0; k < vers.Length; k++)
                            {
                                if (vers[k] > max[k] || vers[k] < min[k])
                                {
                                    LastError = $"Dependency version {temp.Version} exceeds our module's required version (Min {dep.RequiredVersion.Min} & Max {dep.RequiredVersion.Max})";
                                    return false;
                                }
                            }
                            continue;
                        }

                        //================================================================
                        // Dependency doesn't seem to exist
                        //================================================================
                        if (dep.External) // if it's external, we load the library
                        {
                            string externalFolder = $"{ProjectFilePath}Src\\External\\";
                            var folders = Directory.GetDirectories(externalFolder);
                            foreach (var folder in folders)
                            {
                                string folderName = folder.Substring(folder.LastIndexOf('\\') + 1);
                                if (folderName == dep.Name)
                                {
                                    // check whether we can actually find the dependency file
                                    string depFile = $"{folder}\\{dep.Name}.fmodule";
                                    if (!File.Exists(depFile))
                                    {
                                        LastError = $"Unable to find external dependency in folder: {folder}";
                                        return false;
                                    }

                                    string externalFile = FileToString(depFile);                                    
                                    ExternalDependency exdep = JsonConvert.DeserializeObject<ExternalDependency>(externalFile);
                                    exdep.RequiredVersion = dep.RequiredVersion;
                                    exdep.External = dep.External;
                                    exdep.IncludeFolder = $"{folder}\\include";
                                    exdep.LibraryFolder = $"{folder}\\lib";
                                    // external dependency doesn't have a library name
                                    if (exdep.LibraryName == null)
                                    {
                                        exdep.UsesLibrary = false;
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
                            LastError = $"Dependency \"{dep.Name}\" in \"{mod.Name}\" not found in module list\n\tYou've either forgotten to add this module to the config file.\n\tOr you didn't mark it as external in the '{mod.Name}.fmodule' file";
                            return false;
                        }
                    }
                }

                return true;
            }

            private bool GetSourceFiles(string Path, string Denom, ref List<SourceFile> HeaderFiles, ref List<SourceFile> SourceFiles, ref List<string> Filters)
            {
                if (!Directory.Exists(Path))
                {
                    LastError = $"Could not find part of the path: {Path}";
                    return false;
                }
                var files = Directory.GetFiles(Path);
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

                //================================================================
                // Iterate through current directory
                //================================================================
                foreach (var file in files)
                {
                    SourceFile temp = new SourceFile();
                   

                    temp.Directory = newPath;
                    temp.FileName = file;

                    string ext = file.Substring(file.LastIndexOf('.'));
                    if (ext == ".h" || ext == ".hpp" || ext == ".inl")
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

                return true;
                
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

                    if (!GetSourceFiles(mod.Path + "\\Public", "\\Public",ref headerFiles, ref sourceFiles, ref filters) ||
                        !GetSourceFiles(mod.Path + "\\Implementations\\" + mod.ActiveImplementation, "\\" + mod.ActiveImplementation, ref headerFiles, ref sourceFiles, ref filters))
                    {
                        return false;
                    }

                    string projectResult = Engine.Razor.RunCompile(projectTemplate, "project", null, new { ProjectName = Cfg.Application, Module = mod, Dependencies = dependencies, HeaderFiles = headerFiles, SourceFiles = sourceFiles });
                    string projectPath;
                    if (mod.DynamicallyLoaded)
                    {

                        projectPath = $"{mod.Path}Intermediate\\{mod.Name}{mod.ActiveImplementation}.vcxproj";
                    }
                    else
                    {
                        projectPath = $"{mod.Path}Intermediate\\{mod.Name}.vcxproj";
                    }


                    string filterResult = Engine.Razor.RunCompile(filterTemplate, "filter", null, new { ModuleName = mod.Name, Filters = filters, HeaderFiles = headerFiles, SourceFiles = sourceFiles, ModuleFile = mod.ModuleFilePath, ImplFile = mod.ImplementationFilePath, ActiveImplementation = mod.ActiveImplementation });
                    string filterPath;
                    if (mod.DynamicallyLoaded)
                    {
                        filterPath = $"{mod.Path}Intermediate\\{mod.Name}{mod.ActiveImplementation}.vcxproj.filters";
                    }
                    else
                    {
                        filterPath = $"{mod.Path}Intermediate\\{mod.Name}.vcxproj.filters";
                    }

                    if (File.Exists(projectPath))
                    {
                        File.Delete(projectPath);
                    }

                    string projectDirectoryPath = projectPath.Substring(0, projectPath.LastIndexOf('\\'));
                    if (!Directory.Exists(projectDirectoryPath))
                    {
                        Directory.CreateDirectory(projectDirectoryPath);
                    }
                    File.WriteAllText(projectPath, projectResult);

                    if (File.Exists(filterPath))
                    {
                        File.Delete(filterPath);
                    }

                    string filterDirectoryPath = filterPath.Substring(0, filterPath.LastIndexOf('\\'));
                    if (!Directory.Exists(filterDirectoryPath))
                    {
                        Directory.CreateDirectory(filterDirectoryPath);
                    }
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