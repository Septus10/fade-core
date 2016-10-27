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
                Console.WriteLine($"Filepath: {filePath} \nConfig: {config}");
            }

            private string FileToString(string Path)
            {
                StreamReader file = new StreamReader(Path);
                if (file == null)
                {
                    LastError = $"File not found ({Path})";
                    return "";
                }

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

                Console.WriteLine($"Project: {Proj.Name}\nConfigs:\n-----");
                for (int i = 0; i < Proj.Configs.Count; i++)
                {
                    Console.WriteLine("\tConfig: " + Proj.Configs[i].Name + "\n\tPath: " + Proj.Configs[i].Path);
                    Console.WriteLine("-----");
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

                Console.WriteLine($"Config: {Config}\nProjectType: {Cfg.ProjectType}\nModules:\n-----");
                for (int i = 0; i < Cfg.Modules.Count; i++)
                {
                    Console.WriteLine($"\tModule: {Cfg.Modules[i].Name}\n\tPath: {Cfg.Modules[i].Path}\n\tImplementation: {Cfg.Modules[i].Implementation}");
                    Console.WriteLine("-----");
                }

                return true;
            }

            public bool ExecuteModules()
            {
                for (int i = 0; i < Cfg.Modules.Count; i++)
                {
                    string moduleFile = ProjectFilePath + Cfg.Modules[i].Path;
                    string moduleJson = FileToString(ProjectFilePath + Cfg.Modules[i].Path);
                    Module module = JsonConvert.DeserializeObject<Module>(moduleJson);
                    if (module.Name == null)
                    {
                        LastError = "Unable to deserialize module file";
                        return false;
                    }
                    module.GUID = Guid.NewGuid();
                    module.Path = moduleFile.Substring(0, moduleFile.LastIndexOf('/') + 1);
                    module.ActiveImplementation = Cfg.Modules[i].Implementation;
                    Console.WriteLine($"Path: {module.Path}");
                    Modules.Add(module);
                }

                for (int i = 0; i < Modules.Count; i++)
                {
                    Console.WriteLine($"Module:\n-----");
                    Module temp = Modules[i];
                    Console.WriteLine($"\tName: {temp.Name}\n\tVersion: {temp.Version}\n\tDependencies:\n\t-----");
                    for (int j = 0; j < temp.Dependencies.Count; j++)
                    {
                        Dependency dep = temp.Dependencies[j];
                        Console.WriteLine($"\t\tName: {dep.Name}\n\t\tRequired version:\n\t\t\tMin: {dep.RequiredVersion.Min}\n\t\t\tMax: {dep.RequiredVersion.Max}");
                        Console.WriteLine("\t-----");
                    }
                    Console.WriteLine($"\tImplementations\n\t-----");
                    for (int j = 0; j < temp.Implementations.Count; j++)
                    {
                        Console.WriteLine($"\t\tName: {temp.Implementations[j]}");
                        Console.WriteLine("\t-----");
                    }
                    Console.WriteLine($"\tGUID: {temp.GUID}");
                    Console.WriteLine("-----");
                }

                return true;
            }

            private void GetSourceFiles(string Path, string Denom, ref List<SourceFile> HeaderFiles, ref List<SourceFile> SourceFiles)
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
                    if (!filters.Contains(newPath))
                    {
                        filters.Add(newPath);
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
                    GetSourceFiles(dir, Denom, ref HeaderFiles, ref SourceFiles);
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

                    GetSourceFiles(mod.Path + "\\Interfaces", "\\Interfaces",ref headerFiles, ref sourceFiles);
                    GetSourceFiles(mod.Path + "\\Implementations\\" + mod.ActiveImplementation, "\\" + mod.ActiveImplementation, ref headerFiles, ref sourceFiles);

                    string projectResult = Engine.Razor.RunCompile(projectTemplate, "project", null, new { Module = mod, HeaderFiles = headerFiles, SourceFiles = sourceFiles });            
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
            private List<string> filters = new List<string>();
        }
    }
}
