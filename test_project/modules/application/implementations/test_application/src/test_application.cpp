#include "application/interface/application.hpp"

#include "test_application.hpp"

#include <iostream>

namespace test_project {

void test_application::initialize(std::vector<std::string> in_args)
{
    std::cout << "Initializing..." << std::endl;
}

void test_application::deinitialize()
{
    std::cout << "Deinitializing..." << std::endl;
}

void test_application::start()
{
    std::cout << "Starting application!" << std::endl;
}

}

namespace fade::application {

std::unique_ptr<application_base> create()
{
    return std::make_unique<test_project::test_application>();
}

}