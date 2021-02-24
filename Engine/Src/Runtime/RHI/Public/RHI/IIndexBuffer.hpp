#pragma once

#include <RHI/RHIApi.hpp>
#include <RHI/IBuffer.hpp>

#include <memory>

namespace Fade { inline namespace Graphics {

class IIndexBuffer : public IBuffer
{
public:
	

private:
	

};

std::shared_ptr<IIndexBuffer> CreateIndexBuffer()
{
	return std::shared_ptr<IIndexBuffer>();
}

} }