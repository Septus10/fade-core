#include <engine_loop.hpp>

fade::engine_loop::engine_loop() : run_(true)
{
	
}

fade::engine_loop::~engine_loop()
{
	
}

fade::i32 fade::engine_loop::pre_initialize()
{
	return 0;
}

fade::i32 fade::engine_loop::initialize()
{
	return 0;
}

fade::i32 fade::engine_loop::post_initialize()
{
	return 0;
}

fade::i32 fade::engine_loop::deinitialize()
{
	return 0;
}

void fade::engine_loop::tick()
{
	
}

void fade::engine_loop::should_close(bool should_close)
{
	run_ = should_close;
}

bool fade::engine_loop::should_close() const
{
	return run_;
}






