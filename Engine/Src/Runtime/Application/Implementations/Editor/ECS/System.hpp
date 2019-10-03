#pragma once

#include <Core/definitions.hpp>
#include <Core/Utility/TemplateUtil.hpp>

#include <ECS/SystemManager.hpp>

namespace Fade { namespace ECS {

class CComponentStore;
class CSystem
{
public:
	virtual ~CSystem() = default;

	virtual void Initialize() = 0;
	virtual void Update(double a_DeltaTime, const CComponentStore& a_ComponentStore) = 0;
};

} }