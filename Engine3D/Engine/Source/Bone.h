#pragma once

#include "Resource.h"
#include "MathGeoLib/src/MathGeoLib.h"

struct Weight {
	unsigned int vertexId;
	float weight;
};

class Bone :public Resource
{
public:
	Bone(uint uid, std::string& assets, std::string& library);
	~Bone();

	void Load() override;
	//void UnLoad() override;

	inline const Weight* GetWeight() const { return weights; }
	inline const int& GetNumWeights() const { return numWeights; }
	inline const float4x4& GetOffset() const { return offset; }
	inline const float* GetPosition() const { return position; }
	inline const float* GetRotation() const { return rotation; }
	inline const float* GetScale() const { return scale; }

public:

	Weight* weights;
	unsigned int numWeights;

	unsigned int boneMeshUId;

	float4x4 offset;
	float* position;
	float* rotation;
	float* scale;
};