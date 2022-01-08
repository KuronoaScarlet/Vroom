#include "Application.h"
#include "Globals.h"
#include "AnimationComponent.h"
#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

class TransformComponent;

AnimationComponent::AnimationComponent(GameObject* own, TransformComponent* trans) 
{
	type = ComponentType::ANIMATION;

	this->smoothT = true;
	this->showAnimMenu = false;
}

AnimationComponent::~AnimationComponent()
{

}
