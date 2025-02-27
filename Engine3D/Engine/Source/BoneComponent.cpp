#include "BoneComponent.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Bone.h"
#include "TransformComponent.h"

#include "Component.h"

BoneComponent::BoneComponent(GameObject* own) : bone(nullptr)
{
	type = ComponentType::BONE;
	owner = own;

	sphere = new PSphere();
	sphere->color = { 1.0,0.0,0.0,1.0 };
	line = new PLine();
	line->color = { 1.0,0.0,0.0,1.0 };
}

BoneComponent::BoneComponent(BoneComponent* cBone)
{
	bone = cBone->bone;
}

BoneComponent::~BoneComponent()
{
	RELEASE(sphere);
	RELEASE(line);
}

void BoneComponent::SetDebugDraw()
{
	debugDraw = !debugDraw;
}

bool BoneComponent::Update(float dt)
{
	DebugDraw();
	return true;
}

void BoneComponent::DebugDraw()
{
	if (debugDraw)
	{
		TransformComponent* transform = owner->GetComponent<TransformComponent>();
		vec pos = float3::zero;
		vec scale = float3::one;
		Quat rot = Quat::identity;
		transform->GetGlobalTransform().Decompose(pos, rot, scale);
		sphere->SetPos(pos.x, pos.y, pos.z);

		line->origin = pos;
		owner->GetParent()->GetComponent<TransformComponent>()->GetGlobalTransform().Decompose(pos, rot, scale);
		line->destination = pos; 

		sphere->Render();
		line->Render();
	}
}

void BoneComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Bone Component"))
	{
		Checkbox(this, "Active", active);
		if (bone != nullptr)
		{
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", bone->GetAssetsPath().c_str());
			ImGui::Text("Debug: ");
			ImGui::SameLine();
			if(debugDraw) ImGui::TextColored(ImVec4(1, 1, 0, 1), "Activated");
			if(!debugDraw) ImGui::TextColored(ImVec4(1, 1, 0, 1), "Deactivated");
		}
	}
}

bool BoneComponent::OnLoad(JsonParsing& node)
{
	bone = std::static_pointer_cast<Bone>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Path"))));
	active = node.GetJsonBool("Active");
	return true;
}

bool BoneComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", bone->GetAssetsPath().c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void BoneComponent::SetBone(std::shared_ptr<Resource> b)
{
	bone = std::static_pointer_cast<Bone>(b);

	float3 pos = float3(bone->position[0], bone->position[1], bone->position[2]);
	Quat rot = Quat(bone->rotation[0], bone->rotation[1], bone->rotation[2], bone->rotation[3]);
	float3 sc = float3(bone->scale[0], bone->scale[1], bone->scale[2]);

	float4x4 tMatrix = float4x4::FromTRS(pos, rot, sc);
	owner->GetComponent<TransformComponent>()->SetTransform(tMatrix);
}