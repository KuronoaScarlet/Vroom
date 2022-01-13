#pragma once

#include <string>

#include "assimp/anim.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "Animation.h"

class JsonParsing;

namespace AnimationImporter
{
	void ImportAnimations(std::string& path, const aiScene* scene, JsonParsing& json, std::vector<uint>& uids);
	void ImportAnimation(std::string& path, const aiAnimation* animation, JsonParsing& json, std::vector<uint>& uids);
	void CreateMetaAnimation(std::string& library, std::string& assets, uint uid);
	void SaveAnimation(std::string& name, float ticks, float ticksPerSecond, int numBones, BoneTransform* boneTransformations);
	void LoadAnimation(const char* path, float& ticks, float& ticksPerSecond, int& numBones, BoneTransform* boneTransformations);
}