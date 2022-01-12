#include "MeshImporter.h"

#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Component.h"
#include "TextureImporter.h"
#include "Mesh.h"
#include "Model.h"
#include "MathGeoLib/src/Algorithm/Random/LCG.h"
#include "AnimationImporter.h"
#include "Globals.h"
#include "Animation.h"

#include "Profiling.h"
#include "AnimationImporter.h"

bool AnimationImporter::AnimationImport(aiAnimation* animation, unsigned int UID, std::string& newpath)
{
	bool ret = false;

	Animation* newAnimation = new Animation(0, ResourceType::ANIMATION);

	newAnimation->ticks = animation->mDuration;
	newAnimation->ticksXsecond = animation->mTicksPerSecond;
	newAnimation->numBones = animation->mNumChannels;

	if (animation->mNumChannels > 0)
	{
		newAnimation->boneTransformations = new BoneTransform[newAnimation->numBones];
		for (int i = 0; i < newAnimation->numBones; i++)
		{
			newAnimation->boneTransformations[i].NodeName = (animation->mChannels[i]->mNodeName.length > 0) ? animation->mChannels[i]->mNodeName.C_Str() : "Unnamed";

			newAnimation->boneTransformations[i].numPosKeys = animation->mChannels[i]->mNumPositionKeys;
			newAnimation->boneTransformations[i].PosKeysValues = new float[newAnimation->boneTransformations[i].numPosKeys * 3];
			newAnimation->boneTransformations[i].PosKeysTimes = new double[newAnimation->boneTransformations[i].numPosKeys];
			for (int j = 0; j < newAnimation->boneTransformations[i].numPosKeys; j++)
			{
				int Jvalue = j * 3;
				newAnimation->boneTransformations[i].PosKeysTimes[j] = animation->mChannels[i]->mPositionKeys[j].mTime;
				newAnimation->boneTransformations[i].PosKeysValues[Jvalue] = animation->mChannels[i]->mPositionKeys[j].mValue.x;
				newAnimation->boneTransformations[i].PosKeysValues[Jvalue + 1] = animation->mChannels[i]->mPositionKeys[j].mValue.y;
				newAnimation->boneTransformations[i].PosKeysValues[Jvalue + 2] = animation->mChannels[i]->mPositionKeys[j].mValue.z;
			}

			newAnimation->boneTransformations[i].numScaleKeys = animation->mChannels[i]->mNumScalingKeys;
			newAnimation->boneTransformations[i].ScaleKeysValues = new float[newAnimation->boneTransformations[i].numScaleKeys * 3];
			newAnimation->boneTransformations[i].ScaleKeysTimes = new double[newAnimation->boneTransformations[i].numScaleKeys];
			for (int j = 0; j < newAnimation->boneTransformations[i].numScaleKeys; j++)
			{
				int Jvalue = j * 3;
				newAnimation->boneTransformations[i].ScaleKeysTimes[j] = animation->mChannels[i]->mScalingKeys[j].mTime;
				newAnimation->boneTransformations[i].ScaleKeysValues[Jvalue] = animation->mChannels[i]->mScalingKeys[j].mValue.x;
				newAnimation->boneTransformations[i].ScaleKeysValues[Jvalue + 1] = animation->mChannels[i]->mScalingKeys[j].mValue.y;
				newAnimation->boneTransformations[i].ScaleKeysValues[Jvalue + 2] = animation->mChannels[i]->mScalingKeys[j].mValue.z;
			}

			newAnimation->boneTransformations[i].numRotKeys = animation->mChannels[i]->mNumRotationKeys;
			newAnimation->boneTransformations[i].RotKeysValues = new float[newAnimation->boneTransformations[i].numRotKeys * 4];
			newAnimation->boneTransformations[i].RotKeysTimes = new double[newAnimation->boneTransformations[i].numRotKeys];
			for (int j = 0; j < newAnimation->boneTransformations[i].numRotKeys; j++)
			{
				int Jvalue = j * 4;
				newAnimation->boneTransformations[i].RotKeysTimes[j] = animation->mChannels[i]->mRotationKeys[j].mTime;
				newAnimation->boneTransformations[i].RotKeysValues[Jvalue] = animation->mChannels[i]->mRotationKeys[j].mValue.x;
				newAnimation->boneTransformations[i].RotKeysValues[Jvalue + 1] = animation->mChannels[i]->mRotationKeys[j].mValue.y;
				newAnimation->boneTransformations[i].RotKeysValues[Jvalue + 2] = animation->mChannels[i]->mRotationKeys[j].mValue.z;
				newAnimation->boneTransformations[i].RotKeysValues[Jvalue + 3] = animation->mChannels[i]->mRotationKeys[j].mValue.w;
			}
		}

		ret = true;
	}

	RELEASE(newAnimation);

	return ret;
}