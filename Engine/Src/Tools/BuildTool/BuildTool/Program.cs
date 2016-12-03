using System;
using System.IO;
using System.Security;
using System.Security.Policy;
using System.Security.Permissions;
using System.Reflection;
using RazorEngine;

namespace Fade
{
    namespace BuildTool
    {
        class Program
        {
            public static void Main(string[] args)
            {
                // Create new appdomain
                if (AppDomain.CurrentDomain.IsDefaultAppDomain())
                {
                    // RazorEngine cannot clean up from the default appdomain...
                    Console.WriteLine("Switching to secound AppDomain, for RazorEngine...");
                    AppDomainSetup adSetup = new AppDomainSetup();
                    adSetup.ApplicationBase = AppDomain.CurrentDomain.SetupInformation.ApplicationBase;
                    var current = AppDomain.CurrentDomain;

                    // You only need to add strongnames when your appdomain is not a full trust environment.
                    var strongNames = new StrongName[0];
                    var domain = AppDomain.CreateDomain("RazorDomain", null, current.SetupInformation, new PermissionSet(PermissionState.Unrestricted), strongNames);
                    var exitCode = domain.ExecuteAssembly(Assembly.GetExecutingAssembly().Location, args);

                    // RazorEngine will cleanup. 
                    AppDomain.Unload(domain);
                    return;
                }

                BuildTask buildTask;
                if (!CheckCommandLine(args, out buildTask))
                {
                    Console.ReadLine();
                    return;
                }

                if (!ExecuteBuildTask(buildTask))
                {
                    Console.ReadLine();
                    return;
                }
            }

            private static bool CheckCommandLine(string[] args, out BuildTask buildTask)
            {
                buildTask = null;
                string projectFilePath = null;
                Task task = Task.ENull;
                if (args.Length == 0)   // when no arguments are passed we will try to find a project file in this directory
                {                       // Then we will generate project files and build said project
                    var files = Directory.GetFiles(Directory.GetCurrentDirectory(), "*.fade_project");
                    if (files.Length == 0)
                    {
                        Console.WriteLine($"No project file found in current directory {Directory.GetCurrentDirectory()}");
                        return false;
                    }
                    if (files.Length != 1)
                    {
                        Console.WriteLine($"Multiple project files found in current directory {Directory.GetCurrentDirectory()}");
                        return false;
                    }

                    projectFilePath = files[0];
                    task = Task.EBuild;
                }
                else
                {
                    if (args.Length == 1 && args[0] == "help" || args[0] == "-help")
                    {
                        Console.WriteLine("Usage:\n FadeBuild.exe \"task\" \"project path\" \"config\"");
                        Console.WriteLine("Example: \n FadeBuild.exe \"build\" \"Path\\To\\Project\\Project.fade_project\" \"debug\"");
                        return false;
                    }
                    else if (args.Length != 3)
                    {
                        Console.WriteLine($"Expected 3 arguments but got {args.Length} instead");
                        return false;
                    }

                    switch(args[0])
                    {
                        case "build":
                            task = Task.EBuild;
                            break;
                        case "generate":
                            task = Task.EGenerate;
                            break;
                        default:
                            task = Task.ENull;
                            return false;
                    }
                    
                    projectFilePath = Path.GetFullPath(args[1]);
                    if (!Path.HasExtension(projectFilePath)) projectFilePath += ".fade_project";
                    if (!File.Exists(projectFilePath))
                    {
                        Console.WriteLine($"Couldn't find project file at {projectFilePath}.");
                        return false;
                    }

                    int index = projectFilePath.LastIndexOf('\\');
                    if (index == -1)
                    {
                        index = projectFilePath.LastIndexOf('/');
                    }
                    string dirPath = projectFilePath.Substring(0, index);
                    buildTask = new BuildTask(projectFilePath, task, args[2], dirPath);
                }
                return true;
            }

            private static bool ExecuteBuildTask(BuildTask buildTask)
            {
                //===================================================
                // Read the project files
                //===================================================
                bool res = buildTask.ExecuteProject();
                if (!res)
                {
                    Console.WriteLine("Error getting project information");
                    return false;
                }

                res = buildTask.ExecuteConfig();
                if (!res)
                {
                    Console.WriteLine("Error getting build config: " + buildTask.LastError);
                    return false;
                }

                res = buildTask.ExecuteModules();
                if (!res)
                {
                    Console.WriteLine("Error getting modules: " + buildTask.LastError);
                    return false;
                }

                res = buildTask.GenerateProjectFiles();
                if (!res)
                {
                    Console.WriteLine("Error generating project files: " + buildTask.LastError);
                    return false;
                }
                
                // Might just leave this out and let users build their modules with visual studio
                if(buildTask.Task == Task.EBuild)
                {
                    // Call msbuild to build the project
                }

                return true;
            }
        }
    }
}
