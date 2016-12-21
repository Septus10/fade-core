#include <memory/memory.hpp>
#include <cassert>
#include <iostream>

namespace fade {

memory::memory_chunk::memory_chunk(void* addr, usize size) : chunk_size_(size), size_left_(chunk_size_), start_(addr), chunk_start_(addr), next_(nullptr)
{}

memory::memory_block::memory_block(void* addr) : address_(addr), next_(nullptr)
{}

memory::memory_manager::memory_manager() : chunk_first_(nullptr), chunk_last_(nullptr), block_first_(nullptr), block_last_(nullptr)
{
	allocate_block(2048, 256);
}

memory::memory_manager::~memory_manager()
{
	memory_block* block = block_first_;
	while (block != nullptr)
	{
		free(block->address_);
		block = block->next_;
	}
}

void* memory::memory_manager::allocate(usize size)
{
	std::cout << "Allocating memory for object with size: " << size << std::endl;
	memory_chunk* chunk = chunk_first_;
	while (chunk != nullptr)
	{
		if(chunk->size_left_ > size)
		{
			void* ret = chunk->start_;
			std::cout << "Allocating at address: " << ret << std::endl;
			chunk->start_ = static_cast<void*>(static_cast<char*>(chunk->start_) + size);
			std::cout << "New chunk start: " << chunk->start_ << std::endl;
			return ret;
		}
		chunk = chunk->next_;
	}

	// no free chunks, so we allocate a new block
	usize temp = size;
	// this has to be initialized as 2, so it will always be one bit more than the number we're shifting right
	int new_chunk_size = 2;
	while(temp != 0)
	{
		temp >>= 1;
		new_chunk_size <<= 1;
	}
	std::cout << "Initial size: " << size << std::endl;
	std::cout << "New size: " << new_chunk_size << std::endl;
	allocate_block(new_chunk_size * 4, new_chunk_size);
	
	return allocate(size);
}

void memory::memory_manager::deallocate(void* ptr)
{

}

void memory::memory_manager::allocate_block(usize block_size, usize chunk_size)
{
	assert(block_size % chunk_size == 0);

	void* mem = malloc(block_size);
	memory_block* block = new memory_block(mem);
	if(block_first_ == nullptr)
	{
		block_last_ = block_first_ = block;
	}
	else
	{
		block_last_->next_ = block;
		block_last_ = block;
	}	

	u32 num_chunks = static_cast<u32>(block_size / chunk_size);
	for (u32 i = 0; i < num_chunks; i++)
	{
		char* addr = static_cast<char*>(mem) + chunk_size * i;
		memory_chunk* chunk = new memory_chunk(static_cast<void*>(addr), chunk_size);
		if(chunk_first_ == nullptr)
		{
			chunk_last_ = chunk_first_ = chunk;
		}
		else
		{
			chunk_last_->next_ = chunk;
			chunk_last_ = chunk;
		}		
	}
}

}