#include "Application.h"
#include "Globals.h"
#include "AnimationComponent.h"
#include "BoneComponent.h"
#include "GameObject.h"
#include "Animation.h"
#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

class TransformComponent;

AnimationComponent::AnimationComponent(GameObject* own) 
{
	type = ComponentType::ANIMATION;

	this->smoothT = true;
	this->showAnimMenu = false;
}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::ActivateBonesDebug(GameObject* go, bool isActive)
{
}

void AnimationComponent::AssignResource(uint id, bool blend, bool blendLoop)
{
}

void AnimationComponent::OnEditor()
{
	ImGui::PushID(this);

	if(ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
	{
		Checkbox(this, "Active", active);

	}

	ImGui::PopID();
}

bool AnimationComponent::OnLoad(JsonParsing& node)
{
	return false;
}

bool AnimationComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}

void AnimationComponent::ActivateDebugBones(GameObject* GO, bool active)
{
	BoneComponent* bone = GO->GetComponent<BoneComponent>();
	if (bone != nullptr)
		bone->debugDraw = active;

	for (std::vector<GameObject*>::iterator it = GO->GetChilds().begin(); it != GO->GetChilds().end(); it++)
	{
		ActivateDebugBones((*it), active);
	}
}
