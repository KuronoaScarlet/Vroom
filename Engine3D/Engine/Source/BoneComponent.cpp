#include "BoneComponent.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Bone.h"

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
	
}

void BoneComponent::OnLoad(JSON_Value* component)
{

}

void BoneComponent::OnSave(JSON_Value* component) const
{

}
