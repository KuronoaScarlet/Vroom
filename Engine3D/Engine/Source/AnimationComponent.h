#ifndef __ANIMATIONCOMPONENT_H__
#define __ANIMATIONCOMPONENT_H__

#include "Component.h"
#include <vector>
#include <map>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class GameObject;
class Resource;
class TransformComponent;
class AnimationComponent : public Component
{
public:

	AnimationComponent(GameObject* own);
	~AnimationComponent();

public:
	bool Update(float dt) override { return true; };
	void ActivateBonesDebug(GameObject* go, bool isActive);
	void AssignResource(uint id, bool blend = false, bool blendLoop = true);
	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void ActivateDebugBones(GameObject* GO, bool active);

private:
	//std::shared_ptr<Animation> animation;
	std::map<unsigned int, unsigned int> bones;
	std::map<unsigned int, unsigned int> blendedBones;

	bool loop = false;
	bool blend = false;
	bool blendLoop = false;
	bool debugDraw = false;
	bool bonesLoaded = false;
	bool smoothT = false;
	bool frozenT = false;
	bool interpolation = false;
	bool TestPause = false;
	bool TestPlay = false;
	bool useGraph = false;
	bool showAnimMenu = false;

	unsigned int blendRUID = 0;

	float animTime = 0.0f;
	float blendTime = 0.0f;
	float blendAnimTime = 0.0f;
	float blendPercent = 0.0f;
	float totalBlendTime = 0.1f;
	float speed = 1.0f;
	float walkBlendTime = 0.1f;
	float attackBlendTime = 0.1f;

	/*std::map<uint, uint> bones;
	std::map<uint, uint> blendBones;*/
};

#endif