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

BoneComponent::BoneComponent(GameObject* owner)
{
	sphere = new Sphere();
	//sphere. = { 1.0,0.0,0.0,1.0 };
	line = new Line();
	//line->color = { 1.0,0.0,0.0,1.0 };
}

BoneComponent::~BoneComponent()
{
	/*RELEASE(sphere);
	RELEASE(line);*/
}

bool BoneComponent::drawDebugInfo()
{
	//Bone* bone = (Bone*)app->resources->GetResource(RUID);
	Resource* guid = nullptr;
	guid->GetType();
	Bone* bone = (Bone*)guid;

	if (debugDraw)
	{
		//TransformComponent* transform = (TransformComponent*)owner->GetComponent(TRANSFORMATION);
		TransformComponent* transform = (TransformComponent*)owner->GetComponent<TransformComponent>();
		vec position = float3::zero;
		vec scale = float3::one;
		Quat rot = Quat::identity;
		//transform->globalMatrix.Decompose(position, rot, scale);
		transform->GetGlobalTransform().Decompose(position, rot, scale);
		//sphere->SetPos(position.x, position.y, position.z);
		RSphere::CreateSphere(,)
		sphere->pos = vec(position.x, position.y, position.z);

		//if (gameObject->parent != nullptr && gameObject->parent->GetComponent(ANIMATION) == nullptr)
		if (owner != nullptr && owner->GetParent()->GetComponent<AnimationComponent>() == nullptr)
		{
			//line->origin = position;
			line->pos = position;
			float4x4 parentMat = owner->GetParent()->GetComponent<TransformComponent>()->GetGlobalTransform();
			parentMat.Decompose(position, rot, scale);
			//line->GetPoint() = position;
		}

		/*sphere.
		line->Render();*/
	}

	return true;
}

void BoneComponent::OnEditor()
{
	//ResourceBone* bone = (ResourceBone*)App->resources->GetResource(RUID);
	Resource* guid = nullptr;
	guid->GetType();
	Bone* bone = (Bone*)guid;

	std::string tag = (bone != nullptr) ? bone->GetName() : "Unknown";

	if (ImGui::CollapsingHeader(("Bone - "+tag).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
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