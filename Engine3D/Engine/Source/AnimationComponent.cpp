#include "Application.h"
#include "Globals.h"
#include "AnimationComponent.h"

#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

AnimationComponent::AnimationComponent(GameObject* own) 
{
	type = ComponentType::ANIMATION;
	owner = own;

	smoothT = true;
	showAnimMenu = false;
}

AnimationComponent::~AnimationComponent()
{

}
