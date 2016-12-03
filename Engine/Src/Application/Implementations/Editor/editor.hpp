#pragma once

#include <application.hpp>

namespace fade
{

namespace editor
{
	
class __declspec(dllexport) editor_application : public application
{
public:
	void tick() override;

private:


};

}

}