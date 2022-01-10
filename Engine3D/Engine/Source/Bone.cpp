#include "Bone.h"
#include "Application.h"
#include "FileSystem.h"
#include "MeshComponent.h"
#include "ResourceManager.h"
Bone::Bone(unsigned int uid, std::string& assets, std::string& library) : Resource(uid, ResourceType::BONE, assets, library)
{
	std::string mPath = ANIMATIONS_FOLDER + std::string("bone_") + std::to_string(uid) + ".meta";
}

Bone::~Bone()
{
}
void Bone::BoneImporter(std::string& path)
{
	if (ResourceManager::GetInstance()->CheckResource(path))
	{
		return;
	}
	else
	{
		std::string libPath;


		ResourceManager::GetInstance()->CreateResource(ResourceType::BONE, path, libPath);
	}
}

//	CREATE FUNCTION TO SAVE AND LOAD FROM JSON .vrbone

//bool Bone::LoadInMemory()
//{
//	LoadBone();
//
//	return true;
//}

void Bone::Load()
{
	bool ret = true;

	//Get the buffer
	char* buffer = nullptr;
	app->fs->Load(libraryPath.c_str(), &buffer);
	char* cursor = buffer;

	uint bytes = sizeof(uint);
	memcpy(&numWeights, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 3;
	pos = new float[3];
	memcpy(pos, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 4;
	rot = new float[4];
	memcpy(rot, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 3;
	scale = new float[3];
	memcpy(scale, cursor, bytes);
	cursor += bytes;

	Offset = float4x4::FromTRS({ pos[0],pos[1],pos[2] }, { rot[0],rot[1],rot[2],rot[3] }, { scale[0],scale[1],scale[2] });

	if (numWeights > 0)
	{
		weights = new Weight[numWeights];

		for (int i = 0; i < numWeights; i++)
		{
			bytes = sizeof(uint);
			memcpy(&weights[i].vertexID, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float);
			memcpy(&weights[i].weight, cursor, bytes);
			cursor += bytes;
		}
	}
}

//bool Bone::UnloadFromMemory()
//{
//
//	//RELEASE_ARRAY(pos);
//	//RELEASE_ARRAY(rot);
//	//RELEASE_ARRAY(scale);
//	//RELEASE_ARRAY(weights);
//
//	return true;
//}
