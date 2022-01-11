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

	//if(ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
	//{
	//	Checkbox(this, "Active", active);
	//	if (animation != nullptr)
	//	{
	//		ImGui::Text("Select animation: ");
	//		ImGui::SameLine();
	//		if (ImGui::Button(diff ? diff->GetName().c_str() : ""))
	//		{
	//			showAnimMenu = true;
	//		}
	//		ImGui::Checkbox("Loop", &loop);
	//		ImGui::Checkbox("Interpolation", &interpolation);

	//		if (ImGui::Checkbox("Draw Bones", &debugDraw))
	//		{
	//			ActivateDebugBones(owner, debugDraw);
	//		}

	//		ImGui::Text("Animation Speed:");
	//		ImGui::SameLine();

	//		ImGui::PushID("Speed");
	//		ImGui::InputFloat("", &speed, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
	//		ImGui::PopID();

	//		ImGui::Text("Animation Times:\nDuration: %.2f | Speed: %f", animation->ticks, animation->ticksXsecond);
	//		ImGui::Text("Number of bones: %d", animation->numBones);
	//	}

	//	ImGui::NewLine();

	//	if (ImGui::TreeNodeEx("Blend Animation when pressing 1"))
	//	{
	//		//Hacer retrieve de la Animación que sea considerada Attack
	//		Animation* attack = (Animation*)App->resources->GetResource(attackUID);
	//		ImGui::Text((attack != nullptr) ? attack->GetName() : "No animation");
	//		ImGui::SameLine();
	//		//Abrir menú de seleccionar animaciones
	//		if (attack != nullptr)
	//		{
	//			ImGui::Text("Blend time:");
	//			ImGui::SameLine();

	//			ImGui::PushID("total blend time");
	//			ImGui::InputFloat("", &attackBlendTime, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
	//			ImGui::PopID();
	//		}
	//		ImGui::TreePop();
	//	}

	//	if (ImGui::TreeNodeEx("Blend Animation when pressing 2"))
	//	{
	//		//Hacer retrieve de la Animación que sea considerada Walk
	//		Animation* walk = (Animation*)App->resources->GetResource(walkUID);
	//		ImGui::Text((walk != nullptr) ? walk->GetName() : "No animation");
	//		ImGui::SameLine();
	//		//Abrir menú de seleccionar animaciones
	//		if (walk != nullptr)
	//		{
	//			ImGui::Text("Blend time:");
	//			ImGui::SameLine();

	//			ImGui::PushID("total blend time");
	//			ImGui::InputFloat("", &walkBlendTime, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
	//			ImGui::PopID();
	//		}
	//		ImGui::TreePop();
	//	}

	//	if (ImGui::Checkbox("Smooth Transition", &smoothT))
	//	{
	//		frozenT = false;
	//	}

	//}

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
