#pragma once

#include <Core/Bootstrapping/module_info.hpp>
#include <Core/fstl/Memory.hpp>
#include <ResourceImporter/ResourceImporter.hpp>

FADE_MODULE_DEFINE(TextureImporter)

namespace Fade { namespace Resources {

class CResource;
class CTextureImporter : public CResourceImporter
{
public:
	CTextureImporter();
	~CTextureImporter() override;

	fstl::UniquePtr<CResource> ImportResource(std::string a_FilePath) override;
	bool ImportsExtension(std::string a_Extension) override;
};

} }
