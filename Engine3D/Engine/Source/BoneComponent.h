#pragma once

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"
#include "Primitive.h"
#include "Bone.h"
#include <memory>

class BoneComponent : public Component
{
public:
	BoneComponent(GameObject* own);
	BoneComponent(BoneComponent* bone);
	~BoneComponent();

	bool DrawDebug();
	void OnEditor() override;

	void OnLoad(JSON_Value* component);
	void OnSave(JSON_Value* component) const;

	void SetBone(std::shared_ptr<Resource> b);

public:

	bool debugDraw = false;

	PSphere* sphere = nullptr;
	PLine* line = nullptr;

	float4x4 globalOffset = float4x4::identity;

	std::shared_ptr<Bone> bone;
};