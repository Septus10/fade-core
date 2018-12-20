#pragma once

namespace Fade { namespace ECS {

class CEntity {};

class CComponent {};

class CComponentStore 
{
public:

};

class CSystem 
{
public:
	virtual ~CSystem() = default;

	virtual void Initialize() = 0;
	virtual void Update(double a_DeltaTime, const CComponentStore& a_ComponentStore) = 0;
};

} }