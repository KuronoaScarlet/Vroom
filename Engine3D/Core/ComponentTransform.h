#pragma once

#include "Component.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"

class ComponentTransform : public Component {

public:

	ComponentTransform(GameObject* parent);

	bool Update(float dt) override;
	void OnGui() override;

	void SetPosition(const float3& newPosition);
	void SetRotation(const float3& newRotation);
	void SetScale(const float3& newScale);
	inline const float3& Right() const { return right; }
	inline const float3& Up() const { return up; }
	inline const float3& Front() const { return front; }

	void Save();
	void Load();

	inline float3 GetPosition() const { return position; };
	inline float3 GetRotation() const { return rotationEuler; };
	inline float3 GetScale() const { return scale; };
	inline float3 GetForward() const { return  transformMatrix.RotatePart().Col(2).Normalized(); };
	inline float3 GetUp() const { return  transformMatrix.RotatePart().Col(1).Normalized(); };

	void NewAttachment();
	void OnParentMoved();

	void RecomputeGlobalMatrix();

	void Save(JSONWriter& writer);
	void Load(const JSONReader& reader);
	
	float4x4 transformMatrix;
	float4x4 transformMatrixLocal;
	float3 savedPos;
	float3 savedRot;
	float3 savedSca;
	float3 position;
	Quat rotation;
	float3 rotationEuler;
	float3 scale;
private:
	
	bool isDirty = false;

	float3 front = float3::unitZ;
	float3 up = float3::unitY;
	float3 right = float3::unitX;

};