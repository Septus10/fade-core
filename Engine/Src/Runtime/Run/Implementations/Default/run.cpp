#include <iostream>

#include <engine_loop.hpp>

int fade::main()
{
	std::cout << "Test\n";	

	g_engine_loop.pre_initialize();

	g_engine_loop.initialize();

	g_engine_loop.post_initialize();

	while(!g_engine_loop.run_)
	{
		g_engine_loop.tick();
	}

	g_engine_loop.deinitialize();

	return 0;
}