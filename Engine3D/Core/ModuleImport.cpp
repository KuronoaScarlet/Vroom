#include "Globals.h"

#include "Application.h"
#include "ModuleImport.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include <vector>
#include <queue>
#include "SDL/include/SDL_opengl.h"
#include "Math/float2.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"

#include <fstream>


ModuleImport::ModuleImport(Application* app, bool start_enabled) : Module(app, start_enabled) {}


// Called before render is available
bool ModuleImport::Init()
{
	//LOG("Creating 3D Renderer context");
	bool ret = true;

	//Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

update_status ModuleImport::Update(float dt) {

	return UPDATE_CONTINUE;
}

bool ModuleImport::LoadGeometry(const char* path) {

	//-- Own structure	
	GameObject* root = nullptr;
	std::string new_root_name(path);

	//-- Assimp stuff
	aiMesh* assimpMesh = nullptr;
	const aiScene* scene = nullptr;
	aiMaterial* texture = nullptr;
	aiString texturePath;

	//Create path buffer and import to scene
	char* buffer = nullptr;
	uint bytesFile = App->fileSystem->Load(path, &buffer);

	if (buffer == nullptr) 
	{
		std::string normPathShort = "Assets/Models/" + App->fileSystem->SetNormalName(path);
		App->fileSystem->NormalizePath(normPathShort.c_str());
		bytesFile = App->fileSystem->Load(normPathShort.c_str(), &buffer);
	}
	if (buffer != nullptr) 
	{
		scene = aiImportFileFromMemory(buffer, bytesFile, aiProcessPreset_TargetRealtime_MaxQuality, NULL);
	}
	else 
	{
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	}


	if (scene != nullptr && scene->HasMeshes()) {
		//Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{		
			bool nameFound = false;
			std::string name;
			aiMatrix4x4 t;
			FindNodeName(scene, i, name);

			GameObject* newGameObject = App->scene->CreateGameObject(name);
			ComponentMesh* mesh = newGameObject->CreateComponent<ComponentMesh>();
			assimpMesh = scene->mMeshes[i];
			
			if (scene->HasMaterials()) 
			{
				texture = scene->mMaterials[assimpMesh->mMaterialIndex];

				if (texture != nullptr) 
				{
					aiGetMaterialTexture(texture, aiTextureType_DIFFUSE, assimpMesh->mMaterialIndex, &texturePath);
					std::string new_path(texturePath.C_Str());
					new_path = App->fileSystem->NormalizePath(texturePath.C_Str());
					if (new_path.size() > 0) 
					{
						mesh->texturePath = "Assets/Textures/" + App->fileSystem->SetNormalName(new_path.c_str());
						if (!App->textures->Find(mesh->texturePath))
						{
							const TextureObject& textureObject = App->textures->Load(mesh->texturePath);							
							ComponentMaterial* materialComp = newGameObject->CreateComponent<ComponentMaterial>();
							materialComp->SetTexture(textureObject);
							
						}
						else
						{
							const TextureObject& textureObject = App->textures->Get(mesh->texturePath);
							ComponentMaterial* materialComp = newGameObject->CreateComponent<ComponentMaterial>();
							materialComp->SetTexture(textureObject);
						}
					}
				}
			}
	
			mesh->numVertices = assimpMesh->mNumVertices;
			mesh->vertices.resize(assimpMesh->mNumVertices);
			
			memcpy(&mesh->vertices[0], assimpMesh->mVertices, sizeof(float3) * assimpMesh->mNumVertices);
			LOG("New mesh with %d vertices", assimpMesh->mNumVertices);

			// -- Copying faces --//
			if (assimpMesh->HasFaces()) {
				mesh->numIndices = assimpMesh->mNumFaces * 3;
				mesh->indices.resize(mesh->numIndices);

				for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
				{
					if (assimpMesh->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!")
					}
					else {
						memcpy(&mesh->indices[i * 3], assimpMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}
			
			// -- Copying Normals info --//
			if (assimpMesh->HasNormals()) {

				mesh->normals.resize(assimpMesh->mNumVertices);
				memcpy(&mesh->normals[0], assimpMesh->mNormals, sizeof(float3) * assimpMesh->mNumVertices);
			}
			
			// -- Copying UV info --//
			if (assimpMesh->HasTextureCoords(0))
			{
				mesh->texCoords.resize(assimpMesh->mNumVertices);
				for (size_t j = 0; j < assimpMesh->mNumVertices; ++j)
				{
					memcpy(&mesh->texCoords[j], &assimpMesh->mTextureCoords[0][j], sizeof(float2));
				}
			}

			ThroughTheFireAndTheNodes(scene->mMeshes[i], scene->mRootNode, t, scene);

			aiVector3D position, scale;
			aiQuaternion rotation;

			t.Decompose(scale, rotation, position);

			newGameObject->transform->SetPosition(float3(position.x, position.y, position.z));
			newGameObject->transform->SetRotation(Quat(rotation.x, rotation.y, rotation.z, rotation.w).ToEulerXYZ());
			newGameObject->transform->SetScale(float3(scale.x, scale.y, scale.z));

			std::string newName("Library/Meshes/" + name + ".mesh");
			Save(mesh, newName.c_str());
			Load(mesh, newName.c_str());
			//gen buf should not be here
			mesh->GenerateBuffers();
			mesh->GenerateBounds();
			mesh->ComputeNormals();
		}
		aiReleaseImport(scene);		
		RELEASE_ARRAY(buffer);
	}
	else 
		LOG("Error loading scene %s", path);

	RELEASE_ARRAY(buffer);

	return true;
}

void ModuleImport::ThroughTheFireAndTheNodes(const aiMesh* mS, aiNode* n, aiMatrix4x4& t, const aiScene* s)
{
	for (int i = 0; i < n->mNumChildren; i++)
	{
		if (n->mChildren[i]->mNumMeshes > 0)
		{
			for (int j = 0; j < n->mChildren[i]->mNumMeshes; j++)
			{
				if (s->mMeshes[n->mChildren[i]->mMeshes[j]] == mS)
				{
					t = n->mChildren[i]->mTransformation;
				}
			}
		}
		if (n->mChildren[i]->mNumChildren > 0)
		{
			ThroughTheFireAndTheNodes(mS, n->mChildren[i], t, s);
		}
	}
}

void ModuleImport::FindNodeName(const aiScene* scene, const size_t i, std::string& name)
{
	bool nameFound = false;
	std::queue<aiNode*> Q;
	Q.push(scene->mRootNode);
	while (!Q.empty() && !nameFound)
	{
		aiNode* node = Q.front();
		Q.pop();
		for (size_t j = 0; j < node->mNumMeshes; ++j)
		{
			if (node->mMeshes[j] == i)
			{
				nameFound = true;
				name = node->mName.C_Str();
			}
		}
		if (!nameFound)
		{
			for (size_t j = 0; j < node->mNumChildren; ++j)
			{
				Q.push(node->mChildren[j]);
			}
		}
	}
}

void ModuleImport::Save(const ComponentMesh* mesh, const char* name)
{
	uint ranges[2] =
	{
		mesh->numIndices,
		mesh->numVertices
	};
	uint size = sizeof(ranges)
		+ sizeof(uint) * mesh->numIndices
		+ sizeof(float3) * mesh->numVertices;

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	bytes = sizeof(uint) * mesh->numIndices;
	memcpy(cursor, &mesh->indices, bytes);
	cursor += bytes;

	bytes = sizeof(float3) * mesh->numVertices;
	memcpy(cursor, &mesh->vertices, bytes);
	cursor += bytes;

	std::ofstream outfile(name, std::ofstream::binary | std::ofstream::trunc);
	outfile.write(buffer, size);
	outfile.close();
	delete[] buffer;
	LOG("file %s saved successfully", name);
}

void ModuleImport::Load(ComponentMesh* mesh, const char* name)
{
	std::ifstream infile(name, std::ifstream::binary | std::ifstream::in);
	if (!infile.is_open())
	{
		LOG("ERROR opening file: %s", name);
		return;
	}
	infile.seekg(0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);

	char* buffer = new char[size];
	infile.read(buffer, size);
	infile.close();
	uint ranges[2];
	char* cursor = buffer;

	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	mesh->numIndices = ranges[0];
	bytes = sizeof(uint) * mesh->numIndices;
	mesh->indices.resize(mesh->numIndices);
	memcpy(&mesh->indices, cursor, bytes);
	cursor += bytes;

	mesh->numVertices = ranges[1];
	bytes = sizeof(float3) * mesh->numVertices;
	mesh->vertices.resize(mesh->numVertices);
	memcpy(&mesh->vertices, cursor, bytes);
	cursor += bytes;

	LOG("file %s loaded successfully", name);
}

// Called before quitting
bool ModuleImport::CleanUp()
{
	//-- Detach log stream
	aiDetachAllLogStreams();

	return true;
}