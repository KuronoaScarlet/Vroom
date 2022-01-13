#pragma once

#include "Resource.h"
#include "MathGeoLib/src/MathGeoLib.h"

struct BoneTransform
{
	~BoneTransform();

	//bool CalcCurrentIndex(float time, bool test);
	//void CalcTransfrom(float time, bool interpolation);

	//void BlendAnimation();
	//void SmoothBlending(const float4x4& blendtrans, float time);
	//void FrozenBlending(const BoneTransform& bone, float time, float blendTime);

	std::string nodeName = "";

	float4x4 lastTransform = float4x4::identity;
	int currentPosIndex = -1;
	int currentRotIndex = -1;
	int currentScaleIndex = -1;
	int nextPosIndex = -1;
	int nextRotIndex = -1;
	int nextScaleIndex = -1;

	int numPosKeys = 0;
	int numScaleKeys = 0;
	int numRotKeys = 0;

	float* posKeysValues = nullptr;
	double* posKeysTimes = nullptr;

	float* scaleKeysValues = nullptr;
	double* scaleKeysTimes = nullptr;

	float* rotKeysValues = nullptr;
	double* rotKeysTimes = nullptr;
};


class Animation : public Resource
{
public:
	Animation(uint uid, std::string& assets, std::string& library);
	~Animation();
	
	void Load() override;
	void UnLoad() override;

	inline const float& GetTicks() const { return ticks; }
	inline const float& GetTicksPerSecond() const { return ticksPerSecond; }
	inline const float& GetDuration() const { return ticks / ticksPerSecond; }

public:

	unsigned int id = 0;

	float ticks = 0.0f;
	float ticksPerSecond = 0.0f;

	int numBones = 0;

	BoneTransform* boneTranformations = nullptr;
};