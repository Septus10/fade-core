#pragma once

#include <application.hpp>
#include <application_api.hpp>

namespace fade
{

namespace rendering
{
	class window;
}

namespace editor
{
	
class editor_application : public application
{
public:
	~editor_application() = default;

	void tick(double delta_time) override;

	void fixed_tick(double fixed_delta_time) override;

	void pre_initialize() override;

	void initialize() override;

	void post_initialize() override;

	void deinitialize() override;
private:
	std::unique_ptr<rendering::window> wnd_;
};

}

}
