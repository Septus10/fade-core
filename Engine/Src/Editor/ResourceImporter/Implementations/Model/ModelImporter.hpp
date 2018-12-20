#pragma once

#include <core/bootstrapping/module_info.hpp>
#include <ResourceImporter/ResourceImporter.hpp>

FADE_MODULE_DEFINE(ModelImporter)

namespace Fade { namespace Resources {

class CResource;
class CModelImporter : public CResourceImporter
{
public:
	CModelImporter();
	~CModelImporter() override;
	
	TUniquePtr<CResource> ImportResource(std::string file_path) override;
	bool ImportsExtension(std::string extension) override;
};

} }