# Fade-Engine

Fade engine is a relatively simple 2D/3D cross-platform game engine.

## Motivation

The project was founded by Lars "Septus10" de Smit, as a hobby project and a way to get better at programming.

## Installation

In order to build please compile the build tool in `Engine/Src/Applications/BuildTool/` (C#, VS2015, Windows)  
After building run the executable in the following format:
```cpp
// usage
BuildTool.exe "task" "project path" "build config"
// example
BuildTool.exe "build" "Path/To/Project/Project.fade_project" "editor"
```  
These are the available tasks
 
Tasks | Description 
------------ | -------------
build | Generates project files and builds the project
generate | Only generates the project files

Right now any config file will have to be manually created and modified.  
As an example:
```json
FadeEditor.fade_build  

{
	"ProjectType": "Application",
	"Modules": 
	[
		{ 
			"Name": "Run",
			"Path": "Src/Runtime/Run/Run.fade_module",
			"Implementation": "Default",
		},
	]
}
```
