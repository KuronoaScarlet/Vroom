#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Resource.h"
#include "MathGeoLib/src/MathGeoLib.h"

struct ModelParameters;

struct BoneTransform
{
	//~BoneTransform();

	//bool CalcCurrentIndex(float time, bool test);
	//void CalcTransfrom(float time, bool interpolation);

	//void BlendAnimation();
	//void SmoothBlending(const float4x4& blendtrans, float time);
	//void FrozenBlending(const BoneTransform& bone, float time, float blendTime);

	std::string NodeName = "";

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

	float* PosKeysValues = nullptr;
	double* PosKeysTimes = nullptr;

	float* ScaleKeysValues = nullptr;
	double* ScaleKeysTimes = nullptr;

	float* RotKeysValues = nullptr;
	double* RotKeysTimes = nullptr;
};

class Animation : public Resource
{
public:

	Animation(uint uid, std::string& assets, std::string& library);
	~Animation();

	//void Load() override;
	//void UnLoad() override;

	//void ResetFrames();
	//float GetDuration() const;

	inline const char* GetPath() const { return path.c_str(); }
	//void Reimport(ModelParameters& data);

public:

	float ticks = 0.0f;
	float ticksXsecond = 0.0f;

	int numBones = 0;

	BoneTransform* boneTransformations = nullptr;

private:
	std::string path;
};

#endif // !__RESOURCEANIMATION_H__