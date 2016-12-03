#include <editor.hpp>
#include <application.hpp>

using namespace fade;
using namespace editor;

void editor_application::tick()
{
	
}


std::unique_ptr<application> get_application()
{
	return std::make_unique<editor_application>();
}

