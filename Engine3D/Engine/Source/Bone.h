#ifndef __BONE_H__
#define __BONE_H__

#include "Resource.h"
#include "MathGeoLib/src/MathGeoLib.h"

struct Weight
{
	unsigned int vertexID;
	float weight;
};

class ComponentMesh;

class Bone : public Resource
{
public:
	Bone(unsigned int uid, std::string& assets, std::string& library);
	~Bone();

	void Load() override;
	//void UnLoad() override;
	void BoneImporter(std::string& path);

public:
	unsigned int numWeights;

	float4x4 Offset;
	float* pos;
	float* rot;
	float* scale;

	Weight* weights;
	uint boneMeshUID;
};

#endif