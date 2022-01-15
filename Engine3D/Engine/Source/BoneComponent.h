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

	bool Update(float dt) override;
	void DebugDraw();
	void OnEditor() override;

	bool OnLoad(JsonParsing& node);
	bool OnSave(JsonParsing& node, JSON_Array* array) const;

	void SetBone(std::shared_ptr<Resource> b);

	void SetDebugDraw();

public:

	bool debugDraw = false;

	PSphere* sphere = nullptr;
	PLine* line = nullptr;

	float4x4 globalOffset = float4x4::identity;

	std::shared_ptr<Bone> bone;
};