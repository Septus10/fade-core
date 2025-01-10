#ifndef TEST_APPLICATION_HPP
#define TEST_APPLICATION_HPP

#include "application/interface/application.hpp"

namespace test_project {

class test_application : public fade::application::application_base
{
public:
    virtual void initialize(std::vector<std::string> in_args) override;

    virtual void deinitialize() override;

    virtual void start() override;
};

}

#endif