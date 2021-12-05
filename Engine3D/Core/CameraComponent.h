#pragma once
#include "Globals.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Geometry/Frustum.h"
#include "Component.h"

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* parent);
	virtual ~CameraComponent(){}

	bool Start();
	bool Update(float dt)override;
	bool PostUpdate(float dt);
	bool PreUpdate(float dt);
	void RecalculateProjection();

	void LookAt(const float3&point);
	void CalculateViewMatrix();
	void OnGui() override;
	Frustum GetCamera()
	{
		return frustrum;
	}

	float3 right, up, front, position, reference;
	Frustum cameraFrustum;
	float4x4 viewMatrix;
	float aspectRatio = 1.f;
	float verticalFOV = 60.f;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 60000.f;
	float cameraSensitivity = .5f; 
	float cameraSpeed = 30.f;
	bool projectionIsDirty = false;

	unsigned int framebuffer;
	unsigned int texColorBuffer;
	unsigned int rbo;
	bool movedCamera;

	Frustum frustrum;
	float FOV = 60.f;

	void Buffer();

private:
	float lastDeltaX = 0.f, lastDeltaY = 0.f;

};