#include "Application.h"
#include "Globals.h"
#include "MeshComponent.h"

#include "ModuleScene.h"
#include "CameraComponent.h"
#include "FileSystem.h"
#include "ResourceManager.h"

#include "Mesh.h"

#include "Imgui/imgui.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

MeshComponent::MeshComponent(GameObject* own, TransformComponent* trans) : material(nullptr), transform(trans), faceNormals(false), verticesNormals(false), normalLength(1.0f), colorNormal(150.0f, 0.0f, 255.0f)
{
	type = ComponentType::MESH_RENDERER;
	owner = own;
	mesh = nullptr;
	material = owner->GetComponent<MaterialComponent>();

	showMeshMenu = false;
}

MeshComponent::MeshComponent(MeshComponent* meshComponent, TransformComponent* trans) : material(nullptr), showMeshMenu(false)
{
	transform = trans;
	mesh = meshComponent->GetMesh();

	faceNormals = meshComponent->faceNormals;
	verticesNormals = meshComponent->verticesNormals;
	normalLength = meshComponent->normalLength;
	colorNormal = meshComponent->colorNormal;

	localBoundingBox = meshComponent->localBoundingBox;
}

MeshComponent::~MeshComponent()
{
	if (mesh.use_count() - 1 == 1) mesh->UnLoad();
}

bool MeshComponent::Update(float dt)
{
	if (mesh->GetBonesUidList().size() > 0) Skinning();

	return true;
}

void MeshComponent::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	glMultMatrixf(transform->GetGlobalTransform().Transposed().ptr());
	
	if (material != nullptr && material->GetActive()) material->BindTexture();
	
	if (mesh != nullptr) mesh->Draw(verticesNormals, faceNormals, colorNormal, normalLength);

	if (material != nullptr && material->GetActive()) material->UnbindTexture();
	
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshComponent::DrawOutline()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	float4x4 testGlobal = transform->GetGlobalTransform();

	testGlobal.scaleX += 0.05f;
	testGlobal.scaleY += 0.05f;
	testGlobal.scaleZ += 0.05f;
	glMultMatrixf(testGlobal.Transposed().ptr());

	if (mesh != nullptr) mesh->Draw(verticesNormals, faceNormals, colorNormal, normalLength);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Mesh Renderer"))
	{
		Checkbox(this, "Active", active);
		ImGui::Text("Select mesh");
		ImGui::SameLine();
		if (ImGui::Button(mesh ? mesh->GetName().c_str() : "No Mesh"))
		{
			showMeshMenu = true;
		}
		ImGui::Text("Number of vertices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh ? mesh->GetVerticesSize() : 0);
		ImGui::Text("Number of indices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh ? mesh->GetIndicesSize() : 0);
		ImGui::Checkbox("Vertices normals", &verticesNormals);
		ImGui::Checkbox("Face normals", &faceNormals);
		ImGui::DragFloat("Normal Length", &normalLength, 0.200f);
		ImGui::DragFloat3("Normal Color", colorNormal.ptr(), 1.0f, 0.0f, 255.0f);
		ImGui::Text("Reference Count: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh ? mesh.use_count() : 0);
		ImGui::Separator();
		ImGui::Text("Bones Count: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh ? mesh->GetBonesCount() : 0);
	}

	if (showMeshMenu)
	{
		ImGui::Begin("Meshes", &showMeshMenu, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetIO().MousePos;
		if (!(mouse.x < winPos.x + size.x && mouse.x > winPos.x && 
			mouse.y < winPos.y + size.y && mouse.y > winPos.y))
		{
			if (ImGui::GetIO().MouseClicked[0]) showMeshMenu = false;
		}

		std::vector<std::string> files;
		app->fs->DiscoverFiles("Library/Meshes/", files);
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			if ((*it).find(".vrmesh") != std::string::npos)
			{
				app->fs->GetFilenameWithoutExtension(*it);
				*it = (*it).substr((*it).find_last_of("_") + 1, (*it).length());
				uint uid = std::stoll(*it);
				std::shared_ptr<Resource> res = ResourceManager::GetInstance()->LoadResource(uid);
				if (ImGui::Selectable(res->GetName().c_str()))
				{
					if (mesh.use_count() - 1 == 1) mesh->UnLoad();
					SetMesh(res);
				}
			}
		}

		ImGui::End();
	}

	ImGui::PopID();
}

bool MeshComponent::OnLoad(JsonParsing& node)
{
	mesh = std::static_pointer_cast<Mesh>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Path"))));

	active = node.GetJsonBool("Active");

	if (mesh)
	{
		localBoundingBox.SetNegativeInfinity();
		localBoundingBox.Enclose(mesh->GetVerticesData(), mesh->GetVerticesSize());

		owner->SetAABB(localBoundingBox);
	}

	return true;
}

bool MeshComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", mesh->GetAssetsPath().c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void MeshComponent::SetMesh(std::shared_ptr<Resource> m)
{
	mesh = std::static_pointer_cast<Mesh>(m);

	if (mesh)
	{
		localBoundingBox.SetNegativeInfinity();
		localBoundingBox.Enclose(mesh->GetVerticesData(), mesh->GetVerticesSize());

		owner->SetAABB(localBoundingBox);
	}

	int numBones = mesh->GetBonesCount();

	if (numBones > 0)
	{
		for (int i = 0; i < mesh->GetBonesUidList().size(); i++)
		{
			GameObject* bone = app->scene->CreateGameObject(owner);
			bone->CreateComponent(ComponentType::BONE);

			std::shared_ptr<Resource> rBone = ResourceManager::GetInstance()->GetResource(mesh->GetBonesUidList().at(i));
			rBone->Load();
			bone->GetComponent<BoneComponent>()->SetBone(rBone);
			bone->SetName(rBone->GetName().c_str());
			boneList.push_back(bone);
		}
	}
}

void MeshComponent::Skinning()
{
	// Skinning Matrix
	for (int i = 0; i < boneList.size(); i++)
	{
		float4x4 delta = boneList.at(i)->GetComponent<TransformComponent>()->GetGlobalTransform();
		delta = owner->GetComponent<TransformComponent>()->GetGlobalTransform().Inverted() * delta;
		delta = delta * boneList.at(i)->GetComponent<BoneComponent>()->bone->GetOffset();
		skinningMatrix.insert(std::make_pair(mesh->GetBonesUidList().at(i), delta));
	}

	mesh->skinnedVertices.resize(mesh->vertices.size());

	memcpy(&mesh->skinnedVertices[0], &mesh->vertices[0], sizeof(float3) * mesh->vertices.size());

	for (int i = 0; i < boneList.size(); i++)
	{
		int it = boneList.at(i)->GetComponent<BoneComponent>()->bone->weights.size();
		for (int j = 0; j < it; j++)
		{
			Weight w = boneList.at(i)->GetComponent<BoneComponent>()->bone->weights.at(j);
			float3 original = mesh->vertices[w.vertexId];
			float3 newVertex = skinningMatrix[boneList.at(i)->GetComponent<BoneComponent>()->bone->GetUID()].TransformPos(original);
			mesh->skinnedVertices[w.vertexId].x = newVertex.x * w.weight;
			mesh->skinnedVertices[w.vertexId].y = newVertex.y * w.weight;
			mesh->skinnedVertices[w.vertexId].z = newVertex.z * w.weight;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo->buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * mesh->vertices.size(), &mesh->skinnedVertices[0], GL_DYNAMIC_DRAW);
}
