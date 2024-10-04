# 1. Introduction

In this page I will introduce the framework, it's intended purpose and a high level overview of its structure and workings.  

The idea of this framework evolved from a personal project of mine called "Fade Engine", of which the goal was to create a functioning albeit relatively simple video game engine. Which, in turn, evolved from a student project with the same name. The video game engine project was supposed to be highly modular and, after having ideas of creating applications that are not video game engines, I figured why not generalize these modules into a framework that can be used for any application. 

This framework will function pretty much the same as Qt, which can be seen as me stupidly trying to reinvent the wheel.  
That said, my main goal is to study and improve. Not just my programming skills, but also my software architecturing skills. And if this turns out as something usable, great!

## 1.1 Overview

The idea behind this framework is to split functionality into separate modules, the main thought behind what each module should implement is that each module should try to minimize the amount of functionality that an application may not use.  To try and keep module sizes and their complexity to a minimum, and by extension the size of the application.  As an example, specific application layer protocols should not be part of a generic networking module as a game engine will not require the SMTP protocol. Instead, a module for each application layer protocol should be created, so that applications may choose which modules/protocols they want to use.  
  
## 1.2 Modular design
### Interfaces and implementations
Modules, in the Fade Framework, are declared by interfaces but can have different implementations. Meaning, that as a user of this framework you can write your own implementation based on the available interface.  

As an example, a graphics module (or render hardware interface) can have different implementations for OpenGL, DirectX, Vulkan and Metal. But they will all work with the same interface. However, unlike this example, where only a single implementation should be used at runtime. We should also be able to load multiple implementations of a module at the same time. For example, modules that convert data that may come in many different shapes and sizes into a common internal format used by our framework.  

That said, most, if not all, modules should come with a default implementation provided by the framework.

### Testing

Another concept that can get quite interesting with the way this framework's modules work, are tests. The idea behind testing is that this framework should support both testing of module interfaces as well as module implementations.  The main purpose of interface testing is to ensure that whoever is working on an implementation knows when their implementation adheres to the usage of said interface, which should help a lot with development as this system can get very complex very fast.

[<< Table of contents](./../index.md) | [2. Modules >> ](./../modules/modules.md)