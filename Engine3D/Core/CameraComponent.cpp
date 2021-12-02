#include "Globals.h"
#include "Application.h"
#include "CameraComponent.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleViewportFrameBuffer.h"
#include "SDL/include/SDL_opengl.h"

CameraComponent::CameraComponent(GameObject* parent) : Component(parent)
{
	frustrum.type = FrustumType::PerspectiveFrustum;
	frustrum.nearPlaneDistance = 0.2f;
	frustrum.farPlaneDistance = 500.f;
	frustrum.front = owner->transform->GetForward();
	frustrum.up = owner->transform->GetUp();
	frustrum.verticalFov = 60.0f * DEGTORAD;
	frustrum.horizontalFov = 2.0f * atanf(tanf(frustrum.verticalFov / 2) * (16.f / 9.f));
}

CameraComponent::~CameraComponent()
{}

// -----------------------------------------------------------------
bool CameraComponent::Start()
{
	LOG("Setting up the camera");

	LookAt(float3::zero);

	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
bool CameraComponent::Update(float dt)
{

		frustrum.pos = owner->transform->GetPosition();
		frustrum.front = owner->transform->GetForward();
		frustrum.up = owner->transform->GetUp();
		viewMatrix = frustrum.ViewMatrix();


	return true;
}

bool CameraComponent::PreUpdate(float dt)
{
	App->viewportBufferGame->PreUpdate(dt);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(frustrum.ProjectionMatrix().Transposed().ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix.Transposed().ptr());

	return true;
}

// -----------------------------------------------------------------
void CameraComponent::LookAt(const float3& point)
{		
	float3 right;

	frustrum.front = (point - frustrum.pos).Normalized();
	right = float3(0.0f, 1.0f, 0.0f).Cross(frustrum.front).Normalized();
	frustrum.up = frustrum.front.Cross(right);

	movedCamera = true;
}
// -----------------------------------------------------------------
void CameraComponent::CalculateViewMatrix()
{
	if (projectionIsDirty)
		RecalculateProjection();

	cameraFrustum.pos = position;
	cameraFrustum.front = front.Normalized();
	cameraFrustum.up = up.Normalized();
	float3::Orthonormalize(cameraFrustum.front, cameraFrustum.up);
	right = up.Cross(front);
	viewMatrix = cameraFrustum.ViewMatrix();
}

void CameraComponent::RecalculateProjection()
{
	frustrum.type = FrustumType::PerspectiveFrustum;
	frustrum.verticalFov = (FOV * 3.141592 / 2) / 180.f;
	frustrum.horizontalFov = 2.f * atanf(tanf(frustrum.verticalFov * 0.5f) * aspectRatio);
}

void CameraComponent::OnGui()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::DragFloat("Near Plane", &frustrum.nearPlaneDistance, 0.2, 0.1, 500);
		ImGui::DragFloat("Far Plane", &frustrum.farPlaneDistance, 0.2, 0.1);
		if (ImGui::DragFloat("V FOV", &FOV, 0.2, 0.1))
		{
			RecalculateProjection();
		}
	}
}