#pragma once
#include "Module.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "Assimp/include/matrix4x4.h"
#include <string>

class ComponentMesh;
struct aiScene;
struct aiMesh;
struct aiNode;

class ModuleImport : public Module
{
public:
	
	ModuleImport(Application* app, bool start_enabled = true);

	bool Init() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	bool LoadGeometry(const char* path);

	void FindNodeName(const aiScene* scene, const size_t i, std::string& name);
	void ThroughTheFireAndTheNodes(const aiMesh* meshScene, aiNode* node, aiMatrix4x4& transform, const aiScene* scene);
	
	void Save(const ComponentMesh* mesh, const char* name);
	void Load(ComponentMesh* mesh, const char* name);

	std::vector<std::string> check;
};