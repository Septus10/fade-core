#pragma once

#include <core/core_api.hpp>
#include <definitions.hpp>
#include <vector>

namespace fade
{

namespace memory
{

/*
 * 
 */
struct memory_chunk
{
	memory_chunk(void* addr, usize size);
	/*
	 * size of this chunk
	 */
	usize chunk_size_;	//  8 bytes

	/*
	 * size left in current chunk
	 */
	usize size_left_;	//  8 bytes

	/*
	 * pointer to start of free memory
	 */
	void* start_;

	/*
	 * pointer to start of chunk
	 */
	void* chunk_start_;

	/*
	 * pointer to next chunk
	 */
	memory_chunk* next_;
};

struct memory_block
{
	memory_block(void* addr);

	void*	address_;

	memory_block*	next_;
};

class FADE_API memory_manager
{
public:
	memory_manager();

	~memory_manager();

	void* allocate(usize size);

	void deallocate(void* ptr);

protected:
	void allocate_block(usize block_size, usize chunk_size);

	memory_chunk*	chunk_first_;
	memory_chunk*	chunk_last_;
	memory_block*	block_first_;
	memory_block*	block_last_;
private:
	

};

}

}
