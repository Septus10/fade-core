#include <engine_loop.hpp>
#include <application.hpp>
#include <timer.hpp>
#include <iostream>

namespace fade {

engine_loop::engine_loop()
{
	run_ = true;
	app_ = get_application();
	timer_ = get_timer();
}

engine_loop::~engine_loop()
{
	
}

i32 engine_loop::pre_initialize()
{
	app_->pre_initialize();
	return 0;
}

i32 engine_loop::initialize()
{
	app_->initialize();
	return 0;
}

i32 engine_loop::post_initialize()
{
	timer_->start();
	std::cout << "engine_loop::post_initialize()\n";
	app_->post_initialize();
	return 0;
}

i32 engine_loop::deinitialize()
{
	app_->deinitialize();
	return 0;
}

void engine_loop::tick()
{	
	app_->tick(timer_->elapsed());
}

void engine_loop::should_stop(bool should_stop)
{
	run_ = should_stop;
}

bool engine_loop::should_stop() const
{
	return run_;
}

}




