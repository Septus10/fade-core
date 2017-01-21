#include <iostream>

#include <run/engine_loop.hpp>

int fade::main()
{
	fade::engine_loop g_engine_loop;	
	
	g_engine_loop.pre_initialize();
	
	g_engine_loop.initialize();

	g_engine_loop.post_initialize();

	while(g_engine_loop.should_stop())
	{
		g_engine_loop.tick();
	}

	g_engine_loop.deinitialize();

	return 0;
}