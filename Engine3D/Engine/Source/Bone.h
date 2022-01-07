#ifndef __BONE_H__
#define __BONE_H__

#include "Resource.h"

struct Weight
{
	unsigned int vertexID;
	float weight;
};

class ComponentMesh;

class Bone : public Bone
{
	Bone(unsigned int uid);
	~Bone();

	void Load() override;
	void UnLoad() override;

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