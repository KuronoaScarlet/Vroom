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
	// Constructor / Destructor
	// Métodos

public:

	Weight* weights;
	unsigned int numWeights;

	unsigned int boneMeshUId;

	float4x4 offset;
	float* position;
	float* rotation;
	float* scale;
};