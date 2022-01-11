#include "Application.h"
#include "BoneComponent.h"
#include "GameObject.h"
#include "Resource.h"
#include "Bone.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Mesh.h"
#include <vector>
#include "Primitive.h"
#include "AnimationComponent.h"
#include "Globals.h"

BoneComponent::BoneComponent(GameObject* owner)
{
	sphere = new MSphere();
	sphere->color = { 1.0,0.0,0.0,1.0 };
	line = new MLine();
	line->color = { 1.0,0.0,0.0,1.0 };
}

BoneComponent::~BoneComponent()
{
	RELEASE(sphere);
	RELEASE(line);
}

bool BoneComponent::drawDebugInfo()
{
	if (debugDraw)
	{
		TransformComponent* transform = (TransformComponent*)owner->GetComponent<TransformComponent>();
		vec position = float3::zero;
		vec scale = float3::one;
		Quat rot = Quat::identity;
		transform->GetGlobalTransform().Decompose(position, rot, scale);
		sphere->SetPos(position.x, position.y, position.z);

		if (owner != nullptr && owner->GetParent()->GetComponent<AnimationComponent>() == nullptr)
		{
			line->origin = position;
			float4x4 parentMat = owner->GetParent()->GetComponent<TransformComponent>()->GetGlobalTransform();
			parentMat.Decompose(position, rot, scale);
			line->destination = position;
		}

		sphere->Render();
		line->Render();
	}

	return true;
}

bool BoneComponent::OnLoad(JsonParsing& node)
{
	return false;
}

bool BoneComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}

void BoneComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Text("This component defines current game object\nas a bone for executing animations");

		ImGui::Checkbox("DebugDraw", &debugDraw);
	}
}

//void BoneComponent::Save(JSON_Value * component) const
//{
//	ResourceBone* rBone = (ResourceBone*)App->resources->GetResource(RUID);
//
//	JSON_Value* bone = component->createValue();
//
//	bone->addInt("Type", type);
//	bone->addUint("UID", UID);
//	bone->addBool("debugDraw", debugDraw);
//
//	if (rBone != nullptr)
//	{
//		bone->addString("FBX", rBone->GetFile());
//		bone->addString("bone", rBone->GetName());
//	}
//
//	component->addValue("", bone);
//}
//
//void BoneComponent::Load(JSON_Value * component)
//{
//	RUID = App->resources->getResourceUIDFromMeta(component->getString("FBX"), "bones", component->getString("bone"));
//
//	Resource* res = App->resources->GetResource(RUID);
//	if (res != nullptr)
//		res->LoadtoMemory();
//
//	debugDraw = component->getBool("debugDraw");
//}