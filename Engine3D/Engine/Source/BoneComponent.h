#ifndef __BONECOMPONENT_H__
#define __BONECOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

class MSphere;
class MLine;

class BoneComponent : public Component
{
public:
	BoneComponent(GameObject* owner);
	~BoneComponent();

	//bool Update(float dt) override;
	void OnEditor() override;
	bool drawDebugInfo();

	/*void Save(JSON_Value* component) const;
	void Load(JSON_Value* component);*/

public:

	bool debugDraw = false;

	MSphere* sphere = nullptr;
	MLine* line = nullptr;

	float4x4 globalOffset = float4x4::identity;
};

#endif // !__COMPONENTBONE_H__