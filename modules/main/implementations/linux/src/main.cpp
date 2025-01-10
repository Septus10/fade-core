#include <iostream>
#include <vector>
#include <utility>

#include "application/interface/application.hpp"
#include "core/include/type_definitions.hpp"

int main(int in_argc, char** in_args)
{
    // get the application
    std::unique_ptr<fade::application::application_base> application =
        fade::application::create();

    if (application.get() == nullptr)
    {
        // log error
        return 1;
    }

    std::vector<std::string> args;
    for (fade::int32 i = 0; i < in_argc; ++i)
    {
        args.push_back(in_args[i]);
    }
    

    // Initialize application using gathered args
    application->initialize(args);

    try 
    {
        application->start();
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    application->deinitialize();
}