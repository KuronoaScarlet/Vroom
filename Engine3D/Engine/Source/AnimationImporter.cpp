#include "AnimationImporter.h"

#include "ResourceManager.h"
#include <vector>
#include "Globals.h"
#include "FileSystem.h"
#include "Application.h"
#include "ResourceManager.h"
#include "Component.h"

void AnimationImporter::ImportAnimations(std::string& path, const aiScene* scene, JsonParsing& json, std::vector<uint>& uids)
{
	for (unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		ImportAnimation(path, scene->mAnimations[i], json, uids);
	}
}

void AnimationImporter::ImportAnimation(std::string& path, const aiAnimation* animation, JsonParsing& json, std::vector<uint>& uids)
{
	float ticks = animation->mDuration;
	float ticksPerSecond = animation->mTicksPerSecond;
	int numBones = animation->mNumChannels;
	BoneTransform* boneTransformations = new BoneTransform[numBones];

	if (animation->mNumChannels > 0)
	{
		for (int i = 0; i < numBones; i++)
		{
			boneTransformations[i].nodeName = (animation->mChannels[i]->mNodeName.length > 0) ? animation->mChannels[i]->mNodeName.C_Str() : "Unnamed";
			boneTransformations[i].numPosKeys = animation->mChannels[i]->mNumPositionKeys;
			boneTransformations[i].posKeysValues = new float[boneTransformations[i].numPosKeys * 3];
			boneTransformations[i].posKeysTimes = new double[boneTransformations[i].numPosKeys];
			for (int j = 0; j < boneTransformations[i].numPosKeys; j++)
			{
				int Jvalue = j * 3;
				boneTransformations[i].posKeysTimes[j] = animation->mChannels[i]->mPositionKeys[j].mTime;
				boneTransformations[i].posKeysValues[Jvalue] = animation->mChannels[i]->mPositionKeys[j].mValue.x;
				boneTransformations[i].posKeysValues[Jvalue + 1] = animation->mChannels[i]->mPositionKeys[j].mValue.y;
				boneTransformations[i].posKeysValues[Jvalue + 2] = animation->mChannels[i]->mPositionKeys[j].mValue.z;
			}

			boneTransformations[i].numScaleKeys = animation->mChannels[i]->mNumScalingKeys;
			boneTransformations[i].scaleKeysValues = new float[boneTransformations[i].numScaleKeys * 3];
			boneTransformations[i].scaleKeysTimes = new double[boneTransformations[i].numScaleKeys];
			for (int j = 0; j < boneTransformations[i].numScaleKeys; j++)
			{
				int Jvalue = j * 3;
				boneTransformations[i].scaleKeysTimes[j] = animation->mChannels[i]->mScalingKeys[j].mTime;
				boneTransformations[i].scaleKeysValues[Jvalue] = animation->mChannels[i]->mScalingKeys[j].mValue.x;
				boneTransformations[i].scaleKeysValues[Jvalue + 1] = animation->mChannels[i]->mScalingKeys[j].mValue.y;
				boneTransformations[i].scaleKeysValues[Jvalue + 2] = animation->mChannels[i]->mScalingKeys[j].mValue.z;
			}

			boneTransformations[i].numRotKeys = animation->mChannels[i]->mNumRotationKeys;
			boneTransformations[i].rotKeysValues = new float[boneTransformations[i].numRotKeys * 4];
			boneTransformations[i].rotKeysTimes = new double[boneTransformations[i].numRotKeys];
			for (int j = 0; j < boneTransformations[i].numRotKeys; j++)
			{
				int Jvalue = j * 4;
				boneTransformations[i].rotKeysTimes[j] = animation->mChannels[i]->mRotationKeys[j].mTime;
				boneTransformations[i].rotKeysValues[Jvalue] = animation->mChannels[i]->mRotationKeys[j].mValue.x;
				boneTransformations[i].rotKeysValues[Jvalue + 1] = animation->mChannels[i]->mRotationKeys[j].mValue.y;
				boneTransformations[i].rotKeysValues[Jvalue + 2] = animation->mChannels[i]->mRotationKeys[j].mValue.z;
				boneTransformations[i].rotKeysValues[Jvalue + 3] = animation->mChannels[i]->mRotationKeys[j].mValue.w;
			}
		}
	}

	std::string animName;
	std::string assetsPath(path);
	std::string name("__");
	name += animation->mName.C_Str();

	assetsPath.insert(assetsPath.find_last_of("."), name.c_str());

	uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::ANIMATION, assetsPath, animName);
	uids.push_back(uid);

	SaveAnimation(animName, ticks, ticksPerSecond, numBones, boneTransformations);

	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Components");
	JsonParsing parse = JsonParsing();
	parse.SetNewJsonNumber(parse.ValueToObject(parse.GetRootValue()), "Type", (int)ComponentType::ANIMATION);
	parse.SetNewJsonString(parse.ValueToObject(parse.GetRootValue()), "Animation Path", animName.c_str());

	json.SetValueToArray(array, parse.GetRootValue());
}

void AnimationImporter::CreateMetaAnimation(std::string& library, std::string& assets, uint uid)
{
	JsonParsing metaFile;

	metaFile.SetNewJsonString(metaFile.ValueToObject(metaFile.GetRootValue()), "Assets Path", assets.c_str());
	metaFile.SetNewJsonNumber(metaFile.ValueToObject(metaFile.GetRootValue()), "Uuid", uid);

	char* buffer = nullptr;
	size_t size = metaFile.Save(&buffer);

	app->fs->Save(library.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);
}

void AnimationImporter::SaveAnimation(std::string& name, float ticks, float ticksPerSecond, int numBones, BoneTransform* boneTransformations)
{
	unsigned int header[3] = { ticks, ticksPerSecond, numBones };
	uint size = sizeof(header);

	for (int i = 0; i < numBones; i++)
	{
		unsigned int boneHeader[4] = { boneTransformations[i].numPosKeys, boneTransformations[i].numScaleKeys, boneTransformations[i].numRotKeys, boneTransformations[i].nodeName.size() };
		size += sizeof(boneHeader);
		size += boneTransformations[i].nodeName.size();

		size += sizeof(double) * boneTransformations[i].numPosKeys;
		size += sizeof(float) * boneTransformations[i].numPosKeys * 3;

		size += sizeof(double) * boneTransformations[i].numScaleKeys;
		size += sizeof(float) * boneTransformations[i].numScaleKeys * 3;

		size += sizeof(double) * boneTransformations[i].numRotKeys;
		size += sizeof(float) * boneTransformations[i].numRotKeys * 4;
	}

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(header);
	memcpy(cursor, header, bytes);
	cursor += bytes;

	for (int i = 0; i < numBones; i++)
	{
		uint boneHeader[4] = { boneTransformations[i].numPosKeys, boneTransformations[i].numScaleKeys, boneTransformations[i].numRotKeys, boneTransformations[i].nodeName.size() };

		bytes = sizeof(boneHeader);
		memcpy(cursor, boneHeader, bytes);
		cursor += bytes;

		bytes = boneTransformations[i].nodeName.size();
		memcpy(cursor, boneTransformations[i].nodeName.c_str(), bytes);
		cursor += bytes;

		bytes = boneTransformations[i].numPosKeys * sizeof(double);
		memcpy(cursor, boneTransformations[i].posKeysTimes, bytes);
		cursor += bytes;
		bytes = boneTransformations[i].numPosKeys * sizeof(float) * 3;
		memcpy(cursor, boneTransformations[i].posKeysValues, bytes);
		cursor += bytes;

		bytes = boneTransformations[i].numScaleKeys * sizeof(double);
		memcpy(cursor, boneTransformations[i].scaleKeysTimes, bytes);
		cursor += bytes;
		bytes = boneTransformations[i].numScaleKeys * sizeof(float) * 3;
		memcpy(cursor, boneTransformations[i].scaleKeysValues, bytes);
		cursor += bytes;

		bytes = boneTransformations[i].numRotKeys * sizeof(double);
		memcpy(cursor, boneTransformations[i].rotKeysTimes, bytes);
		cursor += bytes;
		bytes = boneTransformations[i].numRotKeys * sizeof(float) * 4;
		memcpy(cursor, boneTransformations[i].rotKeysValues, bytes);
		cursor += bytes;
	}

	if (app->fs->Save(name.c_str(), buffer, size) > 0)
		DEBUG_LOG("Animation %s saved succesfully", name.c_str());

	RELEASE_ARRAY(buffer);
}
