#include <iostream>

#include <engine_loop.hpp>

fade::engine_loop g_engine_loop;

int fade::main()
{
	std::cout << "Test\n";	
	
	g_engine_loop.pre_initialize();
	
	g_engine_loop.initialize();

	g_engine_loop.post_initialize();

	while(!g_engine_loop.should_close())
	{
		g_engine_loop.tick();
	}

	g_engine_loop.deinitialize();

	system("PAUSE");

	return 0;
}