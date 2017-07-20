#pragma once

#include <application/application.hpp>
#include <application/application_api.hpp>

#include <vector>

namespace fade
{

class window;

namespace graphics
{
	class renderer;
    struct render_batch;
}

namespace resources
{
	class resource_importer_hub;
}

namespace editor
{
	
class editor_application : public application
{
public:
	editor_application() = default;

	~editor_application() = default;

	int tick(double delta_time) override;

	void fixed_tick(double fixed_delta_time) override;

	void pre_initialize() override;

	void initialize() override;

	void post_initialize() override;

	void deinitialize() override;

private:
	std::unique_ptr<window> wnd_;
	std::unique_ptr<graphics::renderer> renderer_;
	std::unique_ptr<resources::resource_importer_hub> importer_hub_;

	float cur_time_;
	bool* should_stop_;

    std::vector<graphics::render_batch> batches_;
};

}

}
