#ifndef TEST_APPLICATION_CPP
#define TEST_APPLICATION_CPP

#define FADE_PLATFORM_UNIX 1

#include "application.generated.hpp"

#include <string>
#include <vector>
#include <memory>

namespace fade::application {

class FADE_APPLICATION_API application_base
{
public:
    /** Initializes the application */
    virtual void initialize(std::vector<std::string> in_args) = 0;

    /** Deinitializes the application */
    virtual void deinitialize() = 0;

    /** Starts the application */
    virtual void start() = 0;
};

/** 
* Getter function for the implementation
* 
* This could be a singleton, if an application can only have one instance.
* Or it can behave as a factory, if the application supports it and the user desires it.
*/
std::unique_ptr<application_base> FADE_APPLICATION_API create();

}

#endif // TEST_APPLICATION_CPP