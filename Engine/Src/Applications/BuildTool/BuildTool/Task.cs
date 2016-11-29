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
        enum Task
        {
            ENull,
            EGenerate,
            EBuild
        }

        struct SourceFile
        {
            public string FileName { get; set; }
            public string Directory { get; set; }
        }

        class BuildTask
        {
            public BuildTask(string filePath, Task task, string config, string path)
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
                string json = FileToString(ProjectFilePath);
                if (json == "")
                {
                    return false;
                }
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
                ConfigPath configPath = new ConfigPath();

                for (int i = 0; i < Proj.Configs.Count; i++)
                {
                    if (Proj.Configs[i].Name.ToLower() == Config.ToLower())
                    {
                        configPath = Proj.Configs[i];
                    }
                }

                string json = FileToString(ProjectFilePath + configPath.Path);
                if (json == "")
                {
                    return false;
                }

                Cfg = JsonConvert.DeserializeObject<Config>(json);
                if (Cfg.Modules == null)
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

            public bool ExecuteModules()
            {
                for (int i = 0; i < Cfg.Modules.Count; i++)
                {
                    string moduleFile = ProjectFilePath + Cfg.Modules[i].Path;
                    string moduleJson = FileToString(moduleFile);
                    Module module = JsonConvert.DeserializeObject<Module>(moduleJson);
                    if (module.Name == null)
                    {
                        LastError = "Unable to deserialize module file";
                        return false;
                    }
                    module.GUID = Guid.NewGuid();
                    module.Path = moduleFile.Substring(0, moduleFile.LastIndexOf('/') + 1);
                    module.ActiveImplementation = Cfg.Modules[i].Implementation;

                    if (ModuleMap.ContainsKey(module.Name))
                    {
                        LastError = $"Module {module.Name} already exists\n\tPath: {moduleFile}";
                        return false;
                    }

                    string implementationFile = ProjectFilePath + Cfg.Modules[i].Path.Substring(0, Cfg.Modules[i].Path.LastIndexOf('/')) + "\\Implementations\\" + module.ActiveImplementation + $"\\{module.ActiveImplementation}.fade_implementation";
                    string implementationJson = FileToString(implementationFile);
                    if (implementationJson != "")
                    {
                        Implementation impl = JsonConvert.DeserializeObject<Implementation>(implementationJson);
                        foreach (var dep in impl.Dependencies)
                        {
                            module.Dependencies.Add(dep);
                        }
                    }

                    Modules.Add(module);
                    ModuleMap.Add(module.Name, module);
                }

                // check if dependencies exist and if versions are correct

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

                            if (vers[0] > max[0] || vers[0] < min[0] ||
                                vers[1] > max[1] || vers[1] < min[1] ||
                                vers[2] > max[2] || vers[2] < min[2])
                            {
                                LastError = $"Dependency version {temp.Version} exceeds our module's required version (Min {dep.RequiredVersion.Min} & Max {dep.RequiredVersion.Max})";
                                return false;
                            }

                            continue;
                        }

                        LastError = $"Dependency \"{dep.Name}\" in \"{mod.Name}\" not found in module list";
                        return false;
                    }
                }

                return true;
            }

            private void GetSourceFiles(string Path, string Denom, ref List<SourceFile> HeaderFiles, ref List<SourceFile> SourceFiles, ref List<string> Filters)
            {
                var files = Directory.GetFiles(Path);

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
                        HeaderFiles.Add(temp);
                    }
                    else if (ext == ".cpp")
                    {
                        SourceFiles.Add(temp);
                    }
                }

                var dirs = Directory.GetDirectories(Path);
                foreach (var dir in dirs)
                {
                    GetSourceFiles(dir, Denom, ref HeaderFiles, ref SourceFiles, ref Filters);
                }
                
            }

            public bool GenerateProjectFiles()
            {
                //==============================================================
                // Generate solution
                //==============================================================
                string solutionTemplate = FileToString("solution_template.txt");
                if (solutionTemplate == "")
                {
                    LastError = "Unable to find file: \"./solution_template.txt\"";
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
                    LastError = "Unable to find file: \"./solution_template.txt\"";
                    return false;
                }

                string filterTemplate = FileToString("filter_template.txt");
                if (filterTemplate == "")
                {
                    LastError = "Unable to find file: \"./filter_template.txt\"";
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
                        Module temp = Modules.First(item => item.Name == dep.Name);
                        dependencies.Add(temp);
                    }

                    GetSourceFiles(mod.Path + "\\Interfaces", "\\Interfaces",ref headerFiles, ref sourceFiles, ref filters);
                    GetSourceFiles(mod.Path + "\\Implementations\\" + mod.ActiveImplementation, "\\" + mod.ActiveImplementation, ref headerFiles, ref sourceFiles, ref filters);
                    GetSourceFiles(mod.Path + "\\Includes", "\\Includes", ref headerFiles, ref sourceFiles, ref filters);

                    string projectResult = Engine.Razor.RunCompile(projectTemplate, "project", null, new { Module = mod, Dependencies = dependencies, HeaderFiles = headerFiles, SourceFiles = sourceFiles });            
                    string projectPath = $"{ProjectPath}\\Intermediate\\{mod.Name}.vcxproj";
    
                    string filterResult = Engine.Razor.RunCompile(filterTemplate, "filter", null, new { Filters = filters, HeaderFiles = headerFiles, SourceFiles = sourceFiles });
                    string filterPath = $"{ProjectPath}\\Intermediate\\{mod.Name}.vcxproj.filters";

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
            public Task Task;
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
