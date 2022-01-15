#include "BoneComponent.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Bone.h"
#include "TransformComponent.h"

#include "Component.h"

BoneComponent::BoneComponent(GameObject* own)
{
	sphere = new PSphere();
	sphere->color = { 1.0,0.0,0.0,1.0 };
	line = new PLine();
	line->color = { 1.0,0.0,0.0,1.0 };

	
}

BoneComponent::BoneComponent(BoneComponent* bone)
{

}

BoneComponent::~BoneComponent()
{
	RELEASE(sphere);
	RELEASE(line);
}

bool BoneComponent::DrawDebug()
{
	return false;
}

void BoneComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Animation Component"))
	{
		Checkbox(this, "Active", active);
		if (bone != nullptr)
		{
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", bone->GetAssetsPath().c_str());
		}
	}
}

void BoneComponent::OnLoad(JSON_Value* component)
{

}

void BoneComponent::OnSave(JSON_Value* component) const
{

}

void BoneComponent::SetBone(std::shared_ptr<Resource> b)
{
	bone = std::static_pointer_cast<Bone>(b);
	owner->GetComponent<TransformComponent>()->SetTransform(bone->GetOffset());
}
